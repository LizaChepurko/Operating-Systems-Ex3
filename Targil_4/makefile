# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -g

# Include directories
INCLUDES = -I.

# Source files
POLL_SRCS = pollserver_graph.cpp
GRAPH_SRCS = Graph.cpp graph_main.cpp

# Object files
POLL_OBJS = $(POLL_SRCS:.cpp=.o)
GRAPH_OBJS = $(GRAPH_SRCS:.cpp=.o)

# Executables
POLL_TARGET = server
GRAPH_TARGET = graph

# Default rule
all: $(POLL_TARGET) $(GRAPH_TARGET)

# Rule to link the object files and create the server executable
$(POLL_TARGET): $(POLL_OBJS) Graph.o
	$(CXX) $(CXXFLAGS) $(POLL_OBJS) Graph.o -o $(POLL_TARGET)

# Rule to link the object files and create the graph executable
$(GRAPH_TARGET): $(GRAPH_OBJS)
	$(CXX) $(CXXFLAGS) $(GRAPH_OBJS) -o $(GRAPH_TARGET)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(POLL_OBJS) $(GRAPH_OBJS) $(POLL_TARGET) $(GRAPH_TARGET)

# Phony targets
.PHONY: all clean
