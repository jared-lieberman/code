//ClassList.h
//
// This file includes the function and variable declarations for the 
// ClassList class.
//

#include <cstdlib>
#include "ClassName.h"


using namespace std;

class ClassList 
{
public:
	//Constructor
	ClassList();

	//Overloaded Constructor
	ClassList(int cap);

	//add to class list
	void add_student_to_class(string name, string course_name);
	void add_ta_to_class(string course_name);
	
	//print functions
	void print_courses();
	void print_roster(string course_name);
	
	int get_roster_size(string class_taed);
	
	string get_stu_name(string class_taed, int j);
	
	//Destructor
	~ClassList();

private:
	//string class_name;
	ClassName *classlist; //pointer to variable of type ClassName
	static const int INIT_CAPACITY = 10;
	int       cap; //capacity of vector
	int       current_size; //int of elements in vector
	void      expand(); //function to expand dynamic array
};