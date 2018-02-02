#include <stdio.h>
#include <math.h>
#include <complex.h>

#define MAXITER 255
#define ESCAPE 2

int mandelbrot(complex double c)
{    
    int i = 0;
    complex double x = c;

    while (cabs(x) < ESCAPE && i < MAXITER) {
        x = x*x + c;
        ++i;
    }
    
    return i;
}

int main()
{
    complex double c = 0.1 - 1*I;
    int i = mandelbrot(c);
    printf("%d\n", i);
}
