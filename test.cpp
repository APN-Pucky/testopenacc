#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main( int argc, char* argv[] )
{
    printf("dev: %d",omp_get_num_devices());

    return 0;
}
