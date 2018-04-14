//
// hw6 main.cpp
//
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include <stack>

#include "ClassList.h"
#include "MasterList.h"

using namespace std;

void read_file(char *argv[], ClassList &clist, MasterList &mlist);
void send_to_ds(ClassList &clist, MasterList &mlist, int i, 
		string name, string course_name);
void readCommand(istream &input, ClassList &clist, MasterList &mlist);
void paths_sender(istream &input, ClassList &clist, MasterList &mlist,
		  string command);
void all_paths(ClassList &clist, MasterList &mlist, string student_a,
				string student_b, stack<string> &path);
void path_recurse_all(ClassList &clist, MasterList &mlist, string student_a,
		  string class_taed, string next_stu, string student_b, 
		  stack<string> &path);
void shortest_path(ClassList &clist, MasterList &mlist, string student_a,
				string student_b, stack <string> &path);
void path_recurse_short(ClassList &clist, MasterList &mlist, string student_a,
		  string class_taed, string next_stu, string student_b, 
		  stack<string> &path);
void print_paths(stack <string> path);
void find_shortest(stack <string> short_path);
void print_short_path(stack <string> &path);
const string BLANK = "";
int short_path_size = 100000000; //initialize size of shortest path
stack <string> short_path;


int main(int argc, char *argv[]) {
	ClassList clist;
	MasterList mlist;
	read_file(argv, clist, mlist);
	readCommand(cin, clist, mlist);

}

//read_file -- function to read in the date from a file and organize it 
//	       based on a person's name and the class session in which
//	       they were a student or TA. The function assumes that two files
//	       will be read in, the first for students and the second for TAs
//args: a pointer to the array of chars of command-line args, and references
//	to variable of type ClassList and MasterList
//rets: none - void function
void read_file(char *argv[], ClassList &clist, MasterList &mlist) {
	ifstream finput;
	if (finput.fail() ) {
		cerr << "Cannot open " << endl;
		exit(1);
	}
	string line;
	for (int i = 1; i < 3; i++) {
		finput.open(argv[i]);
		while (!getline(finput, line).eof()) {
			string name = BLANK;
			string course_name   = BLANK;
			int j = 0;
			//add chars to name
			while (line[j] != ':') {
				name += line[j];
				j++;
			}
			j++;
			//add chars to class_name unless it's a colon
			while (line[j] != '\0') {
				if (line[j] != ':') 
					course_name += line[j];
				j++;
			}
			send_to_ds(clist, mlist, i, name, course_name);
		}
		finput.close();
	}
}

//send_to_ds -- function to add data that is read in to the data structures,
//		which are included in the instances of ClassList and 
//		MasterList. The function is passed a integer, which 
//		distinguishes between the student and TA files. 
//args: two references to instances of ClassList and MasterList, an int,
//	and two strings for name of person and course
//rets: none - void function
void send_to_ds(ClassList &clist, MasterList &mlist, int i, 
		string name, string course_name) {
	if (i == 1) {
		clist.add_student_to_class(name, course_name);
		mlist.add_to_hash_table(name, course_name, false);
	}
	if (i == 2) {
		clist.add_ta_to_class(course_name);
		mlist.add_to_hash_table(name, course_name, true);
	}
}

//readCommand -- function to read in a certain command from the command line
//		 and call helper functions to execute certain tasks on the data
//args: the filestream, and two references to instances of ClassList
//	and MasterList
//rets: none - void function
void readCommand(istream &input, ClassList &clist, MasterList &mlist) {
	while (!input.eof()) {
		string command;
		input >> command;
		if (!input.eof()) {
			if (command == "ls") {
				mlist.print_students();
			}
			if (command == "lc") {
				clist.print_courses();
			}
			if (command == "taed") {
				string ta_name;
				input >> ta_name;
				mlist.print_courses_taed(ta_name);
			}
			if (command == "roster") {
				string course_name;
				input >> course_name;
				clist.print_roster(course_name);
			}
			if (command == "paths" or command == "shortestpath")
				paths_sender(input, clist, mlist, command);
			if (command == "EOF") {
				break;
			}
		}
	}
}

