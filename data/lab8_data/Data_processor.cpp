#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <string.h>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

typedef enum ARG_MODE_T {
	MODE_NULL = -1,
	MODE_PERSON,
	MODE_FACULTY,
	MODE_STUDENT,
} arg_mode_t;

int main(int argc, char *argv[]) {

	arg_mode_t mode = MODE_NULL;
	string mytype = argv[1];

	if (argc != 2 && mytype != "-person") {
		cerr << "Usage: ./Data_processor -person|student|faculty < file.xml";
		return 0;
	}

	if (argc != 2 && mytype != "-faculty") {
		cerr << "Usage: ./Data_processor -person|student|faculty < file.xml";
		return 0;
	}

	if (argc != 2 && mytype != "-student") {
		cerr << "Usage: ./Data_processor -person|student|faculty < file.xml";
		return 0;
	}


	if(strcmp(argv[1], "-person")  == 0) mode = MODE_PERSON;
	if(strcmp(argv[1], "-faculty") == 0) mode = MODE_FACULTY;
	if(strcmp(argv[1], "-student") == 0) mode = MODE_STUDENT;


	if (mode == MODE_NULL) {
		cerr << "Usage: ./Data_processor -person|student|faculty < file.xml";
		return 0;
	}

	person *n_person;
	faculty *fperson;
	student *sperson;

	vector<person *> person_list;
	vector<faculty *> faculty_list;
	vector<student *> student_list;

	//type is student or faculty
	//g is what kind of faculty or student year
	type_enum type = UNKNOWN;
	//type_enum g = UNKNOWN;

	//gp is a float and course is a string
	vector<float> gp;
	string course;

	float gpa;
	int line = 1;
	size_t iL, iR;
	string input;
	string name;
	string category;

	//CS307: CHECK CORRECTESS OF XML HEADER 

	while (getline(cin, input)) {
		++line;

		if ((iL = input.find('<')) == string::npos) {
			continue;

		} else if (input.compare(iL,9,"<faculty>") == 0) {
			n_person = new faculty();
			fperson = static_cast<faculty*> (n_person);
			type = FACULTY;	  
			course.clear();

		} else if (input.compare(iL,10,"</faculty>") == 0) {

			//CS307: CHECK CORRECTESS OF XML DATA
			//CODE FOR ADDING FACULTY PERSON TO DATABASE

			//check type 
			//set subtype

			//n_person = ?

			person_list.push_back(n_person);
			type = UNKNOWN;
			//reset type
			//reset subtype

			continue;

		} else if (input.compare(iL,9,"<student>") == 0) {
			//set type

			type = STUDENT;
			n_person = new student();
			sperson = static_cast<student*> (n_person);
			course.clear();
			gp.clear();

		} else if (input.compare(iL,10,"</student>") == 0) {

			//CS307: CHECK CORRECTESS OF XML DATA
			//CODE FOR ADDING STUDENT PERSON TO DATABASE

			//check type 
			//set subtype

			//n_person = ?

			person_list.push_back(n_person);

			type = UNKNOWN;
			//reset type
			//reset subtype

			continue;

		} else if (input.compare(iL,5,"<name") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);

			n_person->get_name() = input.substr(iL + 2, iR - (iL + 2));
			//CS307: CHECK CORRECTESS OF XML DATA
			//CODE FOR EXTRACTING NAME (using iL, iR)

		} else if (input.compare(iL, 10, "<category=") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL + 2);

			n_person->get_type() = str2type(input.substr(iL + 2, iR - (iL + 2)));

		} else if (input.compare(iL,10,"<g=") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);


			//CS307: CHECK CORRECTESS OF XML DATA
			//CODE FOR EXTRACTING CATEGORY (using iL, iR)

		} else if (input.compare(iL,7,"<course") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"", iL+2);

			course = input.substr(iL + 2, iR - (iL + 2));
			//CS307: CHECK CORRECTESS OF XML DATA
			//CODE FOR EXTRACTING COURSE (using iL, iR)

			iL = iR;
			if (type == FACULTY) {
				fperson->add_course(course);
				iR = input.find("/>", iL+1);
			} else if (type == STUDENT) {
				iL = input.find("gp=\"", iL);
				iR = input.find("\"/>", iL+4);

				gpa = atof( input.substr(iL + 4, iR - iL - 4).c_str());

				sperson->add_course(course, gpa);

				//CS307: CHECK CORRECTESS OF XML DATA
				//CODE FOR EXTRACTING GRADE (using iL, iR)
			}
		}
	}

	//EXTRACT LIST OF POINTERS TO PERSON, FACULTY OR STUDENT
	//ENTRY, SORT USING SPTRSORT AND PRINT RESULT TO STDOUT

	//NOTE: PERSON, FACULTY AND STUDENT EACH HAVE THEIR OWN
	//OVERLOADED COMPARISON OPERATOR (--> DIFFERENT SORTING)

	//RELEASE ALL DYNAMICALLY ALLOCATED MEMORY
	unsigned int i;
	switch (mode) {

		case MODE_NULL:
			break;


		case MODE_PERSON:

			sptrsort(person_list);

			for (i = 0; i < person_list.size(); i++)
				cout << *person_list[i] << endl;

			break;

		case MODE_FACULTY: 
			for (i = 0; i < person_list.size(); i++) {

				if ((*person_list[i]).get_type() < STUDENT) {
					faculty_list.push_back(static_cast<faculty *> (person_list[i]));
				}

			}	

			sptrsort(faculty_list);

			for (i = 0; i < faculty_list.size(); i++)
				cout << *faculty_list[i] << endl;

			break;

		case MODE_STUDENT: 
			for (i = 0; i < person_list.size(); i++) {

				if ((*person_list[i]).get_type() >= STUDENT) {
					student_list.push_back(static_cast<student *> (person_list[i]));
				}

			}	

			sptrsort(student_list);

			for (i = 0; i < student_list.size(); i++)
				cout << *student_list[i] << endl;

			break;


	}

}
