#ifndef __CLIENT_NETWORK_H
#define __CLIENT_NETWORK_H

#include <network.h>

//Get message, spawn child thread and save it locally
int handle_connection(int socket_fd);
//TODO: Send message
//int send_message();

#endif
