#ifndef __NETWORK_H
#define __NETWORK_H

#define PORT "4321"
#define BACKLOG_SIZE 10

//Setup socket and start listening
int setup_socket();
//Close connection
int close_socket(int fd);
//Close everything (just a wrapper)
int close_network();
//Get message, spawn child thread and relay it to the right person
int handle_message();

#endif
