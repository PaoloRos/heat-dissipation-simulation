# Heat Dissipation Project

**Advanced Programming Course**, _Università di Trento_

**Author:** Paolo Rossi

**Date:** 07/06/2025

## Documentation

[View the documentation](https://paoloros.github.io/heat-dissipation-simulation/).

## Usage Instructions

1. **Build:** compile the project from the command line using one of these targets:
    ```
    make seq # sequential code
    make par # parallel code
    ```
    These commands generate the corresponding executable.

2. **Quick test:** verify correct behavior from the command line:
    ```
    ./seq 32     # sequential code
    ./par 32 4   # parallel code
    ```
    This runs the computation on a 32x32 matrix.

3. **Data collection:** run from the command line:
    ```
    python3 SEQ_launcher.py [num_matrices] # sequential measurements
    python3 PAR_launcher.py [num_matrices] # parallel measurements
    ```
    Here `[num_matrices]` is the number of matrix sizes to measure. Matrix sizes are powers of two starting from 32.

4. **Plot generation:** run from the command line:
    ```
    make fig f=[output_file]
    ```
    Replace `[output_file]` with the name of the produced output file.

5. **Cleanup:** remove executables and generated text/csv files:
    ```
    make clean
    make ctxt
    ```

## Doxygen Documentation

To generate technical documentation (headers + implementations), run:

```
make docs
```

Main HTML output:

- `docs/index.html`

The documentation includes:

- project purpose (sequential vs parallel comparison)
- physical model overview
- DOT diagrams for module dependencies and solver workflow

## Measurement Report

Full measurement report and analysis:

- [HD_PAOLO_ROSSI.pdf](HD_PAOLO_ROSSI.pdf)