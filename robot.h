#include <iostream>
using namespace std;
#include <vector>
#include <stopwatch.h>
// robot headers
#include <robot-link.h>
#include <robot-instr.h>
#include <robot-delay.h>

/* ----- CLASSES ----- */
class motor_action {
	public:
		int speed[2]; // 0 - 255 for left [0] and right [1] motors
		int time; // milliseconds
};

class path {
	public:
		vector<int> nodes;
		vector<int> node_times;
		int num_nodes;
		int start;
		int end;
};

/* ----- GLOBAL DEFINITIONS ----- */
extern robot_link rlink;

#define ROBOT_NUM 8 // ID of network interface card
#define DEBUG 1 // comment out when not in debug mode

extern vector<motor_action> motor_history;
extern path vpaths[8];
extern int current_shirt;

extern int motor_speed;
extern float correction_rate;
extern int robot_direction;
extern int prev_direction;
extern int MOTOR_1_DIRECTION;
extern int MOTOR_2_DIRECTION;
extern int correction;
extern int grabber_on;

extern int left_ir;
extern int mid_ir;
extern int right_ir;
extern int grabber_bit;
extern int shirt_bit5;
extern int shirt_bit6;

#define STOP 0
#define START_FORWARD 1
#define START_REVERSE -1

#define PATH_FORWARD 2
#define PATH_RIGHT 3
#define PATH_LEFT 4
#define PATH_REVERSE 5
#define PATH_RIGHT_REVERSE 6
#define PATH_LEFT_REVERSE 7
#define PICK_UP 8
#define DELIVER 9
#define RAISE_LIFT 10
#define LOWER_LIFT 11
#define PICK_SHIRT 12
#define DROP_SHIRT 13
#define SHIRT_COLOUR 14

// #define CORRECT_RIGHT 0
// #define CORRECT_LEFT 1

#define S 20
#define C1 21
#define C2 22
#define D1 23
#define D2 24
#define D3 25

#define SHIRT_NONE 30
#define SHIRT_GREEN 31
#define SHIRT_YELLOW 32
#define SHIRT_WHITE 33

/* ----- FUNCTION PROTOTYPES ----- */
// sensor functions
int read_line();
int read_LDR();
int shirt_colour();
void update_IO();

// motor functions
void init_motor();
void init_motor_history();
void motor_go(int motor_1_speed, int motor_2_speed);
void motor_turn(int turn, int target_direction);
void stop_motors();
void motor_direction(int direction);
void forklift_go(int speed, int time);
void grabber_go(int on);

// fault recovery
int find_line();
void undo_step(int step);
void undo_last();

// path finding
void init_paths (path paths[]);
int find_path(int start, int end);
void follow_path(int path_key);
void follow_line(int state, int &prev_state);
void take_action(int path_key, int current_node);

// general functions
int initialise();
