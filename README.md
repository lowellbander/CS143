# CS 143: Database Systems
### Fall 2014 - Professor John Cho
##### Authors: Lowell Bander, Akshay Bakshi

# Usage Notes for Project 2

* call `write()` after inserting into any node
* set the pid of the cursor to the pid of the root node before calling `index::locate()`
* whenever `index::locate()` is called, except by itself, the 3rd argument `depth` should be `ROOT_DEPTH`.
* the maximum number of keys in a `BTNonLeafNode` is 85
