#include "robot.h"

stopwatch watch;
vector<motor_action> motor_history;
int current_speed[2];

int grabber_bit = 0x10; // bit 4: ’0001 0000’
int grabber_on = 0;

int motor_speed = 127;
float correction_rate = 0.5;
int robot_direction = 1;
// int prev_direction;
int MOTOR_1_DIRECTION = 1; // multiply by 128 and add to motor_speed
int MOTOR_2_DIRECTION = 0; // corrects motor rotation direction

void init_motor() {
	rlink.command (RAMP_TIME, 0); // 255 for default
}

void init_motor_history() { // call after successful journey to prevent clutter
	motor_history.clear();
	current_speed[0] = 0;
	current_speed[1] = 0;
}

void motor_go(int motor_1_speed, int motor_2_speed) { // use for wheels 1 and 2
	int corrected_m1_speed = (motor_1_speed+MOTOR_1_DIRECTION*128)%256;
	int corrected_m2_speed = (motor_2_speed+MOTOR_2_DIRECTION*128)%256;
	#ifdef DEBUG
	cout << "Motors: " << motor_1_speed << ", " << motor_2_speed << " (" << corrected_m1_speed << ", " << corrected_m2_speed << ")" << endl;
	#endif
	// add previous motion to history
	motor_action past;
	past.speed[0] = current_speed[0];
	past.speed[1] = current_speed[1];
	// memcpy(past.speed, current_speed, 2); // does the job of the above 2 lines
	past.time = watch.read();
	motor_history.push_back(past);
	// set new speed
	current_speed[0] = corrected_m1_speed;
	current_speed[1] = corrected_m2_speed;
	rlink.command(MOTOR_1_GO, corrected_m1_speed);
	rlink.command(MOTOR_2_GO, corrected_m2_speed);
	watch.start(); // start timing after motor command
}

void motor_turn(int turn, int target_direction) {
	motor_speed = 64;
	// check each sensor, run wheels until aligned on cross
	motor_direction(1);
	motor_go(motor_speed, motor_speed); // change these to align individually?
	if(robot_direction == -1) {
		// delay(250);
		motor_direction(target_direction);
	}
	bool run = true;
	while(run) {
		int state = read_line();
		delay(5);
		// cout << state << endl;
		if(state == 110 || state == 11 || state == 10) {
			if(turn == PATH_LEFT || turn == PATH_LEFT_REVERSE) {
				motor_go(motor_speed+128, motor_speed);
			} else if(turn == PATH_RIGHT || turn == PATH_RIGHT_REVERSE) {
				motor_go(motor_speed, motor_speed+128);
			}
			run = false;
		} // currently no backup plan if 110, 100, 011 or 001 ... [FIX THIS]
	}
	// go into stop mode
	stopwatch watch;
	stopwatch overshoot_timer;
	int count = 0;
	int last = 0;
	int state;
	watch.start();
	overshoot_timer.start();
	while(count < 2 && overshoot_timer.read() < 2000) {
		// cout << count << ", " << last << ", " << watch.read() << endl;
		state = read_line();
		if((state%10 == 1 && turn == PATH_LEFT) || (state >= 100 && turn == PATH_RIGHT)) {
			if(last == 0 && watch.read() > 50) {
				count ++;
			}
			last = 1;
			watch.start();
		} else {
			last = 0;
		}
	}
	// straighten off
	int overshoot_time = 1100; // for t1... 950 for t2
	run = true;
	watch.start();
	while(run) {
		int turn_time = watch.read();
		if(turn_time > overshoot_time) {
			run = false;
		}
		if(turn == PATH_LEFT) {
			motor_go(0, motor_speed);
		} else if(turn == PATH_RIGHT) {
			motor_go(motor_speed, 0);
		}
	}
	/* if(robot_direction == target_direction ) {
		// do nothing?
	} else {
		if(robot_direction == -1) {
			motor_direction(1);
		} else if(robot_direction == 1) {
			motor_direction(-1);
		}
	} */
	// stop_motors();
	motor_speed = 127; // speed up again
}

void stop_motors() {
	// add previous motion to history
	cout << "Stopping motors" << endl;
	motor_action past;
	past.speed[0] = current_speed[0];
	past.speed[1] = current_speed[1];
	past.time = watch.read();
	motor_history.push_back(past);
	// set everything to 0
	current_speed[0] = 0;
	current_speed[1] = 0;
	watch.stop(); // reset
	rlink.command(MOTOR_1_GO, 0);
	rlink.command(MOTOR_2_GO, 0);
	robot_direction = 0;
}

void forklift_go(int speed, int time) { // forklift platform
	rlink.command(MOTOR_3_GO, speed);
	delay(time);
	rlink.command(MOTOR_3_GO, 10);
}

void grabber_go(int on) {
	/* int param;
	if (on == 0) {
		param = left_ir bitor mid_ir bitor right_ir bitor grabber_bit;
	} else {
		param = left_ir bitor mid_ir bitor right_ir;
	}
	rlink.command (WRITE_PORT_1, param); */
	grabber_on = on;
	update_IO();
}

void pick_shirt() {
	grabber_go(1);
	delay(500);
	forklift_go(127, 3000);
}

void drop_shirt() {
	grabber_go(0);
	motor_direction(-1);
	delay(1000);
	forklift_go(128+64, 3000);
}

void motor_direction(int direction) {
	stop_motors();
	delay(250);
	cout << "Change direction: " << direction << "          Current: " << robot_direction << endl;
	if(direction == 1 && robot_direction < 1) { // forwards
		robot_direction = 1;
		MOTOR_1_DIRECTION = 1;
		MOTOR_2_DIRECTION = 0;
		correction_rate = 0.5;
	} else if(direction == -1 && robot_direction > -1) {
		robot_direction = -1;
		MOTOR_1_DIRECTION = 0;
		MOTOR_2_DIRECTION = 1;
		correction_rate = 0.9;
	}
	delay(250);
	motor_go(motor_speed, motor_speed);
}
