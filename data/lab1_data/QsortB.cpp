// Austin Maryanski - CS302 - Lab 1
// include header files needed
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

class data {
  public:
	// add operator< using lastname, firstname, phone number
	bool operator<(const data &) const;
    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};

bool data::operator<(const data &rhs) const {
	//if last names are same check first
	if (lastname  != rhs.lastname)
		return lastname < rhs.lastname;
	
	//if first names are the same check phonenum
	if (firstname != rhs.firstname)
		return firstname < rhs.firstname;
	
	return phonenum < rhs. phonenum;
}
istream & operator>>(istream &in, data &r) { 
  // write this to read data object data
  in >> r.firstname >> r.lastname >> r.phonenum;
  return in;
}

ostream & operator<<(ostream &out, const data &r) {
  // write this to write data object data
  string name = r.lastname + ' ' + r.firstname;
  cout << setw(24) << left << name << r.phonenum << '\n';
  return out;
}

template <typename T>
void quicksort(vector<T> &A, int left, int right) {
	
  if (left < right) {
    int middle = left + (rand() % (right - left + 1));
     
      //select pivot: median-of-three
    int pindex = middle;
    T pivot = A[pindex];

      //partition A: {<=}, {pivot}, {=>}
      swap(A[pindex], A[right]);

      int i = left - 1;
      int j = right;

      while (1) {
	      while (A[++i] < pivot) { 
		    if (i == right) break; 
		  }
	      while (pivot < A[--j]) { 
		    if (j == left) break;
		  }
	      if (i>=j) break;
	      swap(A[i], A[j]);
      }

      pindex = i;
      swap(A[pindex], A[right]);
//if statements from quickselect and if pindex = k change the if to a while
    quicksort(A, left, pindex-1);
    quicksort(A, pindex+1, right);
  }
}

template <typename T>
void quickselect(vector<T> &A, int left, int k, int right) {
 
	while (left < right) {
      int middle = left + (rand() % (right - left + 1));
     
      //select pivot: median-of-three
      int pindex = middle;
      T pivot = A[pindex];

      //partition A: {<=}, {pivot}, {=>}
      swap(A[pindex], A[right]);

      int i = left - 1;
      int j = right;

      while (1) {
	      while (A[++i] < pivot) { 
		    if (i == right) break; 
		  }
	      while (pivot < A[--j]) { 
		    if (j == left) break;
		  }
	      if (i>=j) break;
	      swap(A[i], A[j]);
      }

      pindex = i;
      swap(A[pindex], A[right]);

	  if (pindex == k)
		  break;
	  if (k < pindex) right = pindex-1;
	  else            left = pindex+1;
	//if statements from quickselect and if pindex = k change the if to a while
    //quicksort(A, left, pindex-1);
    //quicksort(A, pindex+1, right);i
	}

}

template <typename T>
void printlist(T start, T end) { 
  // template based iterator code for printing data to stdout
  for (T it = start; it != end; it++)
	  cout << *it;

}

int main(int argc, char *argv[]) {
  
  if (argc != 2 && argc != 4) {
	  cerr << "usage: " << argv[0] << " -stl | -rpivot [k0 k1] < file.txt\n";
	  return 1;
  }
  string option = argv[1];
  if (option != "-stl" && option != "-rpivot") {
	  cerr << "usage: " << argv[0] << " -stl | -rpivot [k0 k1] < file.txt\n";
	  return 1;
  }
  if (option == "-stl" && argc != 2) {
	  cerr << "usage: " << argv[0] << " -stl | -rpivot [k0 k1] < file.txt\n";
    return 1;
  }
  vector<data> A;

  data din;
  while (cin >> din)
    A.push_back(din);
  
  if (option == "-stl") {
    std::sort(A.begin(), A.end());
  }

  if (option == "-rpivot") {
    int N = (int)A.size();

	int k0 = 0;
	int k1 = N-1;
    
	if (argc == 4) {
      k0 = atoi(argv[2]);
	  k1 = atoi(argv[3]);
	  
	  if (k0 < 0) k0 = 0;
	  if (k1 >= N) k1 = N-1;

	  quickselect(A, 0, k0, N-1);
	  quickselect(A, k0, k1, N-1);
	  //call quickselect here
	}
    quicksort(A, k0, k1);
  }
  printlist(A.begin(), A.end());
}
