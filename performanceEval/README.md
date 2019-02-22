To install performance evaluation specific requirements, and download the input
matricies used in the evaluations run:

```bash
./getDataAndInstall.sh
```

The script will install the Metis library (requirement), get the input matrices
(listed in Table 3 of the paper), and compile and run the performance drivers.


Run following to reproduce the performance evaluation results:

```bash
./performance execList.txt
```
After the run finishes, the results, Table 4, and Figure 7 and 8 of the paper,
will be in the results directory (performanceEval/results).

