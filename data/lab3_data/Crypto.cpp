#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;

#include "Support.h"

void set_pixel_list(ppm &image, vector<pixel> &list) {
	int x, y, w, h; 
	unsigned int color;

	w = image.get_Ncols();
	h = image.get_Nrows();

	vector<int> histogram;
	unsigned int prevcol = 0;	
	
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {			
			RGB &ref = image[x][y];
			//fills the histogram vector with color values
			color = ((ref.R >> 3) << 10) | ((ref.G >> 3) << 5) | (ref.B >> 3) | (prevcol << 15);
			histogram.push_back(color);
			prevcol = color;
			list.push_back(pixel(x, y));
		}
	}

	rnumgen RNG(0, histogram);
	for (int i = (int)list.size() - 1; i > 0; --i)
		swap(list[i], list[RNG.rand() % (i + 1)]);

	//cout << (int)list.size() << endl;

}

void encode(ppm &image) {
	
	vector<pixel> pix;
	set_pixel_list(image, pix);

	//must read in text from cin to encode one character at a time
	char c;
	int x, y, i_col, i_pix, i_bit, psz;
	bool check;

	i_col = 0; i_pix = 0;
	check = false;
	psz   = pix.size();
	
	
	while(check == false) {
		
		//if we are at the end of the text then insert an ETX character
		if (!cin.get(c)) {
			check = true;
		 	c = ETX;
		}

		//now we have to process the char bit by bit excluding the most significant bit
		for (i_bit = 0; i_bit < 7; i_bit++) {
			if (i_pix > psz) {
				cerr << "Error: pixel_list exhausted!\n";
				exit(0);
			}

			//get which pixel we are dealing with
			x = pix[i_pix].row;
			y = pix[i_pix].col;

			RGB &ref = image[x][y];		
			//alternate between red green and blue pixels
			switch (i_col%3) {
				case 0:
					ref.R = (ref.R & 0xFE) | (c & 0x1);
					break;
				case 1:
					ref.G = (ref.G & 0xFE) | (c & 0x1);
					break;
				case 2:
					ref.B = (ref.B & 0xFE) | (c & 0x1);
					break;
			}
			c = c >> 1;

			//increments the color index so that we can alternate the pixels
			i_col++;
			i_pix++;
		}
	}

	string newfname;
}

void decode(ppm &img) {
	
	vector<pixel> pix;
	set_pixel_list(img, pix);

	char c, tmp;
	int x, y, i_col, i_bit, i_pix, psz;
	bool check = false;
	string message = "";

	i_col = 0; i_pix = 0;
	psz = pix.size();

	while (!check) {
		//we need to get the RBG data first to extract the data before checking anything
		//this is because we check for a certain character at the end of the message.
		c = 0x00;	
	
		//initialize c to this so that we can use bitwise OR operator to push the next LSB on
		for (i_bit = 0; i_bit < 7; i_bit++) {
			if (i_pix > psz) {
				cerr << "Error: pixel_list exhausted!\n";
				exit(0);
			}
			
			x = pix[i_pix].row;
			y = pix[i_pix].col;
			
			RGB &ref = img[x][y];
			switch (i_col%3) {
				case 0:
					//we need to get the LSB of the color
					tmp = ref.R & 0x1;
					//get the bit from the char
					tmp <<= (i_bit);
					//sets ref.r LSB to the bit from char
					c |= tmp;
					break;
				case 1:
					tmp = ref.G & 0x1;
					tmp <<= (i_bit);
					c |= tmp;
					break;
				case 2:
					tmp = ref.B & 0x1;
					tmp <<= (i_bit);
					c |= tmp;
					break;
			}
				
			i_col++;
			i_pix++;
			
		}
		
		if (c == ETX) 
			check = true;
		else
			cout.put(c);

	}

}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cerr << argv[0] << " -encode|decode image.ppm\n";
		return 1;
	}

	string option   = argv[1];
	char * fname_1 = argv[2];

	if (option != "-decode" && option != "-encode") {
		cerr << argv[0] << " -encode|decode image.ppm\n";
		return 1;
	}


  ppm img;
  img.read(fname_1);

  string filename = fname_1;
  int lastindex = filename.find_last_of(".");
  string rawname = filename.substr(0, lastindex);
  
  rawname += "_wmsg.ppm";
  int size = rawname.length();
  char * fname = new char[size + 1];
  strcpy(fname, rawname.c_str());
  
  //once we read the image it is stored in img
  //this will be passed to encode with the const vector list which we fill
  //using set_pixel_list prior to passing it into encode

  //we can now encode using img and list
  
	if (option == "-encode") {
		encode(img);
		img.write(fname);
	}
	if (option == "-decode")
		decode (img);
}





