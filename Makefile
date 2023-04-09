CC := g++
OBJ_DIR := obj
SRC_DIR := src
BIN_DIR := bin
OUT := $(BIN_DIR)/billard
LDFLAGS := $(shell pkgconf sdl2 --libs --static)
INCFLAGS := -Isrc -Ishaders_out
CFLAGS := -std=c++20 $(shell pkgconf sdl2 --cflags)
SRC := $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: validate_shaders compile_shaders create_dirs copy_res compile

compile: $(OBJ) $(OUT)

$(OBJ_DIR)/%.o: %.cpp
	@echo "Building $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	@$(CC) $(OBJ) $(LDFLAGS) -o $@

create_dirs:
	@echo "Creating directories..."
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

validate_shaders:
	@echo "Validating shaders..."
	@glslangValidator shaders/*

compile_shaders:
	@echo "Compiling shaders..."
	@./scripts/compile_shaders.sh

copy_res:
	@echo "Copying resource files..."
	@cp -r res bin/

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) shaders_out
