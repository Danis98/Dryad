#include <iostream>

#include <main.h>
#include <network.h>

int main(){
    std::cout<<"Starting up WeirdCom Server version "<<VERSION<<"...\n";

    if(setup_socket()==-1)
        exit(1);
    std::cout<<"Successfully set up socket, listening on port "<<PORT<<"...\n";

    if(handle_message()==-1)
        exit(1);
    std::cout<<"Successfully handled message\n";

    if(close_network()==-1){
        exit(1);
    }
    std::cout<<"Succesfully closed socket\n";

    return 0;
}
