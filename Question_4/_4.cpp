#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <time.h>

double LiToDouble (LARGE_INTEGER x) {
double result = 
 ((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
 return result;
}
// Function that gets the timestamp in seconds
double GetTime() {
 LARGE_INTEGER lpFrequency, lpPerfomanceCount;
 QueryPerformanceFrequency (&lpFrequency);
 QueryPerformanceCounter (&lpPerfomanceCount);
 return LiToDouble(lpPerfomanceCount)/LiToDouble(lpFrequency);
}
void RandomDataInitialization(double* pMatrix, int Size) {
 int i, j; 
 time_t t;
 srand((unsigned) time(&t) );
//  srand(time());
 for (i=0; i<Size; i++) {
    for (j=0; j<Size; j++)
        pMatrix[i*Size+j] = rand()/ double(100);
 }
}

void ProcessInitialization (double* &pMatrix, int &Size) {
    do {
    printf("\nEnter size of the initial objects: ");
    scanf("%d", &Size);
    printf("\nChosen objects size = %d\n", Size);
    if (Size <= 0)
    printf("\nSize of objects must be greater than 0!\n");
    }
    while (Size <= 0);
    // Memory allocation 
    pMatrix = new double [Size*Size];
    RandomDataInitialization(pMatrix, Size);
}

void PrintMatrix (double* pMatrix, int RowCount, int ColCount) {
    int i, j; 
    for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
    printf("%7.4f ", pMatrix[i*RowCount+j]);
    printf("\n");
    }
}

double ParallelMinMax(double *pMatrix, int size)
{
    double max = -10e4, min = -10e4;
    omp_set_num_threads(4);
    #pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        min = pMatrix[i *size + 0];

        //#pragma omp parallel for
        for (int j = 1; j < size; j++)
        {
            if (min > pMatrix[i*size+ j]) 
                min = pMatrix[i*size+ j];
        }
        if (max < min)
            max = min;
    }
    return max;
}

void ProcessTermination(double* pMatrix) {
 delete [] pMatrix;
}

int main()
{
    double* pMatrix;
    int size;
    double Start, Finish, Duration, result;

    ProcessInitialization(pMatrix, size);
    Start = GetTime();
    result=  ParallelMinMax(pMatrix, size);
    Finish = GetTime();
    //PrintMatrix(pMatrix, size, size);
    printf("Result: %f\n", result);
    printf("Time of execution: %f\n", Finish - Start);

    ProcessTermination(pMatrix);
}