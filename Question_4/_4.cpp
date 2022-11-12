#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <time.h>
using namespace std;

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
void pickNumThreads(int threads)
{
    printf("Choose the number of threads: ");
    scanf("%d", &threads);
}

void ProcessInitialization (double* &pMatrix, int &Size, int &threads) {
    // do {
    // printf("\nEnter size of the initial objects: ");
    // scanf("%d", &Size);
    // printf("\nChosen objects size = %d\n", Size);
    // if (Size <= 0)
    // printf("\nSize of objects must be greater than 0!\n");
    // }
    // while (Size <= 0);
    // Memory allocation 
    pMatrix = new double [Size*Size];
    RandomDataInitialization(pMatrix, Size);
    //pickNumThreads(threads);
}

void PrintMatrix (double* result, int RowCount, int ColCount) {
    int i, j; 
    for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
    printf("%7.4f ", result[i*ColCount+j]);
    printf("\n");
    }
}

double ParallelMinMax(double *pMatrix, int size, int threads)
{
    double max = -10e4, min = -10e4;
    omp_set_num_threads(threads);
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

// void ProcessTermination(double* pMatrix) {
//  delete [] pMatrix;
// }

void test(int* arrayTestSize, byte *arrayNumThreads, double * result,  int numTest, int numThreads)
{
    double* pMatrix;
    // int size, threads=  8;
    double Start, Finish;
    result = new double[numTest * numThreads];

    for(int i = 0; i < numTest; i++)
    {
        for (int j = 0; j < numThreads; j++)
        {
            int size = arrayTestSize[i];
            int threads = arrayNumThreads[j];
            ProcessInitialization(pMatrix, size, threads);
            Start = GetTime();
            double max = ParallelMinMax(pMatrix, size, threads);
            Finish = GetTime();
            result[i*numThreads +j] = Finish - Start;
        }
    }
    delete [] pMatrix;
}

int main()
{
    int arrayTestSize[5] = { 6000, 7000, 8000, 9000, 10000};
    byte arrayNumThreads[4] = {1, 4, 6, 8};
    double * result;
    test(arrayTestSize, arrayNumThreads, result, 10, 4);
    PrintMatrix(result, 5, 4);
    delete [] result;
}