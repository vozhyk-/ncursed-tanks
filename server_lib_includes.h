#ifndef SERVER_LIB_INCLUDES_H
#define SERVER_LIB_INCLUDES_H

#define _POSIX_C_SOURCE 200809L

#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "dyn_arr.h"
#include "debug.h"
#include "map_gen.h"
#include "server_updates_queue.h"

#define DEBUG 0 /* DEBUG: set 5 for ERROR, set 3 for INFO, set 0 for DEBUG
                   VALUES higher than 5 will result in silent execution*/

#endif /* SERVER_LIB_INCLUDES_H */