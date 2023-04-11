CC := x86_64-w64-mingw32-g++
PLATFORM := windows

SFML_DIR := vendor/sfml
CFLAGS := -DNO_IMGUI -DSFML_STATIC
INCFLAGS := -I$(SFML_DIR)/include
LDFLAGS := -L$(SFML_DIR)/lib -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -static-libgcc -static-libstdc++ -lopenal32 -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg -lopengl32 -lwinmm -lgdi32 -lfreetype -pthread --static

include make/shared.mk

all: setup copy_vendor_binaries compile

copy_vendor_binaries:
	@echo "Copying vendor binaries..."
	@cp vendor/*/bin/*.dll $(BIN_DIR)/
