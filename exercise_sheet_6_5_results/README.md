# Optional Homework 5.5 Results

Here are the results for the sixth and final round of the DPA Cup. As promised, the results have been anonymized. To find out how you performed, check your submission feedback on Moodle to view your hidden ID (if you submitted before, your ID will be the same). The entry marked **ref** is the reference implementation. The code for this has been added to the __exercise_sheet_6_5__ folder with the same name.

All experiments were done on an AMD EPYC 9754 128-Core Processor. Each configuration was tested $8$ times, and the fastest run was used. We again use the following instance for testing.

| Instance | Vertices | Edges |
| :- | -: | -: |
| road_usa | 23,947,347 | 57,708,624 |
| hugebubbles-00020 | 21,198,119 | 63,580,358 |
| delaunay_n24 | 16,777,216 | 100,663,202 |
| europe_osm | 50,912,018 | 108,109,320 |
| rgg_n_2_24_s0 | 16,777,216 | 265,114,400 |

## Time

The first set of tables and figures gives the absolute running times in seconds. Having the lowest value in the 128 column is what matters for the competition.

### road_usa

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 2.5648 | 1.3534 | 0.7119 | 0.3845 | 0.2139 | 0.1226 | 0.0749 | 0.0642 |
| 6 | 1.3029 | 0.6624 | 0.3400 | 0.1767 | 0.0997 | 0.0529 | 0.0516 | 0.0269 |
| 10 | 2.3835 | 1.2436 | 0.6378 | 0.3291 | 0.1691 | 0.0887 | 0.0499 | 0.0343 |
| ref | 2.4631 | 1.2526 | 0.6398 | 0.3283 | 0.1749 | 0.0954 | 0.0534 | 0.0344 |

![results](tex/plot-0.png)

### hugebubbles-00020

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 3.7220 | 2.0356 | 1.1020 | 0.5823 | 0.3097 | 0.1691 | 0.0996 | 0.0828 |
| 6 | 0.9534 | 0.7374 | 0.4462 | 0.2535 | 0.1437 | 0.0807 | 0.0789 | 0.0451 |
| 10 | 2.9024 | 1.5759 | 0.8140 | 0.4182 | 0.2116 | 0.1105 | 0.0627 | 0.0487 |
| ref | 2.8475 | 1.5111 | 0.7883 | 0.4038 | 0.2075 | 0.1100 | 0.0615 | 0.0455 |

![results](tex/plot-1.png)

### delaunay_n24

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 3.7022 | 1.9674 | 1.0995 | 0.6066 | 0.3369 | 0.1884 | 0.1072 | 0.0743 |
| 6 | 2.4337 | 1.2578 | 0.6543 | 0.3432 | 0.1725 | 0.0901 | 0.0490 | 0.0268 |
| 10 | 3.1046 | 1.6397 | 0.8445 | 0.4383 | 0.2330 | 0.1245 | 0.0712 | 0.0442 |
| ref | 3.2140 | 1.6806 | 0.9219 | 0.5013 | 0.2680 | 0.1462 | 0.0797 | 0.0455 |

![results](tex/plot-2.png)

### europe_osm

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 6.1919 | 3.4210 | 1.8508 | 1.0010 | 0.5553 | 0.3116 | 0.2014 | 0.1775 |
| 6 | 1.4668 | 0.9037 | 0.5137 | 0.2714 | 0.1517 | 0.0786 | 0.0431 | 0.0265 |
| 10 | 5.0870 | 2.6569 | 1.3957 | 0.7320 | 0.3951 | 0.2157 | 0.1481 | 0.1458 |
| ref | 4.9565 | 2.6227 | 1.3827 | 0.7297 | 0.3925 | 0.2149 | 0.1485 | 0.1496 |

![results](tex/plot-3.png)

### rgg_n_2_24_s0

| ID / P | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
| -: | -: | -: | -: | -: | -: | -: | -: | -: |
| 2 | 8.3903 | 4.2479 | 2.1687 | 1.1235 | 0.5848 | 0.3143 | 0.1770 | 0.1259 |
| 6 | 9.9204 | 4.9551 | 2.4848 | 1.2369 | 0.6130 | 0.3097 | 0.1564 | 0.0802 |
| 10 | 7.2753 | 4.1216 | 2.0959 | 1.0624 | 0.5439 | 0.2855 | 0.1647 | 0.1177 |
| ref | 7.6418 | 3.8285 | 1.9421 | 0.9881 | 0.5048 | 0.2643 | 0.1482 | 0.1161 |

![results](tex/plot-4.png)

## Points Scored

To assign points for this round, we consider each graph to be a separate competition and use the average placement to determine the final list.

| ID | Points | Average Rank |
| -: | -: | -: |
| 6 | 20 | 1 |
| 10 | 19 | 2.4 |
| ref | 18 | 2.6 |
| 2 | 17 | 4 |
