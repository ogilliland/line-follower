#include "robot.h"

int param;
int left_ir = 0x01;  // bit 0: ’0000 0001’
int mid_ir = 0x02;   // bit 1: ’0000 0010’
int right_ir = 0x04; // bit 2: ’0000 0100’
int shirt_bit5 = 0x20; // bit 5: '0010 0000'
int shirt_bit6 = 0x40; // bit 6: '0100 0000'

int read_line() {
	int result = 0;
	param = rlink.request(READ_PORT_1);
	if (param bitand left_ir) {
		result += 100;
	}
	if (param bitand mid_ir) {
		result += 10;
	}
	if (param bitand right_ir) {
		result += 1;
	}
	return result; // binary representation of IR sensor readings: left, mid, right
}

int read_LDR() {
	int param = rlink.request(ADC1);
	return param;
}

int shirt_colour() {
	int bucket_white = 0;
	int bucket_yellow = 0;
	int bucket_green = 0;
	int bucket_none = 0;
	for(int i=0;i<100;i++) {
		int v = rlink.request (ADC3);
		cout << v << endl;
		if(v >= 95 && v <= 105) {
			bucket_white++;
		} else if(v >= 65 && v <= 70) {
			bucket_green++;
		} else if(v >= 72 && v <= 77) {
			bucket_yellow++;
		} else if(v >= 155) {
			bucket_none++;
		}
		delay(10);
	}
	#ifdef DEBUG
	cout << bucket_yellow << " yellow" << endl;
	cout << bucket_green << " green" << endl;
	cout << bucket_white << " white" << endl;
	cout << bucket_none << " none" << endl;
	#endif
	int shirt = SHIRT_NONE;
	if(bucket_white >= bucket_yellow && bucket_white >= bucket_green && bucket_white >= bucket_none) {
		shirt = SHIRT_WHITE;
		cout << "Shirt: white (11)" << endl;
	} else if(bucket_yellow >= bucket_white && bucket_yellow >= bucket_green && bucket_yellow >= bucket_none) {
		shirt = SHIRT_YELLOW;
		cout << "Shirt: yellow (01)" << endl;
	} else if(bucket_green >= bucket_yellow && bucket_green >= bucket_white && bucket_green >= bucket_none) {
		shirt = SHIRT_GREEN;
		cout << "Shirt: green (10)" << endl;
	} else {
		shirt = SHIRT_NONE;
		cout << "Shirt: none (00)" << endl;
	}
	current_shirt = shirt;
	update_IO();
	return shirt;
}

void update_IO() {
	int line_param = left_ir bitor mid_ir bitor right_ir;
	int grabber_param = 0x00;
	if(grabber_on == 0) {
		grabber_param = grabber_bit;
	}
	int shirt_param = shirt_bit5 bitor shirt_bit6;
	if(current_shirt == SHIRT_WHITE) {
		shirt_param = 0x00;
	} else if(current_shirt == SHIRT_YELLOW) {
		shirt_param = shirt_bit5;
	} else if(current_shirt == SHIRT_GREEN) {
		shirt_param = shirt_bit6;
	}
	param = line_param bitor grabber_param bitor shirt_param;
	rlink.command (WRITE_PORT_1, param);
}
