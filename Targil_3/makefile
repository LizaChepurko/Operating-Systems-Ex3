OBJS	= Graph.o
SOURCE	= Graph.cpp
HEADER	= Graph.hpp
OUT	= graph
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

Graph.o: Graph.cpp
	$(CC) $(FLAGS) Graph.cpp 


clean:
	rm -f $(OBJS) $(OUT)