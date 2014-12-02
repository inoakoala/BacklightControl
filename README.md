# BacklightControl

Change LCD back light on windows via command line.

## Usage

### lcd up

increase lcd brightness by 1 step

### lcd down

decrease lcd brightness by 1 step

### lcd set value

set lcd brightness value where value is in the list returned by get

### lcd get

list all lcd brightness values supported for set action

## TODO

The screen brightness is updated and the new value is correctly displayed in the 
Control Panel > Hardware and Sound > Power Options window. However, no laptops tested
display their manufacturer brightness overlay when BacklightControl changes the 
lcd brightness.

## Installation

Download the standalone win32 executable [here](https://raw.githubusercontent.com/inoakoala/BacklightControl/downloads/lcd.exe)

## Building

Create a new Visual Studio project, copy the repository files into the project, and then build.

## LICENSE

GNU v3, see the file COPYING for more information.
