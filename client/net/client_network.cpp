#include <iostream>
#include <errno.h>
#include <arpa/inet.h>

#include <network.h>

//Get message, spawn child thread and save it locally
int handle_connection(int socket_fd){
    int recv_bytes;
    char s[INET6_ADDRSTRLEN];
    char buff[MAX_PACKET_DATA_BYTES];

    if((recv_bytes=recv(socket_fd, buff, MAX_PACKET_DATA_BYTES-1, 0))==-1){
        std::cerr<<"ERROR WHILE RECEIVING MESSAGE:\n"
            "in recv(): "<<errno<<"\n";
        return -1;
    }

    buff[recv_bytes]='\0';

    std::cout<<"Received message from server: "<<buff<<"\n";

    return 0;
}
