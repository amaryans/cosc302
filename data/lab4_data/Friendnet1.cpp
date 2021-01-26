#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <iomanip>
using namespace std;

void set_oldfriends(vector <string> &name, vector < vector<int> > &friends, int M0, int M1) {
  int N = (int)name.size();
  //initialize 2D-vector-array friends (hint: use vector::assign())
  //need to fix the assignment of friends for 2D vector
  friends.assign(N, vector<int> (N));

  for (int i=0; i<N; i++) {
	//declare std::set called doknow
    
	set<int> doknow;
	//initialize M (random number of friends: function of M0, M1)
	int M = M0 + rand() % M1;

	while ((int)doknow.size() < M) {
	  //compute j (random friend index: hint j != i)
	  //this is a random number between 0 and N where i!=j
	  int j;
	  do {
		j = rand() % N;
	  } while (i == j);
		
	  doknow.insert(j);
	}

	//use iterator to sweep thru doknow set
	//update the corresponding pairs of friends entries
	for (set<int>::iterator it = doknow.begin(); it != doknow.end(); ++it) {
	  friends[i][*it] = 1;
	  friends[*it][i] = 1;
	}
  }
}

void set_newfriends(vector < vector<int> > &friends, vector < vector<int> > &new_friends) {
  int N = (int)friends.size();
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.assign(N, vector<int> (N));

  for (int i = 0; i < N; i++) {
    //for each adjacent friend
	for (int j = 0; j < N; j++) {
		if (friends[i][j] == 1) {
			//for each friend of friend
			for (int k = 0; k < N; k++) {
				//update to being friends if not friends already
				if (k != i && friends[i][k] != 1 && friends[j][k]) {
					new_friends[k][i] = 1;
					new_friends[i][k] = 1;
				}
			}
		}
	}
  }
  /*for (int i=0; i<N; i++) {
	for (each adjacent friend: friends[i][j] == 1) {
	  for (each adjacent friend-of-friend: friends[j][k] == 1) {
	    if (k-is-not-i && k-is-not-friend-of-i)
	      update the corresponding pairs of new_friends entries
	  }
    }
  }*/
}

void writetofile(const char *fname, vector<string> &name, vector< vector<int> > &friends) {
  //open file
  ofstream out;
  out.open(fname);

  if (!out.is_open()) {
	  cerr << "Unable to open file to write\n";
	  exit(1);
  }

  int N = (int)name.size();
  int longestname = 0;
  for (int k = 0; k < N; k++) {
	if ((int)name[k].length() > longestname)
		longestname = name[k].length();
  }

  int maxout = 0;
  //determine max name length
  for (int i = 0; i < N; i++) {
	out << setw(longestname) << left << name[i] << " :";
	for (int j = 0; j < N; j++) {
	    if (friends[i][j] == 1 && maxout % 8 == 0 && maxout != 0) {
			out << endl;
			out << setw(longestname) << left << name[i] << " :";
			out << " " << setw(longestname) << left << name[j];
			maxout++;
		}
		else if (friends[i][j] == 1) {
			out << " " << setw(longestname) << left << name[j];
			maxout++;
		}
	  
	}
	maxout = 0;
	out << endl;
  } 
  
  out.close();
  /*for (int i=0; i<N; i++) {
	for (each adjacent friend: friends[i][j] == 1) {
	  pretty-print name[i] and name[j] (see assignment) 
  }

  close file
  */
}

int main(int argc, char *argv[]) {
  //parse argc, argv arguments
  if (argc != 1 && argc != 3) {
	cerr << "usage: " << argv[0] << " [-seed N]\n";
	return 1;
  }
  string check;
  if (argc == 3)
	  check = argv[1];

  if (argc == 3 && check != "-seed") {
	cerr << "usage: " << argv[0] << " [-seed N]\n";
	return 1;
  }

  //print usage message and exit if invalid
  //seed random number generator
  int seed = 0;

  if (argc == 3)
	seed = atoi(argv[2]);

  srand(seed);

  string inname;
  vector<string> name;
  //read strings from stdin into name vector
  cin >> inname;
  name.push_back(inname);

  while (cin >> inname) {
	  for (int i = 0; i < (int)name.size(); i++) {
		  if (inname == name[i]){}
		  else {
			  name.push_back(inname);
			  i = (int)name.size() + 1;
		  }
  
	  }
  }
  

  int M0 = 1; // min number of friends
  int M1 = 3; // potential extra friends

  //v[rows][cols]
  vector < vector<int> > friends;
  vector < vector<int> > new_friends;
  //declare 2D-vector-array called friends
  //declare 2D-vector-array called new_friends

  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow1.txt", name, friends);

  set_newfriends(friends, new_friends);
  writetofile("mightknow1.txt", name, new_friends);
}
