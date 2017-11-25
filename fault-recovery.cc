#include "robot.h"

stopwatch error_clock;

int find_line() { // pass useful information about last known state?
	int num_steps = motor_history.size() - 1;
	int found_line = false;
	for(int step = num_steps; step > 0; step--) {
		rlink.command(MOTOR_1_GO, motor_history[step].speed[0]);
		rlink.command(MOTOR_2_GO, motor_history[step].speed[1]);
		error_clock.start();
		bool run = true;
		while(run) {
			if(error_clock.read() > motor_history[step].time) {
				run = false;
			}
			int line_result = read_line();
			if(line_result > 0) { // we have found a line
				found_line = true;
				step = 0;
				run = false;
			}
		}
	}
	if(found_line) {
		// navigate using newly found line
		return 1;
	} else {
		// try some other method of fault recovery
		// if that fails:
		return 0;
	}
}

void undo_last() {
	int step = motor_history.size() - 1;
	undo_step(step);
	motor_history.erase(motor_history.begin() + step);
}

void undo_step(int step) {
	cout << "Undoing step " << step << " = " << motor_history[step].speed[0] << ", " << motor_history[step].speed[1] << " (" << motor_history[step].time << " ms)" << endl;
	rlink.command(MOTOR_1_GO, (motor_history[step].speed[0]+128)%256);
	rlink.command(MOTOR_2_GO, (motor_history[step].speed[1]+128)%256);
	error_clock.start();
	bool run = true;
	while(run) {
		if(error_clock.read() > motor_history[step].time) {
			run = false;
		}
	}
}
