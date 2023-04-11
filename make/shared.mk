.PHONY: setup compile validate_shaders compile_shaders create_dirs copy_assets clean

OBJ_DIR += obj/$(PLATFORM)
SRC_DIR += src/$(PLATFORM)
BIN_DIR += bin/$(PLATFORM)
OUT := $(BIN_DIR)/billiards
INCFLAGS += -Isrc -Ishaders_out
CFLAGS += -std=c++20
SRC += $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
OBJ += $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

setup: validate_shaders compile_shaders create_dirs copy_assets
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

copy_assets:
	@echo "Copying assets..."
	@cp -r assets $(BIN_DIR)/

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) shaders_out
