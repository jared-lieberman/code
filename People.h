//ClassName.h
//

#include <stdlib>

using namespace std;

Class ClassName 
{
public:
	//Constructor
	ClassName();

	//Destructor
	~ClassName();

	
private:
	static const int INIT_CAPACITY = 5;
	string *students[INIT_CAPACITY];
	int 	cap = 0; //capacity of vector
	int	current_size = 0; //int of elements in vector
	void	expand(); //expand function
}