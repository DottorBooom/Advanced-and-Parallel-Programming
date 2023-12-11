# Advanced and Parallel Programming
![Static Badge](https://img.shields.io/badge/Version-0.2.0-limegreen) ![Static Badge](https://img.shields.io/badge/VSCode-%23007ACC?logo=visualstudiocode) ![Static Badge](https://img.shields.io/badge/Version-17.0.0-blue?logo=C)


Repository dedicated to the course of 'Advanced and Parallel Programming' taught by Professor Luca Manzoni in the undergraduate degree of "Artificial Intelligence and Data Analytics" at the University of Trieste, during the academic year 2023/2024.
## Description
It consists of a project divided into 2 parts: 
+ the first part entirely in C, involves calculating values belonging to the Mandelbrot set and drawing its fractal (in grayscale);
+ the second part will be provided later and will be written in Python.
## C part
### Mandelbrot set 
The Mandelbrot set is the set of all points in the complex plane (i.e., all c in C) such that the sequence does not go to infinity (i.e., it is bounded). Visualizing the points within and outside the set using different colors produces the well-known fractal figure. <br /><br />
Since it is not easily determinable in every case whether the sequence {zn}n∈N remains bounded for a given c in C, the following procedure is employed:
+ Choose a fixed radius r such that if an element of the sequence exceeds r in magnitude, the sequence is considered unbounded. We typically choose r = 2 as it ensures that if the sequence surpasses 2, it will diverge to infinity;
+ Set a maximum number of iterations M;
+ Starting from a point c in C, iterate the function f(z) = z^2 + c from c (i.e., calculate (f(c), f(f(c)),...). If |f_n(c)| > r, then consider the point as exterior to the Mandelbrot set and regard the minimum n such that |f_n(c)| > r as an indication of the 'speed' at which it diverges;
+ If after M iterations f^M(c) has never exceeded the radius r in magnitude, then consider the point c as interior to the Mandelbrot set. <br /><br />
### Program implementation
The program must accept, from the command line, the following arguments (in this order):
+ The name of the file to create;
+ The maximum number of iterations M to perform;
+ The vertical resolution of the image. The horizontal resolution will always be 1.5 times the vertical resolution. <br /><br />
The program should compute the set of points belonging to the Mandelbrot set (using M iterations and radius r = 2) in the part of the complex plane between -2 and 1 for the real part, and -i to i for the imaginary part. To achieve this, it will use a grid of equispaced points with nrows rows and ncols columns. For each point c ∈ C that does not belong to the Mandelbrot set, it is necessary to store the minimum n for which |f_n(c)| ≥ r.
In the specified file, the program must save a grayscale image of the Mandelbrot set in binary PGM format (link to documentation: https://netpbm.sourceforge.net/doc/pgm.html) with the following characteristics:
+ The image must have a height of nrows pixels and a width of ncols pixels.
+ Points inside the Mandelbrot set must be white (255).
+ Points outside must have a color of 255 × (log(n) / log(M)), where n is defined as the minimum value for which |f_n(c)| ≥ r for the corresponding point c. <br /><br />
### Example
An example of output is shown in the figure. In this example, nrows = 1000 (thus ncols = 1500) and M = 1000. <br /><br />
![Immagine 2023-12-11 000824](https://github.com/DottorBooom/Advanced-and-Parallel-Programming/assets/94912692/fede54e9-3583-43c9-99ea-6e6567a1f623)
### Requirements
+ The program must be structured across multiple files, including at least one main.c file, two mandelbrot.h and mandelbrot.c files for handling the fractal calculation, and two pgm.h and pgm.c files for saving the image in the correct format.
+ The program must be compilable using a makefile.
+ It is possible to use complex.h as a library for managing complex numbers, or you can implement your own solution.
+ The computation of the points' membership in the Mandelbrot set must be executed in parallel using OpenMP.
+ Image saving must utilize 'mmap'.
+ The program must correctly release allocated resources.
+ In case of an error (e.g., an unopenable file), the program must report the error and exit normally with a non-zero error code.
+ Every relevant function (i.e., longer than 2-3 lines) must be adequately commented, explaining its task, required inputs, and generated outputs.
+ Each .c and .h file must include at the beginning a comment with the author's name, surname, and student ID number.

## Python part
Nothing new for now.
