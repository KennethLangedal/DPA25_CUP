# Optional Homework 3.5 Results

Here are the results for round three of the DPA Cup. As promised, the results have been anonymized. To find out how you performed, check your submission feedback on Moodle to view your hidden ID (if you submitted before, your ID will be the same). The entry marked **radix-x** is a Radix sort implementation that uses **x** bits to represent one digit. The code for this algorithm has been added to the __exercise_sheet_3_5__ folder with the same name.

All experiments were done on an AMD EPYC 9754 128-Core Processor. Each configuration was tested $8$ times, and the fastest run was used. Unlike the default value set in the main.c file, we increased $n$ to $2^{27} \approx 134M$ for these experiments.

The first table gives the absolute running times in seconds. Having the lowest value in the 128 column is what matters for the competition.

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 4.7872 | 2.4886 | 1.3757 | 0.8042 | 0.5880 | 0.3796 | 0.2859 | 0.2630 |
| 3 | 5.4047 | 5.0469 | 2.6754 | 1.3897 | 0.7691 | 0.6310 | 0.4216 | 0.3245 |
| 6 | 42.3461 | 22.3697 | 16.2475 | 12.3142 | 15.0417 | 15.1193 | 14.8308 | 15.2567 |
| 8 | 5.4337 | 2.8600 | 1.6059 | 0.9192 | 0.7470 | 0.4424 | 0.2977 | 0.2486 |
| 9 | 4.7100 | 2.5261 | 1.3818 | 0.9104 | 0.8800 | 0.5781 | 0.6112 | 0.9540 |
| 10 | - | - | - | - | - | - | - | - |
| 11 | 9.3529 | 5.0423 | 2.8232 | 1.5215 | 0.9841 | 0.5872 | 0.3717 | 0.2924 |
| 14 | 5.0193 | 2.6419 | 1.4109 | 0.9121 | 0.7147 | 0.4328 | 0.3037 | 0.2672 |
| radix-8 | 5.4198 | 2.8244 | 1.5820 | 1.0528 | 0.8611 | 0.5088 | 0.2883 | 0.1919 |
| radix-1 | 30.2697 | 15.1896 | 7.7118 | 6.0483 | 6.0914 | 3.1291 | 2.7966 | 1.8968 |

The next table gives the relative speedup $T_1 / T_p$ computed individually for each submission.

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 1.0000 | 1.9237 | 3.4798 | 5.9527 | 8.1415 | 12.6112 | 16.7443 | 18.2023 |
| 3 | 1.0000 | 1.0709 | 2.0201 | 3.8891 | 7.0273 | 8.5653 | 12.8195 | 16.6555 |
| 6 | 1.0000 | 1.8930 | 2.6063 | 3.4388 | 2.8152 | 2.8008 | 2.8553 | 2.7756 |
| 8 | 1.0000 | 1.8999 | 3.3836 | 5.9113 | 7.2740 | 12.2823 | 18.2523 | 21.8572 |
| 9 | 1.0000 | 1.8645 | 3.4086 | 5.1736 | 5.3523 | 8.1474 | 7.7062 | 4.9371 |
| 10 | - | - | - | - | - | - | - | - |
| 11 | 1.0000 | 1.8549 | 3.3129 | 6.1472 | 9.5040 | 15.9280 | 25.1625 | 31.9867 |
| 14 | 1.0000 | 1.8999 | 3.5575 | 5.5030 | 7.0229 | 11.5973 | 16.5272 | 18.7848 |
| radix-8 | 1.0000 | 1.9189 | 3.4259 | 5.1480 | 6.2940 | 10.6521 | 18.7992 | 28.2428 |
| radix-1 | 1.0000 | 1.9928 | 3.9251 | 5.0047 | 4.9693 | 9.6736 | 10.8238 | 15.9583 |

## Points Scored

Here are the preliminary points scored for this exercise (awaiting nr. 10).

| ID | Points | Time |
| -: | -: | -: |
| radix-8 | 20 | 0.1919 |
| 8 | 19 | 0.2486 |
| 2 | 18 | 0.2630 |
| 14 | 17 | 0.2672 |
| 11 | 16 | 0.2924 |
| 3 | 15 | 0.3245 |
| 9 | 14 | 0.9540 |
| radix-1 | 13 | 1.8968 |
| 6 | 12 | 15.2567 |
| 10 | 11 | - |
