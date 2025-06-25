# Optional Homework 5.5

For the fifth **optional** exercise, write a shared memory parallel program for maximum cardinality matching. You can use any algorithm you want to solve this task. We will use multiple different graphs to evaluate your implementation, listed below.

| Instance | Vertices | Edges |
| :- | -: | -: |
| road_usa | 23,947,347 | 57,708,624 |
| hugebubbles-00020 | 21,198,119 | 63,580,358 |
| delaunay_n24 | 16,777,216 | 100,663,202 |
| europe_osm | 50,912,018 | 108,109,320 |
| rgg_n_2_24_s0 | 16,777,216 | 265,114,400 |

You can download them directly from the [SuiteSparse Matrix Collection](https://sparse.tamu.edu/), or from our own [heiBOX](https://heibox.uni-heidelberg.de/d/28a5881feafa4b038011/). You can also download smaller instances for testing from the same website.

As in the previous weekly exercise, your matching function should take the bipartite input graph $G = (X \cup Y)$ on the compressed sparse
row format. The partition $X$ consists of the first $nx$ vertices, followed by vertices in the partition $Y$ (of size $ny$). The vertices in $X$ have the IDs 0 to $nx − 1$, while the vertices in $Y$ have IDs starting
at $nx$ up to $nx + ny − 1$. Additionally, the function should take the pre-allocated array $M$ as input, which is an integer array of length $nx + ny$, initialized with -1. If an edge $\{u, v\}$ belongs to the matching,
you should set $M [u] = v$ and $M [v] = u$.

As before, you should only write code in the file **your_code.c** and implement the function **maximum_matching**. You are free to look at the main.c file, but any changes you make will not be included in the final testing. 

```c
void maximum_matching(const int *V, const int *E, int nx, int ny, int *M, void *your_data)
```

The final argument is for any additional data you want to use. There are two optional functions defined in the **your_code.c** file.

```c
void *your_data_init(int n) {
    return NULL;
}

void your_data_free(void *your_data) {
}
```

The first will be called before **maximum_matching** and the second after. The pointer you return from **your_data_init** will be passed as the last argument in the **maximum_matching** call. The number of threads you get in a parallel region will be the same for all three functions. This allows you to allocate and initialize any additional memory you might need during your algorithm without it being counted in the execution time. The free function should release the memory you allocated before.

## Compiling and Testing

To test your implementation, compile the project by using
```
make
```
This will create an executable file called **prog** that automatically performs strong scalability experiments on your code. The first command-line argument to **prog** should be the path to the graph you want to use. After that, you can specify the number of threads you want to run by listing the numbers as command-line arguments to the program. For instance, running
```
./prog road_usa.mtx 2 4 8 16
```
Could generate the following output
```
road_usa.mtx, 47894694 vertices, 57708624 edges
  P,          T,      S_rel,          M
  1,    86.4945,     1.0000,   22725305
  2,    45.2269,     1.9125,   22725305
  4,    26.8181,     3.2252,   22725305
  8,    18.0379,     4.7951,   22725305
 16,    13.8901,     6.2271,   22725305
```
Where **P** is the number of threads used, **T** is the time spent in seconds, **S_rel** is the relative speedup, and **M** is the size of your matching. The program will output a warning if it detects that your solution is not correct.

## The Distributed and Parallel Algorithms Cup

This is the fifth problem included in the **completely optional** implementation challenge for the lecture. If you choose to submit your implementation, we will run your code on one of the large machines (likely an AMD 128-core CPU), and you will be able to see how your code performs. It is strongly recommended to take part, even if you do not intend to compete for first place. Spending time to optimize small parts of code like this is an important learning experience for writing more efficient parallel code. The students in the two best teams (with a maximum of two people per team) will also walk away with the prestigious Algorithm Engineering coffee cup. The rest of the results will be anonymous, so you do not have to worry about getting a bad placement.

### Scoring

As before, the most important metric is the absolute performance (meaning the **T** column from above). As in the previous graph exercise, we will rank submissions on each graph individually and use the average placement to decide the final ranking. Since these graphs are really large, it is recommended to work with smaller instances during development.

## Tips and Other Details

For best performance, consider using the following commands once in the terminal you will use to run your code.
```
export OMP_NUM_THREADS=[number of cores on your machine]
export OMP_PLACES=cores
export OMP_PROC_BIND=spread/close
```
Note that this is completely outside the code, and you do not need to think about this for your submission. We will ensure the machine we use is configured for best performance.