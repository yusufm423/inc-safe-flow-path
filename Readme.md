# Incremental computation of safe flow path

This work is an outcome of my summer research internship under the guidance of Dr. Shahbaz Khan, Asst prof at Deptt of CSE, IIT Roorkee, Roorkee, over a tenure of 2 months from June, 2022 to July, 2022. This repository contains an implementation of the static algorithm, incremental algorithm, a network flow generator and sample input.

## Introduction

Numerous fields, from networking and transportation planning to bioinformatics, can benefit from the decomposition of network flow into weighted paths. In the former, we seek out decomposition, for example, to find number of paths or longest path. On contrary, in bioinformatic applications, we need some underlying data that generated the flow, but for realistic inputs, no optimization criterion is guraranteed to identify the correct decomposition. Here comes the idea to report safe paths, which are subpaths of atleast one path in every flow decomposition as stated in [1].

The aim of this project is to design and implement a dynamic algorithm to verify and enumerate safe path when:
(i) flow is increased along the path.
(ii) new edge is inserted.
We present this simple algorithm to improve the time complexity from $O(m^2n)$ of static algorithm to $O(Δp)$, where $Δp$ is the number of safe path changed due to above operations. We also implement both the static as well as its incremental version in C++ and can compare their run time over randomly generated flow networks.

### Applications

We have seen in [1] a possible application of such partial and reliable solution (safe path) is to consider them as constrains of real RNA transcript assemblers, to guide the assembly process of such heuristics. Another possible application could be to evaluate the accuracy of assemblers (does the output contain safe and complete solution or not).

## Approach towards dynamic algorithm

Before calculating safe path, flow decomposition is done on the input graph to get a set of source-sink path which is done in $O(m)$ time.

### Static Algorithm

Once this is done, each flow decomposition is selected one at a time and a two-pointer approach is used. This is done by using a local property called excess flow fp [1]. Using fp, we can calculate maximal safe path over a flow decomposition until fp>0. When fp<=0, the current path becomes unsafe and the maximal safe path {l,r} is reported, where l and r are pointers to the nodes in flow decomposition. Each flow decomposition takes $O(n)$ time and hence m number of decomposition takes $O(mn)$ time. So if we run such a program over m updates, $O(m^2n)$ time is seen as a result. Hence, we look towards a dynamic approach which will operate on some underlying data and give better time complexities.

### Dynamic Algorithm

Here, we aim to store the excess flow fp as it will determine whether a path is safe or not. fp values are generally stored non-positive as its value is stored after path becomes unsafe, but if it is positive then it means that pointer r has reached the end (sink). In order to do so, we create an array/vector of structure/class named pathinfo to store all the maximal safe path of each flow decomposition. Within each structure/class, a 2D table is used. A row of {l,r,fp} is pushed into the table, where l,r and fp are sufficient conditions to identify a maximal safe path. The idea is that if flow is increased over a flow decomposition or a new edge is added, we can recover the values l,r and fp to verify and further enumerate if the earlier safe path is safe after update or not. **Outflow**: Consider a path $X ={u_1,u_2,...u_k}$ and ${u_i},{u_{i+1}}∈X$, then outflow at i (i is pointing to ${u_i}$) is the sum of all weighted edges $(u_i,v)$, where ${v}$ is any node in graph but ${v≠u_{i+1}}$. <br>
<br>
If we look furhter into this approach, we find that this problem can be further categorized into 3 sub-problems:
<br>(i) right_extension():

```
Require:  user entered flow integer b
1. Retrieve stored values of safe path i.e. l,r,fp.
2. fp = fp + b
3. Repeat until fp>0 and r doesn't cross sink node.
    a. fp = fp - (outflow at r).
    b. Increment r.
4. Store values l,r,fp back into table.
```

<br>(ii) new_subpath():

```
1. Repeat until fp<=0 and r crosses last diverge node
    a. Repeat until fp<=0 :
        i. increment l
        ii. fp = fp - (weighted edge {l-1, l} - (outflow at l)) + (weighted edge {l, l+1})
    b. fp = fp - (outflow at r)
    c. increment r
    d. Repeat until fp>0 and r doesn't cross sink node.
        i. fp = fp - (outflow at r)
        ii. increment r
    e. Traverse over table of maximal path and check if a safe path {l,r} exist
    f. If safe path {l,r} does not exist in table, then save values l,r,fp into the table of maximal safe path and go to step 1.
    g. return
```

<br>(iii) handle_intersecting_path():

