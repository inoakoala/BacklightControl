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

#ifndef LCD_H
#define LCD_H

#include <vector>
#include <exception>

#include <Windows.h>

#define IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS \
  CTL_CODE(FILE_DEVICE_VIDEO, 0x125, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS \
  CTL_CODE(FILE_DEVICE_VIDEO, 0x126, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS \
  CTL_CODE(FILE_DEVICE_VIDEO, 0x127, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _DISPLAY_BRIGHTNESS {
  UCHAR ucDisplayPolicy;
  UCHAR ucACBrightness;
  UCHAR ucDCBrightness;
} DISPLAY_BRIGHTNESS, *PDISPLAY_BRIGHTNESS;

class LCDException : public std::exception {

private:
	char _message[256];

public:
	LCDException();

	const char* what() { return _message; }
};

class LCD {
private:
	HANDLE _hLCD;
	std::vector<int> _supportedBrightness;

public:
	LCD() throw (LCDException);
	~LCD();

	const std::vector<int>& supportedBrightness() { return _supportedBrightness; }

	DISPLAY_BRIGHTNESS brightness() throw (LCDException);

	void increaseBrightness() throw (LCDException);
	void decreaseBrightness() throw (LCDException);

	void setBrightness(int b) throw (LCDException);

private:
	void fillSupportedBrightness() throw (LCDException);
};

#endif