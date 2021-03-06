/**
 * @file network.c
 * @brief uSched
 *        Network configuration and administration interface
 *
 * Date: 20-02-2015
 * 
 * Copyright 2014-2015 Pedro A. Hortas (pah@ucodev.org)
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

#include <fsop/file.h>

#include "config.h"
#include "network.h"
#include "file.h"
#include "log.h"
#include "mm.h"
#include "usched.h"
#include "print.h"

int network_admin_commit(void) {
	int errsv = 0;

	/* bind.addr */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_commit(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* bind.port */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_PORT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_PORT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_commit(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* conn.limit */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_commit(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* conn.timeout */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_commit(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* sock.name */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_commit(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* All good */
	return 0;
}

int network_admin_rollback(void) {
	int errsv = 0;

	/* bind.addr */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_rollback(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* bind.port */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_PORT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_PORT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_rollback(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* conn.limit */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_rollback(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* conn.timeout */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_rollback(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* sock.name */
	if (fsop_cp(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, 128) < 0) {
		errsv = errno;
		log_crit("network_admin_rollback(): fsop_cp(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	/* All good */
	return 0;
}

int network_admin_show(void) {
	int errsv = 0;

	if (network_admin_bind_addr_show() < 0) {
		errsv = errno;
		log_crit("network_admin_show(): network_admin_bind_addr_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_bind_port_show() < 0) {
		errsv = errno;
		log_crit("network_admin_show(): network_admin_bind_port_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_conn_limit_show() < 0) {
		errsv = errno;
		log_crit("network_admin_show(): network_admin_conn_limit_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_conn_timeout_show() < 0) {
		errsv = errno;
		log_crit("network_admin_show(): network_admin_conn_timeout_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_sock_name_show() < 0) {
		errsv = errno;
		log_crit("network_admin_show(): network_admin_sock_name_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int network_admin_bind_addr_show(void) {
	int errsv = 0;
	char *value = NULL, *value_tmp = NULL, *value_print = NULL;

	if (!(value_tmp = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_ADDR))) {
		errsv = errno;
		log_crit("network_admin_bind_addr_show(): file_read_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (!(value = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_ADDR))) {
		errsv = errno;
		log_crit("network_admin_bind_addr_show(): file_read_line_single(): %s\n", strerror(errno));
		mm_free(value_tmp);
		errno = errsv;
		return -1;
	}

	/* Check which value to print */
	if (!strcmp(value, value_tmp)) {
		if (!(value_print = mm_alloc(strlen(value) + 1))) {
			errsv = errno;
			log_crit("network_admin_bind_addr_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		strcpy(value_print, value);
	} else {
		if (!(value_print = mm_alloc(strlen(value_tmp) + 2))) {
			errsv = errno;
			log_crit("network_admin_bind_addr_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		/* Show the temporary value with a trailing '*' */
		strcpy(value_print, value_tmp);
		strcat(value_print, "*");
	}

	/* Print the output */
	print_admin_category_var_value(USCHED_CATEGORY_NETWORK_STR, CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, value_print);

	/* Free memory */
	mm_free(value);
	mm_free(value_tmp);
	mm_free(value_print);

	/* All good */
	return 0;
}

int network_admin_bind_addr_change(const char *bind_addr) {
	int errsv = 0;

	if (file_write_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_ADDR, bind_addr) < 0) {
		errsv = errno;
		log_crit("network_admin_bind_addr_change(): file_write_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_bind_addr_show() < 0) {
		errsv = errno;
		log_crit("network_admin_bind_addr_change(): network_admin_bind_addr_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int network_admin_bind_port_show(void) {
	int errsv = 0;
	char *value = NULL, *value_tmp = NULL, *value_print = NULL;

	if (!(value_tmp = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_PORT))) {
		errsv = errno;
		log_crit("network_admin_bind_port_show(): file_read_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (!(value = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_BIND_PORT))) {
		errsv = errno;
		log_crit("network_admin_bind_port_show(): file_read_line_single(): %s\n", strerror(errno));
		mm_free(value_tmp);
		errno = errsv;
		return -1;
	}

	/* Check which value to print */
	if (!strcmp(value, value_tmp)) {
		if (!(value_print = mm_alloc(strlen(value) + 1))) {
			errsv = errno;
			log_crit("network_admin_bind_port_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		strcpy(value_print, value);
	} else {
		if (!(value_print = mm_alloc(strlen(value_tmp) + 2))) {
			errsv = errno;
			log_crit("network_admin_bind_port_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		/* Show the temporary value with a trailing '*' */
		strcpy(value_print, value_tmp);
		strcat(value_print, "*");
	}

	/* Print the output */
	print_admin_category_var_value(USCHED_CATEGORY_NETWORK_STR, CONFIG_USCHED_FILE_NETWORK_BIND_PORT, value_print);

	/* Free memory */
	mm_free(value);
	mm_free(value_tmp);
	mm_free(value_print);

	/* All good */
	return 0;
}

int network_admin_bind_port_change(const char *bind_port) {
	int errsv = 0;

	if (file_write_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_BIND_PORT, bind_port) < 0) {
		errsv = errno;
		log_crit("network_admin_bind_port_change(): file_write_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_bind_port_show() < 0) {
		errsv = errno;
		log_crit("network_admin_bind_port_change(): network_admin_bind_port_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int network_admin_conn_limit_show(void) {
	int errsv = 0;
	char *value = NULL, *value_tmp = NULL, *value_print = NULL;

	if (!(value_tmp = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT))) {
		errsv = errno;
		log_crit("network_admin_conn_limit_show(): file_read_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (!(value = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT))) {
		errsv = errno;
		log_crit("network_admin_conn_limit_show(): file_read_line_single(): %s\n", strerror(errno));
		mm_free(value_tmp);
		errno = errsv;
		return -1;
	}

	/* Check which value to print */
	if (!strcmp(value, value_tmp)) {
		if (!(value_print = mm_alloc(strlen(value) + 1))) {
			errsv = errno;
			log_crit("network_admin_conn_limit_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		strcpy(value_print, value);
	} else {
		if (!(value_print = mm_alloc(strlen(value_tmp) + 2))) {
			errsv = errno;
			log_crit("network_admin_conn_limit_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		/* Show the temporary value with a trailing '*' */
		strcpy(value_print, value_tmp);
		strcat(value_print, "*");
	}

	/* Print the output */
	print_admin_category_var_value(USCHED_CATEGORY_NETWORK_STR, CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, value_print);

	/* Free memory */
	mm_free(value);
	mm_free(value_tmp);
	mm_free(value_print);

	/* All good */
	return 0;
}

int network_admin_conn_limit_change(const char *conn_limit) {
	int errsv = 0;

	if (file_write_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_LIMIT, conn_limit) < 0) {
		errsv = errno;
		log_crit("network_admin_conn_limit_change(): file_write_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_conn_limit_show() < 0) {
		errsv = errno;
		log_crit("network_admin_conn_limit_change(): network_admin_conn_limit_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int network_admin_conn_timeout_show(void) {
	int errsv = 0;
	char *value = NULL, *value_tmp = NULL, *value_print = NULL;

	if (!(value_tmp = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT))) {
		errsv = errno;
		log_crit("network_admin_conn_timeout_show(): file_read_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (!(value = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT))) {
		errsv = errno;
		log_crit("network_admin_conn_timeout_show(): file_read_line_single(): %s\n", strerror(errno));
		mm_free(value_tmp);
		errno = errsv;
		return -1;
	}

	/* Check which value to print */
	if (!strcmp(value, value_tmp)) {
		if (!(value_print = mm_alloc(strlen(value) + 1))) {
			errsv = errno;
			log_crit("network_admin_conn_timeout_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		strcpy(value_print, value);
	} else {
		if (!(value_print = mm_alloc(strlen(value_tmp) + 2))) {
			errsv = errno;
			log_crit("network_admin_conn_timeout_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		/* Show the temporary value with a trailing '*' */
		strcpy(value_print, value_tmp);
		strcat(value_print, "*");
	}

	/* Print the output */
	print_admin_category_var_value(USCHED_CATEGORY_NETWORK_STR, CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, value_print);

	/* Free memory */
	mm_free(value);
	mm_free(value_tmp);
	mm_free(value_print);

	/* All good */
	return 0;
}

int network_admin_conn_timeout_change(const char *conn_timeout) {
	int errsv = 0;

	if (file_write_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_CONN_TIMEOUT, conn_timeout) < 0) {
		errsv = errno;
		log_crit("network_admin_conn_timeout_change(): file_write_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_conn_timeout_show() < 0) {
		errsv = errno;
		log_crit("network_admin_conn_timeout_change(): network_admin_conn_timeout_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

int network_admin_sock_name_show(void) {
	int errsv = 0;
	char *value = NULL, *value_tmp = NULL, *value_print = NULL;

	if (!(value_tmp = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_SOCK_NAME))) {
		errsv = errno;
		log_crit("network_admin_sock_name_show(): file_read_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (!(value = file_read_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/" CONFIG_USCHED_FILE_NETWORK_SOCK_NAME))) {
		errsv = errno;
		log_crit("network_admin_sock_name_show(): file_read_line_single(): %s\n", strerror(errno));
		mm_free(value_tmp);
		errno = errsv;
		return -1;
	}

	/* Check which value to print */
	if (!strcmp(value, value_tmp)) {
		if (!(value_print = mm_alloc(strlen(value) + 1))) {
			errsv = errno;
			log_crit("network_admin_sock_name_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		strcpy(value_print, value);
	} else {
		if (!(value_print = mm_alloc(strlen(value_tmp) + 2))) {
			errsv = errno;
			log_crit("network_admin_sock_name_show(): mm_alloc(): %s\n", strerror(errno));
			mm_free(value);
			mm_free(value_tmp);
			errno = errsv;
			return -1;
		}

		/* Show the temporary value with a trailing '*' */
		strcpy(value_print, value_tmp);
		strcat(value_print, "*");
	}

	/* Print the output */
	print_admin_category_var_value(USCHED_CATEGORY_NETWORK_STR, CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, value_print);

	/* Free memory */
	mm_free(value);
	mm_free(value_tmp);
	mm_free(value_print);

	/* All good */
	return 0;
}

int network_admin_sock_name_change(const char *sock_name) {
	int errsv = 0;

	if (file_write_line_single(CONFIG_USCHED_DIR_BASE "/" CONFIG_USCHED_DIR_NETWORK "/." CONFIG_USCHED_FILE_NETWORK_SOCK_NAME, sock_name) < 0) {
		errsv = errno;
		log_crit("network_admin_sock_name_change(): file_write_line_single(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (network_admin_sock_name_show() < 0) {
		errsv = errno;
		log_crit("network_admin_sock_name_change(): network_admin_sock_name_show(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
}

