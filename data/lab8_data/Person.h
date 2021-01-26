#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <iomanip>
#include <cstring>

using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
	STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

type_enum str2type(const string &s);
string type2srt(const type_enum &t);

class person {
	//DEFINITION -- base class

	public:
		person();
		person(const string &, type_enum = UNKNOWN);

		string &get_name();
		type_enum &get_type();
		void set_type(type_enum);
		void set_name(string);

		bool operator<(const person &);

	private:
		map<string,float> courses;

	protected:

		string name;
		type_enum type;

		virtual void print_person_info(ostream &) = 0;
		virtual void print_course_info(ostream &) = 0;


		friend ostream &operator<<(ostream &, person &);


};

class faculty : public person {
	//DEFINITION -- derived class
	public:
		faculty();
		faculty(const string &, type_enum = UNKNOWN);

		void add_course(string &);

		bool operator<(const faculty &);


	private:

		set<string> courses;

		void print_person_info(ostream &);
		void print_course_info(ostream &);

};

class student : public person {
	//DEFINITION -- derived class
	public:
		student();
		student(const string &, type_enum = UNKNOWN);

		void add_course (string &, float);

		bool operator<(const student &);

	private:

		map<string, float> courses;

		void print_person_info(ostream &);
		void print_course_info(ostream &);

};

#endif

