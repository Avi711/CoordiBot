# Set the output directory
OUT_DIR = build

# Set the source directory
SRC_DIR = src

#JSONCPP_CFLAGS = $(shell pkg-config --cflags jsoncpp)
#JSONCPP_LDLIBS = $(shell pkg-config --libs jsoncpp)


#CPPREST_CFLAGS = -I/usr/include/cpprest

#BOOST_CFLAGS = -I/usr/include/boost
#BOOST_LDLIBS = -lboost_system -lboost_thread -lboost_chrono

CXXFLAGS = `pkg-config --cflags playerc++` -g
CFLAGS = `pkg-config --cflags playerc` -g
LDLIBS = `pkg-config --libs playerc++` -lcpprest -lssl -lcrypto
CC = g++

$(shell mkdir -p build)

# List all the source files here
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Create a list of object files by replacing the .cpp extension with .o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(SRCS))

# Default target: build the executable
all: main

# The main target depends on the object files
main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

# Rules to build the object files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the object files and the executable
clean:
	rm -rf $(OBJS) $(OUT_DIR)
