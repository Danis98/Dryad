#include <iostream>

#include <main.h>
#include <server_network.h>

int main(){
    std::cout<<"Starting up Dryad Server version "<<VERSION<<"\n";

    int socket;
    //Setup listening socket
    if((socket=setup_socket(NULL))==-1)
        exit(1);
    std::cout<<"Successfully set up socket, listening on port "<<PORT<<"...\n";

    for(int i=0;i<10;i++){
        if(handle_connection(socket)==-1)
            exit(1);
        std::cout<<"Successfully handled message\n";
    }

    if(close_socket(socket)==-1){
        exit(1);
    }
    std::cout<<"Succesfully closed socket\n";

    return 0;
}
