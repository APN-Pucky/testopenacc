#include <iostream>
#include <omp.h>

int main(int argc, char** argv)
{
	const unsigned long int N = 100000;

	std::cout << "Number of devices: " << omp_get_num_devices() << std::endl;
	std::cout << "Default device: " << omp_get_default_device() << std::endl;
/*
	bool offload;
#pragma omp target defaultmap(tofrom:scalar)
	offload = !omp_is_initial_device();
	int device = offload ? 0: omp_get_initial_device();

	if (offload) {
		printf("Able to use offloading!\n");
	}

	// Allocate
	float* buffer = (float*)omp_target_alloc(N*sizeof(float), device);

	// Evaluate
#pragma omp target is_device_ptr(buffer)
#pragma omp parallel for
	for(unsigned long int i = 0; i < N; ++i) buffer[i] = i;

	// Cleanup
	omp_target_free(buffer, device);
*/
	return 0;
}
