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

Output

