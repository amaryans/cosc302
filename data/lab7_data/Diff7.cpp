#include <iostream>
#include <vector>
#include <string> 
#include <fstream>
#include <iomanip>
#include <stack>

using namespace std;

template <typename T>
class matrix {
  // see handout from class or make your own
	public:
		matrix(int n_Nrows=0, int n_Ncols=0) {
			Nrows = n_Nrows;
			Ncols = n_Ncols;

			buf = new T * [Nrows];
			buf[0] = new T [Nrows*Ncols];
			for (int i=1; i < Nrows; i++)
				buf[i] = buf[i-1] + Ncols;
		}
		~matrix() {
			delete [] buf[0];
			delete [] buf;
		}

		void assign(int n_Nrows=0, int n_Ncols=0) {
			Nrows = n_Nrows;
			Ncols = n_Ncols;

			buf = new T * [Nrows];
			buf[0] = new T [Nrows*Ncols];
			for (int i=1; i < Nrows; i++)
				buf[i] = buf[i-1] + Ncols;
		}

		T * operator[] (int i) { return buf[i]; }

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }

	private:
		int Nrows, Ncols;
		T **buf;
};

class LCS {
  public:
	LCS() { VERT = 1; HORZ = 2; DIAG = 4; }
    void text1_push_back(string push) { text1.push_back(push); }
    void text2_push_back(string push) { text2.push_back(push); }
	void compute_alignment();
	void report_difference();

  private:
	// support functions
	matrix<int> cost; // edit costs
	matrix<int> link; // alignment info

	//need a private recursive function
	int report_difference(int, int);
	void print_edits(int, int, int , int);
	int VERT;
	int HORZ;
	int DIAG;
	
	int rec_x, rec_y;

	vector<int> trace;
    // text1 buffer
	vector<string> text1;
    // text2 buffer
	vector<string> text2;
};

void LCS::compute_alignment() {

	int m = text1.size();
	int n = text2.size();

	cost.assign(m+1, n+1);
	link.assign(m+1, n+1);

	//get the cost for deletion of all elements
	//since the cost of deletion is 1 then we know
	//that the cost will be the same as its position vertically
	for (int i=0; i<=m; i++) {
		cost[i][0] = i;
		link[i][0] = VERT;
	}

	//get the cost for insertion of all elements
	//similarly for insertions and its position horizontally
	for (int j = 0; j <= n; j++) {
		cost[0][j] = j;
		link[0][j] = HORZ;
	}

	link[0][0] = 0;

	//now we have to check if the lines are the same and calculate the cost
	//since the cost of matches is zero
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (text1[i] == text2[j]) {
				//we know that there is a match here so the cost
				//of the diagonal is the same as its previous
				cost[i+1][j+1] = cost[i][j];
			}
			else {
				//must find out if we need to insert or delete the element
				if (cost[i][j+1] < cost[i+1][j]) {
					cost[i+1][j+1] = cost[i][j+1] + 1;
				}
				else {
					cost[i+1][j+1] = cost[i+1][j] + 1;
				}
				//if they are equal then it does not matter which one we
				//add one to, and we want the smallest possible value so
				//we choose the index with the lowest total cost
			}
		}
	}

	//need to link all of the interior points
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (text1[i-1] == text2[j-1]) {
				link[i][j] = DIAG;
			}
			else if (cost[i][j]-1 == cost[i][j-1]) {
				link[i][j] = HORZ;
			}
			else if (cost[i][j]-1 == cost[i-1][j]) {
				link[i][j] = VERT;
			}
			else {
				cout << "Invalid link\n";
				break;
			}
		}
	}

	
	/* check to make sure cost matrix and link matrix match solution
	for (int i = 0; i <= n+1; i++) {
		for (int j = 0; j < m; j++) {
			cout << setw(4) << cost[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i <= n+1; i++) {
		for (int j = 0; j < m; j++) {
			cout << setw(4) << link[i][j] << " ";
		}
		cout << endl;
	}
	*/

}

