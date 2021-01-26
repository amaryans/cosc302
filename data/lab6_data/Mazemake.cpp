//if find returns a number different call merge
//if there is only set in the image it is done
//use k twice -> use again to write remaining walls
//cell wall will have ( (number-of-columns) - 1 ) * Nrows
//horizontal amount  =    (Nrows-1) * Ncols
//those added equal N
//one unique solution
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"

struct cell {
	int x, y;
};

void cell_swap(cell &c1, cell &c2) {
	cell c3(c1);
	c1 = c2;
	c2 = c3;
}

void cell_set(cell & c, int i, int j) { 
	c.x = i;
	c.y = j;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
	//print usage error message to stderr
		return 0;
	}

	int Nrow = atoi(argv[1]);
	int Ncol = atoi(argv[2]);
	
	FILE * fp;
	// Grid wall:     (i0,j0) | (i1,j1)
	// Horizontal wall: (i,j) | (i+1,j)
	// Vertical wall:   (i,j) | (i,j+1)

	int N = ( (Ncol - 1) * Nrow) + ( (Nrow - 1) * Ncol );

	cell wall[N][2];

	//populate wall[][] with interior grid walls
	int k = 0;
	int i = 0;
	int j = 0;

	//for every cell @ (i,j) except the last column
	for (i = 0; i < Ncol - 1; i++) {
		for (j = 0; j < Nrow; j++) {
			cell_set(wall[k][0], i    , j);
			cell_set(wall[k][1], i + 1, j);
			k++;
		}
	}

	i = 0;
	j = 0;

	for (j = 0; j < Nrow - 1; j++) {
		for (i = 0; i < Ncol; i++) {
			cell_set(wall[k][0], i,     j);
			cell_set(wall[k][1], i, j + 1);
			k++;
		}
	}

	//randomly perturb list order: swap based
	srand(time(NULL));
	
	for (i = N - 1; i > 0; i--) {
		j = rand() % (i + 1);
		cell_swap(wall[i][0], wall[j][0]);
		cell_swap(wall[i][1], wall[j][1]);
	}

	//open output file for writing (argv[3])
	fp = fopen(argv[3], "w");

	//write MAZE, Nrow, Ncol header 
	fprintf(fp, "MAZE %d %d\n", Nrow, Ncol);

	//create dset of size Ncol * Nrow for merging
	dset DS(Nrow * Ncol);

	for (k=0; k<N; k++) {
		//if: pair of cells given by wall[k][] are not
		//connected (belong to different disjoint sets),
		//merge them
	
		int ii = wall[k][0].x + (wall[k][0].y * Ncol);
		int jj = wall[k][1].x + (wall[k][1].y * Ncol);

		if (DS.find(ii) != DS.find(jj))
			DS.merge(ii, jj);	//else: write wall to file in the form of the
		else {
			fprintf(fp, "%3d %3d %3d %3d\n" , wall[k][0].y, wall[k][0].x, wall[k][1].y, wall[k][1].x);
		}
			

		if(DS.size() == 1)
			break;
		//two pairs of cell indices (i0,j0) (i1,j1)

	}

	k++;

	for (k; k < N; k++) {
		fprintf(fp, "%3d %3d %3d %3d\n" , wall[k][0].y, wall[k][0].x, wall[k][1].y, wall[k][1].x);
	}
	//write any remaining walls to file
	
	fclose(fp);
	//close output file
}
