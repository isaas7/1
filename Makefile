# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I$(HTTP_DIR)/include -I$(APP_DIR)/include -I$(LOG_DIR)/include

# Libraries
LIBS = -lpthread -lboost_system -lboost_filesystem -lboost_thread -lssl -lcrypto -ldl -lm -lSQLiteCpp -lsqlite3

# Directories
APP_DIR = app
HTTP_DIR = http
LOG_DIR = log
SRC_DIR = $(HTTP_DIR)/src $(APP_DIR)/src $(LOG_DIR)/src $(OLLAMA_DIR)/src
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/main

# Source files
MAIN_SRC_FILE = main.cpp
HTTP_SRC_FILES = $(wildcard $(HTTP_DIR)/src/*.cpp)
APP_SRC_FILES = $(wildcard $(APP_DIR)/src/*.cpp)
LOG_SRC_FILES = $(wildcard $(LOG_DIR)/src/*.cpp)
SRC_FILES = $(MAIN_SRC_FILE) $(HTTP_SRC_FILES) $(APP_SRC_FILES) $(LOG_SRC_FILES)

# Object files
MAIN_OBJ_FILE = $(OBJ_DIR)/main.o
HTTP_OBJ_FILES = $(patsubst $(HTTP_DIR)/src/%.cpp,$(OBJ_DIR)/http_%.o,$(HTTP_SRC_FILES))
APP_OBJ_FILES = $(patsubst $(APP_DIR)/src/%.cpp,$(OBJ_DIR)/app_%.o,$(APP_SRC_FILES))
LOG_OBJ_FILES = $(patsubst $(LOG_DIR)/src/%.cpp,$(OBJ_DIR)/log_%.o,$(LOG_SRC_FILES))
OBJ_FILES = $(MAIN_OBJ_FILE) $(HTTP_OBJ_FILES) $(APP_OBJ_FILES) $(LOG_OBJ_FILES)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile main.cpp to object file
$(MAIN_OBJ_FILE): $(MAIN_SRC_FILE)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files to object files
$(OBJ_DIR)/http_%.o: $(HTTP_DIR)/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/app_%.o: $(APP_DIR)/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/log_%.o: $(LOG_DIR)/src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the application
run: $(TARGET)
	./$(TARGET) 0.0.0.0 8080 www 2

.PHONY: all clean run

