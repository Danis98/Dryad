#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <network.h>
#include <server_network.h>

//Accepts a connection, reads the message, and does stuff with it
int handle_connection(int socket_fd){
    int msg_fd;
    struct sockaddr_storage client_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    //Accept a connection
    sin_size=sizeof client_addr;
    msg_fd=accept(socket_fd, (struct sockaddr*)&client_addr, &sin_size);
    //Skip if it's bad
    if(msg_fd==-1){
        std::cerr<<"PROBLEM WHILE ACCEPTING CONNECTION:\n"
            "in accept(): "<<errno<<"\n";
        return -1;
    }
    inet_ntop(client_addr.ss_family,
            get_in_addr((struct sockaddr*)&client_addr),
            s, sizeof s);
    std::cout<<"Got a connection from "<<s<<"\n";

    //Get the message in a child thread, so we don't clog everything up
    if(fork()==0){ //CHILD THREAD
        //Close it down, it ain't this thread's business
        if(close_socket(socket_fd)==-1) return -1;

        //TODO: Do some actual message handling, not just this test
        if(send(msg_fd, "SERVER WORKING\n", 15, 0)==-1){
            std::cerr<<"PROBLEM WHILE SENDING MESSAGE;\n"
                "in send(): "<<errno<<"\n";
        }
        //Do some cleanup and exit
        if(close_socket(msg_fd)==-1) return -1;
        exit(0);
    } //END OF CHILD THREAD
    //Close it down, it ain't ths thread's business
    if(close_socket(msg_fd)==-1) return -1;

    return 0;
}
