#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

#define NUMB_COLORS 255
#define ESCAPE_DIST 2.0

#define PIXEL_W 1024
#define PIXEL_H 1024
#define X_START -2.0
#define X_W      4.0
#define Y_START -2.0
#define Y_H      4.0


int mandelbrot(complex double c)
{    
    int i = 0;
    complex double x = c;

    while (cabs(x) < ESCAPE_DIST && i < NUMB_COLORS) {
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

    double dx = X_W/(PIXEL_W - 1);
    double dy = Y_H/(PIXEL_H - 1); 

    for (j = 0; j < p_height; ++j) {
        y = (j + y_offset) * dy + Y_START;
        for (i = 0; i < PIXEL_W; ++i) {
            x = i * dx + X_START;
            c = x + y * I;
            M[j*PIXEL_W + i] = mandelbrot(c);
        }
    }
}

void write_to_file(int *M)
{
    int i, j;
    FILE *fp = fopen("color.txt", "w");
    for (j = 0; j < PIXEL_H; ++j) {
        for (i = 0; i < PIXEL_W; ++i) {
            fprintf(fp, "%hhu ", M[i + j*PIXEL_W]);
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

    p_h = PIXEL_H / size;
    y_offset = rank * p_h;

    int M[PIXEL_W*p_h];
    mandelbrot_grid(M, y_offset, p_h);
    if (rank != 0) {
        MPI_Send(M, PIXEL_H*p_h, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int full_array[PIXEL_H*PIXEL_W];
        int *p = full_array;
        for (i = 0; i < PIXEL_W*p_h; ++i) {
            *p++ = M[i];
        }
        for (i = 1; i < size; ++i) {
            MPI_Recv(p, PIXEL_W*p_h, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            p += PIXEL_W*p_h;
        }
        write_to_file(full_array);
    }

    MPI_Finalize();
    return 0;
}
