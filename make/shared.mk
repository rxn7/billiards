# .PHONY: setup compile validate_shaders compile_shaders create_dirs copy_assets clean all

TARGET ?= debug
OBJ_DIR += obj/$(PLATFORM)/$(TARGET)
BIN_DIR += bin/$(PLATFORM)/$(TARGET)
OUT := $(BIN_DIR)/billiards
INCFLAGS += -Isrc -Ishaders_out -Ivendor/imgui -Ivendor/imgui-sfml
CFLAGS += -std=c++20
SRC += $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)
SRC += $(wildcard vendor/*.cpp vendor/*/*.cpp vendor/*/*/*.cpp)
OBJ += $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

ifeq ($(TARGET), debug)
	CFLAGS += "-DDEBUG"
	CFLAGS += "-g3"
else
	CFLAGS += "-DRELEASE"
	CFLAGS += "-O3"
endif

setup: validate_shaders compile_shaders create_dirs copy_assets
compile: $(OBJ) $(OUT)

$(OBJ_DIR)/%.o: %.cpp
	@echo "Building $@"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

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
	rm -rf obj bin shaders_out
