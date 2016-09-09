#ifndef __SERVER_NETWORK_H
#define __SERVER_NETWORK_H

#include <network.h>

//Get message, spawn child thread and relay it to the right person
int handle_connection(int socket_fd);

#endif
