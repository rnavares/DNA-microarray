# DNA-microarray
C implementation to Simulate the assembly of DNA sequences to use the traveler's problem

DNA microarrays (also known as DNA chips) are a collection of microscopic DNA fragments attached to a solid surface such as a glass, a plastic or a silicon chip. The purpose of these mircroarrays is the study of the genetic expression of living beings: What genes are part of a cell of an organism, at a given time and under particular conditions? [details](https://en.wikipedia.org/wiki/DNA_microarray#Statistical_analysis)

This type of studies are widely used in areas such as biology or medicine for research and study of certain types of diseases.

1 .- Simulate the assembly of DNA sequences using the problem of the traveler (TSP). The final goal of this assembly process lies in obtaining the shortest common macrosequence of DNA that contains all the assembled sequences.

2.- Solve the problem of obtaining Eulerian roads in a graph. This functionality will be used later to simulate the process of recovering DNA chains from its spectrum.

3.- Simulate the genetic processes of:

Generation of a spectrum of length l from a DNA sequence.
Recovery of a DNA sequence from its spectrum using the algorithm for obtaining Eulerian paths.

Requirements
```
CFLAGS = -ansi -Wall -pedantic

COMPILER = gcc
IDIRS = -I /usr/include
LDIRS = -L /usr/lib
LDIRS = -L .

```
A makefile was included. To compile:
```
$make
```

Output: main_euler

```
ricardo@ricardo-XPS13-9333:~ ./main_euler 10 5
=== SEQUENCE ===
long: 10	 data:CGCCCAGCGT
=== SPECTRUM ===
Num_mers:  6
long_mers: 5
CCCAG
CAGCG
CCAGC
GCCCA
AGCGT
CGCCC
=== SPECTRUM ===
Num_mers:  7
long_mers: 4
CCCA
CCAG
CAGC
AGCG
GCCC
GCGT
CGCC

=== GRAPH ===
7
6
0 1 1
1 2 1
2 3 1
3 5 1
4 0 1
6 4 1


=== EULER PATH ===
 6->  4->  0->  1->  2->  3->  5

=== SEQUENCE ===
long: 10	 data:CGCCCAGCGT

```


