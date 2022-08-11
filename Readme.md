# Incremental computation of safe flow path
This work is an outcome of my summer research internship under the guidance of Dr. Shahbaz Khan, Asst prof at Deptt of CSE, IIT Roorkee, Roorkee, over a tenure of 2 months (June-July). This repository contains an implementation of the static algorithm, incremental (dynamic) algorithm, a network flow generator and sample input.

**Keywords:** incremental computation ·safety · flow networks · flow decomposition · directed acyclic graphs · RNA assembly

## Introduction
Numerous fields, from networking and transportation planning to bioinformatics, can benefit from the decomposition of network flow into weighted paths. In the former, we seek out decomposition, for example, to find number of paths or longest path. On contrary, in bioinformatic applications, we need some underlying data that generated the flow, but for realistic inputs, no optimization criterion is guraranteed to identify the correct decomposition. Here comes the idea to report safe paths, which are subpaths of atleast one path in every flow decomposition as stated in [1].

The aim of this project is to design and implement a dynamic algorithm to verify and enumerate safe path when:
(i) flow is increased along the path.
(ii) new edge is inserted.
We present this simple algorithm to improve the time complexity from $O(m^2n)$ for static algorithm to O(Δp), where Δp is the number of safe path changed due to above operations. We also implement both the static as well as its incremental version in C++ and compare their run time over randomly generated flow networks.

### Applications
We have seen in [1] a possible application of such partial and reliable solution (safe path) is to consider them as constrains of real RNA transcript assemblers, to guide the assembly process of such heuristics. Another possible application could be to evaluate the accuracy of assemblers (does the output contain safe and complete solution). 

## References
[1]S. Khan, M. Kortelainen, M. Cáceres, L. Williams, and A. I. Tomescu, “Safety and Completeness in Flow Decompositions for RNA Assembly,” arXiv.org, Jan. 25, 2022. https://arxiv.org/abs/2201.10372 (accessed Aug. 11, 2022).