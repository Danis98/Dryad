#ifndef __NETWORK_H
#define __NETWORK_H

#define MAX_PACKET_DATA_BYTES 200
#define BACKLOG_SIZE 10
#define PORT "4321"

//Get address in IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa);
//Setup socket and start listening, returns socket file descriptor or -1
int setup_socket(const char *host);
//Close connection
int close_socket(int fd);

#endif
