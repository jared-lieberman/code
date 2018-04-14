//ClassList.h
//

#include <cstdlib>

using namespace std;

class ClassList 
{
public:
	//Constructor
	ClassList();

	//Overloaded Constructor
	ClassList(int cap);

	//add to class
	void add(string name, string course, string semester);
	
	//Destructor
	~ClassList();

private:
	struct Person {
		string name;
		string class_sess;
		string course;
		//Person *next;
	};

	static const int INIT_CAPACITY = 5;
	static const int TSIZE = 17;
	Person   *people;//[TSIZE];  // dynamic array of ints
	int       cap; //capacity of vector
	int       current_size; //int of elements in vector
	void      expand(); //function to expand dynamic vector
	
};