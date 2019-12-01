Each of these assignments were performed in CPSC 223 at Gonzaga University.
CPSC223 is the third computer science class for most students and focuses
on Abstract Data Structures and Algorithms. Each of these assignments has
a testing suite, data inputs, and are based off the abstract collection class
outlined in the file collection.h.

I did not intend for these to be ran, but for you to just look at. If you
wish to run them, read how I did it at the bottom.

Some of these assignments built off each other. For example, we did a
Linked List implementation before adding the different sorting alogrithms,
and we did a vector implementation before adding binary search. Because of this,
I decided to add the end product, or blend the two together.

All of these were compiled using CMake. The appropriate CMake files are
included, but in general, you need the following files if you need to
compile these yourself:
			The files listed in CMakeLists.txt
			collection.h
			testdriver.h (if doing performance testing, this was provided by instructor)
			And the header file with the implementation itself.
			These have the convention <implementation>_collection.h


To run and test these assignments, I compiled and executed them from
the command line in Linux. Since these are not projects with more than a
few files, the command line is sufficient enough. Our professor has
instructed us to use CMake, and provided us with the file called
"CMakeLists.txt" for our first assignment. With this, we were able to
compile by typing "cmake CMakeLists.txt" on the command line, and it
then creates usually two executables. One for performance, and one for
testing.