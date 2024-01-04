# Advanced and Parallel Programming
![Static Badge](https://img.shields.io/badge/VSCode-%23007ACC?logo=visualstudiocode) ![Static Badge](https://img.shields.io/badge/Version-17.0.0-blue?logo=C) ![Static Badge](https://img.shields.io/badge/Version-3.11.6-%233776AB?logo=python)

Repository dedicated to the course of 'Advanced and Parallel Programming' taught by Professor Luca Manzoni in the undergraduate degree of "Artificial Intelligence and Data Analytics" at the University of Trieste, during the academic year 2023/2024.
## Description
It consists of a project divided into 2 parts: 
+ the first part entirely in C, involves calculating values belonging to the Mandelbrot set and drawing its fractal (in grayscale);
+ the second part, entirely in Python, involves receiving an input string in reverse Polish notation and processing the commands within it.

There will be a second folder for the C part containing a more optimized (and uncommented) version but that satisfy less the requirements imposed by the professor. Let me explain further: the mandelbrot library will only handle the calculation of the N-th value of the series, and all the for loops will be done inside the pmg library during image processing.

## C part
### Mandelbrot set 
The Mandelbrot set is the set of all points in the complex plane (i.e., all c in C) such that the sequence does not go to infinity (i.e., it is bounded). Visualizing the points within and outside the set using different colors produces the well-known fractal figure. Source: https://en.wikipedia.org/wiki/Mandelbrot_set<br /><br />
Since it is not easily determinable in every case whether the sequence {zn}n∈N remains bounded for a given c in C, the following procedure is employed:
+ Choose a fixed radius r such that if an element of the sequence exceeds r in magnitude, the sequence is considered unbounded. We typically choose r = 2 as it ensures that if the sequence surpasses 2, it will diverge to infinity;
+ Set a maximum number of iterations M;
+ Starting from a point c in C, iterate the function f(z) = z^2 + c from c (i.e., calculate (f(c), f(f(c)),...). If |f_n(c)| > r, then consider the point as exterior to the Mandelbrot set and regard the minimum n such that |f_n(c)| > r as an indication of the 'speed' at which it diverges;
+ If after M iterations f^M(c) has never exceeded the radius r in magnitude, then consider the point c as interior to the Mandelbrot set. <br /><br />
### Program implementation
The program must accept, from the command line, the following arguments (in this order):
+ The name of the file to create;
+ The maximum number of iterations M to perform;
+ The vertical resolution of the image. The horizontal resolution will always be 1.5 times the vertical resolution.
 
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
### Reverse Polish notation
Reverse Polish prefix notation or simply postfix notation, is a mathematical notation in which operators follow their operands, in contrast to prefix or Polish notation (PN), in which operators precede their operands. The notation does not need any parentheses for as long as each operator has a fixed number of operands. Source: https://en.wikipedia.org/wiki/Reverse_Polish_notation <br /><br />
### Overview
The purpose of this project is to create a system for building and evaluating expressions. The system should leverage the ability to define class hierarchies to avoid code duplication. The main idea is to create a system where an expression like the following: 2 x + 3 * can be interpreted as (2 + x) * 3. To achieve this, it's necessary to use a stack (whose implementation is provided in the Stack class) where: 
+ If a string that can be interpreted as an integer is encountered, create an object of type Constant, and push it onto the stack;
+ If a string not corresponding to any function (e.g., x or y) is encountered, create an object of type Variable and push it onto the stack;
+ If a string corresponding to an operator op (e.g., +) is encountered, check the arity k of the function (e.g., 2 for +), pop k elements from the stack to obtain the arguments for op, create an object of type op (e.g., Addition), and push it onto the stack.

If the string representing the expression is correct, then at the end of reading the input string, the stack will contain a single element corresponding to the expression represented by the string.
### Requirements
The following classes need to be implemented:

- `Expression`: Represents a generic expression. It must have a class method `from_program(cls, text, dispatch)` that, given a text representing an expression in reverse Polish notation, returns an object of type (a subclass of) expression.

- `Variable`: Represents a variable. The name is indicated when the object is constructed. If, during evaluation, the environment does not have a value for the variable, the `MissingVariableException` exception must be raised.

- `Constant`: Represents a constant whose value is provided at the time of construction.

- `Operation`: Represents an abstract operation. The constructor receives the arguments of the expression in the form of a list.

- `BinaryOp` and `UnaryOp`: Specialize `Operation` for binary and unary cases, respectively.

The following binary operations (and their corresponding textual forms) must be implemented:
- `Addition`: (+) x + t
- `Subtraction`: (-) x - y
- `Multiplication`: (*) x × y
- `Division`: (/) x / y
- `Modulus`: (%) x mod y
- `Power`: (**) xy
- `Inequality`: (>,>=,=,!=,< and <=) x > y

The following unary operations (and corresponding textual forms) must be implemented:
- `Reciprocal`: (1/) 1 / x
- `AbsoluteValue`: (abs) | x |

The methods to implement are as follows:
- `evaluate(self, env)`: Evaluates the expression using variable assignments in the `env` dictionary, which has associations between variable names (strings) and values.
- `__str__(self)`: String representation.
- `op(self, x, y)` (for binary operations) and `op(self, x)` (for unary operations): These methods implement the operations defined by their class (e.g., addition for `Addition`). These methods make sense to exist only for classes representing binary operations.

Each specialized class must have a variable `arity` to specify the arity of the function. The choice of where to implement each method is left open.
Variable Definition

To make the language more akin to a real programming language, it is necessary to be able to define variables, specifically:

- `var alloc`: where `var` is a variable. Makes the variable available for subsequent parts of the code. This can be done by modifying the environment (`env`). The default value of the variable should be zero. The evaluation does not return any value.

- `n var alloc`: where `var` is a variable, and the result of evaluating an expression returning an integer value. Makes available an array of elements (all zero by default).

- `expr x set`: sets the value of the variable `x` to the result of the expression `expr`, returning the new value of `x`.

- `expr n x set`: sets the value of the n-th position of the array indicated by the variable `x` to the value returned by `expr`. Note that `n` can also be an expression. Returns the new value of `x` at position `n` (arrays are zero-indexed).

Sequences

It should be possible to execute multiple operations in sequence:

- `expr1 expr2 prog2`, `expr1 expr2 expr3 prog3`, and `expr1 expr2 expr3 expr4 prog4` evaluate the two, three, or four preceding expressions, returning the value of `expr1`.

Conditionals and Iterations

There should be support for:

- `if-no if-yes cond if`: If the evaluation of `cond` returns a true value, then `if-yes` is evaluated, otherwise `if-no`. The value of the branch that was evaluated is returned.

- `expr cond while`: evaluates `cond`, and if it's true, evaluates `expr` until `cond` becomes false.

- `expr end start i for`: Evaluates `expr` multiple times with the value of `i` (a variable) from `start` to `end - 1` with increments of 1.

Subroutines

It is possible to define subroutines (which do not take arguments) by associating code with a variable that can then be executed by making a call using `call`:

- `expr f defsub`: Does not evaluate `expr` but associates it with the variable `f`, which can then be called using `call`.

- `f call`: Evaluates the expression associated with `f` (defined using `defsub`).

Additional Features

- `expr print`: Evaluates `expr` and prints the result. Returns the value of `expr`.

- `nop`: Does not perform any operation.
### Requirements
+ In case of an error, the code must raise exceptions consistent with the type of condition that caused the error. It is possible to define specific exceptions or use predefined ones (it is discouraged to raise overly generic exceptions, such as Exception).
+ Each relevant function (i.e., not 2-3 lines) must be adequately commented, explaining its purpose, required inputs, and generated outputs.
+ Each .py file must contain, at the beginning, a comment with the author's name, surname, and student ID.
### Notes
- In the conversion from string to expression, the `dispatch` argument is a dictionary associating strings with the corresponding class to use. For example, `a +` will be associated with `Addition`. This allows for easy construction of the corresponding expression: when encountering `+`, it is checked that the corresponding class has an `arity` attribute equal to 2. Therefore, two elements are removed from the stack, and the object representing the addition is created by passing these two arguments to the constructor.
- The `split()` method available for strings returns an array of strings obtained by splitting the original string on separators (by default, spaces). So, `"a b c".split()` returns `['a', 'b', 'c']`. This can be useful for handling the input string that needs to be transformed into an expression.
- Note that it's possible to force a subclass (if it should work correctly) to implement one of the methods of the base class by raising a `NotImplementedError` exception when the method is called.
