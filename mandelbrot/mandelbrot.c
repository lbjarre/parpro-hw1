#include <stdio.h>
#include <math.h>
#include <complex.h>

#define MAXITER 255

#define W 256
#define H 256
#define B 2

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

    double dx = 2*B / (W - 1);
    double dy = 2*B / (H - 1);

    for (int i = 0; i < H; ++i) {
        x = i * dx - B;
        for (int j = 0; j < W; ++j) {
            y = j * dy - B;
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
