// include header file(s) needed
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

#include "Slist.h"

// modify the data class and the associated input 
// and output operators to store, set and print a 
// line number identifier called ID

class data { 
	public:
		bool operator<(const data &) const;
		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);

	private:
		string firstname;
		string lastname;
		string phonenum;
		int ID;
};

bool data::operator<(const data &rhs) const {
	
	if (lastname != rhs.lastname)
		return lastname < rhs.lastname;
	if (firstname != rhs.firstname)
		return firstname < rhs.firstname;

	return phonenum < rhs.phonenum;

}
istream & operator>> (istream &in, data &r) { 
	in >> r.firstname >> r.lastname >> r.phonenum >> r.ID;
	return in;
}
ostream & operator<< (ostream &out, const data &r) { 
	string name = ' ' + r.lastname + ' ' + r.firstname;
	cout << setw(25) << left << name << setw(12) << r.phonenum << setw(9) << right << r.ID << endl;
	return out;
}

template <typename T>
void printlist(T start, T end) { 
	for (T it = start; it != end; ++it)
		cout << *it;
}

int main(int argc, char *argv[]) {
  // copy command-line check from QsortB.cpp,
  // then modify to apply to this assignment
  string option = argv[1];

  if (argc != 2) {
	  cerr << "usage: " << argv[0] << " -quicksort|mergesort < file.txt\n";
	  return 1;
  }
  if (option != "-quicksort" && option != "-mergesort") {
	  cerr << "usage: " << argv[0] << " -quicksort|mergesort < file.txt\n";
	  return 1;
  }

  slist<data> A;
  int i = 1;
  data din;
  string line;
  
  while (getline(cin, line)) {
	stringstream id;
	id << i;
	string id_str = id.str();

	line = line + " " + id_str;
	stringstream ss(line);
	ss >> din;
    A.push_back(din);
	i++;
	
	id.str("");
	id.clear();
  }
  
  A.sort(option);

  printlist(A.begin(), A.end());
}
