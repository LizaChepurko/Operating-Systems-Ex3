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
TARGET_LIB = $(LIB_DIR)/libreactor.so

# Source files
SRC_SERVER = $(SRC_DIR)/server_reactor.cpp $(SRC_DIR)/Reactor.cpp $(SRC_DIR)/Graph.cpp
SRC_LIB = $(SRC_DIR)/Reactor.cpp

# Object files
OBJ_SERVER = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SERVER))
OBJ_LIB = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_LIB))

# Include paths
INCLUDES = -I$(SRC_DIR)

# Create object and library directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(LIB_DIR))

# Default target
all: $(TARGET_LIB) $(TARGET_SERVER) $(TARGET_CLIENT)

# Rule to create shared library
$(TARGET_LIB): $(OBJ_LIB)
	$(CXX) -shared -o $@ $^

# Rule to create server executable
$(TARGET_SERVER): $(OBJ_SERVER)
	$(CXX) -o $@ $(filter-out $(OBJ_DIR)/Reactor.o, $(OBJ_SERVER)) $(LIBS) -L$(LIB_DIR) -lreactor -Wl,-rpath=$(LIB_DIR)

# Rule to create client executable
$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CXX) -o $@ $^ $(LIBS)

# Pattern rule to compile object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean files generated during compilation
clean:
	rm -rf $(OBJ_DIR) $(TARGET_SERVER) $(TARGET_CLIENT) $(TARGET_LIB)

.PHONY: all clean
