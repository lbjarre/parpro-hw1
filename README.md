# ParPro HW1 - Parallel Mandelbrot

A repo containing my code and report for the first homework
in the Parallel Computing course
given at KTH Royal Institute of Technology during spring 2018.

The code implements a parallelized version of calculating the Mandelbrot set
for a viewport in the complex plane.
Since calculating the value at each pixel is independent of any other pixel,
each calculation can be assigned to different processes
and combined in the end to produce the image.

The parallelization is implemented using MPI.
OpenMPI was used as the MPI implementation while testing the code,
but it should run fine on any other implementation.

Testing if one can embedd images in MD:
![Mandelbrot](mandelbrot/zoom1.pdf)
