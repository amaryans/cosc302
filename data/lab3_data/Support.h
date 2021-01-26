#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <iostream>
#include <fstream>
#include <vector>
typedef unsigned char uchar;
extern const char ETX;

template <typename T>
class matrix { 
	public:
		matrix(int n_Ncols, int n_Nrows) {
			Ncols = n_Ncols;
			Nrows = n_Nrows;

			buf = new T *[Ncols];
			for (int i = 0; i < Ncols; i++)
				buf[i] = new T[Nrows];
		}

		~matrix() {
			delete [] buf;
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }
		T * operator[] (int i) { return buf[i]; }

	private:
		int Nrows, Ncols;
		T **buf;
};

//use this for pixel list
struct pixel { 
		int row, col;
		pixel() { row = 0; col = 0; }
		pixel(int n_row, int n_col) { row = n_row; col = n_col; }
};

struct RGB { 
		char R, G, B;	
		RGB() { R = 0; G = 0; B = 0; }
};

class ppm { 

	public:
		//constructor
		ppm() {
			pixels = NULL;				
		}
		void read(const char *fname);
		void write(const char *fname);

		RGB * operator[] (int i) { return (*pixels)[i]; }
		int get_Nrows() const { return pixels->get_Nrows(); }
		int get_Ncols() const { return pixels->get_Ncols(); }
	
	private:
		string magicid;
		int w,h, maxvalue;

		matrix<RGB> *pixels;
		void new_image(int n_w, int n_h);
};

class rnumgen {
	public:
		rnumgen(int seedvalue, std::vector<int> &v);
		int rand();

	private:
		std::vector<float> F;
};

#endif
