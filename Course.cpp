//ClassList.cpp
//

#include <iostream>
#include <cstdlib>

#include "ClassList.h"

using namespace std;

//Constructor
ClassList::ClassList() {
	/*for (int i = 0; i < TSIZE; i++) {
		people[i]  = NULL;
	}*/
	
	people       = new Person[INIT_CAPACITY];
	cap          = INIT_CAPACITY;
	current_size = 0;
}

//Overloaded Constructor
/*Course::Course(int capacity) {
	people       = new Person[capacity];
	cap	     = capacity;
	current_size = 0;
}*/

//Destructor
ClassList::~ClassList() {
	delete [] people;
	/*for (int i = 0; i < TSIZE; i++) {
		Person *temp = people[i];
		
		while (temp != NULL) {
			Person *prev = temp;
			temp = temp -> next;
			delete prev;
		}
	}*/
}

void ClassList::add(string name, string course, string semester) {
	if (current_size == cap) {
		expand();
	}
	people[current_size].name = name;
	people[current_size].course = course;
	people[current_size].class_sess = semester;
	current_size++;
}
	

void ClassList::expand() {
	Person *new_vector = new Person[(cap * 2) + 1];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = people[i];
	}
	delete [] people;
	people = new_vector;
	cap = (cap * 2) + 1;
}