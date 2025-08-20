# ShipStats
Auto-generate ship stats for the game Freelancer (2003).

## Installation instructions
Download the latest `ShipStats.dll` from [Releases](https://github.com/BC46/ShipStats/releases). Extract the `dll` to the `EXE` folder of your Freelancer installation.
Next, open the `dacom.ini` file add `ShipStats.dll` to the `[Libraries]` section.

## Build instructions
Build the project using the `NMAKE` tool from Microsoft Visual C++ 6.0 (VC6 or Visual Studio 98) with the included makefile.
This will output the `ShipStats.dll` file in the root project folder.
Building the project using other compilers may require the code to be modified.
