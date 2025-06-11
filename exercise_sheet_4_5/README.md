# Optional Homework 4.5

For the fourth **optional** exercise, write a shared memory parallel implementation of breadth first search (BFS). We will use multiple different graphs to evaluate your implementation, listed below.

| Instance | Vertices | Edges |
| :- | -: | -: |
| road_usa | 23,947,347 | 57,708,624 |
| hugebubbles-00020 | 21,198,119 | 63,580,358 |
| delaunay_n24 | 16,777,216 | 100,663,202 |
| europe_osm | 50,912,018 | 108,109,320 |
| rgg_n_2_24_s0 | 16,777,216 | 265,114,400 |

You can download them directly from the [SuiteSparse Matrix Collection](https://sparse.tamu.edu/), or from our own [heiBOX](https://heibox.uni-heidelberg.de/d/28a5881feafa4b038011/). You can also download smaller instances for testing from the same website.

As before, you should only write code in the file **your_code.c** and implement the function **bfs**. You are free to look at the main.c file, but any changes you make will not be included in the final testing. 

```c
void bfs(const int *V, const int *E, int n, int s, int *D, int *P, void *your_data);
```

The first three parameters **V**, **E**, and **n**, comprise the input graph in the compressed sparse row format. From a graph point of view, the neighborhoods are stored one after the other in **E**, and **V** holds the starting point for each vertex. The following is an example of how to iterate over all neighbors $v \in N(u)$.

```c
for (int i = V[u]; i < V[u + 1]; i++) {
    int v = E[i];
}
```

The next three parameters **s**, **D**, and **P**, are the starting vertex, distance array, and parent array. Your algorithm should compute the shortest path from **s** to all other vertices in the graph. The minimum distance between **s** and some vertex $u$ should be stored in $D[u]$, and $P[u]$ should hold the previous vertex on a shortest path from **s** to **u**. Note that there are several valid results for **P**, but only one correct solution for **D**.

The final argument is for any additional data you want to use. There are two optional functions defined in the **your_code.c** file this time.

```c
void *your_data_init(int n) {
    return NULL;
}

void your_data_free(void *your_data) {
}
```

The first will be called before **bfs** and the second after. The pointer you return from **your_data_init** will be passed as the last argument in the **bfs** call. The number of threads you get in a parallel region will be the same for all three functions. This allows you to allocate and initialize any additional memory you might need during your algorithm without it being counted in the execution time. The free function should release the memory you allocated before.

We have included a sequential version, **seq_bfs.c**, that you can use to get started.

## Compiling and Testing

To test your implementation, compile the project by using
```
make
```
This will create an executable file called **prog** that automatically performs strong scalability experiments on your code. The first command-line argument to **prog** should be the path to the graph you want to use. After that, you can specify the number of threads you want to run by listing the numbers as command-line arguments to the program. For instance, running
```
./prog road_usa.mtx 2 4 6
```
Could generate the following output
```
road_usa.mtx, 23947347 vertices, 28854312 edges
  P,          T,      S_rel
  1,     0.5836,     1.0000
  2,     0.3872,     1.5074
  4,     0.2813,     2.0745
  6,     0.2423,     2.4083
```
Where **P** is the number of threads used, **T** is the time spent in seconds, and **S_rel** is the relative speedup. The program will output a warning if it detects that your solution is not correct. Since this feature adds significant overhead to the running time, it is recommended to disable it for larger instances. We added a **VALIDATE** variable to the main file, changing this to **0** turns off validation.

## The Distributed and Parallel Algorithms Cup

This is the fourth problem included in the **completely optional** implementation challenge for the lecture. If you choose to submit your implementation, we will run your code on one of the large machines (likely an AMD 128-core CPU), and you will be able to see how your code performs. It is strongly recommended to take part, even if you do not intend to compete for first place. Spending time to optimize small parts of code like this is an important learning experience for writing more efficient parallel code. The students in the two best teams (with a maximum of two people per team) will also walk away with the prestigious Algorithm Engineering coffee cup. The rest of the results will be anonymous, so you do not have to worry about getting a bad placement.

### Scoring

As before, the most important metric is the absolute performance (meaning the **T** column from above). For the final scoring, the submissions will be sorted based on parallel running time using the full machine (128 cores). The fastest submission will score 20 points, the second 19, and so on down to 1. You will accumulate points for each optional exercise, and the winner in the end is the one with the most points. We plan to have six optional exercises in total. As mentioned, the first optional exercise gives only half the points.

## Tips and Other Details

For best performance, consider using the following commands once in the terminal you will use to run your code.
```
export OMP_NUM_THREADS=[number of cores on your machine]
export OMP_PLACES=cores
export OMP_PROC_BIND=spread/close
```
Note that this is completely outside the code, and you do not need to think about this for your submission. We will ensure the machine we use is configured for best performance.