//ClassName.cpp
//

#include <iostream>
#include <cstdlib>

#include "ClassName.h"

using namespace std;

//Constructor
ClassName::ClassName() {
	students     = new string[INIT_CAPACITY];
	cap	     = 0;
	current_size = 0;
}

//Destructor
ClassName::~ClassName() {
	delete [] students;
}

void ClassName::expand() {
	string new_vector = new string[(cap * 2) + 1];
	for (int i = 0; i < cap; i++) {
		new_vector[i] = students[i];
	}
	delete [] students;
	students = new_vector;
	cap = (cap * 2) + 1];
}