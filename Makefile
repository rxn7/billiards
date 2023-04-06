CC := g++
OBJ_DIR := obj
SRC_DIR := src
BIN_DIR := bin
OUT := $(BIN_DIR)/billard
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
INCFLAGS := -Isrc
CFLAGS := -std=c++20
SRC := $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: create_dirs $(OBJ) $(OUT) copy_res

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

create_dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

copy_res:
	@cp -r res bin/

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
