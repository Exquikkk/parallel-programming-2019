// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h" 
#include "mpi.h" 
#include <stdio.h> 
#include <iostream> 
#include <math.h> 
#include <ctime> 
#include <cmath> 
#include <string> 
using namespace std;

bool testLinearTopology(MPI_Comm commLinear, int new_coords[]) {
	int rank, size;
	int sourceLess, sourceBig, destLess, destBig;
	int val, valFromLess, valFromBig;
	MPI_Status status;
	MPI_Comm_rank(commLinear, &rank);
	MPI_Comm_size(commLinear, &size);
	MPI_Cart_coords(commLinear, rank, 1, new_coords);
	val = new_coords[0];
	valFromLess = -1;
	valFromBig = -1;
	MPI_Cart_shift(commLinear, 0, 1, &sourceLess, &destBig);
	MPI_Cart_shift(commLinear, 0, -1, &sourceBig, &destLess);
	MPI_Sendrecv(&val, 1, MPI_INT, destBig, 4, &valFromLess, 1, MPI_INT,
		sourceLess, 4, commLinear, &status);
	MPI_Sendrecv(&val, 1, MPI_INT, destLess, 4, &valFromBig, 1, MPI_INT,
		sourceBig, 4, commLinear, &status);
	MPI_Comm_free(&commLinear);

	if ((rank + 1) != valFromBig) {
		if (new_coords[0] != size - 1) return false;
	}
	if ((rank - 1) != valFromLess) {
		if (new_coords[0] != 0) return false;
	}
	return true;
}

int main(int argc, char** argv) {
	int size = 5;
	int new_coords[1];
	int reorder = 0;
	int dims[1]{ 0 }, periods[1]{ 0 };
	int oldSize, rankProc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
	MPI_Comm_size(MPI_COMM_WORLD, &oldSize);
	size = atoi(argv[1]);
	new_coords[atoi(argv[2])];
	if (size <= 0 || size > oldSize)
		MPI_Dims_create(oldSize, 1, dims);
	else
		MPI_Dims_create(size, 1, dims);
	MPI_Comm commLinear;
	MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, reorder, &commLinear);
	bool test = testLinearTopology(commLinear, new_coords);
	if (rankProc == 0)
		cout << test; fflush(stdout);;
	MPI_Finalize();
	return 0;
}

