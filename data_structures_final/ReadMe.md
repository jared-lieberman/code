## ReadMe for COMP 15 HW6
## 6-Degrees Final Assigment

By: Jared R. Lieberman
Date: 25 April 2016

1.  The purpose of the program is to implement a way to connect students and
    TAs from different courses. It is able to read in data about each person,
    and then retrieve information from that data. It can print the student 
    names, the courses, the courses a certain person TAed, the roster of a 
    certain class, all paths between two people, and the shortest path between
    two people. 

2.  -main.cpp includes functions that hold numerous purposes. The functions 
     read in the data, pass the data to other functions to execute their
     respective functions, find all the paths between two people, find the
     shortest path between two people, and print the paths
    -ClassList.cpp includes functions that makeup the ClassList class. They
     implement an array of courses that includes instances of the class ClassName.
    -ClassList.h uncludes the function declarations for the ClassList class.
    -ClassName.cpp includes functions that makeup the ClassName class. They
     implement a vector of strings for the student names.
    -MasterList.cpp includes functions that makeup the MasterList class. They
     implement a hash table that includes instances of the class Person.
    -MasterList.h includes the function declarations for the MasterList class.
    -Person.cpp includes the functions that makeup the Person class. They 
     implement an array of courses that a person TAed if they are a TA.
    -Person.h includes the function declarations for the Person class
    -hashfunc.cpp includes functions to assist in getting the index to place
     an element in the hash table
    -hashfunc.h includes the function delcarations for hashfunc.cpp

3.  The program should be compiled by using the included Makefile.

4.  The data structures I use are three arrays and a hash table, which is
    implemented as an array. There are four classes, each which contains
    one of these arrays. The MasterList class includes the hash table, which
    hold elements of type Person. The Person class includes an array of strings
    for courses that a person TAed if they were a TA. The ClassList class holds
    an array of strings that include elements of type ClassName. The ClassName
    class holds an array of string for the roster of each class.

5.  Algorithms:
	Much of the algorithm used in implementing the hash table was to 
	a correct index to place an element in the array. This was done
	using linear probing to find an empty element. The expand function
	for the hash table had nested loops that looped through the current
	vector and placed the elements at a new index in the new array. The
	algorithm for finding all paths and the shortest path was a Depth 
	First Search. The function used a stack and recursion to find paths.
	It look at all of the connected neighbors of a certain
	TA and continued to recurse through the function if it continued to
	find TAs. It then returned if a current person was not a TA or had 
	already been passed in that path. The shortest path algorithm used
	numerous helper functions to print prepare the path to be printed in
	the correct order, and then be printed. An integer for the initial 
	shortest path size and initial shortest path were initialized in order
	to create have the variables to be compared to. The shortest path size
	is initialized at 100000000 because there will not be a path that long.

6.  I collaborated with and received assistance with conceptualizing the 
    program from Jordan Stone and Hazen Breen. 



