#include <thrust\host_vector.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <thrust\device_vector.h>
#include <SDL.h>
#undef main


const int SCREEN_WIDTH = 1980;
const int SCREEN_HEIGHT = 1080;
const int FRAME_RATE = 165;
const double SCREEN_MAX_ANGLE = 22.5;
const int DESIRED_MILLISECOND_PER_FRAME = 1000/FRAME_RATE;

class vectorInformation {
public:
	double x;
	double y;
	double z;
};


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("3D Render hopefully", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	bool rendererIsRunning = true;


	while (rendererIsRunning) {
		// SetsFrameRate
		int startOfLoop = SDL_GetTicks();

		// Inputs section
		SDL_Event event;
		SDL_PollEvent(&event);



		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);



		// Detects if the screen is being closed
		switch (event.type) {
		case SDL_QUIT:
			rendererIsRunning = false;
			break;
		default:
			break;
		}

		int delta = SDL_GetTicks() - startOfLoop;
		if (delta < DESIRED_MILLISECOND_PER_FRAME) {
			SDL_Delay(DESIRED_MILLISECOND_PER_FRAME - delta);
		}
		else {
			std::cout << "FRAMEDROP! " << delta << "ms instead of " << DESIRED_MILLISECOND_PER_FRAME << "ms" << std::endl;
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}