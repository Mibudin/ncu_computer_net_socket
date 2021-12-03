# For NCU Operating System Homework
# 	For programs uploaded onto Online Judge.
# 
# 	Specified Language:
# 		C++
# 	Specified Compiler:
# 		G++ 5.4
# 	Specified Compiling Command:
# 		"/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14 {src_path} -lm -o {exe_path}"


# Settings

# Set the target for this makefile
INDEX	:= 4
TARGET	:= test

# Set the compiling settings
CC		:= g++
C_FLAGS	:= -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14

# Set the file structure
BIN_DIR	:= .
SRC_DIR	:= .
INC_DIR	:= .
LIB_DIR	:= .

# Set the libraries
LIBS	:= -lm

# Problrms about names of files
ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(TARGET).exe
else
EXECUTABLE	:= $(TARGET)
endif


# Commands

# Compile all the specified files (if needed)
all: $(BIN_DIR)/$(EXECUTABLE)

# Clean all the executable files
clean:
	$(RM) $(BIN_DIR)/$(EXECUTABLE)

# Compile all the specified files (if needed), and excute them.
run: all
	$(BIN_DIR)/$(EXECUTABLE)

# Compile the specified file
$(BIN_DIR)/$(EXECUTABLE): $(SRC_DIR)/*.cpp
	$(CC) $(C_FLAGS) $^ $(LIBS) -o $@
