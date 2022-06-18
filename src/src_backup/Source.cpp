//#include <Game.h>
#include <iostream>

//int main(int argc, char *argv[]) {
//	auto *game = new Game();
//
//	game->init("20 Minutes Till Sunrise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, false);
//	while (game->running()) {
//		game->frameStart = SDL_GetTicks();
//		game->handleEvents();
//		//game->update();
//		game->render();
//		game->frameTime = SDL_GetTicks() - game->frameStart;
//		if (game->frameDelay > game->frameTime) {
//			SDL_Delay(game->frameDelay - game->frameTime);
//		}
//	}
//	game->clean();
//	delete game;
//	return 0;
//}