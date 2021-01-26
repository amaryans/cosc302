#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct cell {
	int x, y;
};

int DIR_LEFT = 0; int DIR_RIGHT = 1; int DIR_UP = 2; int DIR_DOWN = 3;

int get_dir(int x1, int y1, int x2, int y2) {
	if (x1 = x2) {
		if (y1 < y2)
			return DIR_DOWN;
		if (y1 > y2)
			return DIR_UP;
	}
	if (y1 = y2) {
		if (x1 < x2)
			return DIR_RIGHT;
		if (x1 > x2)
			return DIR_LEFT;
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		//print usage error message to stderr
		return 0;
	} 

	//open input file for reading
	FILE * in;
	in = fopen(argv[1], "r");
	FILE * out;
	out = fopen(argv[2], "w");
	//open output file for writing

	int Nrow;
	int Ncol;

	//determine Nrow,Ncol from input file
	fscanf(in, "MAZE %d %d", &Nrow, &Ncol);

	//create array of walls for each grid cell
	//initialize to have boundary walls set and
	//interior walls unset

	bool wall[Ncol][Nrow][4];
	int i, j, k;

	for (i = 0; i < Ncol; i++) {
		for (j = 0; j < Nrow; j++) {
			for (k = 0; k < 4; k++)
				wall[i][j][k] = 0;
		}
	}
	
	for (i = 0; i < Nrow; i++) {
		wall[0][i][DIR_LEFT] = true;
		wall[Ncol-1][i][DIR_RIGHT] = true;
	}

	for (i = 0; i < Ncol; i++)
		wall[i][0][DIR_UP]   = wall[i][Nrow-1][DIR_DOWN]  = true;

	int x1, x2, y1, y2;
	//read input file, set interior walls
	while (fscanf(in, "%d %d %d %d", &y1, &x1, &y2, &x2) == 4) {
			wall[x1][y1][ get_dir(x1, y1, x2, y2) ] = 1;
			wall[x2][y2][ get_dir(x2, y2, x1, y1) ] = 1;
	}

	//initalize DFS path computation
	
	//source = (0,0)
	//sink = (Nrow-1,Ncol-1)

	//when visited index becomes one
	bool iswhite[Nrow][Ncol];

	//assign to white = false
	for (i = 0; i < Nrow; i++) {
		for (j = 0; j < Ncol; j++) {
			iswhite[i][j] = false;
		}
	}

	//each cell has another cell linked to it
	//the link is the cell you came from
	cell link[Ncol][Nrow];

	//setting all points in link to itself
	for (i = 0; i < Ncol; i++) {
		for (j = 0; j < Nrow; j++) {
			link[i][j].x = i;
			link[i][j].y = j;
		}
	}

	//initializing source and sink
	cell source;
	source.x = 0; source.y = 0;
	cell sink;
	sink.x = Ncol-1; sink.y = Nrow-1;

	//carry out DFS source-to-sink computation
	
	//stack as an array
	//stack_point is where we are in the stack
	cell *stack;
	stack = new cell[Nrow*Ncol];
	int stack_point;

	int D[Nrow*Ncol];
	for (i = 0; i < Nrow*Ncol; i++)
		D[i] = 0;
	//solve

	cell point, to;
	stack[0].x = source.x;
	stack[0].y = source.y;
	D[0] = DIR_LEFT;
	iswhite[source.x][source.y] = true;
	stack_point = 1;
	
	while (stack_point > 0) {
		if (D[stack_point - 1] >= 4) {
			iswhite[stack[stack_point - 1].x][stack[stack_point - 1].y] = false;
			stack_point -= 1;
			//std::cout << "subtract\n";
		}

		if (stack_point == 0)
			break;

		point.x = stack[stack_point - 1].x;
		point.y = stack[stack_point - 1].y;

		if (point.x == sink.x && point.y == sink.y)
			break;

		to.x = point.x;
		to.y = point.y;
		//computing where the next cell in the path should be
		if (D[stack_point - 1] == 0) {
			//left
			//std::cout << "left\n";
			to.x -= 1;
			//to.y = point.y;
		}
		else if (D[stack_point - 1] == 1) {
			//right
			//std::cout << "right\n";
			to.x += 1;
			//to.y = point.y;
		}
		else if (D[stack_point - 1] == 2) {
			//up
			//std::cout << "up \n";
			//to.x = point.x;
			to.y -= 1;
		}
		else if (D[stack_point - 1] == 3) {
			//down
			//std::cout << "down\n";
			//to.x = point.x;
			to.y += 1;
		}

		D[stack_point - 1] += 1;

		//std::cout << " point " << point.x << " " << point.y << '\n';
		if (wall[point.x][point.y][D[stack_point - 1] - 1]) {
			//std::cout << point.y << "  " << point.x << " " << D[stack_point - 1] - 1 << '\n';
			continue;
		}

		if (iswhite[to.x][to.y])
			continue;

		//std::cout << "adding: " << to.y << " " << to.x << '\n';
		stack[stack_point].x = to.x;
		stack[stack_point].y = to.y;

		D[stack_point] = DIR_LEFT;
		iswhite[to.x][to.y] = true;
		stack_point++;
		//std::cout << to.x << " " << to.y << '\n';
	}

	fprintf(out, "PATH %d %d\n", Nrow, Ncol);

	for (i = 0; i < stack_point; i++) {
		fprintf(out, "%d %d\n", stack[i].y, stack[i].x);
	}

	fclose(in);
	fclose(out);
	//write PATH, Nrow, Ncol header
	//write cells on path to file

	//close open files
}
