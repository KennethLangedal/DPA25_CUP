# Optional Homework 3.5

For the third **optional** exercise, write a shared memory parallel implementation of a sorting algorithm of your choice. For this task, we are sorting unsigned 64-bit integers. You may optimize your implementation specifically for this type of input data.

As before, you should only write code in the file **your_code.c** and implement the function **sort**. However, you are free to look at the main.c file, but any changes you make will not be included in the final testing. 

```c
void sort(uint64_t *A, uint64_t n);
```

## Compiling and Testing

To test your implementation, compile the project by using
```
make
```
This will create an executable file called **prog** that automatically performs strong scalability experiments on your code. You can specify the number of threads you want to run by listing the numbers as command-line arguments to the program. For instance, running
```
./prog 2 4 6
```
Could generate the following output
```
Sorting n=33554432 elements
  P,          T,      S_rel
  1,     0.9463,     1.0000
  2,     0.7206,     1.3132
  4,     0.5395,     1.7541
  6,     0.5010,     1.8888
```
Where **P** is the number of threads used, **T** is the time spent in seconds, and **S_rel** is the relative speedup. The program will output a warning if it detects that the list is not sorted in ascending order. Please let us know if you find any bugs with this validator or other parts of the main file. The value of **n** is set as a constant in the main.c file, feel free to change this value during testing, as well as the **NUM_RUNS** constant. For the final testing, we will increase these values to ensure a fair evaluation.

## The Distributed and Parallel Algorithms Cup

This is the second problem included in the **completely optional** implementation challenge for the lecture. If you choose to submit your implementation, we will run your code on one of the large machines (likely an AMD 128-core CPU), and you will be able to see how your code performs. It is strongly recommended to take part, even if you do not intend to compete for first place. Spending time to optimize small parts of code like this is an important learning experience for writing more efficient parallel code. The students in the two best teams (with a maximum of two people per team) will also walk away with the prestigious Algorithm Engineering coffee cup. The rest of the results will be anonymous, so you do not have to worry about getting a bad placement.

### Scoring

As before, the most important metric is the absolute performance (meaning the **T** column from above). For the final scoring, the submissions will be sorted based on parallel running time using the full machine (128 cores). The fastest submission will score 20 points, the second 19, and so on down to 1. You will accumulate points for each optional exercise, and the the winner in the end is the one with the most points. We plan to have six optional exercises in total. As mentioned, the first optional exercise gives only half the points.

## Tips and Other Details

For best performance, consider using the following commands once in the terminal you will use to run your code.
```
export OMP_NUM_THREADS=[number of cores on your machine]
export OMP_PLACES=cores
export OMP_PROC_BIND=spread/close
```
Note that this is completely outside the code, and you do not need to think about this for your submission. We will ensure the machine we use is configured for best performance.