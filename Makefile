#Application names
APPNAME_CLIENT=weirdchat
APPNAME_SERVER=weirdserv

#Code directories
COMMON_DIR=common
CLIENT_DIR=client
SERVER_DIR=server

#Includes
COMMON_INCLUDE=$(COMMON_DIR)/include
CLIENT_INCLUDE=$(CLIENT_DIR)/include
SERVER_INCLUDE=$(SERVER_DIR)/include

#Object files
COMMON_OBJS=					\
	$(COMMON_DIR)/net/network.o

CLIENT_OBJS=$(COMMON_OBJS)		\
	$(CLIENT_DIR)/main.o		\
	$(CLIENT_DIR)/net/client_network.o

SERVER_OBJS=$(COMMON_OBJS)		\
	$(SERVER_DIR)/main.o		\
	$(SERVER_DIR)/net/server_network.o

#Flags
CPP_FLAGS= -std=c++11

.PHONY: all client server clean

all: client server

client: $(CLIENT_OBJS)
	g++ -o $(APPNAME_CLIENT) $(CLIENT_OBJS) -I$(COMMON_INCLUDE) -I$(CLIENT_INCLUDE) $(CPP_FLAGS)

server: $(SERVER_OBJS)
	g++ -o $(APPNAME_SERVER) $(SERVER_OBJS) -I$(COMMON_INCLUDE) -I$(SERVER_INCLUDE) $(CPP_FLAGS)

$(CLIENT_DIR)/%.o: $(CLIENT_DIR)/%.cpp
	g++ -c -o $@ $< -I$(COMMON_INCLUDE) -I$(CLIENT_INCLUDE) $(CPP_FLAGS)

$(SERVER_DIR)/%.o: $(SERVER_DIR)/%.cpp
	g++ -c -o $@ $< -I$(COMMON_INCLUDE) -I$(SERVER_INCLUDE) $(CPP_FLAGS)

$(COMMON_DIR)/%.o: $(COMMON_DIR)/%.cpp
	g++ -c -o $@ $< -I$(COMMON_INCLUDE) $(CPP_FLAGS)

clean:
	rm -rf $(APPNAME_SERVER) $(APPNAME_CLIENT) $(CLIENT_OBJS) $(SERVER_OBJS)
