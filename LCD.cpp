/*
This file is part of BacklightControl

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

#include "LCD.h"

#include <PowrProf.h>
#include <winioctl.h>

#include <algorithm>

LCD::LCD() : _hLCD(0) {

	HANDLE hLCD = CreateFile( "\\\\.\\LCD", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hLCD == INVALID_HANDLE_VALUE) { 
		throw LCDException();
	}

	_hLCD = hLCD;

	fillSupportedBrightness();	
}

LCD::~LCD() {
	if (_hLCD) {
		CloseHandle(_hLCD);
	}
}

void LCD::fillSupportedBrightness() throw (LCDException) {
	
	char supportedBrightness[256];
	DWORD bytesReturned;

	int r = DeviceIoControl(
		_hLCD,
		IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS,
		NULL,
		0,
		supportedBrightness,
		sizeof(supportedBrightness),
		&bytesReturned,
		NULL
	);

	if (r == 0) {
		throw LCDException();
	}
		
	for (int i = 0; i < bytesReturned; i++) {
		_supportedBrightness.push_back( supportedBrightness[i] );
	}	
}

DISPLAY_BRIGHTNESS LCD::brightness() throw (LCDException) {

	DISPLAY_BRIGHTNESS queryDb;
	DWORD bytesReturned;

	int r = DeviceIoControl(
	  _hLCD,
	  IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS,
	  NULL,  
	  0,     
	  &queryDb,
	  sizeof(queryDb),
	  &bytesReturned,
	  NULL
	);

	if (r == 0) {
		throw LCDException();
	}
	
	return queryDb;
}

void LCD::increaseBrightness() throw (LCDException) {

	DISPLAY_BRIGHTNESS queryDb = brightness();

	std::vector<int>::iterator ac = std::find( _supportedBrightness.begin(), _supportedBrightness.end(), queryDb.ucACBrightness );
	std::vector<int>::iterator dc = std::find( _supportedBrightness.begin(), _supportedBrightness.end(), queryDb.ucDCBrightness );

	int nextAc = ac + 1 != _supportedBrightness.end() ? *(ac + 1) : *ac;
	int nextDc = dc + 1 != _supportedBrightness.end() ? *(dc + 1) : *dc;

	if (nextAc != *ac || nextDc != *dc) {
		setBrightness( max( nextAc, nextDc ) );
	}
}

void LCD::decreaseBrightness() throw (LCDException) {

	DISPLAY_BRIGHTNESS queryDb = brightness();

	std::vector<int>::iterator ac = std::find( _supportedBrightness.begin(), _supportedBrightness.end(), queryDb.ucACBrightness );
	std::vector<int>::iterator dc = std::find( _supportedBrightness.begin(), _supportedBrightness.end(), queryDb.ucDCBrightness );

	int nextAc = ac - 1 >= _supportedBrightness.begin() ? *(ac - 1) : *ac;
	int nextDc = dc + 1 >= _supportedBrightness.begin() ? *(dc - 1) : *dc;

	if (nextAc != *ac || nextDc != *dc) {
		setBrightness( min( nextAc, nextDc ) );
	}
}

void LCD::setBrightness(int b) throw (LCDException) {

	DISPLAY_BRIGHTNESS setDb;
	setDb.ucDisplayPolicy = brightness().ucDisplayPolicy;
	setDb.ucACBrightness = b;
	setDb.ucDCBrightness = b;
	
	DWORD bytesReturned;

	int r = DeviceIoControl(
	  _hLCD,
	  IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS,
	  &setDb,
	  sizeof(setDb),
	  NULL,
	  0,
	  &bytesReturned,
	  NULL
	);
	
	if (r == 0) {
		throw LCDException();
	}
}
