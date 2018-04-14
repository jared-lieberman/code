//Person.cpp
//
// This file includes the implementation for the Person class. It includes
// numerous variables for the data structure, an array of strings, and other
// important aspects of the class. It also includes numerous functions to add
// data to the array, retrieve information from it, and print data from 
// it.

#include <cstdlib>
#include "Person.h"


using namespace std;

const string BLANK = "";

//Constructor
Person::Person() {
	courses_taed = new string[INIT_CAPACITY];
	person_name   = BLANK;
	cap          = INIT_CAPACITY;
	current_size = 0;
	is_ta =  false;
	marked = false;
}

//Destructor
Person::~Person() {
	delete [] courses_taed;
}


//Assignment Overload
Person &Person::operator=(const Person &rhs) {
	if (this != &rhs) {
		//copy all data to this
		delete [] this -> courses_taed;
		//need to copy capacity before copying vector
		this -> cap   	     = rhs.cap;
		this -> courses_taed = copylist(rhs.courses_taed);
		this -> person_name  = rhs.person_name;
		this -> current_size = rhs.current_size;
		this -> is_ta        = rhs.is_ta;
		this -> student	     = rhs.student;
		this -> marked	     = rhs.marked;
	}
	return *this;
}

//Copy constructor -- copies a Person to a new instance
Person::Person(const Person &source) {
	courses_taed     = copylist(source.courses_taed);
	person_name	 = source.person_name;
	current_size     = source.current_size;
	cap	         = source.cap;
}

//copylist -- helper function to copy list
//args: a reference to a constant variable of type Person
//rets: none - void function
string *Person::copylist(string *courses_taed) {
	string *new_vector = new string[cap];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = courses_taed[i];
	}
	return new_vector;
}

//set_taed -- function to set variable is_ta to true
//args: none
//rets: none - void function
void Person::set_taed() {
	is_ta = true;
}

//get_taed -- function to return the bool variable is_ta whether or not 
//	      person is a TA
//args: none
//rets: true/false - bool function
bool Person::get_taed() {
	return is_ta;
}

//set_stu -- function to set variable student to true
//args: none
//rets: none - void function
void Person::set_stu() {
	student = true;
}

//is_stu -- function to return the bool variable student whether or not 
//	      person is a student
//args: none
//rets: true/false - bool function
bool Person::is_stu() {
	return student;
}

//get_size -- function to return the an integer for the current size of
//	      the courses_taed array of strings
//args: none
//rets: true/false - bool function
int Person::get_size() {
	return current_size;
}

//add_to_courses_taed -- function to add the name of a course to the 
//			 courses_taed array of strings at the next empty 
//			 element
//args: a string for the class name
//rets: none - void function
void Person::add_to_courses_taed(string class_name) {
	//expand if necessary
	if (current_size == cap) {
		expand();
	}
	courses_taed[current_size] = class_name;
	current_size++;
}

//set_person_name -- function to set variable person_name to a new given string
//args: string for name
//rets: none - void function
void Person::set_person_name(string name) {
	person_name = name;
}

//get_person_name -- function to return the person_name variable
//args: none
//rets: a string for person_name
string Person::get_person_name() {
	return person_name;
}

//print_courses_taed -- function to print each element in the courses_taed
//			array of strings
//args: none
//rets: none - void function
void Person::print_courses_taed() {
	for (int i = 0; i < current_size; i++) {
		cout << courses_taed[i] << endl;
	}
}

//mark -- function to set the marked bool variable to true
//args: none
//rets: none - void function
void Person::mark() {
	marked = true;
}

//unmark -- function to se the marked bool variable to false
//args: none
//rets: none - void function
void Person::unmark() {
	marked = false;
}
	
//is_marked -- function to return a bool whether or not the variable
//	       marked is true
//args: none
//rets: true/false - bool function
bool Person::is_marked() {
	return marked;
}

//get_class_name -- function to return the name of a class in the courses_taed
//		    array at a certain index
//args: int for index
//rets: a string for class name
string Person::get_class_name(int i) {
	return courses_taed[i];
}

//expand -- function to expand array of strings
//args: none
//rets: none - void function
void Person::expand() {
	string *new_vector = new string[(cap * 2) + 1];
	for (int i = 0; i < current_size; i++) {
		new_vector[i] = courses_taed[i];
	}
	delete [] courses_taed;
	courses_taed = new_vector;
	cap = (cap * 2) + 1;
}
		
		
		
		
		