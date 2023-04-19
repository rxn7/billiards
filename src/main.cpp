#include "game.h"
#include <memory>

int main(int argc, const char **argv) {
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->start();
}