```
Require: array diverge[], user entered flow integer b
1. Retrieve stored values of safe path i.e. l,r,fp and store a copy of fp in temporary variable fp'.
2. If diverge is empty:
    a. If weighted edge {l, l+1} was affected due to increment in flow, then use right_extension.
    b. else no change is needed, as it is unrelated path.
    c.return.
3. Check weighted edge {l, l+1} was affected due to increment in flow, fp = fp + b
    a. If number of elements in diverge := 1, then return.
4. fp = fp - (number of elements in diverge) * b
5. If fp>0 and fp'>0 or fp<=0 and fp'<0 :
    a. If fp<=0 and fp'<0 :
        i. If fp + (outflow at r) <=0 :
            I. decrement r
            II. fp = fp + (outflow at r)
            III. go to step I. if fp<=0
            IV. fp = fp - (outflow at r)
            V. Store l,r,fp and call new_subpath().
        ii. else
            I. store value of fp.
            II. call check_for_new_path().
    b. else store value of fp.
6. else
    a. if fp>0:
        i. call right_extension.
    b. else
        i. decrement r
        ii. fp = fp + (outflow at r)
        iii. go to i. if fp<=0
        iv. fp = fp - (outflow at r)
        v. Store l,r,fp and call new_subpath().
```

<br>Any other flow decomposition that is neither an intersecting path nor the updated path itself, such paths remain unchanged and hence are left untouched. By using this approach, we achieve less computation than static algorithm already. This technique helps us to reduce time complexity to the number of safe path changed over m updates = $O(Δp)$.

## Implementation in C++

The above incremental algorithm is implemented using STL by creating a structure for pathinfo (mentioned above). A Trie data structure is also used to store and search for existing flow decomposition.<p>
(i) main(): The main function starts with reading the network flow graph and then calculating flow decomposition. Then calculate_safe_static() function (static algorithm) is run once over all the flow decomposition and all necessary information is stored in the vector of structure. An infinite loop is used to keep the program running until the user commands to terminate it. The user can enter the path P along which flow is increased (The user should keep in mind that the path entered should be from source to sink, else network flow graph's property of flow conservation will violate). Using Trie, P is searched. If P already is an existing flow decomposition, its rank is retrieved else P is added as a new flow decomposition and then rank is given.This also works the same if a new edge is added into the graph. Either way, the amount of flow to be incremented is taken from the user in integer b and array outdegree[] and map of pair w (weights) are updated accordingly. right_extension() function is run for P. For every flow decomposition other than P, divergence is calculated and handle_intersecting_path() is run. After every iteration, check_redundancy() is also run to make sure only maximal safe paths and their data only is stored.</p>
<p>
(ii) flowDecomp(): This function is used to compute flow decomposition for the network flow graph. Depth first search is called until sink is found. Then the minimum flow along this path is subtracted from the weighted graph. This is repeated until all the weights become 0. All the flow decomposition are stored in a vector in vector.</p>
<p>
(iii) check_redundancy(): This function is used to check over the maximal safe path in pathinfo structure for redundant path. It means if two path X and Y exists and Y is a subpath of X, then Y is redundant and such path are deleted.</p>

<p>(iv) divergence(): In [1] we saw that fp and ultimately safe path can be calculated using diverging (subtracting outflows) and converging (subtracting inflows) formulations, but here, we are using diverging formulation. This function stores the pointers to at which updated path P and current processing flow decomposition diverges.
</p>
<p>(v) right_extension(): When flow is increased along a path, this function retrieves the l,r pointers and fp and extends r until fp becomes non-positive or reaches sink node. It then saves values of {l,r,fp}.</p>

<p>(vi) handle_intersectingpath(): This function uses divergence() to find if P intersects (diverges) at any node with current processing path and determines according to above pseudocode whether the safe path shrinks, extends or doesn't change.</p>

<p>(vii) new_subpath() and check_for_new_path(): When a safe path breaks into multiple safe path, these multiple safe path are calculated using these two functions. These functions are called until left pointer l crosses last diverging node.</p>

## Conclusion and Future Works

In this project we saw how we built upon static algorithm and succesfully proposed an incremental algorithm of time complexity $O(Δp)$ by verifying and enumerating maximal safe path, by saving their sufficient conditions, i.e. l,r and fp. The above mentioned algorithm is accurate but has room for improvement left which will be dealt with, in future updates. This work can be further extended to a decremental algorithm for computation of safe flow path, which is out of scope of this project.

## References

[1]S. Khan, M. Kortelainen, M. Cáceres, L. Williams, and A. I. Tomescu, “Safety and Completeness in Flow Decompositions for RNA Assembly,” arXiv.org, Jan. 25, 2022. https://arxiv.org/abs/2201.10372 (accessed Aug. 11, 2022).
