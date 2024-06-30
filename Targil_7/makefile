# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -g

# Include directories
INCLUDES = -I.

# Source files
SRCS = server.cpp
GRAPH_SRCS = Graph.cpp graph_main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
GRAPH_OBJS = $(GRAPH_SRCS:.cpp=.o)

# Executables
TARGET = server
GRAPH_TARGET = graph

# Default rule
all: $(TARGET) $(GRAPH_TARGET)

# Rule to link the object files and create the server executable
$(TARGET): $(OBJS) Graph.o
	$(CXX) $(CXXFLAGS) $(OBJS) Graph.o -o $(TARGET)

# Rule to link the object files and create the graph executable
$(GRAPH_TARGET): $(GRAPH_OBJS)
	$(CXX) $(CXXFLAGS) $(GRAPH_OBJS) -o $(GRAPH_TARGET)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(GRAPH_OBJS) $(TARGET) $(GRAPH_TARGET)

# Phony targets
.PHONY: all clean
