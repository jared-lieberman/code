//MasterList.h
//
// This file includes the function and variable declarations for the 
// MasterList class.
//

#include <cstdlib>
//#include "hashfunc.h"
#include "Person.h"

using namespace std;

class MasterList 
{
public:
	//Constructor
	MasterList();
	
	//add to hash table
	void add_to_hash_table(string person_name, string course_name, bool ta);
	void add_data(string course_name, int index, bool ta);
	
	//print functions
	void print_students();
	void print_courses_taed(string ta_name);
	
	//Copy Constructor
	MasterList (const MasterList &source);
	
	//Assignment Overload
	MasterList &operator=(const MasterList &rhs);
	
	//mark functions
	bool get_marked(string student_a);
	void mark(string student_a);
	void unmark(string student_a);
	void unmark_all();
	
	bool did_ta(string student_a);
	
	bool is_student(string student_b);
	
	int get_courses_taed_size(string student_a);
	
	string get_taed_class_name(string student_a, int i);
	
	//Destructor
	~MasterList();

private:
	Person *masterlist;
	static const int INIT_CAPACITY = 20;
	int hash_size;
	int current_size;
	void expand();
	Person *copylist(Person *masterlist);
};
	
		