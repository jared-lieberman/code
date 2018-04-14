//ClassName.cpp
//
// This file includes the implementation for the ClassName class. It includes
// numerous variables for the data structure, an array, and other important
// aspects of the class. Also, it includes functions to add students to
// the class, to print, and numerous setter and getter functions. 
//

#include <iostream>
#include <cstdlib>
#include <cassert>

#include "ClassName.h"

using namespace std;

//Constructor
ClassName::ClassName() {
	students     = new string[INIT_CAPACITY];
	cap	     = INIT_CAPACITY;
	current_size = 0;
	is_ta	     = false;
}

//Destructor
ClassName::~ClassName() {
	delete [] students;
}

//Assignment overload
ClassName &ClassName::operator=(const ClassName &rhs) {
	if (this != &rhs) {
		//copy all data to this
		delete [] this -> students;
		//need to copy capacity before copying vector
		this -> cap   	     = rhs.cap;
		this -> students     = copylist(rhs.students);
		this -> class_name   = rhs.class_name;
		this -> current_size = rhs.current_size;
		this -> is_ta	     = rhs.is_ta;
	}
	return *this;
}

//Copy constructor -- copies a ClassName to a new instance
ClassName::ClassName(const ClassName &source) {
	students     = copylist(source.students);
	current_size = source.current_size;
	cap	     = source.cap;
}

//copylist -- helper function to copy list
//args: a reference to a constant variable of type ClassName
//rets: none - void function
string *ClassName::copylist(string *students) {
	//cerr << "IN COPYLIST " << cap << endl;
	string *new_vector = new string[cap];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = students[i];
	}
	return new_vector;
}

//set_class_name -- function to set the variable class_name
//args: string for course name
//rets: none - void function
void ClassName::set_class_name(string course_name) {
	class_name = course_name;
}

//get_class_name -- function to return the class_name variable
//args: none
//rets: a string for class_name
string ClassName::get_class_name() {
	return class_name;
}

//add_student -- function to add name to students array at next element
//args: string for name of student
//rets: none - void function
void ClassName::add_student(string name) {
	//check if need to expand
	if (current_size == cap) {
		expand();
	}
	//set name at next element
	students[current_size] = name;
	current_size++;
}

//print_class_roster -- function to print the array of students
//args: none
//rets: none - void function
void ClassName::print_class_roster() {
	//loop to print each element of array
	for (int i = 0; i < current_size; i++) {
		cout << students[i] << endl;
	}
}

//get_class_size -- function return the size of the array
//args: none
//rets: int for current size of array
int ClassName::get_class_size() {
	return current_size;
}

//get_stu_name -- function to return the name of a student and a certain index
//args: int for certain index of array
//rets: string for student name
string ClassName::get_stu_name(int j) {
	return students[j];
}
	
//expand -- function to expand array of strings
//args: none
//rets: none - void function
void ClassName::expand() {
	string *new_vector = new string[((cap * 2) + 1)];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = students[i];
	}
	delete [] students;
	students = new_vector;
	cap = (cap * 2) + 1;
}