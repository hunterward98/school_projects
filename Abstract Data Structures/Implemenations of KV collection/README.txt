Each of these projects were performed in Abstract Data Structures
at Gonzaga University. This class is the third computer science course
for most, and is a pre-requisite for all 300 and 400 level classes.
Most students saw this as a "weed out course" because of the sheer
amount of students that withdraw and change their major. Each
project has it's own testing suite, data files, and are based off
the abstract collection class defined in collection.h.

The purpose of this course is to gain a better understanding of
how classes work, how data structures can perform differently,
how to draw insight from testing, and how to create algorithms
for different data structures to optimize them.

Like most classes, some of these assignments built off each other. For example,
we did a Linked List implementation before adding the different sorting alogrithms,
and we did a vector implementation before adding binary search. Because of this,
I decided to add the end product, or blend the two together, when possible.

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
testing our implementations were correct.
