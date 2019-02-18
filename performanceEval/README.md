## SET UP
First set the MKLROOT, SUITEROOT, METISROOT variables to where you have installed in order MKL, Suitesparse, and Metis libraries.
```bash
mkdir build
cmake ..
make
```


## run
### Simplifying inspector
```bash
simplification path\to\*.mtx
```

### Triangular solve
```bash
triangular path\to\*.mtx numthread numIterations
```


### Incomplete Cholesky 0
```bash
ichol_HLevel path\to\symmetric\matrix.mtx numThreads tune1 tune2
```