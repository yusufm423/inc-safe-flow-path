# Incremental computation of safe flow path
This work is an outcome of my summer research internship under the guidance of Dr. Shahbaz Khan, Asst prof at Deptt of CSE, IIT Roorkee, Roorkee, over a tenure of 2 months (June-July). This repository contains an implementation of the static algorithm, incremental algorithm, a network flow generator and sample input.

## Introduction
Numerous fields, from networking and transportation planning to bioinformatics, can benefit from the decomposition of network flow into weighted paths. In the former, we seek out decomposition, for example, to find number of paths or longest path. On contrary, in bioinformatic applications, we need some underlying data that generated the flow, but for realistic inputs, no optimization criterion is guraranteed to identify the correct decomposition. Here comes the idea to report safe paths, which are subpaths of atleast one path in every flow decomposition as stated in [1].

The aim of this project is to design and implement a dynamic algorithm to verify and enumerate safe path when:
(i) flow is increased along the path.
(ii) new edge is inserted.
We present this simple algorithm to improve the time complexity from $O(m^2n)$ of static algorithm to $O(Δp)$, where $Δp$ is the number of safe path changed due to above operations. We also implement both the static as well as its incremental version in C++ and can compare their run time over randomly generated flow networks.

### Applications
We have seen in [1] a possible application of such partial and reliable solution (safe path) is to consider them as constrains of real RNA transcript assemblers, to guide the assembly process of such heuristics. Another possible application could be to evaluate the accuracy of assemblers (does the output contain safe and complete solution or not). 

## Approach towards dynamic algorithm
Before calculating safe path, flow decomposition is done on the input graph to get a set of source-sink path which is done in O(m) time.

### Static Algorithm
Once this is done, each flow decomposition is selected one at a time and a two-pointer approach is used. This is done by using a local property called excess flow fp [1]. Using fp, we can calculate maximal safe path over a flow decomposition until fp>0. When fp<=0, the current path becomes unsafe and the maximal safe path {l,r} is reported, where l and r are pointers to the nodes in flow decomposition. Each flow decomposition takes O(n) time and hence m number of decomposition takes O(mn) time. So if we run such a program over m updates, O(m^2n) time is seen as a result. Hence, we look towards a dynamic approach which will operate on some underlying data and give better time complexities.

### Dynamic Algorithm
Here, we aim to store the excess flow fp as it will determine whether a path is safe or not. fp values are generally stored non-positive as its value is stored after path becomes unsafe, but if it is positive then it means that path has ended (we are at the sink). In order to do so, we created a vector of structure pathinfo to store all the maximal safe path of each flow decomposition. Within each structure, a vector of vectors is used. A vector of {l,r,fp} is pushed into the vector, where l,r and fp are sufficient conditions to identify a maximal safe path. The idea is that if flow is increased over a flow decomposition or a new edge is added, we can recover the values l,r and fp to verify and further enumerate if the earlier safe path is safe after update or not.<br>
If we look furhter into this approach, we find that this problem can be further categorized into 2 sub-problems:
<br>(i) right_extension: when flow along a flow decomposition is increased then there are 2 possibilities, either the maximal safe path extends to right (this happens because fp becomes greater than 0) or it doesn't extend,
<br>(ii) handle_intersecting_path: when a flow decompostion intersects the updated flow decomposition, there is a great chance of safe path to break into multiple small safe path (becomes unsafe), else it doesn't change. In the above cases, values of excess flow change regardless of change in "structure" of safe path.
<br>Any other flow decomposition that is neither an intersecting path nor the updated path itself, such paths remain unchanged and hence are left untouched. By using this approach, we achieve less computation than static algorithm already. This technique helps us to reduce time complexity to the number of safe path changed over m updates = $O(Δp)$.

## References
[1]S. Khan, M. Kortelainen, M. Cáceres, L. Williams, and A. I. Tomescu, “Safety and Completeness in Flow Decompositions for RNA Assembly,” arXiv.org, Jan. 25, 2022. https://arxiv.org/abs/2201.10372 (accessed Aug. 11, 2022).