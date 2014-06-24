/**
 * @file thread.h
 * @brief uSched
 *        Thread handlers interface header
 *
 * Date: 24-06-2014
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



#ifndef USCHED_THREAD_H
#define USCHED_THREAD_H

/* Prototypes */
int thread_daemon_mutexes_init(void);
void thread_daemon_mutexes_destroy(void);
int thread_exec_behaviour_init(void);
void thread_exec_behaviour_destroy(void);

#endif

