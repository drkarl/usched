/**
 * @file users.c
 * @brief uSched
 *        Users configuration interface
 *
 * Date: 06-08-2014
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

#include <sys/types.h>

#include "config.h"
#include "runtime.h"

int users_admin_config_add(const char *username, uid_t uid, gid_t gid, const char *password) {
	errno = ENOSYS;
	return -1;
}

int users_admin_config_delete(const char *username) {
	errno = ENOSYS;
	return -1;
}

int users_admin_config_change(const char *username, uid_t uid, gid_t gid, const char *password) {
	errno = ENOSYS;
	return -1;
}

int users_admin_config_show(void) {
	errno = ENOSYS;
	return -1;
}

