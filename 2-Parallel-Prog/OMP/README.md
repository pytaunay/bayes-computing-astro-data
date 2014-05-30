## Parallel Programming for Multicore and Distributed Systems
### OpenMP Examples
Compile the example with the provided Makefile. Simply run the executable.
To adjust the number of threads, edit the environment variable OMP_NUM_THREADS. For Bash users:
```bash
export OMP_NUM_THREADS=4
```
For CSH users:
```csh
setenv OMP_NUM_THREADS 4
```

List of examples:
* Sections -- illustrate the usage of the "sections" and "section" directives
* Single -- illustrate the usage of the "single" directive
* Race -- illustrate race conditions. You can check for race conditions with valgrind:
```bash
valgrind --tool=helgrind ./race
```
* Log likelihood -- the calculation of the log likelihood of synthetic data with multiple threads. 
