CC := g++
PLATFORM := linux

LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lImGui-SFML

include make/shared.mk

all: setup compile
