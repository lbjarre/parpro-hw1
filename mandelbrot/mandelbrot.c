#include <stdio.h>
#include <math.h>
#include <complex.h>

#define MAXITER 255

#define W 16
#define H 16
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

void mandelbrot_grid(int *M)
{
    double x, y;
    complex double c;
    int color;

    double dx = 2*B/(W - 1);
    double dy = 2*B/(H - 1);
    
    for (int j = 0; j < H; ++j) {
        y = j * dy - B;
        for (int i = 0; i < W; ++i) {
            y = i * dx - B;
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
            fprintf(fp, "%hhu", M[i + j*W]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main()
{
    int M[W*H];
    mandelbrot_grid(M);
    write_to_file(M);
    return 0;
}
