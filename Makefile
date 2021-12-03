# For NCU Operating System Homework
# 	For programs uploaded onto Online Judge. (Or not XD)
# 
# 	Specified Language:
# 		C++
# 	Specified Compiler:
# 		G++ 5.4
# 	Specified Compiling Command:
# 		"/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14 {src_path} -lm -o {exe_path}"


# Settings

# Set the target environment
# TARGET_ENV	:= OJ
TARGET_ENV	:= 

# Set the target for this makefile
# INDEX	:= 4
TARGET	:= gol

# Set the compiling settings
CC		:= g++
C_FLAGS	:= -O2 -w -fmax-errors=3 -std=c++17 -pthread
ifeq ($(TARGET_ENV), OJ)
C_FLAGS += -DONLINE_JUDGE
endif

# Set the file structure
BIN_DIR	:= ./bin
SRC_DIR	:= ./src
OBJ_DIR	:= ./obj
INC_DIR	:= ./inc
LIB_DIR	:= ./lib

# Set the needed object files
OBJS	:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

# Set the libraries
LIBS	:= 
ifeq ($(TARGET_ENV), OJ)
LIBS	+= -lm
endif

# Problrms about names of files
ifeq ($(OS), Windows_NT)
EXECUTABLE	:= $(TARGET).exe
RM	:= del
else
EXECUTABLE	:= $(TARGET)
endif


# Commands

# Compile all the specified files (if needed)
all: $(BIN_DIR)/$(EXECUTABLE)

# Clean all the executable files
cleanbin:
	$(RM) $(BIN_DIR)/$(EXECUTABLE)

# Clean all the object files
cleanobj:
	$(RM) $(OBJS)

# Clean all the gemerated files
clean: cleanbin cleanobj

# Compile all the specified files (if needed), and excute them.
run: all
	$(BIN_DIR)/$(EXECUTABLE)

# Compile all the specific files to the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(C_FLAGS) -I$(INC_DIR) -L$(LIB_DIR) $(LIBS) -c $< -o $@

# Link the specified object file
$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	$(CC) $(C_FLAGS) -I$(INC_DIR) -L$(LIB_DIR) $(LIBS) $^ -o $@
