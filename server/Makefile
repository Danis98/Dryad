APP_NAME = weirdserv

INCLUDE_DIR = ./include

OBJS = main.o	\
net/network.o

FLAGS = -std=c++11

.PHONY: all clean

all: $(OBJS)
	g++ -o $(APP_NAME) $(OBJS) -I$(INCLUDE_DIR) $(FLAGS)

%.o:%.cpp
	g++ -c -o $@ $< -I$(INCLUDE_DIR) $(FLAGS)

clean:
	rm -rf *.o