void LCS::report_difference() {
	
	rec_x = text1.size();
	rec_y = text2.size();

	report_difference(rec_x, rec_y);
	
	//check to see if path good
	/*for (int i = 0; i < trace.size(); i++) {
		cout << trace.top() << " ";
		trace.pop();
	}*/

}

int LCS::report_difference(int x, int y) {
	
	if (x == 0 && y == 0) {
		int num_del = 0;
		int num_ins = 0;

		int i=0, j = 0;
		for (int k = (int)trace.size(); k >= 0; k--) {
			if (trace[k] == DIAG) {
				print_edits(i, j, num_del, num_ins);
				num_del = 0;
				i++;
				num_ins = 0;
				j++;
			}
			else if (trace[k] == HORZ) {
				num_ins++;
				j++;
			}
			else if (trace[k] == VERT) {
				num_del++;
				i++;
			}
		}
			print_edits(i, j, num_del, num_ins);
	}
	
	int direction = link[x][y];

	if (direction == 4) {
		trace.push_back(direction);
		report_difference(x-1, y-1);
	}
	else if (direction == 2) {
		trace.push_back(direction);
		report_difference(x, y-1);
	}
	else if (direction == 1) {
		trace.push_back(direction);
		report_difference(x-1, y);
	}
	
	return 0;
}

void LCS::print_edits(int i, int j, int num_del, int num_ins) {

	int print_i;
	print_i = i - num_del;
	int print_j;
	print_j = j - num_ins;
	//check for insertions
	if (num_ins != 0 && num_del == 0) {
		//if only one insertion
		if (num_ins == 1) {
			cout << i << "a" << j << endl;
			cout << "> " << text2[print_j] << endl;
		}
		else {
			cout << i << "a";
			cout << j - num_ins + 1 << "," << j;
			cout << endl;
			for (int k = 0; k < num_ins; k++) {
				cout << "> " << text2[print_j] << endl;
				print_j++;
			}
		}
	}
	else if (num_del != 0 && num_ins == 0) {
		
		if (num_del == 1) {
			cout << i << "d" << j << endl;
			cout << "< " << text1[print_i] << endl;
		}
		else {
			cout << i - num_del + 1 << "," << i;
			cout << "d" << j << endl;
			for (int k = 0; k < num_del; k++) {
				cout << "< " << text1[print_i] << endl;
				print_i++;
			}
		}
	}
	else if (num_del != 0 && num_ins != 0) {
		//deletes then inserts
		if (num_del == 1) {
			cout << i;
		}
		else {
			cout << i - num_del + 1 << "," << i;
		}
		cout << "c";
		if (num_ins == 1) {
			cout << j;
		}
		else {
			cout << j - num_ins + 1 << "," << j;
		}
		cout << endl;

		for (int k = 0; k < num_del; k++) {
			cout << "< " << text1[print_i] << endl;
			print_i++;
		}
		cout << "---\n";
		for (int k = 0; k < num_ins; k++) {
			cout << "> " << text2[print_j] << endl;
			print_j++;
		}


	}

	//now must print for changes
	
	
}

int main(int argc, char *argv[])
{
  // check two input files are specified on command line

  if (argc != 3) {
	  cerr << "./Diff7 file1.txt file2.txt\n";
	  return 1;
  }

  LCS lcs;
  // must read in the two files
  string file1 = argv[1];
  ifstream infile1;
  infile1.open(argv[1]);
  string file2(argv[2]);
  ifstream infile2(argv[2]);

  while (getline(infile1, file1))
	  lcs.text1_push_back(file1);

  while (getline(infile2, file2))
	  lcs.text2_push_back(file2);

  // read the text from file1 into the lcs.text1 buffer
  // read the text from file2 into the lcs.text2 buffer

  lcs.compute_alignment();
  lcs.report_difference();
}
