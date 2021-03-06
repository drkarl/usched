/**
 * @file print.c
 * @brief uSched
 *        Printing interface - Client
 *
 * Date: 03-03-2015
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
#include <stdint.h>

#include "entry.h"

void print_client_result_error(void) {
	printf("An error ocurred. Check your syslog entries for more details.\n");
}

void print_client_result_empty(void) {
	printf("No results.\n");
}

void print_client_result_run(uint64_t entry_id) {
	printf("Installed Entry ID: 0x%016llX\n", (unsigned long long) entry_id);
}

void print_client_result_del(const uint64_t *entry_list, size_t count) {
	size_t i = 0;

	for (i = 0; i < count; i ++)
		printf("Deleted Entry ID: 0x%016llX\n", (unsigned long long) entry_list[i]);
}

void print_client_result_show(const struct usched_entry *entry_list, size_t count) {
	size_t i = 0;

	printf("                 id | username |   uid |   gid |     trigger |     step |      expire | cmd\n");

	for (i = 0; i < count; i ++) {
		printf(
			"%c0x%016llX | " \
			"%8s | " \
			"%5u | " \
			"%5u | " \
			"%11u | " \
			"%8u | " \
			"%11u | " \
			"%s\n",
			entry_has_flag(&entry_list[i], USCHED_ENTRY_FLAG_INVALID) ? '*' : ' ',
			(unsigned long long) entry_list[i].id,
			!entry_list[i].username[0] ? "-" : entry_list[i].username,
			(unsigned int) entry_list[i].uid,
			(unsigned int) entry_list[i].gid,
			(unsigned int) entry_list[i].trigger,
			(unsigned int) entry_list[i].step,
			(unsigned int) entry_list[i].expire,
			entry_list[i].subj);
	}
}

