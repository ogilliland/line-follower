#include "robot.h"

path vpaths[8];

void array2vec(int a[], vector<int>&v, int thesize)
{
  v.resize(thesize);
  copy(a, a+thesize, v.begin());
}

void init_paths (path paths[])
{
	// time of -1 => until next cross
	// otherwise run until EITHER next cross OR time elapsed
	int a1[] = {START_FORWARD, PATH_FORWARD, PATH_RIGHT, STOP, SHIRT_COLOUR, PICK_SHIRT};
	int a1_times[] = {-1,          -1,          675,      100,      100,        100};
	paths[0].num_nodes = 6;	
	array2vec(a1, paths[0].nodes, sizeof(a1));
	array2vec(a1_times, paths[0].node_times, sizeof(a1));
	paths[0].start = S;
	paths[0].end = C1;
	
	int a2[] = {START_REVERSE, PATH_LEFT, PATH_FORWARD, PATH_FORWARD, STOP, DROP_SHIRT, START_REVERSE,   STOP, LOWER_LIFT};
	int a2_times[] = {-1,           -1,        -1,           1000,      500,    1000,        500,         100,    100};
	paths[1].num_nodes = 8;
	array2vec(a2, paths[1].nodes, sizeof(a2));
	array2vec(a2_times, paths[1].node_times, sizeof(a2));
	paths[1].start = C1;
	paths[1].end = D1;
	
	int a3[] = {START_REVERSE, PATH_LEFT, PATH_FORWARD, PATH_LEFT, PATH_FORWARD, PATH_LEFT, PATH_LEFT, STOP, DROP_SHIRT};
	int a3_times[] = {-1,           -1,       -1,          -1,          -1,         -1,       2000,     500,    5000};
	paths[2].num_nodes = 9;
	array2vec(a3, paths[2].nodes, sizeof(a3));
	array2vec(a3_times, paths[2].node_times, sizeof(a3));
	paths[2].start = C1;
	paths[2].end = D2;
	
	int a4[] = {START_REVERSE, PATH_LEFT, PATH_FORWARD, PATH_LEFT, PATH_FORWARD, PATH_LEFT, PATH_FORWARD, PATH_RIGHT, PATH_FORWARD, STOP, DROP_SHIRT};
	int a4_times[] = {-1,         -1,         -1,          -1,          -1,         -1,          -1,          500,        100,       100,    5000};
	paths[3].num_nodes = 11;
	array2vec(a4, paths[3].nodes, sizeof(a4));
	array2vec(a4_times, paths[3].node_times, sizeof(a4));
	paths[3].start = C1;
	paths[3].end = D3;
	
	int a5[] = {START_REVERSE, PATH_REVERSE, PATH_REVERSE, PATH_REVERSE, PATH_FORWARD, PATH_RIGHT, STOP, SHIRT_COLOUR, PICK_SHIRT};
	int a5_times[] = {-1,            -1,          -1,          -1,           -1,          675,      100,      100,        100};
	paths[4].num_nodes = 8;
	array2vec(a5, paths[4].nodes, sizeof(a5));
	array2vec(a5_times, paths[4].node_times, sizeof(a5));
	paths[4].start = D1;
	paths[4].end = C1;
	
	int a6[] = {START_REVERSE, PATH_LEFT, PATH_RIGHT, PATH_RIGHT, PATH_FORWARD, PATH_LEFT, PATH_FORWARD, STOP, PICK_SHIRT};
	int a6_times[] = {-1,         -1,          -1,       -1,          -1,          -1,         1000,      500,    5000};
	paths[6].num_nodes = 9;
	array2vec(a6, paths[5].nodes, sizeof(a6));
	array2vec(a6_times, paths[5].node_times, sizeof(a6));
	paths[5].start = D2;
	paths[5].end = C1;
	
	int a7[] = {START_REVERSE, PATH_REVERSE, PATH_RIGHT, PATH_RIGHT, PATH_FORWARD, STOP, PICK_SHIRT};
	int a7_times[] = {-1,          -1,          -1,         -1,          2000,      500,    5000};
	paths[6].num_nodes = 7;
	array2vec(a7, paths[6].nodes, sizeof(a7));
	array2vec(a7_times, paths[6].node_times, sizeof(a7));
	paths[6].start = D3;
	paths[6].end = C1;
	
	int a8[] = {START_REVERSE, PATH_RIGHT, PATH_FORWARD, PATH_FORWARD};
	int a8_times[] = {-1,         -1,          -1,            500};
	paths[7].num_nodes = 4;
	array2vec(a8, paths[7].nodes, sizeof(a8));
	array2vec(a8_times, paths[7].node_times, sizeof(a8));
	paths[7].start = C1;
	paths[7].end = S;
}
