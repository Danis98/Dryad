#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <network.h>

int socket_fd;
struct addrinfo hints;
struct sockaddr_storage *client_addr;
struct sigaction sa;

//Destroy zombie processes
void sigchld_handler(int s){
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

//IPv4 or IPv6?
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int setup_socket(){
    struct addrinfo *servinfo, *p;
    int rv;

    //Set basic info
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_UNSPEC; //Don't care if IPv4 or IPv6
    hints.ai_socktype=SOCK_STREAM; //TCP is the way, baby!
    hints.ai_flags=AI_PASSIVE; //Use my own IP

    //Get info about our current network
    if((rv=getaddrinfo(NULL, PORT, &hints, &servinfo))!=0){
        std::cerr<<"ERROR WHILE SETTING UP NETWORKING:\n"
            "in getaddrinfo(): "<<gai_strerror(rv)<<"\n";
        return -1;
    }

    //Bind stuff!
    p=servinfo;
    while(p!=NULL){
        /* One socket to rule them all */
        //Create socket
        if((socket_fd=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1){
            std::cerr<<"Failed to create socket... Retrying...\n";
            p=p->ai_next;
            continue;
        }
        /* Some options to find them */
        //Set some options
        int yes=1;
        if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)==-1){
            std::cerr<<"ERROR WHILE SETTING SOCKET OPTIONS:\n"
                "in setstockopt(): "<<errno<<"\n";
                return -1;
        }
        /* And in the darkness bind them */
        //Bind it!
        if(bind(socket_fd, p->ai_addr, p->ai_addrlen)==-1){
            if(close_socket(socket_fd)==-1) return -1;
            std::cerr<<"Failed to bind socket... Retrying...\n";
            p=p->ai_next;
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if(p==NULL){
        std::cerr<<"FAILED TO BIND SOCKET\n";
        return -1;
    }

    //Start listening
    if(listen(socket_fd, BACKLOG_SIZE)==-1){
        std::cerr<<"FAILED TO LISTEN\n";
        return -1;
    }

    sa.sa_handler=sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL)==-1){
        std::cerr<<"FAILED TO SET UP SIGACTION\n";
        return -1;
    }

    return 0;
}

//Wrapper for the close() syscall, outputs an error if things get weird
int close_socket(int fd){
    if(close(fd)==-1){
        std::cerr<<"PROBLEM CLOSING SOCKET "<<fd<<":\n"
            "in close(): "<<errno<<"\n";
        return -1;
    }
    return 0;
}

//Shutdown time!
int close_network(){
    return close_socket(socket_fd);
}

//Accepts a connection, reads the message, and does stuff with it
int handle_message(){
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
