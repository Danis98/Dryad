#include <iostream>

#include <main.h>
#include <client_network.h>

int main(int argc, char **argv){
    if(argc!=2){
        std::cerr<<"[USAGE] "<<argv[0]<<" <server address>\n";
        return 0;
    }
    std::cout<<"Starting Weirdcom Client version "<<VERSION<<"\n";

    int socket;
    //Setup listening socket
    if((socket=setup_socket(argv[1]))==-1)
        exit(1);
    std::cout<<"Successfully set up socket on port "<<PORT<<"...\n";

    if(handle_connection(socket)==-1)
        exit(1);
    std::cout<<"Successfully handled message\n";

    if(close_socket(socket)==-1){
        exit(1);
    }
    std::cout<<"Succesfully closed socket\n";

    return 0;
}
