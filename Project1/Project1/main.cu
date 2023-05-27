#include <iostream>
#include <thrust\device_vector.h>
#include <thrust\host_vector.h>
#include <cstdlib>
#include <thrust\reduce.h>
#include <SDL.h>
#include <SDL_image.h>
#undef main

#define mormillion 1000000

int main() {
	int startOfLoop = SDL_GetTicks();


	thrust::device_vector<int> dv(0);
	thrust::host_vector<int> hv(0);
	int finalValue = 0;
	//for (int i = 0; i < mormillion; i++) {
	//	dv.push_back(rand());
	//}
	
	int firstLoop = SDL_GetTicks() - startOfLoop;

	std::cout << "Made it past the Random Part!" << std::endl;
	
	//finalValue = thrust::reduce(dv.begin(), dv.end());

	int secondStart = SDL_GetTicks();

	for (int i = 0; i < mormillion; i++) {
		hv.push_back(rand());
	}

	int SecondValue = thrust::reduce(hv.begin(), hv.end());
	
	int SecondFinal = SDL_GetTicks() - secondStart;

	std::cout << finalValue << ", " << SecondValue <<std::endl;
	std::cout << "GPU Vector Time: " << finalValue << std::endl;
	std::cout << "CPU Vector Time: " << SecondFinal << std::endl;

	return 0;
}
