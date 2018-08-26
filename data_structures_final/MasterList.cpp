//MasterList.cpp
//
// This file includes the implementation for the MasterList class. It includes
// numerous variables for the data structure, a hash table, and other
// important aspects of the class. The hash table is implemented as an array of
// variables of type Person. It also includes numerous functions to add data to
// the hash table, retrieve information from it, and print data from it.
//


#include <iostream>
#include <cstdlib>
#include <stdint.h> 

#include "MasterList.h"
#include "hashfunc.h"

using namespace std;

const string BLANK = "";

//Constructor
MasterList::MasterList() {
	masterlist = new Person[INIT_CAPACITY];
	hash_size = INIT_CAPACITY;
	current_size = 0;
}

//Destructor
MasterList::~MasterList() {
	delete [] masterlist;
}

//Assignment Overload
MasterList &MasterList::operator=(const MasterList &rhs) {
	if (this != &rhs) {
		delete [] this -> masterlist;
		this -> hash_size  = rhs.hash_size;
		this -> masterlist = copylist(rhs.masterlist);
		this -> current_size = rhs.current_size;
		
	}
	return *this;
}

//Copy constructor -- copies a MasterList to a new instance
MasterList::MasterList(const MasterList &source) {
	masterlist      = copylist(source.masterlist);
	current_size    = source.current_size;
	hash_size	= source.hash_size;
}

//copylist -- helper function to copy list
//args: a reference to a constant variable of type MasterList
//rets: none - void function
Person *MasterList::copylist(Person *masterlist) {
	Person *new_vector = new Person[hash_size];
	for (int i = 0; i < hash_size; i++) {
		new_vector[i] = masterlist[i];
	}
	return new_vector;
}


//add_to_hash_table -- function to add a person's data to the hash table. This
//		function uses linear probing to search the hash table for an
//		empty element in which to insert the data. 
//args: two string for person_name and course_name, bool if they are ta
//rets: none - void function
void MasterList::add_to_hash_table(string person_name, string course_name, 
				   bool ta) {
	int attempt = 0;
	bool found  = false;
	uint32_t index;
	//expand if necessary
	if (current_size == hash_size)
		expand();
	while (!found) {
		//get index of hash table
		index = (hash_string(person_name) + attempt) % hash_size;
		//if name does not exist yet
		if (masterlist[index].get_person_name() == BLANK) {
			masterlist[index].set_person_name(person_name);
			current_size++;
			add_data(course_name, index, ta);
			//can break here because person was added
			break;
		}
		//if name does exist, add data
		else if (masterlist[index].get_person_name() == person_name) {
			add_data(course_name, index, ta);
			break;
		}
		attempt++;
	}
}

//add_data -- helper function to add certain data to the hash table for the
//	      person. This function checks to see if the person is a TA first.
//args: string for course_name, int for index of hash table, and bool if TA
//rets: none - void function
void MasterList::add_data(string course_name, int index, bool ta) {
	//if they are a TA, then need to set_taed and add course to 
	//courses_taed vector
	if (ta) {
		masterlist[index].add_to_courses_taed(course_name);
		masterlist[index].set_taed();
	} else 
		//set as student
		masterlist[index].set_stu();
}

//print_student -- function to print the students of the entire hash table. This
//		   function loops through the array printing the name of each
//		   student.
//args: none
//rets: none - void function
void MasterList::print_students() {
	for (int i = 0; i < hash_size; i++) {
		string name = masterlist[i].get_person_name();
		//ensure not to print blank names where elements are empty
		if (name != BLANK) {
			cout << name << endl;
		}
	}
}

//print_courses_taed -- function to print the array of courses that a person
//		was a TA for if they were in fact a TA. It uses linear
//		probing to find the person from the hash table.
//args: string for TA name
//rets: none - void function
void MasterList::print_courses_taed(string ta_name) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		//get index of hash table
		int index = (hash_string(ta_name) + attempt) % hash_size;
		//if looked through entire hash table
		if (attempt == hash_size) {
			cout << "Student not found";
			break;
		}
		//if found TA
		if (masterlist[index].get_person_name() == ta_name) {
			masterlist[index].print_courses_taed();
			found = true;
		}
		attempt++;
	}
}

//get_marked -- function to return a bool of whether or not a person is marked
//	This function uses linear probing to find the correct place of the 
//	person in the hash table.
//args: string for student name
//rets: true/false - bool function
bool MasterList::get_marked(string student_a) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		int index = (hash_string(student_a) + attempt) % hash_size;
		//if looked through entire hash table
		if (attempt == hash_size)
			break;
		if (masterlist[index].get_person_name() == student_a) {
			if (masterlist[index].is_marked()) {
				//do not need to set found to true because
				//function ends
				return true;
			}
			//break if the person is not marked
			break;
		}
		attempt++;
	}
	return false;
}

