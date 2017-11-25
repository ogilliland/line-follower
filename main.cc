#include "robot.h"

robot_link rlink;
int current_shirt = 0;

int initialise() {
	int status;
	cout << "Initialising robot link" << endl;	
	#ifdef __arm__
	if (!rlink.initialise ("127.0.0.1")) { // local hardware
	#else
	if (!rlink.initialise (ROBOT_NUM)) { // wireless link
	#endif
		cout << "	- Cannot initialise link" << endl;
	    	rlink.print_errs("    ");
	    	return -1;
	} else {
		cout << "	- Success" << endl;
	}
	// check response from robot
	#ifdef DEBUG
	cout << "	- Sending test instruction" << endl;
	#endif
	int val = rlink.request(TEST_INSTRUCTION);
	if(val == TEST_INSTRUCTION_RESULT) {
		#ifdef DEBUG
		cout << "	- Test passed" << endl;
		#endif
		status = 1;
	} else if (val == REQUEST_ERROR) {
		cout << "	- Fatal errors on link:" << endl;
		rlink.print_errs();
		return -1;
	} else {
		cout << "	- Test failed (bad value returned)" << endl;
		return 0;
	}
	// paths
	init_paths(vpaths);
	// motors
	init_motor();
	init_motor_history();
	// sensors
	update_IO();
	return status;
}

int main () {
	int status = initialise();
	int num_shirts;
	int route;
	int end_node;
	if(status == 1) {
		cout << "Number of shirts to collect: ";
		cin >> num_shirts;
		route = find_path(S, C1);
		follow_path(route);
		for(int i = 0; i < num_shirts; i++) {
			end_node = D1;
			if(current_shirt == SHIRT_WHITE) {
				end_node = D3;
			} else if(current_shirt == SHIRT_YELLOW) {
				end_node = D2;
			} else if(current_shirt == SHIRT_GREEN) {
				end_node = D1;
			}
			route = find_path(C1, end_node);
			follow_path(route);
			// return for another shirt
			route = find_path(end_node, C1);
			follow_path(route);
		}
	}
}
