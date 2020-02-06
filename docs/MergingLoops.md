
# How to merge loops

This document is intended to describe our *compile-time-only* loop merger.

Merging loops requires a few tricks. We can accomplish this by requiring
strict structures and having helper functions that transform normal `looper`
objects to a `looper_union`.

## Structure for already merged loops

What structure (i.e. template parameters) do we expect?
Assume that we have to following ranges for different loops 1 through 5.

```
Ranges [start, end] (inclusive)
0: 100, 200
1: 0,   205
2: 0,   201
3: 2,   200
4: 0,   205
5: 0,   100
```

We now have to create pairs of ranges and the loops to be executed.
For the loops above we get the following structure:

```
Ranges [start, end] (inclusive)
0-1       1,2,5
2-100     1,2,3,5
101-200   0,1,2,3,4
201-201   1,2,4
202-205   1,4
```

This is essentially just a bunch of `looper` objects. So we have to implement a way in `looper_union` so that we can create new `looper` objects.

## How to merge two loops
There are following possible cases for intersecting loop ranges:

```
1. Same:
[-----------]
[-----------]

2. Disjunct:
[-----------]
             [-----------]

3. Disjunct:  
             [-----------]
[-----------]

4. Superset:
    [-----------]
[------------------]

5. Superset with same start:
[-----------]
[------------------]

6. Superset with same end:
       [-----------]
[------------------]

7. Subset with same start:
[-----------------]
[-----------]

8. Subset with same end:
[-----------------]
     [------------]

9. Subset:
[-------------------]
    [-----------]

10. Partially intersected
[-----------]
     [-----------]

11. Partially intersected
     [-----------]
[-----------]
```

Based on the case 1 through 11 we can then create a new looper_union type with
the correct ranges. The reason we differentiate between e.g. 8 and 9 is that we
want to avoid unnecessary ranges, e.g. `[10, 10)`.
