## Parallel Programming for Multicore and Distributed Systems
### MPI Examples
Compile the example with the provided Makefile. Run them with
```bash
mpirun -np [number of cores] [executable] [arguments]
```
e.g.
```bash
mpirun -np 2 ./hello_world
```

List of examples:
* Hello world -- gets all processes to print "Hello".
* Rank and size -- shows how to get the ID of a process and the total number of processes running. 
* Memory example -- illustrates the problems with distributed memory. What happens on rank 1 when trying to access a vector that has been allocated on rank 0 ? 
* Send/Recv -- an example of point-to-point communication.
* Log likelihood -- the calculation of the log likelihood of synthetic data with multiple processes. Contains MPI collective calls. 