//paths_sender -- function to send data to helper functions to exectute tasks
//		  to find all paths between two people or find the shortest
//		  path between two people.
//args: the filestream, two references to instances of ClassList and
//	MasterList, and the string command
//rets: none - void function
void paths_sender(istream &input, ClassList &clist, MasterList &mlist,
		  string command) {
	if (command == "paths") {
		string student_a, student_b;
		stack <string> path;
		input >> student_a;
		input >> student_b;
		mlist.unmark_all();
		all_paths(clist, mlist, student_a, 
			  student_b, path);
	}
	if (command == "shortestpath") {
		string student_a, student_b;
		stack <string> path;
		input >> student_a;
		input >> student_b;
		mlist.unmark_all();
		shortest_path(clist, mlist, student_a,
 			      student_b, path);
		print_short_path(short_path);
	}
}

//all_paths -- function to execute the process to find all of the paths between
//	       two people. The function uses a stack to hold the data for the
//	       path, pushing and popping when necessary. The function uses
//	       recursion to traverse through each path, adding to the stack
//	       by checking each person's neighbor. There are three base cases
//	       if student_a and student_b are the same, if the current student
//             has been passed already, and if student_a or student_b are not 
//	       students or TAs. The function loops through the classes that 
//	       TAs taed and then loops through the roster of that class,
//	       checking to see if they are TAs, continuing the recursion.
//args: two references to instances of ClassList and MasterList, two strings
//	for the students, and a reference to the stack path
//rets: none - void function
void all_paths(ClassList &clist, MasterList &mlist, string student_a, 
	       string student_b, stack<string> &path) {
	if (student_a == student_b and !path.empty()) {
		path.push(student_b +  "\n");
		print_paths(path);
		path.pop();
		return;
	}
	if (mlist.get_marked(student_a)) 
		return;
	if ((!mlist.did_ta(student_a) and (!mlist.is_student(student_a)))
	      or (!mlist.did_ta(student_b) and !mlist.is_student(student_b))) {
		cout << "Student not found";
		return;
	}
	int courses_taed_size = mlist.get_courses_taed_size(student_a);
	for (int i = 0; i < courses_taed_size; i++) {
		string class_taed = mlist.get_taed_class_name(student_a, i);
		int roster_size = clist.get_roster_size(class_taed);
		for (int j = 0; j < roster_size; j++) {
			string next_stu = clist.get_stu_name(class_taed, j);
			if (mlist.did_ta(next_stu) or 
				(next_stu == student_b)) {
				path_recurse_all(clist, mlist, student_a, 
					class_taed, next_stu, student_b,
					path);
			}
		}
	}
}

//path_recurse_all -- function to execute the process of adding a student and
//		      their data to the path. Also, the function calls the
//		      all_paths function in order continue the process of
//		      recursion. This function makes sure the push, pop,
//		      mark, and unmark certain students at the correct times.
//args: two references to instances of ClassList and MasterList, four strings
//	for the students and the class_taed, and a reference to the stack path
//rets: none - void function
void path_recurse_all(ClassList &clist, MasterList &mlist, string student_a,
		  string class_taed, string next_stu, string student_b, 
		  stack<string> &path) {
	string single_step = student_a + " +- " + class_taed + " -> ";
	path.push(single_step);
	//mark student, they have been passed
	mlist.mark(student_a);
	all_paths(clist, mlist, next_stu, student_b, path);
	//unmark student, may traverse path differently
	mlist.unmark(student_a);
	path.pop();
}