//mark -- function to set the variable for marked to true for a person at a 
//	certain index. This function uses linear probing to find the correct
//	place of the person in the hash table.
//args: string for student name
//rets: none - void function
void MasterList::mark(string student_a) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		//if looked through entire hash table
		if (attempt == hash_size) {
			break;
		}
		int index = (hash_string(student_a) + attempt) % hash_size;
		//found student so can mark and set found to true to end loop
		if (masterlist[index].get_person_name() == student_a) {
			masterlist[index].mark();
			found = true;
		}
		attempt++;
	}
}

//unmark -- function to set the variable for marked to false for a person at a 
//	certain index. This function uses linear probing to find the correct
//	place of the person in the hash table.
//args: string for student name
//rets: none - void function
void MasterList::unmark(string student_a) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		//do not have to check if attempt == hash_size because
		//this function is only called when it is known that the person
		//is marked
		int index = (hash_string(student_a) + attempt) % hash_size;
		//found student so can mark and set found to true to end loop
		if (masterlist[index].get_person_name() == student_a) {
			masterlist[index].unmark();
			found = true;
		}
		attempt++;
	}
}

//unmark_all -- function to set the variable for marked to false for all people
//		in the vector.
//args: none
//rets: none - void function
void MasterList::unmark_all() {
	for (int i = 0; i < hash_size; i++) {
		masterlist[i].unmark();
	}
}
	
//did_ta -- function to return a bool if the person at a certain index in the 
//	hash table did TA. This function uses linear probing to find the 
//	correct place of the person in the hash table.
//args: string for student name
//rets: true/false - bool function
bool MasterList::did_ta(string student_a) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		//if looked through entire hash table
		if (attempt == hash_size) 
			break;
		int index = (hash_string(student_a) + attempt) % hash_size;
		if (masterlist[index].get_person_name() == student_a) {
			if (masterlist[index].get_taed()) {
				//do not need to set found to true because
				//function ends
				return true;
			} 
			//break if the person is not TA
			break;
		}
		attempt++;
	}
	return false;
}

//is_student -- function to return a bool if the eprson at a certain index in
//	the hash table is a student. This function uses linear probing to find
//	the correct place of the person in the hash table.
//args: string for student name
//rets: true/false - vool function
bool MasterList::is_student(string student_b) {
	int attempt = 0;
	bool found = false;
	while (!found) {
		//if looked through entire hash table
		if (attempt == hash_size) 
			break;
		int index = (hash_string(student_b) + attempt) % hash_size;
		if (masterlist[index].get_person_name() == student_b) {
			if (masterlist[index].is_stu()) {
				//do not need to set found to true because
				//function ends
				return true;
			}
			//break if the person is not student
			break;
		}
		attempt++;
	}
	return false;
}

//get_courses_taed_size -- function to return the size of the array of courses
//		taed for a person at a certain index. This function uses
//		linear probing to find the correct place of the person in the
//		hash table.
//args: string for student name
//rets: int for size of array
int MasterList::get_courses_taed_size(string student_a) {
	int attempt = 0;
	bool found = false;
	int size = 0;
	while (!found) {
		int index = (hash_string(student_a) + attempt) % hash_size;
		if (masterlist[index].get_person_name() == student_a) {
			size = masterlist[index].get_size();
			//do not need to set found to true because
			//loop ends
			break;
		}
		attempt++;
	}
	return size;
 }

//get_taed_class_name -- function to return the name of the class that a person
//		TAed. This function uses linear probing to find the correct 
//		place of the person in the hash table. It then uses a getter
//		function to retrieve the name of the class at a certain index.
//args: string for student name and integer for index of class 
//rets: string for name of class
string MasterList::get_taed_class_name(string student_a, int i) {
	int attempt = 0;
	bool found = false;
	string class_name = BLANK;
	while (!found) {
		int index = (hash_string(student_a) + attempt) % hash_size;
		if (masterlist[index].get_person_name() == student_a) {
			class_name = masterlist[index].get_class_name(i);
			//do not need to set found to true because
			//loop ends
			break;
		}
		attempt++;
	}
	return class_name;
}


//expand -- function to expand the hash table. This uses a similar structure
//	as expanding dynamic arrays, but is more extensive. This is because it
//	is necessary to rehash the indices of each element in the current hash
//	table. This is done with nested loops, where the inside loop uses 
//	linear probing to search through the new hash table for an empty
//	element, where it will add that data.
//args: none
//rets: none - void function
void MasterList::expand() {
	Person *new_vector = new Person[(hash_size * 2) + 1];
	hash_size = (hash_size * 2) + 1;
	//loop through current hash table
	for (int i = 0; i < current_size; i++) {
		bool found = false;
		int attempt = 0;
		while (!found) {
			//new index is obtained by getting a hash value of 
			//the person's name in the current hash table
			int index = 
				(hash_string(masterlist[i].get_person_name())
					+ attempt) % hash_size;
			//if found an empty element, add that data
			if (new_vector[index].get_person_name() == BLANK) {
				new_vector[index] = masterlist[i];
				found = true;
			}
			attempt++;
		}
	}
	delete [] masterlist;
	masterlist = new_vector;
}

