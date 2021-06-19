# libigl example project with multiple executables

This extension of the [libigl example
project](https://github.com/libigl/libigl-example-project) demonstrates how to
set up a CMake project using libigl that has multiple executables linked to the
same project-specific "core" functionality (in this case the function `func`
which prints `hello`).


    mkdir build
    cmake ../
    make 

will build libcore.* library (containing the compiled `func`) and `example1` and
`example2` executables.

Changing the implementation inside `func.cpp` and then reissuing

    make

will recompile and link libcore.*, but only re-link `example1` and `example2`.

Changing the implementation inside `example1.cpp` and `example2.cpp` and then
issuing:

    make example2

Will only recompile and relink `example2`.
