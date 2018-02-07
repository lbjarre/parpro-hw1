#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUMB_COLORS 255
#define ESC_DIST_SQ 4.0
#define PIXEL_W  2048
#define PIXEL_H  2048

int mandelbrot(double c_real, double c_imag)
{    
    int i = 0;
    double z_real = c_real;
    double z_real_sq = c_real * c_real;
    double z_imag = c_imag;
    double z_imag_sq = c_imag * c_imag;

    while (z_real_sq + z_imag_sq < ESC_DIST_SQ && i < NUMB_COLORS) {
        z_imag = 2 * z_real * z_imag + c_imag;
        z_real = z_real_sq - z_imag_sq + c_real;
        z_real_sq = z_real * z_real;
        z_imag_sq = z_imag * z_imag;
        ++i;
    }
    
    return i;
}

void mandelbrot_grid(int *M, int y_offset, int process_height,
                     double x_start, double y_start, double h_w)
{
    int i, j;
    double x, y;

    double dx = h_w/(PIXEL_W - 1);
    double dy = -h_w/(PIXEL_H - 1); 

    for (j = 0; j < process_height; ++j) {
        y = (j + y_offset) * dy + y_start;
        for (i = 0; i < PIXEL_W; ++i) {
            x = i * dx + x_start;
            *M++ = mandelbrot(x, y);
        }
    }
}

void write_to_file(FILE *fp, int *M, int process_height)
{
    int i, j;
    for (j = 0; j < process_height; ++j) {
        for (i = 0; i < PIXEL_W; ++i) {
            fprintf(fp, "%hhu ", *M++);
        }
        fprintf(fp, "\n");
    }
}

int init_mpi(int *size, int *rank, int argc, char **argv)
{
    int flag = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, size);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);

    if (PIXEL_H % *size) {
        flag = -1;
    }
    
    return flag;
}

int set_viewport(double *x_start, double *y_start, double *h_w,
                  int argc, char **argv)
{
    int flag = 0;
    *x_start = -2.0;
    *y_start = 2.0;
    *h_w = 4.0;
    if (argc == 4) {
        sscanf(argv[1], "%lf", x_start);
        sscanf(argv[2], "%lf", y_start);
        sscanf(argv[3], "%lf", h_w);
    }
    else if (argc != 1) {
        flag = -1;
    }
    return flag;
}

int main(int argc, char **argv)
{
    int rank, size, y_off, process_h, vec_len, i, flag_mpi, flag_arg;
    double x_start, y_start, h_w;
    MPI_Status st;

    flag_arg = set_viewport(&x_start, &y_start, &h_w, argc, argv);
    flag_mpi = init_mpi(&size, &rank, argc, argv);
    if (flag_arg != 0 || flag_mpi != 0) {
        exit(1);
    }

    process_h = PIXEL_H / size;
    vec_len = PIXEL_W * process_h;
    y_off = rank * process_h;
    int M[vec_len];
    mandelbrot_grid(M, y_off, process_h, x_start, y_start, h_w);

    if (rank != 0) {
        MPI_Send(M, vec_len, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        FILE *fp = fopen("color.txt", "w");
        write_to_file(fp, M, process_h);
        for (i = 1; i < size; ++i) {
            MPI_Recv(M, vec_len, MPI_INT, i, 0, MPI_COMM_WORLD, &st);
            write_to_file(fp, M, process_h);
        }
        fclose(fp);
    }

    MPI_Finalize();
    return 0;
}
