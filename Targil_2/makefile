FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= g++

all:	kosaraju_deque kosaraju_adjacency kosaraju_list

kosaraju_deque: KosarajuSharirDeque.o
	$(CC) -g -pg KosarajuSharirDeque.o -o kosaraju_deque $(LFLAGS)

KosarajuSharirDeque.o: KosarajuSharirDeque.cpp
	$(CC) $(FLAGS) KosarajuSharirDeque.cpp 

kosaraju_adjacency: KosarajuSharirAdjacency.o
	$(CC) -g -pg KosarajuSharirAdjacency.o -o kosaraju_adjacency $(LFLAGS)

KosarajuSharirAdjacency.o: KosarajuSharirAdjacency.cpp
	$(CC) $(FLAGS) KosarajuSharirAdjacency.cpp

kosaraju_list: KosarajuSharirList.o
	$(CC) -g -pg KosarajuSharirList.o -o kosaraju_list $(LFLAGS)

KosarajuSharirList.o: KosarajuSharirList.cpp
	$(CC) $(FLAGS) KosarajuSharirList.cpp

clean:
	rm -f KosarajuSharirDeque.o KosarajuSharirAdjacency.o KosarajuSharirList.o kosaraju_deque kosaraju_adjacency kosaraju_list