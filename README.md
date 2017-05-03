# PageRank MPI

Takes a large graphfile as input and splits the graph into several subgraphs, depending on the second file that needs to get provided.
Calculates the pagerank of every node for a specified amount of rounds and dumps the result in several output files.
Uses MPI to calculate several subgraphs simultaneously.

Takes the following input:

__Graphfile:__

A list of all node and their neighbors, e.g.:

| Node          | Neighbor      |
| ------------- |:-------------:|
| 1             | 4             |
| 2             | 7             |
| 3             | 12            |
| 1             | 17            |
| 1             | 10            |
| 5             | 45            |
| ...           | ...           |

__Partitionfile:__

A list of Node IDs, Node degrees and the process ID the subgraph is going to run on. E.g.:

| Node ID       | Node Degree   | Partition ID  |
| ------------- |:-------------:| -------------:|
| 1             | 14            | 1             |
| 2             | 2642          | 1             |
| 3             | 1234          | 0             |
| 4             | 1             | 0             |
| 5             | 345           | 1             |
| ...           | ...           | ...           |

__Rounds:__

Amount of rounds you want PageRank to run for

__Compile with:__
```bash
mpic++ -std=c++11 -O3 (optional, maximum optimization level) pagerank.cpp -o pagerank
```
__Run with:__
```bash
mpiexec -np [amount of processes to run on] ./pagerank [Graphfile] [Partitionfile] [Rounds]
```

- Calculates pagerank of every node. 
- Additionally gives information about the total execution time. 
- The output is saved in several files (depending on the amount of processes).
- Every process saves its subgraph in a file called pagerank.out.[processID].
