/**
 * @file lib.c
 * @brief uSched
 *        uSched Client Library interface
 *
 * Date: 25-06-2014
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

#include <errno.h>

#include "runtime.h"
#include "conn.h"
#include "usage.h"
#include "op.h"
#include "pool.h"
#include "parse.h"

static int _init(void) {
	if (runtime_client_lib_init() < 0)
		return -1;

	return 0;
}

static int _do(char *req) {
	/* Clear data from previous request, if any */
	pool_client_destroy();

	if (runc.req)
		parse_req_destroy(runc.req);

	if (runc.usage_err_offending) {
		mm_free(runc.usage_err_offending);
		runc.usage_err_offending = NULL;
		runc.usage_err = 0;
	}

	/* Parse and process data
	 *
	 * TODO: Set a unique runtime flag for each type of error in order to be returned by usched_error().
	 *
	 */
	runc.req_str = req;

	if (!(runc.req = parse_instruction(runc.req_str)))
		return -1;

	if (pool_client_init() < 0)
		return -1;

	if (op_client_process() < 0)
		return -1;

	if (conn_client_process() < 0)
		return -1;

	return 0;
}

static void _destroy(void) {
	runtime_client_lib_destroy();
}

/* Library Interface */
int usched_init(void) {
	return _init();
}

int usched_request(char *req) {
	return _do(req);
}

int usched_result_get_run(uint64_t **entry_list, size_t *nmemb) {
	/* TODO: Store the IDs of the entries in the 'id' array (NULL terminated) */
	errno = ENOSYS;

	return -1;
}

int usched_result_get_stop(uint64_t **entry_list, size_t *nmemb) {
	/* TODO: Store the IDs of the entries in the 'id' array (NULL terminated) */
	errno = ENOSYS;

	return -1;
}

int usched_result_get_show(struct usched_entry **entry_list, size_t *nmemb) {
	/* TODO: Store the IDs of the entries in the 'id' array (NULL terminated) */
	errno = ENOSYS;

	return -1;
}

usched_usage_client_err_t usched_usage_error(void) {
	return runc.usage_err;
}

char *usched_usage_error_str(usched_usage_client_err_t error) {
	switch (error) {
		case USCHED_USAGE_CLIENT_ERR_INVALID_OP: return USCHED_USAGE_CLIENT_ERR_INVALID_OP_STR;
		case USCHED_USAGE_CLIENT_ERR_INVALID_PREP: return USCHED_USAGE_CLIENT_ERR_INVALID_PREP_STR;
		case USCHED_USAGE_CLIENT_ERR_INVALID_ADVERB: return USCHED_USAGE_CLIENT_ERR_INVALID_ADVERB_STR;
		case USCHED_USAGE_CLIENT_ERR_INVALID_CONJ: return USCHED_USAGE_CLIENT_ERR_INVALID_CONJ_STR;
		case USCHED_USAGE_CLIENT_ERR_INVALID_ARG: return USCHED_USAGE_CLIENT_ERR_INVALID_ARG_STR;
		case USCHED_USAGE_CLIENT_ERR_UNEXPECT_PREP: return USCHED_USAGE_CLIENT_ERR_UNEXPECT_PREP_STR;
		case USCHED_USAGE_CLIENT_ERR_UNEXPECT_CONJ: return USCHED_USAGE_CLIENT_ERR_UNEXPECT_CONJ_STR;
		case USCHED_USAGE_CLIENT_ERR_INSUFF_ARGS: return USCHED_USAGE_CLIENT_ERR_INSUFF_ARGS_STR;
		case USCHED_USAGE_CLIENT_ERR_TOOMANY_ARGS: return USCHED_USAGE_CLIENT_ERR_TOOMANY_ARGS_STR;
	}

	return "Success";
}

void usched_destroy(void) {
	_destroy();
}

