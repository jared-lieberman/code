//Person.h
//
// This file includes the function and variable declarations for the 
// Person class.
//

#include <cstdlib>
#include <iostream>

using namespace std;

class Person
{
public:
	//Constructor
	Person();

	//Destructor
	~Person();

	Person &operator=(const Person &rhs);
	
	Person (const Person &source);
	
	int get_size();
	
	void set_taed();
	
	bool get_taed();

	void set_stu();
	
	bool is_stu();

	void add_to_courses_taed(string class_name);
	
	void print_courses_taed();
	
	void set_person_name(string name);
	
	string get_person_name();
	
	string get_class_name(int i);
	
	void	mark();
	
	void	unmark();
	
	bool	is_marked();

private:
	static const int INIT_CAPACITY = 20;
	string person_name;
	//bool   is_student;
	bool    is_ta;
	bool	student;
	bool	marked;
	string *courses_taed;
	int current_size;
	int cap;
	void expand();
	string *copylist(string *courses_taed);
};