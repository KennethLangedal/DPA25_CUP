# Optional Homework 6.5

For the sixth and final optional exercise, write a shared memory implementation of the Jones-Plassmann parallel coloring algorithm. We will use multiple different graphs to evaluate your implementation, including the ones listed below.

| Instance | Vertices | Edges |
| :- | -: | -: |
| road_usa | 23,947,347 | 57,708,624 |
| hugebubbles-00020 | 21,198,119 | 63,580,358 |
| delaunay_n24 | 16,777,216 | 100,663,202 |
| europe_osm | 50,912,018 | 108,109,320 |
| rgg_n_2_24_s0 | 16,777,216 | 265,114,400 |

You can download them directly from the [SuiteSparse Matrix Collection](https://sparse.tamu.edu/), or from our own [heiBOX](https://heibox.uni-heidelberg.de/d/28a5881feafa4b038011/). You can also download smaller instances for testing from the same website.

```c
void JP(const int *V, const int *E, int n, const int *P, int *C, void *your_data);
```

As in the previous exercises, your coloring algorithm should take the graph in the compressed sparse row format. The first three parameters **V**, **E**, and **n**, contains the input graph. From a graph point of view, the neighborhoods are stored one after the other in **E**, and **V** holds the starting point for each vertex. The following is an example of how to iterate over all neighbors $v \in N(u)$.

```c
for (int i = V[u]; i < V[u + 1]; i++) {
    int v = E[i];
}
```

The input array **C** should hold the coloring after your function finishes. The array **C** has been initialized with all -1, and you should use the color classes starting from zero, that is, 0, 1, 2, etc.

This is a greedy coloring algorithm, and you should color each vertex with the lowest available color in the order defined by **P**. That is, vertices with higher values in **P** should be colored first. To break ties, let the vertex with the lowest ID take priority. To be clear, the input array **P** is given, and you should color the vertices accordingly. The purpose of this assignment is to have a fast implementation of Jones-Plassmann, not to improve the coloring quality. Your algorithm should be deterministic and always produce a valid coloring using the same number of colors (depending on **P**). 

The final argument is for any additional data you want to use. There are two optional functions defined in the **your_code.c** file.

```c
void *your_data_init(int n) {
    return NULL;
}

void your_data_free(void *your_data) {
}
```

The first will be called before **JP** and the second after. The pointer you return from **your_data_init** will be passed as the last argument in the **JP** call. The number of threads you get in a parallel region will be the same for all three functions. This allows you to allocate and initialize any additional memory you might need during your algorithm without it being counted in the execution time. The free function should release the memory you allocated before.

As before, you should only write code in the file **your_code.c** and at least implement the function **JP**. You are free to look at the main.c file, but any changes you make will not be included in the final testing. 

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
road_usa.mtx, 23947347 vertices, 28854312 edges
  P,          T,      S_rel,          C
  1,     2.5894,     1.0000,          5
  2,     1.3191,     1.9629,          5
  4,     0.7050,     3.6729,          5
  8,     0.3969,     6.5238,          5
 16,     0.2677,     9.6719,          5
```
Where **P** is the number of threads used, **T** is the time spent in seconds, **S_rel** is the relative speedup, and **C** is the number of colors your coloring used. The program will output a warning if it detects that your solution is not correct.

## The Distributed and Parallel Algorithms Cup

This is the final problem included in the **completely optional** implementation challenge for the lecture. If you choose to submit your implementation, we will run your code on one of the large machines (likely an AMD 128-core CPU), and you will be able to see how your code performs. It is strongly recommended to take part, even if you do not intend to compete for first place. Spending time to optimize small parts of code like this is an important learning experience for writing more efficient parallel code. The students in the two best teams (with a maximum of two people per team) will also walk away with the prestigious Algorithm Engineering coffee cup. The rest of the results will be anonymous, so you do not have to worry about getting a bad placement.

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