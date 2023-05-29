#include <iostream>
#include <iomanip>
#include <SDL.h>
#include <thread>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>


const int WIDTH = 1980;
const int HEIGHT = 1080;
const int FPS = 165;
const int desiredDelta = 1000 / 165;
const double maxAngleDeviation = 22.5;
const int renderDistance = 255;
const int ThreadCount = 19;

class vectorDirection {
public:
	int x;
	int y;
	int z;
};

class minMaxRenderingAreas {
public:
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

class vectorInherittedColour {
public:
	int r;
	int g;
	int b;
	int a;
};

class vectorInformation {
public:
	vectorDirection VectorDirection;
	vectorInherittedColour Colours;
};

class pixelInformation {
public:
	SDL_Point ScreenPointInformation;
	vectorInherittedColour Colours;
};


void threadPixelDistribution(std::vector<minMaxRenderingAreas> &threadDistribution) {
	int verticalDistribution = 1;
	if (ThreadCount > 3) {
		verticalDistribution = 2;
		std::vector<int> horizontalDistribution;
		horizontalDistribution.resize(verticalDistribution);
		for (int i = 0; i < verticalDistribution; i++) {
			horizontalDistribution[i] = ThreadCount / verticalDistribution;
		}
		if (ThreadCount % 2 == 1 && verticalDistribution == 2) {
			horizontalDistribution[1] = horizontalDistribution[1] + 1;
		}
		int topThreadDistance = WIDTH / horizontalDistribution[0];
		int bottomThreadDistance = WIDTH / horizontalDistribution[1];
		int startingPos = 0;
		int endingPos = topThreadDistance;
		for (int i = 0; i < horizontalDistribution[0] - 1; i++) {
			threadDistribution[i].xMin = startingPos;
			threadDistribution[i].xMax = endingPos;
			threadDistribution[i].yMin = 0;
			threadDistribution[i].yMax = HEIGHT / 2;
			startingPos = endingPos + 1;
			endingPos = endingPos + topThreadDistance;
			std::cout << i + 1 << std::endl;
		}
		threadDistribution[horizontalDistribution[0] - 1].xMin = startingPos;
		threadDistribution[horizontalDistribution[0] - 1].xMax = WIDTH;
		threadDistribution[horizontalDistribution[0] - 1].yMin = 0;
		threadDistribution[horizontalDistribution[0] - 1].yMax = HEIGHT / 2;

		startingPos = 0;
		endingPos = bottomThreadDistance;
		for (int i = 0; i < horizontalDistribution[1] - 1; i++) {
			threadDistribution[horizontalDistribution[0] + i].xMin = startingPos;
			threadDistribution[horizontalDistribution[0] + i].xMax = endingPos;
			threadDistribution[horizontalDistribution[0] + i].yMin = HEIGHT / 2 + 1;
			threadDistribution[horizontalDistribution[0] + i].yMax = HEIGHT;
			startingPos = endingPos + 1;
			endingPos = endingPos + bottomThreadDistance;
			std::cout << horizontalDistribution[0] + i + 1 << std::endl;
		}
		threadDistribution[ThreadCount - 1].xMin = startingPos;
		threadDistribution[ThreadCount - 1].xMax = WIDTH;
		threadDistribution[ThreadCount - 1].yMin = HEIGHT / 2 + 1;
		threadDistribution[ThreadCount - 1].yMax = HEIGHT;
	}
	else {
		int horizontalDistribution = ThreadCount;
		int threadDistance = WIDTH / horizontalDistribution;
		int startingPos = 0;
		int endingPos = threadDistance;
		for (int i = 0; i < horizontalDistribution - 1; i++) {
			threadDistribution[i].xMin = startingPos;
			threadDistribution[i].xMax = endingPos;
			threadDistribution[i].yMin = 0;
			threadDistribution[i].yMax = HEIGHT;
			startingPos = endingPos + 1;
			endingPos = endingPos + threadDistance;
		}
		threadDistribution[ThreadCount - 1].xMin = startingPos;
		threadDistribution[ThreadCount - 1].xMax = WIDTH;
		threadDistribution[ThreadCount - 1].yMin = 0;
		threadDistribution[ThreadCount - 1].yMax = HEIGHT;
	}
}

void getTheVectorWithAllTheTriangleInformation() {

}


int main(int argc, char** argv)
{
	/* Initialises data */
	SDL_Window* window = NULL;

	/*
	* Initialises the SDL video subsystem (as well as the events subsystem).
	* Returns 0 on success or a negative error code on failure using SDL_GetError().
	*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
		return 1;
	}

	/* Creates a SDL window */
	window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0); 
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	/* Checks if window has been created; if not, exits program */
	if (window == NULL) {
		fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
		return 1;
	}



	pixelInformation** pointerToAllDisplayedPoints = new pixelInformation* [HEIGHT];
	for (int i = 0; i < HEIGHT; i++) {
		pointerToAllDisplayedPoints[i] = new pixelInformation[WIDTH];
	}


	std::vector<minMaxRenderingAreas> threadDistribution;
	threadDistribution.resize(ThreadCount);

	threadPixelDistribution(threadDistribution);


	for (int i = 0; i < ThreadCount; i++) {
		std::cout << "Thread Num: " << i + 1 << std::endl
			<< "   Min X Value = " << threadDistribution[i].xMin << std::endl
			<< "   Max X Value = " << threadDistribution[i].xMax << std::endl
			<< "   Min Y Value = " << threadDistribution[i].yMin << std::endl
			<< "   Max Y Value = " << threadDistribution[i].yMax << std::endl << std::endl;
	}

	



	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (j < WIDTH / 2) {
				pointerToAllDisplayedPoints[i][j].Colours.g = 0;
				pointerToAllDisplayedPoints[i][j].Colours.b = 0;
			}
			else {
				pointerToAllDisplayedPoints[i][j].Colours.g = 255;
				pointerToAllDisplayedPoints[i][j].Colours.b = 255;
			}
			pointerToAllDisplayedPoints[i][j].Colours.r = 255;
			pointerToAllDisplayedPoints[i][j].Colours.a = SDL_ALPHA_OPAQUE;
		}
	}
	
	
	std::cout << "Done!" << std::endl;


	while (true)
	{
		int startOfLoop = SDL_GetTicks();

		// Get the next event
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}
		



-



		

		// MAIN PIXEL DRAWING METHOD!
		SDL_SetRenderDrawColor(0, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				SDL_SetRenderDrawColor(renderer, pointerToAllDisplayedPoints[i][j].Colours.r, pointerToAllDisplayedPoints[i][j].Colours.g, pointerToAllDisplayedPoints[i][j].Colours.b, pointerToAllDisplayedPoints[i][j].Colours.a);
				SDL_RenderDrawPoint(renderer, j, i);

			}
		}

		SDL_RenderPresent(renderer);






		int delta = SDL_GetTicks() - startOfLoop;
		if (delta < desiredDelta) {
			SDL_Delay(desiredDelta - delta);
		}
		else {
			std::cout << "FRAMEDROP! " << delta << "ms instead of " << desiredDelta << "ms" << std::endl;
		}

	}

	/* Frees memory */
	SDL_DestroyWindow(window);

	/* Shuts down all SDL subsystems */
	SDL_Quit();

	delete[] pointerToAllDisplayedPoints;

	return 0;
}