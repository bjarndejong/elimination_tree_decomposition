# Directories
SRC_DIR = ../src
BUILD_DIR = ../build

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++20 -O3 -march=native -I$(SRC_DIR)

# Linker flags
LDFLAGS = 
LIBS = 

# Source and target
SRC_FILES = \
	$(SRC_DIR)/graph.cpp \
    $(SRC_DIR)/edge_mutable_graph.cpp \
    $(SRC_DIR)/test.cpp \
	$(SRC_DIR)/rootedtree.cpp $(SRC_DIR)/postprocessor.cpp $(SRC_DIR)/binaryinteger.cpp

TARGET = $(BUILD_DIR)/elimination

# Default target
all: $(TARGET)

# Build rule
$(TARGET): main.cpp $(SRC_FILES)
	$(CXX) $(CXXFLAGS) main.cpp $(SRC_FILES) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Clean
clean:
	rm -f $(TARGET)