OBJS	= KosarajuSharir.o
SOURCE	= KosarajuSharir.cpp
HEADER	= 
OUT	= kosaraju
FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= g++

all:	kosaraju

kosaraju: $(OBJS)
	$(CC) -g -pg $(OBJS) -o $(OUT) $(LFLAGS)

KosarajuSharir.o: KosarajuSharir.cpp
	$(CC) $(FLAGS) KosarajuSharir.cpp 

clean:
	rm -f $(OBJS) $(OUT)