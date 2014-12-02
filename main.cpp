/*
BacklightControl - Change LCD back light on windows via command line.
Copyright (C) 2014  inoakoala

BacklightControl is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <sstream>

#include "LCD.h"

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "PowrProf.lib")

using namespace std;

enum ACTION { UP, DOWN, SET, GET, USAGE };

void usage(int argc, char* argv[]) {

	cout << argv[0] << " action" << endl
		<< '\t' << "up" << '\t' << "increase lcd brightness by 1 step" << endl
		<< '\t' << "down" << '\t' << "decrease lc brightness by 1 step" << endl
		<< '\t' << "set value" << '\t' << "set lcd brightness to value where value is in the list returned by get" << endl
		<< '\t' << "get" << '\t' << "list all lcd brightness values supported for set action" << endl;
}

int main(int argc, char* argv[]) {

	ACTION action = USAGE;
	int value;

	if (argc == 2 && argv[1] == string("up")) {

		action = UP;
	}
	else if (argc == 2 && argv[1] == string("down")) {

		action = DOWN;
	}
	else if (argc == 3 && argv[1] == string("set")) {

		stringstream ss(argv[2]);
		ss >> value;
		if (ss) { action = SET; }
	}
	else if (argc == 2 && argv[1] == string("get")) {

		action = GET;
	}

	if (action == USAGE) {

		usage( argc, argv );
		return 2;
	}

	try {
		LCD lcd;

		switch (action) {
			case UP:
				lcd.increaseBrightness();
				break;
			case DOWN:
				lcd.decreaseBrightness();
				break;
			case SET:
				lcd.setBrightness(value);
				break;
			case GET:
				const vector<int>& bs = lcd.supportedBrightness();
				for (int i = 0; i < bs.size(); i++) {
					cout << bs[i] << endl;
				}
				break;
		}
	}
	catch( LCDException& e) {
		cout << e.what() << endl;
		return 2;
	}
}
