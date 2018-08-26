//ClassList.cpp
//
// This file includes the implementation for the ClassList class. It includes
// numerous variables for the data structure, an array, and other important
// aspects of the class. Also, it includes functions to add students and TAs
// to a certain class and functions to print.
//

#include <iostream>
#include <cstdlib>

#include "ClassList.h"

using namespace std;

const string BLANK = "";

//Constructor
ClassList::ClassList() {
	classlist    = new ClassName[INIT_CAPACITY];
	cap          = INIT_CAPACITY;
	current_size = 0;
}

//Destructor
ClassList::~ClassList() {
	delete [] classlist;
}

//add_student_to_class -- function to add a student to a certain class in the
//		array of classes sessions. This calls functions from the 
//		ClassList class at certain indices in order to add the data.
//args: two strings for the student name and course name
//rets: none - void function
void ClassList::add_student_to_class(string name, string course_name) {
	bool class_made = false;
	//look if class already exhists in class list
	for (int i = 0; i < current_size; i++) {
		if (classlist[i].get_class_name() == course_name) {
			classlist[i].add_student(name);
			class_made = true;
			break;
		}
	}
	//create new class and add student if class does not exist
	if (!class_made) {
		if (current_size == cap) {
			expand();
		}
		classlist[current_size].set_class_name(course_name);
		classlist[current_size].add_student(name);
		current_size++;
	}
}

//add_ta_to_class -- function to add a TA's to the array of class sessions.
//		This calls functions from the ClassList class at certain 
//		indices in order to add the data. This functions is special
//		because it does not add the name of the TA to the class 
//		session. Instead, it creates a new class if it does not
//		exist and then does not add the TA to the class because that
//		only includes the names of students.
//args: a string for the course name
//rets: none - void function 
void ClassList::add_ta_to_class(string course_name) {
	bool class_made = false;
	//look if class already exhists in class list
	for (int i = 0; i < current_size; i++) {
		if (classlist[i].get_class_name() == course_name) {
			class_made = true;
			break; 
		}
	}
	//create new class but do not add TA if class does not exist
	if (!class_made) {
		if (current_size == cap) {
			expand();
		}
		classlist[current_size].set_class_name(course_name);
		current_size++;
	}
}

//print_courses -- function to print the class list. This calls a getter
//		function to access the name of each class.
//args: none
//rets: none - void function
void ClassList::print_courses() {
	for (int i = 0; i < current_size; i++) {
		string name = classlist[i].get_class_name();
		cout << name << endl;
	}
}

//print_roster -- function to print the list of students in a certain class. 
//		Inside each class session element is an instance of ClassName,
//		which holds an array of student names. This is the roster of a
//		certain class. This function prints that roster.
//args: a string for the course name
//rets: none - void function
void ClassList::print_roster(string course_name) {
	//loop to look for class
	for (int i = 0; i < current_size; i++) {
		if (classlist[i].get_class_name() == course_name) {
			classlist[i].print_class_roster();
			return;
		}
	}
	//print if the class is not found
	cout << "Course not found" << endl;
}

//get_roster_size -- function to return the size of a certain class session. 
//args: a string for the class session
//rets: int for size of class roster
int ClassList::get_roster_size(string class_taed) {
	int roster_size = 0;
	//loop through class list looking for the class
	for (int i = 0; i < current_size; i++) {
		if (classlist[i].get_class_name() == class_taed) {
			//sets variable to size of roster array
			roster_size = classlist[i].get_class_size();
		}
	}
	return roster_size;
}

//get_stu_name -- function to return the name of a student in the roster of
//		a certain class.
//args: string for class session and integer for element of roster
//rets: string for name of student 
string ClassList::get_stu_name(string class_taed, int j) {
	string stu_name = BLANK;
	for (int i = 0; i < current_size; i++) {
		if (classlist[i].get_class_name() == class_taed) {
			stu_name = classlist[i].get_stu_name(j);
		}
	}
	return stu_name;
}
	
//expand -- function to expand the list of class sessions
//args: none
//rets: none - void function
void ClassList::expand() {
	ClassName *new_vector = new ClassName[(cap * 2) + 1];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = classlist[i];
	}
	delete [] classlist;
	classlist = new_vector;
	cap = (cap * 2) + 1;
}