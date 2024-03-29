#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    double return_val;
    return_val = exp(x);
    return return_val;
}

int main(int argc, char *argv[]) {
    int rank, size;
    double a = 0.0, b = 1.0;
    long int n = 100000000;
    double x,h;
    double integral = 0.0, total;
    int source, destination = 0;
    int tag = 3;
    double start, finish, maxtime;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    start = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    h = (b - a)/n;

    if (rank == 0)
        integral = (f(a) + f(b)) / 2.0;

    for (x = a + h * (rank + 1); x < b; x += size * h) {
        integral += f(x);
    }

    integral = integral * h;

    if (rank == 0) {
        total = integral;
        for (source = 1; source < size; source++) {
            MPI_Recv(&integral, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
            total += integral;
        }
    } else {
        MPI_Send(&integral, 1, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime() - start;

    MPI_Reduce(&finish, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("With n = %ld.\n", n);
        printf("Integral from %lf to %lf = %lf \n", a, b, total);
        printf("Elapsed time for %d: %lf seconds\n", size, maxtime);
    }

    MPI_Finalize();

    return 0;
}