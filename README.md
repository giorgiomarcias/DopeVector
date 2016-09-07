C++ Dope Vector
===============

This software is a C++11 implementation of [dope vectors](https://en.wikipedia.org/wiki/Dope_vector).

A *dope vector* is a data structure holding information about a data object array, which is typically stored in memory sequentially.
Especially, it provides a layout interface, regardless the effective underlying array memory layout.
Such interface can be changed, meaning that an array can be seen as an array, a 2D NxM matrix, a 2D MxN matrix, or in general a multi-dimensional matrix of any sizes, as long as the total size does not exceed the original array bounds.

A dope vector, hence, is a proxy between some data and users, allowing fast operations, such as extracting windows and slices or making permutations, with easy coding and that would be memory- and time-consuming.
Even if they were not (e.g. working on a window implies just using a subset of indexes), it could be difficult to keep code modularity and independency.  
Suppose you have a function that prints the elements of a matrix:

    void printMatrix(Matrix m);

Unless you pass in the index bounds, this function can not print a window of a matrix.
You are obliged to change the function, or making a copy of the window elements into another matrix object, matching the window size.
With a dope vector, and using them as general matrix representation, it would be sufficient to pass the result of the window extraction method directly in to the print function:

    template < class T, std::size_t D > void print(DopeVector<T, D> m);
    ...
    using Matrix = DopeVector<float,2>;
    Matrix m;
    print(m.window({0, 1}, {3, 4}));

TO ADJUST AND COMPLETE
======================

### License ###
This software is subject to the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0.html) License.