//path_recurse_short -- function to execute the process of adding a student and
//		      their data to the path. Also, the function calls the
//		      all_paths function in order continue the process of
//		      recursion. This function makes sure the push, pop,
//		      mark, and unmark certain students at the correct times.
//args: two references to instances of ClassList and MasterList, four strings
//	for the students and the class_taed, and a reference to the stack path
//rets: none - void function
void path_recurse_short(ClassList &clist, MasterList &mlist, string student_a,
		  string class_taed, string next_stu, string student_b, 
		  stack<string> &path) {
	string single_step = student_a + " +- " + class_taed + " -> ";
	path.push(single_step);
	//mark student, they have been passed
	mlist.mark(student_a);
	shortest_path(clist, mlist, next_stu, student_b, path);
	//unmark student, may traverse path differently
	mlist.unmark(student_a);
	path.pop();
}

//shortest_paths -- function to execute the process to find the shortest path
//		between two people. The function uses a stack to hold the data
//		for the path, pushing and popping when necessary. The function
//		uses recursion to traverse through each path, adding to the 
//		stack by checking each person's neighbor. There are three base 
//		cases if student_a and student_b are the same, if the current
//		student has been passed already, and if student_a or student_b
//		are not students or TAs. The function loops through the classes
//		that TAs taed and then loops through the roster of that class,
//	       	checking to see if they are TAs, continuing the recursion. The
//		function uses helper functions to actually find and print the 
//		shortest path.
//args: two references to instances of ClassList and MasterList, two strings
//	for the students, and a reference to the stack path
//rets: none - void function
void shortest_path(ClassList &clist, MasterList &mlist, string student_a,
				string student_b, stack <string> &path) {
	if (student_a == student_b and !path.empty()) {
		path.push(student_b + "\n");
		find_shortest(path);
		path.pop();
		return;
	}
	if (mlist.get_marked(student_a))
		return;
	if ((!mlist.did_ta(student_a) and (!mlist.is_student(student_a)))
	      or (!mlist.did_ta(student_b) and !mlist.is_student(student_b))) {
		cout << "Student not found";
		return;
	}
	int courses_taed_size = mlist.get_courses_taed_size(student_a);
	for (int i = 0; i < courses_taed_size; i++) {
		string class_taed = mlist.get_taed_class_name(student_a, i);
		int roster_size = clist.get_roster_size(class_taed);
		for (int j = 0; j < roster_size; j++) {
			string next_stu = clist.get_stu_name(class_taed, j);
			if (mlist.did_ta(next_stu) or 
				(next_stu == student_b)) {
				path_recurse_short(clist, mlist, student_a, 
					class_taed, next_stu, student_b,
					path);
			}
		}
	}
}

//find_shortest -- function to find the shortest path of all the paths found
//		   from shortest_path. 
//args: the stack path, passed by value
//rets: none - void function
void find_shortest(stack <string> path) {
	int path_size = path.size();
	//compare the sizes of two different paths
	if (path_size < short_path_size) {
		short_path_size = path_size;
		//empty the short_path stack
		while (!short_path.empty()) {
			short_path.pop();
		}
		//add the shorter path to the short_path stack
		while (!path.empty()) {
			short_path.push(path.top());
			path.pop();
		}
	}
}

//print_short_path -- function to prepare the shortest path to be printed. This
//		      function calls the main function to print a path. It 
//		      creates a new stack in order to switch the order of the
//		      path, so it will not print backwards.
//args: reference to stack path
//rets: none - void function
void print_short_path(stack <string> &path) {
	stack <string> temp_path;
	//add data from path to temp_path
	while (!path.empty()) {
		temp_path.push(path.top());
		path.pop();
	}
	print_paths(temp_path);
}

//print_paths -- function to print a path. It creates a new stack and includes
//		 two loops in order to switch the order of the stack. This is 
//		 to ensure it will not be printed backwards.
//args: reference to stack path
//rets: none - void function
void print_paths(stack <string> path) {
	stack <string> temp_path;
	string path_output;
	//add data from path to temp_path
	while (!path.empty()) {
		temp_path.push(path.top());
		path.pop();
	}
	//print data from temp_path
	while (!temp_path.empty()) {
		path_output = temp_path.top();
		cout << path_output;
		temp_path.pop();
	}
}

	
