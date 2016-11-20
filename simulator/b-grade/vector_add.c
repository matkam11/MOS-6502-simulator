 #include <stdlib.h>


int * vectorAdd(int* A, int* B, size_t size)
{
	int i;
	int * C;
	C = (int *) malloc(size*sizeof(int));

	for(i = 0; i < size; ++i) 
	{
		C[i] = A[i] + B[i];
	}

	return C;
}



