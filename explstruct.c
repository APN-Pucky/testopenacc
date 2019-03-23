#include <stdlib.h>
#include <stdio.h>
#ifndef N
#define N 1024
#endif

typedef struct gpc_vertex_list
{
	int pid;    // polygon id
	int num_vertices;
	double *mbr;   // minimum bounding rectangle of the polygon, so always 4 elements

} gpc_vertex_list;

gpc_vertex_list * allocData(size_t size);
int deleteData(gpc_vertex_list * A, size_t size);
int initData(gpc_vertex_list *Ai, size_t size);

#pragma acc routine seq
int isRectOverlap(double * mbr) {
	int result;
	result = mbr[0];
	result += mbr[1];
	result += mbr[2];
	result += mbr[3];
	return result;
}

int main() {
	gpc_vertex_list *A;
	gpc_vertex_list B;
	size_t size, i;
	int * listOfBoolean;
	size = N;
	A=allocData(size);
	initData(A,size);
	listOfBoolean = (int*) malloc(sizeof(int)*size);

#pragma acc parallel loop present(A) copyout(listOfBoolean[0:size])  private(B)
	for (i=0; i<size; i++){
		B = A[i];
		listOfBoolean[i] = isRectOverlap(B.mbr);
	}

	printf("result: %d %d %d\n",listOfBoolean[0], listOfBoolean[size/2], listOfBoolean[size-1]);
	free(listOfBoolean);
	deleteData(A, size);
	exit(0);
}

gpc_vertex_list * allocData(size_t size) {
	gpc_vertex_list * tmp;
	tmp = (gpc_vertex_list *) malloc(size*sizeof(gpc_vertex_list));
	/* Create the array on device.  */
#pragma acc enter data create(tmp[0:size])
	for (int i=0; i< size; ++i) {
		tmp[i].mbr = (double*) malloc(sizeof(double)*4);
		/* create the member array on the device */
#pragma acc enter data create(tmp[i].mbr[0:4])
	}
	return tmp;
}

int deleteData(gpc_vertex_list * A, size_t size) {
	/* Delete the host copy. */
	for (int i=0; i< size; ++i) {
#pragma acc exit data delete(A[i].mbr)
		free(A[i].mbr);
	}
#pragma acc exit data delete(A)
	free(A);
}

int initData(gpc_vertex_list *A ,size_t size) {
	size_t i;
	for (int i=0; i< size; ++i) {
		A[i].pid = i;
		A[i].num_vertices = 4;
		for (int j=0; j<4;++j) {
			A[i].mbr[j]=(i*4)+j;
		}
#pragma acc update device(A[i].pid,A[i].num_vertices,A[i].mbr[0:4])
	}
}
