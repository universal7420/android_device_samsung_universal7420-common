/*
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Configuration: MULTITHREAD
 * Define for parallel execution 
 *
 * Valid values:
 *   1 - only one context (default).
 *   N>1 - will execute N copies in parallel.
 *
 * Note: 
 *   If this flag is defined to more then 1, an implementation for launching parallel contexts must be defined.
 *
 *   Two sample implementations are provided. Use <USE_PTHREAD> or <USE_FORK> to enable them.
 *
 *   It is valid to have a different implementation of <core_start_parallel> and <core_end_parallel> in <core_portme.c>,
 *   to fit a particular architecture. 
*/
#define MULTITHREAD 8

/*
 * Configuration: USE_PTHREAD
 * Sample implementation for launching parallel contexts 
 * This implementation uses pthread_thread_create and pthread_join.
 *
 * Valid values:
 *   0 - Do not use pthreads API.
 *   1 - Use pthreads API
 *
 * Note: 
 *   This flag only matters if MULTITHREAD has been defined to a value greater then 1.
*/
#define USE_PTHREAD 0

/*
 * Configuration: USE_FORK
 * Sample implementation for launching parallel contexts 
 * This implementation uses fork, waitpid, shmget,shmat and shmdt.
 *
 * Valid values:
 *   0 - Do not use fork API.
 *   1 - Use fork API
 *
 * Note: 
 *   This flag only matters if MULTITHREAD has been defined to a value greater then 1.
*/
#define USE_FORK 1

/*
 * Configuration: USE_SOCKET
 * Sample implementation for launching parallel contexts 
 * This implementation uses fork, socket, sendto and recvfrom
 *
 * Valid values:
 *   0 - Do not use fork and sockets API.
 *   1 - Use fork and sockets API
 *
 * Note: 
 *   This flag only matters if MULTITHREAD has been defined to a value greater then 1.
*/
#define USE_SOCKET 0
