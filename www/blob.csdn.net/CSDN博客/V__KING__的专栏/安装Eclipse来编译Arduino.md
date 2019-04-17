# 安装Eclipse来编译Arduino - V__KING__的专栏 - CSDN博客





2013年10月18日 14:07:11[v__king__](https://me.csdn.net/V__KING__)阅读数：2453








[Eclipse](http://eclipse.org/) is a free, powerful, and full-featured development environment that can be set up
 to work with AVR and Arduino. This page is very much a work-in-progress, please feel free to add to it or improve it.


This page explains how to setup Eclipse with WinAVR and the Eclipse plugin AVR-eclipse.There is an easier 100%free and opensource way to use Eclipse.
 For details see[http://www.baeyens.it/eclipse/](http://www.baeyens.it/eclipse/)

# Navigation
- [Installation](http://playground.arduino.cc/Code/Eclipse#Installation)- [Eclipse and additional plugins](http://playground.arduino.cc/Code/Eclipse#Eclipse_and_additional_plugins)
- [GNU AVR-GCC toolchain](http://playground.arduino.cc/Code/Eclipse#AVR_GCC_toolchain)
- [Obtaining the Arduino Core Library](http://playground.arduino.cc/Code/Eclipse#Arduino_core_library)

- [Eclipse configuration for the Arduino core library](http://playground.arduino.cc/Code/Eclipse#Eclipse_Arduino_core_library)
- [Your first Arduino project](http://playground.arduino.cc/Code/Eclipse#YourFirstArduinoProject)
- [Your second Arduino project](http://playground.arduino.cc/Code/Eclipse#YourSecondArduinoProject)
- [Other issues](http://playground.arduino.cc/Code/Eclipse#OtherIssues)
- [Eclipse template project](http://playground.arduino.cc/Code/Eclipse#EclipseTemplateProject)

# Installation

## Eclipse and additional plugins
- 
Eclipse IDE for C/C++ Developers: [http://www.eclipse.org/downloads](http://www.eclipse.org/downloads)You
 can also just add the C Development Toolkit to an existing installation. This tutorial has been tested successfully with Europa 3.3.2 Winter, Ganymede 3.4.1. and Galileo 3.5. It was also tested with eclipse 3.7.1 indigo and AVR plugin 2.3.4.20100807PRD in
 Oct 2011.
- [AVR Plugins for Eclipse](http://avr-eclipse.sourceforge.net/wiki/index.php/Plugin_Download). Install the most
 recent version (2.3.4 as of this writing) using the Eclipse Software Updates method described on the download page.

If the automatic installation doesn't work, manually copy this "plugin" into the the Eclipse installation. Move the folder which begins with the name "de.innot.avreclipse" to the "dropins" folder located within the eclipse folder. Restart Eclipse if you have
 it already running.

There should now be a new "AVR" toolbar button in your workspace.
- 
Target Management Project. This is optional, it is used to monitor the serial board directly within Eclipse for debugging purposes.

### Notes

#### openSUSE


There are no current packages, but it's easy to install as per above. Create e.g.`/usr/local/pckg/eclipse` and unpack the downloaded eclipse archive. Get the eclipse C/C++ release with all the Linux add-ons. Create a symlink
 in `/usr/local/bin` to the `eclipse`executable in the unpacked eclipse directory. Installing plugins from inside eclipse gets them installed in the user's home directory, `$HOME/.eclipse/`

## GNU AVR-GCC toolchain

### Mac OS X
- 
Fink: fink install avr-libc avr-gcc avr-binutils avrdude
- MacPorts: [Mac
 OS X with MacPorts](http://bot-thoughts.blogspot.com/2008/02/avr-programming-on-mac.html)
- 
Building from source: [installing dev tools on os X](http://wiki.monome.org/view/InstallingDevToolsOnOsX)
- 
Easy installer with [CrossPack for AVR Development](http://www.obdev.at/products/crosspack/index.html)

### openSUSE
- 
Add repository for your version.
 zypper -v ar -k -f [http://download.opensuse.org/repositories/CrossToolchain:/avr/openSUSE_11.4/](http://download.opensuse.org/repositories/CrossToolchain:/avr/openSUSE_11.4/)- 
Install packages. The insight ones are for the debugger GUI. These are the same packages that are also required for the Arduino IDE.
 zypper -v in avrdude avr-libc avr-example cross-avr-gcc cross-avr-insight

### Debian/Ubuntu
- 
apt-get install avrdude binutils-avr gcc-avr avr-libc gdb-avr

### Windows


The avr-eclipse plugin cannot automatically find the WinAVR installed with the IDE. Therefore you need to either configure the Eclipse plugin to find WinAVR in the IDE location, or
 install WinAVR in the default location.

#### Configuring Eclipse to point to the IDE location


In Eclipse, select Window->Preferences, then click the AVR tree view entry.

##### Paths tab
- 
Select "Disable search for system paths at startup", then modify the paths for each of the four entries below, as follows (select the first entry, press 'Edit..', set Path Source to 'Custom', then click on Browse to specify a path):
- 
AVR-GCC: "C:\path-to-arduino\hardware\tools\avr\bin"
- 
GNU make "C:\path-to-arduino\hardware\tools\avr\utils\bin"
- 
AVR Header files "C:\path-to-arduino\hardware\tools\avr\avr\include"
- AVRDude "C:\path-to-arduino\hardware\tools\avr\bin"



Select OK to close the Preferences window.

#### Customising AVRDude

Open the project properties (right click the project and select properties). Select AVR->AVRDude the tab programmer.(**这里应该是Windows->Preferences->AVR->AVRDude->Add**)Create
 a new programmer configuration using the new button.
- 
Programmer hardware: select "Atmel STK500 Version 1.x Firmware".
- 
Overwrite default port: enter the COM port preceded with "\\.\" (e.g., \\.\COM19)
- 
Override default baud rate: enter the value found in boards.txt file, in the arduino IDE.


One more step is needed (under Win7 with Eclipse 4.2, AVR plugin v2.4, and Arduino IDE v1.0.1) to get AVRDude working. When invoked from Eclipse, AVRDude cannot find its config
 file. To fix this, copy the AVRDude config file from:

"C:\path-to-arduino\hardware\tools\avr\etc\avrdude.conf" to

"C:\path-to-arduino\hardware\tools\avr\bin\avrdude.conf".


You should now be able to compile and upload Arduino projects in Eclipse.


Note: As these are workspace settings you may have to redo this setup for each workspace you use.


Install the latest version of [[http://winavr.sourceforge.net/](http://winavr.sourceforge.net/) | WinAVR]].
 This should work "out of the box" with the latest version of the AVR plugin, as long as you install WinAVR to the default directory. If it doesn't, follow the instructions for using WinAVR here:[Programming
 AVR with Eclipse and WinAVR](http://tinkerlog.com/2007/06/03/programming-avr-with-eclipse-and-winavr/)

## Obtaining the Arduino Core Library


There are three ways to get the Arduino core library:
- Getting an existing binary from an Arduino IDE project
- Compiling the library directly into your project
- Compiling your own library file.


The library source code is included in the [Arduino IDE](http://arduino.cc/en/Main/Software) download, in the hardware/cores/arduino
 directory. At the very least, you will need the header files from that directory accessible to your Eclipse project:
    * HardwareSerial.h
    * WProgram.h
    * wiring.h
    * WConstants.h
    * binary.h
    * pins_arduino.h
    * wiring_private.h

You will also need the .a static library.


The resulting program becomes substantially smaller when linking with a static library, so this is the only sensible option. (The link order matters too!)

### 1. Copying the library from an Arduino IDE project


This is the preferred method. However, I (Ecologisto) couldn't use it on Mac as I didn't find the specified files. The third option did work fine on mac though.

From any Arduino IDE project, get the core.a file in the compilation subdirectory. To generate this file you will need to create and verify a project
 in the Arduino IDE. It can be any program, such as the [Blink program](http://www.arduino.cc/en/Tutorial/Blink).
 Make sure that the correct target board is selected in the Arduino IDE.


The compilation subdirectory is a temporary folder. For example, in Windows 7, the object files might be stored in the following directory:
 C:\Users\<username>\AppData\Local\Temp\buildXXXXXXXXXXXXXXXXXXXXX.tmp


And for Linux it will be:
 /tmp/buildXXXXXXXXXXXXXXXXXXXXX.tmp/core.a


And on a Mac (>=10.5) it will be somwhere under:
 /var/folders
 


The X's in the build...tmp directory are a hash that map to the project whose temporary files are held in the directory. The temporary files are named after the project, so you can verify that you're copying the correct library.

Copy the core.a file into your own project directory,and rename it to libArduinoCore.a. You can call it anything, so long as it starts with "lib" and ends with ".a". It might be a good idea to name the file
 after its target, e.g. libArduinoMegaCore.a, so that you can always tell what the library's target architecture is.

### 2. Compiling the library code directly into the Eclipse application project


You can copy the entire contents of the hardware/cores/arduinodirectory into your Eclipse project so that it is compiled into the application every time. This requires that you use C++ projects, and the projects
 will take a bit longer to compile. This generally isn't a very good ideaunless you're hacking the core code.

### 3. Compiling your own static library


You can compile your own static library in Eclipse. The best way is to create this as a library project, and to define a build configuration for each combination of AVR CPU and clock frequency you have.


The sources are copied from the Arduino IDE. Download that, and unpack it somwhere.

# Eclipse configuration for the Arduino core library

## Create eclipse project
- 
In Eclipse, click File->New->C++ Project.
- Select the Empty Project option under AVR
 Cross Target Static Library (not "AVR Cross Target Application").
- Enter the project name (e.g. "DiecimilaCore", or "arduino_core"). Click Next.Deselect all but "Debug".
 Click next again.
- 
Set the MCU type and frequency to the desired target architecture, but it appears to be ignored. The build configurations are defined later.

For example, for the Diecimila, use ATmega168 running at 16000000 Hz. For new versions of the Duemilanove, use ATmega328P running at 16000000 Hz (same for the Uno). If you look closely
 at the Arduino board, you can see the target platform written on the main processor. Click Finish.
- Select the library project you just created, andadd a new source folder
 (File->New->Source folder). It's commonly named "src".

## Configure build targets and target hardware properties
- 
In Eclipse, click Project->Properties.
- 
Select C/C++ Build and expand the category (e.g., click the diamond to the left of "C/C++ Build").
- Click Manage configurations (top right). Click New. Enter a name, e.g. `328P_16MHz` (no
 spaces!). Add a descriptive text. Copy settings from Debug. Click OK.Select
 the configuration just added, click Set active. Click OK.
- Repeat the previous step for the Arduinos you want to program, e.g. `328P_8MHz`, `1280_16MHz`.
- 
Click apply.
- 
Might as well delete the now unused build configuration "Debug".
- Select AVR, and tick Individual
 settings for build configurations. Click Apply!
- 
Select AVR -> Target hardware, and a build configuration.
- 
For each build configuration select the MCU type and clock frequency. Click apply (or better OK, and re-open the dialog).

## Configure C compiler settings
- Select Settings under C/C++ Build. Make sure toselect All
 configurations for the settings below, because you want the settings to be the same for all library builds except for CPU type and speed! Warning: the configuration selection tends to revert back to the active
 one! Don't get confused.
- 
In the right pane, Click AVR Compiler and then Debugging.
- 
Leave the "Generate Debugging Info" on the default of -g2 and stabs(avr-gdb/insight). Turning this off is silly because it does not affect the size of the program and debugging is impossible without this info. The library file gets larger, which is irrelevant.
- In the right pane, Click AVR Compiler, then Optimization
- Set the Optimization Level to "Size Optimizations".(Settings->AVR Compiler->)
- Set "Other Optimization Flags" to "-ffunction-sections -fdata-sections" (this
 is to reduce hex file size).
- To get the same result as from the Arduino IDE,turn off "Pack structs" and "Short enums",（感觉这一步是多余的） although
 they may reduce program size (or increase it).
- Select "AVR Compiler"->"Language standard"andturn
 off "char is unsigned"and "bitfields are unsigned".（根据arduino中也是disabled） The Arduino
 IDE has these disabled too.
- Select "Directories", click on the add icon.Click
 workspace, andnavigate to the library project's "src" folder. Click OK. The directory in the
 input field is now `${workspace_loc:/${ProjName}/src}`. Click OK.

## Configure C++ compiler settings
- 
In the right pane, Click AVR C++ Compiler. Apply the same changes as for the C compiler. Check the build configuration is still set to All
 configurations!
- 
Select Debugging. Leave the "Generate Debugging Info" on the default.
- 
Select AVR C++ Compiler -> Optimization
- 
Set the Optimization Level to "Size Optimizations". Disable "pack structs" and "short enums". Add "-ffunction-sections -fdata-sections".
- 
Select Language standard, leave the "compiler default", disable "char is unsigned" and "bitfields are unsigned". You MUST enable "don't use exceptions".
- 
Select "Directories", add, click "workspace", and add the project's "src" directory.
- 
Click OK.

## Archiver / Listing / Print size settings
- 
The defaults work. When changing the command line pattern for listing or print size there is a bug when using configuration "[all configurations]" that causes input in either field being copied to both. These two patterns can only be changed for individual
 configurations.

## Install/link source files
- Copy the contents of the Arduino IDE's hardware/cores/arduino directory into this "src" folder. If you are
 using Arduino >1.0, you also need to copy the pins_arduino.h file out of the folder that matches your target under hardware/arduino/variants/.
On Linux, the smart way to do this is to create symlinks instead (with `ln -s`).
On Mac, you will have to right click on the Arduino application and show the content of the bundle. Then go to Resources->Java->hardware->Arduino and copy the files from here.
- 
NOTE: Trying to use eclipse file/folder links instead of Linux symlinks always went belly-up with the project settings pretty quickly for me. Either eclipse or the AVR plugin are pretty buggy here. Don't use the eclipse links feature.
- Build the project by clicking the hammer in the tool bar, or by selectingProject->Build
 All. It will build a file called libProjectName.a (e.g. libDiecimilaCore.a). This is the core library file.（在里面会有一个mian.cpp,开始的时候没有发现，可删除，编译的时候，头文件不是#include<file.h>而是#include“file.h”来包括。或者在前面的时候，我们包含头文件的路径应该是src/arduino，而不是src）
- By default only the currently active build configuration is built. This can be changed in the workspace preferences: ClickWindow -> Preferences,
 expand the C/C++ item, and select Build.
 Under "Build scope", select Build all configurations in each project.
- 
The library only needs to be built once (for each build configuration), as long as the library project or workspace are not cleaned.
- 


# Your first Arduino project

## Setup your project
- 
Go to File -> New -> C++ Project
- 
Select Empty Project under AVR-GCC Toolchain Cross Target Application
- 
Give the project a name. Click next, deselect the second build configuration if you don't need it, click next.
- 
Select the appropriate MCU type and clock frequency. For the Diecimila, use ATmega168 with a frequency of 16000000. New versions of the Duemilanove use the ATmega328P (so does the
 Arduino Uno). The Arduino Mega uses the ATmega1280.
- 
Select Project->Properties
- 
Select C/C++ Build -> Settings in the left pane
- 
Select Additional Tools in Toolchain in the right pane
- 
Check Generate HEX file for Flash memory
- 
Check Print Size.
- 
If you want Eclipse to upload the firmware to the Arduino after every build, check AVRDude. Or instead just use from the menu AVR -> Upload project to target, and leave AVRDudeunchecked.

- 
Select AVR Assembler -> Debugging- 
Leave Generate Debugging Info on defaults. Again, this is required for debugging and does not affect program size.

- 
Select AVR Compiler -> Directories- 
If the core header files are not in the project directory, add the path to the header files.
- 
Example: ${workspace_loc:/arduino_core/src} with `arduino_core` the name of the library project created earlier, and `src` the name of the source directory in the library project.

- 
Select AVR Compiler -> Debugging- 
Leave Generate Debugging Info on defaults.

- 
Select AVR Compiler -> Optimization- 
Make the same changes as for the libray project.
- 
Set Optimization Level to Size Optimizations (-Os)
- 
Set "Other Optimization Flags" to "-ffunction-sections -fdata-sections"
- 
Uncheck "pack structs" and "short enums".

- 
Select AVR Compiler -> Language Standard- 
Make the same changes as for the libray project.
- 
Uncheck "char is unsigned" and "bitfields are unsigned".

- 
Select AVR C++ Compiler -> Directories- 
Make the same changes as for the libray project.
- 
If the core header files are not in the project directory, add the path to the header files (as for the C compiler).

- 
Select AVR C++ Compiler -> Debugging- 
Leave Generate Debugging Info on defaults.

- 
Select AVR C++ Compiler -> Optimization- 
Set Optimization Level to Size Optimizations (-Os)
- 
Set "Other Optimization Flags" to "-ffunction-sections -fdata-sections"
- 
Uncheck "pack structs" and "short enums".

- 
Select AVR Compiler -> Language Standard- 
Uncheck "char is unsigned" and "bitfields are unsigned".
- 
"Don't use exceptions" MUST stay checked.

- 
Select AVR C/C++ Linker- 
If you're using C++ then the hex file can get really big. In the linker menu, change the command to avr-gcc and the command line pattern to the following:
- 
${COMMAND} --cref -s -Os ${OUTPUT_FLAG}${OUTPUT_PREFIX}${OUTPUT} ${INPUTS} -lm ${FLAGS}
- 
Try without this change first.

- 
Select AVR C/C++ Linker -> Libraries- 
Select Libraries Path and add the path to the core library. As there are multiple build configurations for the library, you MUST select the one matching this project's CPU type and
 clock!
- 
Select Libraries and add the core library name. This is the same as the library project's name e.g. arduino_core.
- 
For example, if the library is in the project under ${workspace_loc}/arduino_core/328P_16MHz/libarduino_core.a, the path will be "${workspace_loc:/arduino_core/328P_16MHz}" and the library name will be "arduino_core".
- 
As last item on the library list, add the library named "m". This comes with the C compiler, and even if not used, reduces program size. The Arduino IDE adds this too.
- 
Select AVR C/C++ Linker -> General and set Other
 Arguments to "-Wl,--gc-sections"

- 
Create symbol table
- 
Optional. Shows all objects (functions, variables, etc) in the program, sorted by address. In Settings -> AVR C++ Linker, under "expert settings", append (do not replace - append!)
`; avr-nm -C -n ${OUTPUT} >${BuildArtifactFileBaseName}.symbol`



# Your second Arduino project


This project setup is as close as possible to the Arduino IDE. On Linux it is possible to use the same source files for an Arduino sketch and an eclipse project (just don't edit them with both at the same time), and compile/upload either with eclipse or the
 Arduino IDE.
- Create the Arduino core library project as described above. This only needs to exist once in the eclipse workspace.
- Create the C++ project.
- 
Add 3 source folders lib, arduinolib, and src.

- Copy source files
- 
Copy the source files (.cpp, .c, .h) from the Arduino IDE's "libraries" directory into arduinolib (e.g. EEPROM, SPI, Wire). Only copy the ones used by the program. Don't forget to check the utility subdirectories. On Linux create symlinks instead.
- 
Copy the source files from any other Arduino libraries into lib, both the ones downloaded from somewhere and the ones you created. On Linux create symlinks instead.
- 
Copy the program (Arduino sketch) source files into src. On Linux create symlinks instead.
- 
Either rename the .pde to .cpp, or
- 
Project Properties -> C/C++ General -> File Types: add new pattern `*.pde` of type C++ source file,and
- 
Add "`-x c++`" to the C++ compiler settings.

- 
Select the project in the eclipse explorer and press F5 (refresh).
- 
Right-click on the project and select Index -> Rebuild. This seems to be frequently necessary (AVR plugin bug?).

- Configure project
- 
Enter Project -> Properties -> C/C++ Build -> Settings.
- 
Configure additional tools, warnings, optimisations as in the first Arduino project, except for:
- 
AVR Compiler -> Directories: add in this order:
`"${workspace_loc:/arduino_core/src}"`
`"${workspace_loc:/${ProjName}/arduinolib}"`
`"${workspace_loc:/${ProjName}/lib}"`

The select all and copy/paste them into AVR C++ Compiler -> Directories.
- 
AVR Compiler -> Symbols: add under "define" ARDUINO=22, and copy/paste to C++ compiler. This is the Arduino release from which the core and libraries are taken.
- 
AVR C++ Linker -> Libraries: Under libraries, add arduino_core and m (in that order). Under path, add the path to the correct core library, e.g. `"${workspace_loc:/arduino_core/328P_16MHz}"`
- 
Add `-Wl,--gc-sections` to the linker options.
- 
For smaller programs, change the linker from avr-g++ to avr-gcc.

- Programming aspects
- 
Supply prototypes for all functions, even for the .pde files. This doesn't hurt when compiling with the IDE.
- 
All modules using Arduino core or library functions must use `#include <WProgram.h>`, even the .pde files. It doesn't hurt with the IDE, which does it automatically.


# Other issues

## Undefined symbol __cxa_pure_virtual


The C++ standard apparently requires this function when using certain language features. This is e.g. the case when using the Print class, or the Serial object, which probably means most Arduino programs. Interestingly, when compiling programs with the Arduino
 IDE this function remains undefined and no error is produced. It seems this can be achieved with eclipse AVR programs and the following linking order: user program files, user libraries, Arduino libraries, libarduino_core.a, libm.a.


If you do need this function, the most flexible is to put this into a file `pure_virtual.cpp` in the project's source folder:
extern "C" void __cxa_pure_virtual()
{
#ifdef __AVR__
    asm("cli");
#endif
    while (1)
	;
}

The contents of the __cxa_pure_virtual function can be any error handling code; this function will be called whenever a pure virtual function is called. This file can be excluded from being compiled into the project by right-clicking on its name, selecting
 properties, and checking "exclude from build".

## new and delete operators for C++


Using new, delete, and dynamic memory management generally is not recommended for small embedded devices because it can become a resource hog. If you must have it, create a `.cpp` file in the project source folder with this
 content:
#include <stdlib.h>

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new(size_t size) 
{
  return malloc(size);
}

void operator delete(void * ptr) 
{
  free(ptr);
}

void * operator new[](size_t size) 
{
    return malloc(size);
}

void operator delete[](void * ptr) 
{
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}

## Compiler warnings


Turning on "pedantic" compiler warnings by adding '--pedantic' to AVR Compiler -> Miscellaneous -> Other flags and AVR
 C++ Compiler -> Miscellaneous -> Other flags could save some headache.

## printf() float formats


The last trap is in the fact that by default printf is not supporting float numbers. However because the C++ projects can be complex, it is wise to go to Project Options/Libraries section, you might want to include
- 
libprintf_flt
- 
libscanf_flt
- 
libc
- 
libm
- 
libobjc


but NOT libprintf_min, libscanf_min.

## Arduino target reset for uploading


The Arduino environment issues a reset command prior to uploading. This reset command is essential for any kind of bootloader communication. If your hardware is not reset there is no way for avrdude to communicate with the bootloader. Any attempt will result
 in errors like:
"avrdude: stk500_2_ReceiveMessage(): timeout".


Newer versions of AVRDude (such as the one shipped with CrossPack) will also do this.

### Linux


Instead of using programmer stk500v1 with AVRdude, use programmer arduino and it creates the reset automatically. In the project properties, change AVR -> AVRDude, Programmer tab,
 Programmer configuration. Edit the existing one or add a new one.

### MS-Windows


If you have an older version and are using Windows, you can overcome this problem with this little app. [Attach:newavrdude.exe](http://playground.arduino.cc/uploads/Code/newavrdude.exe)


To use this, you need to:


1) rename the avrdude.exe that is currently being used by eclipse to "realavrdude.exe" 2) Make sure that in the eclipse configuration for avrdude, you specify the port override. This should add something like "-P//./COM7"
 and it needs to be argument #3 (if this causes a problem for anyone, let me know and I'll build in some configuration stuff). 3) copy newavrdude.exe into the same directory as realavrdude.exe, and rename it to avrdude.exe

## main()


When using the Arduino IDE, you just have to define setup() and loop() functions and the IDE already has a main functional defined in the core library that calls setup() and loop() functions.


When setting up the Arduino core library as described above, it is functionally identical to the Arduino programming environment. That is, don't provide main(), but do define setup() and loop(). This is the easiest option.


If you must have your own main(), remove it from the arduino_core library. Eclipse does not define the main() function for you. However, you can use Arduino's default main() function by copying the main.cxx file from the hardware/cores/arduino directory in
 the Arduino IDE package. You might have to rename this file to main.c for Eclipse to recognize it as a code file. Linking in main.cxx will allow you just to define the setup() and loop() functions like you would in the Arduino IDE.


You must #include "WProgram.h" in your application program to gain access to the Arduino API.


If you choose to define your own main function, you must NEVER return from main(). I mean, you-MUST-NEVER-return-from-main. In human language, this could be translated as "the main()
 function must contain some kind of endless loop, because if it ends, the Arduino won't stop the program but will just keep reading random data as code". You must also always add a call to init(); as your first instructions. Not doing this will prevent any
 time-related functions from working.


So, your basic code will usually look like :
int main(void) {

  /* Must call init for arduino to work properly */
  init();

  /****************************/
  /* Add your setup code here */
  /****************************/

  for (;;) {

    /****************************/
    /*** write main loop here ***/
    /****************************/

  } // end for

} // end main

## Uploading to the Arduino

### Linux


Configure the correct USB serial port in AVRDude. It's one of /dev/ttyUSBN, with N being some digit. Best in the long run is to configure udev rules for your Arduino board serial numbers that create a symbolic link to
 the USB device currently being used by your board. The link name remains constant.

### MS-Windows

#### Figure out your COM port


Since we either upload our code direct via a serial port as with the ["Arduino
 Serial"](http://arduino.cc/en/Main/ArduinoBoardSerial) or via USB (which also only simulates a serial port with the FTDI chip on the Arduino borad) we need to know which serial port our Board is connected to.


On Windows, the FTDI virtual COM port driver doesn't register its COM port until the Arduino board is plugged in. With the board plugged into USB, type "devmgmt.msc" into CMD to open the device manager. Go to "Ports (COM & LPT)" and find the "USB Serial Port
 (COMx)" where x is the number of the port connected to the Arduino. This COM port will usually be the same on your machine even if you reboot or unplug the Arduino, but might differ on different machines.

### Configuring AVRDude


Go back to your project settings, and go the AVR/AVRDude page. Create a new programmer configuration. You only need to do this once; for other Arduino projects, you can reuse this configuration.


For Diecimila and Duemilanove, the "Programmer Hardware" is "ATMEL STK500 Version 1.x firmware". If you have a recent version of AVRDude, you can also choose "Arduino" which uses
 a protocol similar to STK500 1.x Version. For the Uno it should work as well.


Enter the name of your serial port in the "Override default port" field. Usually this is something like "/dev/cu.usbserialx" in *NIX and something like "//./COMx" in Windows. Override the default baud rate to 19200 for
 Arduinos based on the ATmega168, or 57600 for Arduinos based on the ATmega328p. If you recieve an error similar to
 avrdude: stk500_getsync(): not in sync: resp=0xe0


on a ATmega328p, try 115200 baud rate.


Once you've configured the programmer, save it, then go to the "Advanced" tab and check "Disable device signature check".


If you are using WinAVR-20090313 or earlier on Windows, then you will need to replace the default avrdude.exe and avrdude.conf in the WinAVR directory with the ones from the following
 directories in the Arduino IDE folder:


hardware\tools\avr\bin\avrdude.exe hardware\tools\avr\etc\avrdude.conf

### Uploading


If you've checked "AVR Dude" in the Additional tool in toolchain panel of the C/C++ Build Settings, your program will get upload at each build. Else, you can just right click your project in the Explorer and choose "AVR/Upload Project to Device" or click the
 AVR upload button.


If everything worked out fine, your Eclipse console will print something like
avrdude: verifying ...
avrdude: 1280 bytes of flash verified

avrdude done.  Thank you.

Finished building: avrdudedummy

### Serial port monitor


If you have [Target Management Project](http://www.eclipse.org/dsdp/tm/tutorial/index.php) installed, just go to
 the Window menu / Show View / Other.. And choose Terminal/Terminal. Open it with a serial link and choose the serial port your Arduino is connected to. Notice you'll probably have to disconnect the terminal while uploading.


As the RXTX library is a troublemaker for many people, there is a workaround : Just start a SSH or Telnet (please be aware this may be a potential security leak) server on your computer and open it in the Eclipse terminal. Then, on any unix, just type :


cat /dev/your_serial_port/

# Eclipse Template Project


The Arduino Blink example implemented in an Eclipse project can be found on github right[here](https://github.com/ricovangenugten/Arduino-Blink-Eclipse-Project).
 It contains the Arduino Core Library and is therefore completely self contained, the Arduino IDE is not needed. All command line options are configured so that they are as similar to the Arduino IDE as possible creating equally small binaries. It can be imported
 into Eclipse pretty easily using its Git extension providing a good starting point for your application. An installation manual is available there too.

### External References
- [Programming the Arduino in Eclipse](http://www.wikihow.com/Write-Arduino-Software-in-C)
- [Arduino in Eclipse - RobertCarlsen.net](http://robertcarlsen.net/2009/10/31/arduino-in-eclipse-989)- 
Updated walk-through for Arduino development using Eclipse

- [Programing Arduino with Eclipse on Windows.](http://www.chipkin.com/articles/using-eclipse-with-arduino-duemilanove)
- [Target Management/RSE plug-in](http://www.eclipse.org/dsdp/tm/)[update-site](http://download.eclipse.org/dsdp/tm/updates/3.1/)
- [RXTX Eclipse plug-in](http://users.frii.com/jarvi/rxtx/index.html)[update-site](http://rxtx.qbang.org/eclipse/)
- [http://www.cyberrailguru.com/software-development/arduino-development-on-mac-os](http://www.cyberrailguru.com/software-development/arduino-development-on-mac-os)

### TO-DO list
- 
Make this easier to read ^_^



