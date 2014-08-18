/**
 * @file auth.c
 * @brief uSched
 *        Authentication and Authorization interface - Daemon
 *
 * Date: 18-08-2014
 * 
 * Copyright 2014 Pedro A. Hortas (pah@ucodev.org)
 *
 * This file is part of usched.
 *
 * usched is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * usched is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with usched.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <psec/crypt.h>
#include <psec/decode.h>
#include <psec/generate.h>
#include <psec/hash.h>
#include <psec/kdf.h>

#include "debug.h"
#include "config.h"
#include "runtime.h"
#include "log.h"
#include "local.h"

int auth_daemon_local(int fd, uid_t *uid, gid_t *gid) {
	int errsv = 0;

	if (local_fd_peer_cred(fd, uid, gid) < 0) {
		errsv = errno;
		log_warn("auth_daemon_local(): local_fd_peer_cred(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int auth_daemon_remote_user_token_verify(
	const char *username,
	const char *session,
	unsigned char *dh_shared,
	size_t dh_shared_size,
	unsigned char *nonce,
	const unsigned char *token,
	uid_t *uid,
	gid_t *gid)
{
	int errsv = 0, rounds = CONFIG_USCHED_SEC_KDF_ROUNDS;
	struct usched_config_userinfo *userinfo = NULL;
	unsigned char salt[CONFIG_USCHED_AUTH_USERNAME_MAX];
	unsigned char pwhash_c[HASH_DIGEST_SIZE_SHA512], pwhash_s[HASH_DIGEST_SIZE_SHA512 + 3];
	unsigned char pw_payload[CONFIG_USCHED_AUTH_PASSWORD_MAX + 1];
	unsigned char key[HASH_DIGEST_SIZE_BLAKE2S];
	char plain_passwd[CONFIG_USCHED_AUTH_PASSWORD_MAX];
	size_t out_len = 0;

	/* Session data contents:
	 *
	 * | nonce (24 bytes) | encrypted plain password (16 bytes + 1 byte + 256 bytes) |
	 *
	 * Total session size: 297 bytes
	 *
	 */

	/* Check if username doesn't exceed the expected size */
	if (strlen(username) > sizeof(salt)) {
		log_warn("auth_daemon_remote_user_token_verify(): strlen(username) > sizeof(salt)\n");
		errno = EINVAL;
		return -1;
	}

	/* Craft salt */
	memset(salt, 'x', sizeof(salt));
	memcpy(salt, username, strlen(username));
	
	/* Get userinfo data from current configuration */
	if (!(userinfo = rund.config.users.list->search(rund.config.users.list, (struct usched_config_userinfo [1]) { { (char *) username, NULL, NULL, 0, 0} }))) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_verify(): No such username: %s\n", username);
		errno = errsv;
		return -1;
	}

	/* Extract nonce from the received session field */
	memcpy(nonce, session, CRYPT_NONCE_SIZE_XSALSA20);

	/* Shrink the shared key with a blake2s digest in order to match the encryption key size */
	if (!hash_buffer_blake2s(key, dh_shared, dh_shared_size)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_verify(): hash_buffer_blake2s(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Decrypt client password with DH shared as key */
	if (!crypt_decrypt_xsalsa20(pw_payload, &out_len, (unsigned char *) (session + CRYPT_NONCE_SIZE_XSALSA20), sizeof(pw_payload) + CRYPT_EXTRA_SIZE_XSALSA20, nonce, key)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_verify(): crypt_decrypt_xsalsa20(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Extract the plain password */
	memcpy(plain_passwd, pw_payload + 1, pw_payload[0]);
	plain_passwd[pw_payload[0]] = 0;

	/* Generate the client password hash */
	if (!kdf_pbkdf2_hash(pwhash_c, hash_buffer_sha512, HASH_DIGEST_SIZE_SHA512, HASH_BLOCK_SIZE_SHA512, (unsigned char *) plain_passwd, strlen(plain_passwd), salt, sizeof(salt), rounds, HASH_DIGEST_SIZE_SHA512) < 0) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_verify(): kdf_pbkdf2_hash(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Grant that userinfo->password doesn't exceed the expected length */
	if (decode_size_base64(strlen(userinfo->password)) > sizeof(pwhash_s)) {
		log_warn("auth_daemon_remote_user_token_verify(): pwhash_s buffer is too small to receive the decoded user password.\n");
		errno = EINVAL;
		return -1;
	}

	/* Decode the base64 encoded password hash from current configuration */
	if (!decode_buffer_base64(pwhash_s, &out_len, (unsigned char *) userinfo->password, strlen(userinfo->password))) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_verify(): decode_buffer_base64(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Check if password hashes match */
	if (memcmp(pwhash_s, pwhash_c, HASH_DIGEST_SIZE_SHA512)) {
		log_warn("auth_daemon_remote_user_token_verify(): Server and Client password hashes do not match.\n");
		errno = EINVAL;
		return -1;
	}

	/* Set uid and gid */
	*uid = userinfo->uid;
	*gid = userinfo->gid;

	/* TODO: Cleanup data by memset()ing all arrays to 0 */

	/* All good */
	return 0;
}

int auth_daemon_remote_user_token_create(
	const char *username,
	char *session,
	unsigned char *dh_shared,
	size_t dh_shared_size,
	unsigned char *nonce,
	unsigned char *token)
{
	int errsv = 0, rounds = CONFIG_USCHED_SEC_KDF_ROUNDS;
	struct usched_config_userinfo *userinfo = NULL;
	char *session_pos = session + sizeof(rund.sec.key_pub);
	unsigned char pwhash[HASH_DIGEST_SIZE_SHA512 + 3];
	unsigned char key[HASH_DIGEST_SIZE_BLAKE2S];
	unsigned char client_token[HASH_DIGEST_SIZE_BLAKE2S], client_hash[HASH_DIGEST_SIZE_BLAKE2S];
	unsigned char server_token1[HASH_DIGEST_SIZE_BLAKE2S];
	unsigned char server_token2[HASH_DIGEST_SIZE_BLAKE2S];
	unsigned char server_token3[HASH_DIGEST_SIZE_BLAKE2S];
	size_t out_len = 0;

	/* Session data contents
	 *
	 * | pubkey (512 bytes) | encrypted token (32 bytes) |
	 *
	 * Total session size: 544 bytes
	 *
	 */

	/* Get userinfo data from current configuration */
	if (!(userinfo = rund.config.users.list->search(rund.config.users.list, (struct usched_config_userinfo [1]) { { (char *) username, NULL, NULL, 0, 0 } }))) {
		log_warn("auth_daemon_remote_user_token_create(): No such username: %s\n", username);
		errno = EINVAL;
		return -1;
	}

	/* Grant that userinfo->password doesn't exceed the expected length */
	if (decode_size_base64(strlen(userinfo->password)) > sizeof(pwhash)) {
		log_warn("auth_daemon_remote_user_token_verify(): pwhash buffer is too small to receive the decoded user password.\n");
		errno = EINVAL;
		return -1;
	}

	/* Decode user password hash from base64 */
	if (!decode_buffer_base64(pwhash, &out_len, (unsigned char *) userinfo->password, strlen(userinfo->password))) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): decode_buffer_base64(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Re-hash the pwhash to achieve the decryption key for client token */
	if (!hash_buffer_blake2s(key, pwhash, sizeof(pwhash) - 3)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): hash_buffer_blake2s(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Decrypt client token */
	if (!crypt_decrypt_otp(client_token, &out_len, (unsigned char *) session_pos, HASH_DIGEST_SIZE_BLAKE2S, NULL, key)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): crypt_decrypt_otp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Compute a client hash */
	if (!kdf_pbkdf2_hash(client_hash, hash_buffer_blake2s, HASH_DIGEST_SIZE_BLAKE2S, HASH_BLOCK_SIZE_BLAKE2S, client_token, sizeof(client_token), pwhash, sizeof(pwhash) - 3, rounds, HASH_DIGEST_SIZE_BLAKE2S) < 0) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): kdf_pbkdf2_hash(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Generate a random server token */
	if (!generate_bytes_random(server_token1, HASH_DIGEST_SIZE_BLAKE2S)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): generate_bytes_random(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Encrypt server token1 with re-hashed version of pwhash */
	if (!crypt_encrypt_otp(server_token2, &out_len, server_token1, HASH_DIGEST_SIZE_BLAKE2S, NULL, key)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): crypt_encrypt_otp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Encrypt server token2 with re-hashed version of client token */
	if (!crypt_encrypt_otp(server_token3, &out_len, server_token2, HASH_DIGEST_SIZE_BLAKE2S, NULL, client_token)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): crypt_encrypt_otp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Generate a random nonce for encryption */
	if (!generate_bytes_random(nonce, CRYPT_NONCE_SIZE_XSALSA20)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): generate_bytes_random(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Shrink the shared key with a blake2s digest in order to match the encryption key size */
	if (!hash_buffer_blake2s(key, dh_shared, dh_shared_size)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): hash_buffer_blake2s(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Encrypt the session token with the resulting blake2s digest as key */
	if (!crypt_encrypt_xsalsa20((unsigned char *) (session_pos + CRYPT_NONCE_SIZE_XSALSA20), &out_len, server_token3, CRYPT_KEY_SIZE_XSALSA20, nonce, key)) {
		errsv = errno;
		log_warn("auth_daemon_remote_user_token_create(): crypt_encrypt_xsalsa20(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* Craft session field */
	memcpy(session_pos, nonce, CRYPT_NONCE_SIZE_XSALSA20);

	/* Session contents:
	 *
	 * | pubkey (512 bytes) | nonce (24 bytes) | encrypted token3 (16 + 32 bytes) |
	 *
	 * Total size of session field: 584 bytes
	 */

	/* TODO: Cleanup data by memset()ing all arrays to 0 */

	return 0;
}

