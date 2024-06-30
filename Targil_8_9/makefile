# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -fPIC

# Directories
SRC_DIR = .
OBJ_DIR = ./obj
LIB_DIR = ./lib

# Libraries
LIBS = -lpthread

# Targets
TARGET_SERVER = server
TARGET_LIB = $(LIB_DIR)/libactor.so

# Source files
SRC_SERVER = $(SRC_DIR)/server.cpp $(SRC_DIR)/libactor.cpp $(SRC_DIR)/Graph.cpp
SRC_LIB = $(SRC_DIR)/libactor.cpp

# Object files
OBJ_SERVER = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SERVER))
OBJ_ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_))
OBJ_LIB = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_LIB))

# Include paths
INCLUDES = -I$(SRC_DIR)

# Create object and library directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(LIB_DIR))

# Default target
all: $(TARGET_LIB) $(TARGET_SERVER)

# Rule to create shared library
$(TARGET_LIB): $(OBJ_LIB)
	$(CXX) -shared -o $@ $^

# Rule to create server executable
$(TARGET_SERVER): $(OBJ_SERVER) $(TARGET_LIB)
	$(CXX) -o $@ $^ $(LIBS)

# Rule to create  executable
$(): $(OBJ_)
	$(CXX) -o $@ $^ $(LIBS)

# Pattern rule to compile object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean files generated during compilation
clean:
	rm -rf $(OBJ_DIR) $(TARGET_SERVER) $(TARGET_LIB)

.PHONY: all clean
