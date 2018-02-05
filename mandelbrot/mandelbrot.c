#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

#define MAXITER 255

#define W 1024
#define H 1024
#define B 2.0

int mandelbrot(complex double c)
{    
    int i = 0;
    complex double x = c;

    while (cabs(x) < B && i < MAXITER) {
        x = x*x + c;
        ++i;
    }
    
    return i;
}

void mandelbrot_grid(int *M, int y_offset, int p_height)
{
    int i, j;
    double x, y;
    complex double c;

    double dx = 2*B/(W - 1);
    double dy = 2*B/(H - 1); 

    for (j = 0; j < p_height; ++j) {
        y = (j + y_offset) * dy - B;
        for (i = 0; i < W; ++i) {
            x = i * dx - B;
            c = x + y * I;
            M[j*W + i] = mandelbrot(c);
        }
    }
}

void write_to_file(int *M)
{
    int i, j;
    FILE *fp = fopen("color.txt", "w");
    for (j = 0; j < H; ++j) {
        for (i = 0; i < W; ++i) {
            fprintf(fp, "%hhu ", M[i + j*W]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    int rank, size, y_offset, p_h, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    p_h = H / size;
    y_offset = rank * p_h;

    int M[W*p_h];
    mandelbrot_grid(M, y_offset, p_h);
    if (rank != 0) {
        MPI_Send(M, H*p_h, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int full_array[H*W];
        int *p = full_array;
        for (i = 0; i < W*p_h; ++i) {
            *p++ = M[i];
        }
        for (i = 1; i < size; ++i) {
            MPI_Recv(p, W*p_h, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            p += W*p_h;
        }
        write_to_file(full_array);
    }

    MPI_Finalize();
    return 0;
}
