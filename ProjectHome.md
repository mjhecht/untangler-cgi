## Description ##

A non-memory-resident, efficient solver for arbitrary Boggle boards, written in C++. Uses a heap-based algorithm to extract words in alphabetical order, allowing for a single linear pass through a (compressed) dictionary file.

Currently running here: http://mh-z.com/untangle/

## Code Details ##

There's not much documentation at present (read: none), the style's a bit archaic, but I plan to clean things up when I get a chance, create a proper makefile, and so on.

Basically the solver operates on compressed word lists. A command line argument (IIRC) can be used to create a .trie file out of a .txt file, which is much smaller (crunches the dictionary file to about 1/4 it's original size) and so faster for the program to scan through -- I don't think I used a standalone program to generate these, but if I did, I'll upload that as well.