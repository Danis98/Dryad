#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>

#include <network.h>

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

int setup_socket(const char *host){
    int socket_fd;
    struct addrinfo hints;
    struct sockaddr_storage *client_addr;
    struct sigaction sa;
    struct addrinfo *servinfo, *p;
    int rv;

    //Set basic info
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_UNSPEC; //Don't care if IPv4 or IPv6
    hints.ai_socktype=SOCK_STREAM; //TCP is the way, baby!
    if(host==NULL)
        hints.ai_flags=AI_PASSIVE; //Use my own IP if I'm the server

    //Get info about our current network
    if((rv=getaddrinfo(host, PORT, &hints, &servinfo))!=0){
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
        //If server
        if(host==NULL){
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
        }
        //If client
        else{
            if(connect(socket_fd, p->ai_addr, p->ai_addrlen)==-1){
                if(close_socket(socket_fd)==-1) return -1;
                std::cerr<<"Failed to connect... Retrying...\n";
                p=p->ai_next;
                continue;
            }
        }
        break;
    }

    freeaddrinfo(servinfo);

    if(p==NULL){
        if(host==NULL) //Server
            std::cerr<<"FAILED TO BIND SOCKET\n";
        else //Client
            std::cerr<<"FAILED TO CONNECT\n";
        return -1;
    }

    if(host==NULL){ //Only do if server
        //Start listening
        if(listen(socket_fd, BACKLOG_SIZE)==-1){
            std::cerr<<"FAILED TO LISTEN\n";
            return -1;
        }
    }
    else{ //Only do if client
        char s[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof s);
        std::cout<<"Connecting to "<<s<<"\n";
    }

    sa.sa_handler=sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL)==-1){
        std::cerr<<"FAILED TO SET UP SIGACTION\n";
        return -1;
    }

    return socket_fd;
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
