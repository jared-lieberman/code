#
# Makefile for HW6
#
CXX      = clang++
CXXFLAGS = -g -Wall -Wextra
LDFLAGS  = -g

hw6degrees: main.cpp hashfunc.cpp hashfunc.h ClassList.cpp ClassName.cpp\
 MasterList.cpp Person.cpp
	${CXX} ${CXXFLAGS} -o hw6degrees main.cpp hashfunc.cpp ClassList.cpp\
 ClassName.cpp MasterList.cpp Person.cpp

main.o: main.cpp ClassList.h MasterList.h
ClassList.o: ClassList.cpp ClassList.h
ClassName.o: ClassName.cpp ClassName.h
MasterList.o: MasterList.cpp MasterList.h hashfunc.h Person.h
Person.o: Person.cpp Person.h

clean:
	rm -f hw6degrees

provide:
	provide comp15 hw6 main.cpp ClassList.cpp ClassList.h ClassName.cpp\
 ClassName.h MasterList.cpp MasterList.h Person.cpp Person.h Makefile\
 ReadMe.md hashfunc.cpp hashfunc.h
