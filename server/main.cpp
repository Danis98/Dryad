#include <iostream>

#include <main.h>
#include <network.h>

int main(){
    std::cout<<"Starting up WeirdCom Server version "<<VERSION<<"...\n";

    if(setup_socket()==-1)
        exit(1);
    std::cout<<"Successfully set up socket, listening on port "<<PORT<<"...\n";

    if(close_socket()==-1){
        exit(1);
    }
    std::cout<<"Succesfully closed socket\n";

    return 0;
}
