//ClassName.h
//
// This file includes the function and variable declarations for the 
// ClassList class.
//

#include <cstdlib>

using namespace std;

class ClassName 
{
public:
	//Constructor
	ClassName();

	//Destructor
	~ClassName();
	
	//add student to students array
	void add_student(string name);
	
	//Copy Constructor
	ClassName (const ClassName &source);
	
	//Assignment Overload
	ClassName &operator=(const ClassName &rhs);
	
	void set_class_name(string course_name);
	
	string get_class_name();
	
	void print_class_roster();
	
	int get_class_size();
	
	string get_stu_name(int j);

	
private:
	string class_name;
	static const int INIT_CAPACITY = 5;
	string *students; //pointer to variable of type string
	bool	is_ta; //bool for whether or not student is a TA
	int 	cap; //capacity of vector
	int	current_size; //int of elements in vector
	void	expand(); //expand function
	string *copylist(string *students); //copylist helper function
};