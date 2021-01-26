#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <algorithm>
using namespace std;

#include "Support.h"

const char ETX = 0x3;

void ppm::read(const char *fname) { 
	
	ifstream in;

	int x, y;

	in.open(fname);

	if (!in.is_open()) {
		cerr << "Error: cannot open " << fname << " for reading!\n";
		exit(0);
	}

	string magicid;
	
	in >> magicid;

	if (magicid != "P6") {
		cerr << "Error: magicid is not P6!\n";
		exit(0);
	}

	if (!(in >> w >> h >> maxvalue)) {
		cerr << "Error: invalid width, height, or maxvalue\n";
		exit(0);
	}

	if (maxvalue != 255) {
		cerr << "Error: maxvalue " << maxvalue << " not supported\n";
		exit(0);
	}

	//read one byte
	in.get();
	
	new_image(w, h);
	
	//the address of the RGB value is the address of the pixel we need
	//this allows us to access the RGB values at any index for write
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			RGB &ref = (*pixels)[x][y];
			ref.R = in.get();
			ref.G = in.get();
			ref.B = in.get();
		}
	}

	in.close();
}

void ppm::write(const char *fname) { 
  
	ofstream out;
	int x, y;

	out.open(fname);
	
	if (!out.is_open()) {
		cerr << "Error: cannot open " << fname << " for writing\n";
		exit(0);
	}

	out << "P6" << endl;
	out << w << " " << h << endl;
	out << maxvalue << endl;
		
	for (y = 0; y < pixels->get_Nrows(); y++) {
		for (x = 0; x < pixels->get_Ncols(); x++) {
			RGB &ref = (*pixels)[x][y];
			out << ref.R << ref.G << ref.B;
		}
	}

	out.close();

}

void ppm::new_image(int w, int h) {
	if (pixels != NULL) delete pixels;

	pixels = new matrix<RGB>(w,h);
}

rnumgen::rnumgen(int seedvalue, vector<int> &v) {
	srand(seedvalue);

	F.resize(v.size());
	partial_sum(v.begin(), v.end(), F.begin());
	transform(F.begin(), F.end(), F.begin(),
	bind2nd(divides<float>(), F.back()));
}

int rnumgen::rand() {
	const double randmax = RAND_MAX+1.0;
	const double p = (double)std::rand()/randmax;

	return upper_bound(F.begin(), F.end(), p) - F.begin();
}



