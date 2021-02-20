# Allocator

Since the question was raised around me recently, I looked into memory allocation algorithms. In this repository, I have implemented a simple __First Fit__ allocation algorithm based on a __Circular Single Linked List__. The challendge here to implement this allocation mecanism without using a preexisting `malloc`. Instead here there is a limit on how many allocations can occur at once.

## How it works

For some resources on memory allocation algorithms, see [this article from the Boost project](https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/interprocess/memory_algorithms.html) or [this article from memorymanagement.org](https://www.memorymanagement.org/mmref/alloc.html).

Basically, the memory will be represented with _segments_ that have a start address and a length. Those segments can be merged or split into smaller segments. This is enough to find out if memory can be allocated and where, and it also avoid arbitrary fragmentation of the memory because segments can be merged.

The segments are then sorted by ascending start address in a circular linked list, which simplifies merging adjacent segments when memory is set free. Single or double linked list can be used, but this repository uses single linked list. From this experience, I would say that a double linked list would likely be a better choice, likely worth the overhead.

## Disclaimer

> Even for a side project, this codebase is kind of messy, as I had not written complex C code in a while and forgot how map VS copy of structs worked in C. Also, I didn't care too much, as the code won't be used by anyone and I was more interested in learning the algorithm itself.

### METADATA

Field | Value
--- | ---
:pencil: Contributors | roadelou
:email: Contacts | 
:date: Creation Date | 2021-02-20
:bulb: Language | Markdown Document

### EOF
