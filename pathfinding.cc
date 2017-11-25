#include "robot.h"

#define NUM_PATHS 8
stopwatch action_clock;
stopwatch lost_timer;

int find_path(int start, int end) {
	// find path
	for(int i = 0; i < NUM_PATHS; i++) {
		// cout << i << ", " << NUM_PATHS << " - " << vpaths[i].start << ", " << vpaths[i].end << endl;
	 	if(vpaths[i].start == start && vpaths[i].end == end) {
			cout << "Path " << i << " selected" << endl;
			return i;
		}
	}
	return 0;
}
void follow_path(int path_key) {
	bool run = true;
	int prev_state = -1;
	int current_node = 1;
	action_clock.start();
	delay(1000);
	int distance_trigger = 0;
	motor_direction(vpaths[path_key].nodes[0]);
	while(run) {
		int state = read_line();
		follow_line(state, prev_state);
		if(state != prev_state) { // if state has changed
			if((state == 111 || state == 101) && action_clock.read() > 1000) {
				cout << "Action triggered on cross" << endl;
				take_action(path_key, current_node);
				if(current_node == vpaths[path_key].num_nodes) { // check if there is another node after this one
					stop_motors();
					run = false;
				}
				current_node++;
				action_clock.start();
				distance_trigger = 0;
			}
		}
		if (action_clock.read() > vpaths[path_key].node_times[current_node-1] && vpaths[path_key].node_times[current_node-1] >= 0) {
			cout << "Action triggered on timeout" << endl;
			take_action(path_key, current_node);
			if(current_node == vpaths[path_key].num_nodes) { // check if there is another node after this one
				stop_motors();
				run = false;
			}
			current_node++;
			action_clock.start();
			distance_trigger = 0;
		}
		prev_state = state;
		// DISTANCE SENSOR (doesn't work)
		/* if(vpaths[path_key].end == C1) {
			if(rlink.request(ADC4) > 150 && current_node > vpaths[path_key].num_nodes-2) {
				distance_trigger++;
			} else if(rlink.request(ADC4) < 80 && distance_trigger > 1) {
				cout << "Action triggered on proximity" << endl;
				run = false;
			}
		} */
	}
	stop_motors();
	delay(1000);
	cout << "Stopped, ready to pick up or deliver shirt" << endl;
}

void follow_line(int state, int &prev_state) {
	if(state != prev_state) {
		if(state == 10) { // on line
			motor_go(motor_speed, motor_speed);
		} else if(state == 0) {
			lost_timer.start();
		} else {
			if(state == 110 || state == 100) {
				// drive left slower => turn left
				motor_go(motor_speed*correction_rate, motor_speed);
			} else if(state == 11 || state == 1) {
				motor_go(motor_speed, motor_speed*correction_rate);
			}
		}
	} else if(lost_timer.read() > 2000) {
		find_line();
	}
}

void take_action(int path_key, int current_node) {
	#ifdef DEBUG
	cout << "Taking action " << current_node << " of path " << path_key << " (" << vpaths[path_key].node_times[current_node] << " ms)" << endl;
	#endif
	int action = vpaths[path_key].nodes[current_node];
	switch(action) {
		case PATH_LEFT:
			cout << "Turn left" << endl;
			motor_turn(PATH_LEFT, 1);
			break;
			
		case PATH_FORWARD:
			cout << "Forward" << endl;
			motor_direction(1);
			motor_go(motor_speed, motor_speed);
			break;
			
		case PATH_RIGHT:
			cout << "Turn right" << endl;
			motor_turn(PATH_RIGHT, 1);
			break;
			
		case PATH_REVERSE:
			cout << "Reverse" << endl;
			motor_direction(-1);
			motor_go(motor_speed, motor_speed);
			break;
			
		case PATH_LEFT_REVERSE:
			cout << "Turn left" << endl;
			if(robot_direction == 1) {
				motor_turn(PATH_LEFT, -1);
			} else {
				motor_turn(PATH_RIGHT, -1);
			}
			cout << "Reverse" << endl;
			motor_direction(-1);
			break;
			
		case PATH_RIGHT_REVERSE:
			cout << "Turn right" << endl;
			if(robot_direction == 1) {
				motor_turn(PATH_RIGHT, -1);
			} else {
				motor_turn(PATH_LEFT, -1);
			}
			cout << "Reverse" << endl;
			motor_direction(-1);
			break;
			
		case RAISE_LIFT:
			forklift_go(127, 3000);
			break;
			
		case LOWER_LIFT:
			forklift_go(128+32, 5000);
			break;
			
		case PICK_SHIRT:
			grabber_go(1);
			delay(500);
			forklift_go(127, 3000);
			break;
			
		case START_FORWARD:
			cout << "Start forward" << endl;
			motor_direction(1);
			motor_go(motor_speed, motor_speed);
			break;
			
		case START_REVERSE:
			cout << "Start reverse" << endl;
			motor_direction(-1);
			motor_go(motor_speed, motor_speed);
			break;
			
		case DROP_SHIRT:
			grabber_go(0);
			break;
			
		case STOP:
			cout << "Stop" << endl;
			stop_motors();
			break;
			
		case SHIRT_COLOUR:
			current_shirt = shirt_colour();
			break;
	}
}
