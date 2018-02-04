#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <mpi.h>

#define MAXITER 255

#define W 256
#define H 256
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

void mandelbrot_grid(int *M, int x_offset, int p_width)
{
    double x, y;
    complex double c;

    double dx = 2*B/(W - 1);
    double dy = 2*B/(H - 1);
    
    for (int j = 0; j < H; ++j) {
        y = j * dy - B;
        for (int i = x_offset; i < p_width; ++i) {
            x = i * dx - B;
            c = x + y * I;
            M[i + j*H] = mandelbrot(c);
        }
    }
    
}

void write_to_file(int *M)
{
    FILE *fp = fopen("color.txt", "w");
    for (int j = 0; j < H; ++j) {
        for (int i = 0; i < W; ++i) {
            fprintf(fp, "%hhu ", M[i + j*W]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    int rank, size, x_offset, p_w, tag, rc;
    MPI_Status status;

    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tag = 10;

    p_w = W / size;
    x_offset = rank * p_w;

    int M[H*p_w];

    mandelbrot_grid(M, x_offset, p_w);
    write_to_file(M);

    rc = MPI_Finalize();
    return 0;
}
