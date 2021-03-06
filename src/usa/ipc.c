/**
 * @file ipc.c
 * @brief uSched
 *        Inter-Process Communication interface - Admin
 *
 * Date: 21-03-2015
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

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "config.h"
#include "runtime.h"
#include "log.h"
#include "ipc.h"
#if CONFIG_USE_IPC_PMQ == 1
 #include "pmq.h"
#elif CONFIG_USE_IPC_UNIX == 1
 #include <fsop/path.h>
 #include <panet/panet.h>
#endif

int ipc_admin_create(void) {
#if CONFIG_USE_IPC_PMQ == 1
	int errsv = 0;

	if (pmq_admin_create() < 0) {
		errsv = errno;
		log_warn("ipc_admin_create(): pmq_admin_create(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
#elif CONFIG_USE_IPC_UNIX == 1
	int errsv = 0;
	int ipcd = -1;

	if ((ipcd = panet_server_unix(runa.config.core.ipc_name, PANET_PROTO_UNIX_STREAM, runa.config.core.ipc_msgmax)) < 0) {
		errsv = errno;
		log_warn("ipc_exec_init(): panet_server_unix(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	panet_safe_close(ipcd);

	if (chown(runa.config.core.ipc_name, 0, 0) < 0) {
		errsv = errno;
		log_warn("ipc_exec_init(): chown(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	if (chmod(runa.config.core.ipc_name, 0700) < 0) {
		errsv = errno;
		log_warn("ipc_exec_init(): chmod(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
#elif CONFIG_USE_IPC_INET == 1
	return 0;
#else
	errno = ENOSYS;
	return -1;
#endif
}

int ipc_admin_delete(void) {
#if CONFIG_USE_IPC_PMQ == 1
	int errsv = 0;

	if (pmq_admin_delete() < 0) {
		errsv = errno;
		log_warn("ipc_admin_delete(): pmq_admin_delete(): %s\n", strerror(errno));
		errno = errsv;
		return -1;
	}

	return 0;
#elif CONFIG_USE_IPC_UNIX == 1
	int errsv = 0;

	if (fsop_path_exists(runa.config.core.ipc_name)) {
		if (unlink(runa.config.core.ipc_name) < 0) {
			errsv = errno;
			log_warn("ipc_admin_delete(): unlink(\"%s\"): %s\n", runa.config.core.ipc_name, strerror(errno));
			errno = errsv;
			return -1;
		}
	}

	return 0;
#elif CONFIG_USE_IPC_INET == 1
	return 0;
#else
	errno = ENOSYS;
	return -1;
#endif
}

