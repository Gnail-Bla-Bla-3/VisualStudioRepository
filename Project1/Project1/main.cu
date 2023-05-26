#include <iostream>
#include <thrust\device_vector.h>
#include <thrust\host_vector.h>
#include <cstdlib>
#include <thrust\reduce.h>

#define mormillion 100

int main() {
	thrust::device_vector<int> dv(0);
	thrust::host_vector<int> hv(0);
	float finalValue = 0;
	for (int i = 0; i < mormillion; i++) {
		dv.push_back(rand());
	}

	std::cout << "Made it past the Random Part!" << std::endl;
	
	finalValue = thrust::reduce(dv.begin(), dv.end());
	
	std::cout << finalValue << std::endl;

	return 0;
}
