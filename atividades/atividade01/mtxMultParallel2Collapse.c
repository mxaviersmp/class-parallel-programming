#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

double **mtxMul(double **c, double **a, double **b, int n) 
{
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            c[i][j] = 0.0;
            for (int k = 0; k < n; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
    return 0;
}

void print_matrix(double **mtx, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%f ", mtx[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    
    double *a[n], *b[n], *c[n];
    for(int i = 0; i < n; i++)
    {
        a[i] = (double *) malloc(sizeof(double) * n);
        b[i] = (double *) malloc(sizeof(double) * n);
        c[i] = (double *) malloc(sizeof(double) * n);
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            a[i][j] = 1.0;
            b[i][j] = 1.0;
            c[i][j] = 0.0;
        }
    }

    mtxMul(c, a, b, n);
    // print_matrix(c, n);

    return 0;
}
