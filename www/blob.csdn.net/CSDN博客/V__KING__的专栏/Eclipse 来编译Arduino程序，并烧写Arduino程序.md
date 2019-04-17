# Eclipse 来编译Arduino程序，并烧写Arduino程序 - V__KING__的专栏 - CSDN博客





2013年11月13日 17:29:29[v__king__](https://me.csdn.net/V__KING__)阅读数：2838









If you want to build some electronic stuff that needs some programming, then Arduino is certainly the best choice. It just works, you only need to plug it in, write your code and press on the deploy button to have your board flashed and working.

And this is great, it saves you from tons of troubles, if you worked with microchip PICs in the past you probably know what I’m talking about.


The only thing that I don’t like about Arduino is it’s IDE, which is a very simple code editor written in Java with very few features and many annoying problems. The default IDE is very good to start learning Arduino programming, but the more you use it, the
 more you hate it, at least I do.


However until now I always used the default IDE, but for my last project ([Sir
 Biss, an Arduino based snake robot](http://drstrangecode.org/building-an-arduino-based-snake-robot-part-1/)) I figured out how to setup Eclipse for Arduino, and, after a bit of headache, it works great.


If you Google something like “Arduino development with eclipse” you’ll find some tutorials, some of them are very inaccurate and incomplete, others
 are pretty well explained but they simply don’t work, because some fundamental step is missing.


These tutorials, however, gave me the start to setup my own Eclipse, here they are:
- [Quick
 and dirty tutorial on arduino.cc](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1212279190)
- [More
 accurate tutorial, unfortunately didn’t work for me, I needed some more steps.](http://www.chipkin.com/articles/using-eclipse-with-arduino-duemilanove)


So first of all, thanks to the people who wrote these tutorials.


What follows is a step by step guide of my personal way on how to setup a development environment for Arduino, I’ve done it on a Windows 7 32 bit PC,
 and my Arduino hardware is the Arduino Duemilanoveboard.

I do not own other boards so I cannot test this guide for other types of Arduino (Arduino Mega, Arduino Mini, …) if someone reading this guide is able to test it on different boards, please tell me if and what changes must be done I’ll be glad to update this
 tutorial, thank you!

#### Install Eclipse


First thing to do is download the latest Eclipse IDE for C/C++ developers from the [Eclipse
 website](http://www.eclipse.org/downloads/), as I’m writing this the latest Eclipse version is Indigo.


To run Eclipse you need the Java Runtime Environment (not the JDK, because we are not going to develop in Java, so the JRE is fine), you can download the latest JRE from the [Oracle
 website](http://www.oracle.com/technetwork/java/javase/downloads/index.html), click on the “Download”button under JRE, then choose the right JRE for your system, I downloaded the Win 32 x86 version, for offline
 installation (jre-7u3-windows-i586.exe).


Start Eclipse and choose a workspace, where all your Arduino projects will be saved.

#### Install the Eclipse AVR Plugin


From Eclipse, go to “Help” and then “Install
 New Software”.


In the “Work with:” box write this url:
http://avr-eclipse.sourceforge.net/updatesite

Then click “Add”.

Set the “Name:” to “Eclipse AVR Plugin”,
 leave the “Location” as is.


Click “OK” and wait until the installable components are loaded, then select all and click “Next”,
 a dependencies check will start for the selected components and it should take some minutes.


After the depencencies check completes you will be asked to review the selected components, click “Next”, then accept the license terms and hit “Finish”.
 The components installation will start. An alert dialog will appear saying that you are installing unsigned software, say “OK”.


When the installation completes you will be asked to restart Eclipse.

#### Download the Arduino SDK


Download the latest [Arduino
 SDK](http://arduino.cc/en/Main/Software), at the time I am writing this the latest available SDK is marked as Arduino 1.0. Nedless to say, download the right one for your machine,
 I downloaded arduino-1.0-windows.zip.


Choose a place on your file system for the Arduino SDK and then unzip the downloaded file, I unzipped mine inD:\arduino so from now on remember
 to replace this base path with the one you choose.

#### Setup the Eclipse AVR Plugin


From Eclipse go to “Window” and then “Preferences”,
 open the “AVR” section on the left, then select “Paths”


Select AVR-GCC, click “Edit”,
 in the window that appears set “Path source:” to “Custom”,
 click “Browse” and point it to this path <arduino_base_path>\hardware\tools\avr\bin which is the path to the AVR compiler (avr-gcc.exe)
 inside the arduino SDK directory. I pointed mine to D:\arduino\hardware\tools\avr\bin.


Select GNU Make, click “Edit”,
 in the window that appears set “Path source:” to “Custom”,
 click “Browse” and point it to this path <arduino_base_path>\hardware\tools\avr\utils\bin which is the path to the makeexecutable
 inside the arduino SDK directory. I pointed mine to D:\arduino\hardware\tools\avr\bin.


Select AVR Header Files, click “Edit”,
 in the window that appears set “Path source:” to “Custom”,
 click “Browse” and point it to this path <arduino_base_path>\hardware\tools\avr\avr\include which is the path to the AVR toolchain headers files
 inside the arduino SDK directory. I pointed mine toD:\arduino\hardware\tools\avr\avr\include.


Select AVRDude, click “Edit”,
 in the window that appears set “Path source:” to “Custom”,
 click “Browse” and point it to this path <arduino_base_path>\hardware\tools\avr\bin which is the path to the AVRDude executable (used to deploy
 the binary on the Arduino board) inside the arduino SDK directory. I pointed mine toD:\arduino\hardware\tools\avr\bin.


Now the AVR plugin knows where to find the toolchain to build and deply code for the Arduino board.

Click “Apply” and then “OK”

#### Create the ArduinoCore library


The ArduinoCore library will contain all the basic functionalities to deal with the Arduino board, this library can be seen as the standard c library equivalent for Arduino. If you write some code for Arduino in the Arduino IDE, this library is silently linked
 to your code, but if you plan to use Eclipse you have to manually link the library to each of our projects before deploying them on the Arduino board.

Here are the steps to create the ArduinoCore library (which is just the name I gave it):


From Eclipse go to “New” then “C++ Project” then “AVR
 Cross Target Static Library” and select “Empty Project”.


Set the “Project Name:” to “ArduinoCore” and
 click “Next”.

This library will contain code distributed with the Arduino SDK which is considered to be stable, so deselect the“Debug” configuration and click “Next”.

Set the “MCU Type” and “MCU Frequency”,
 this depends on the specific board, my Arduino Duemilanove is equipped with an ATmega328P chip clocked at 16MHz (16000000 Hz). If you are not sure which values to use here, go to the arduino website in the “Hardware” section and click on your specific board
 to see the tech specs, here’s [the
 link](http://arduino.cc/en/Main/Hardware).

Once done hit “Finish”.


Now you have an empty library project, what we are going to do now is import the Arduino core files from the Arduino SDK folder.


In Eclipse right click on the “ArduinoCore” project, select “Properties” and
 then “C/C++ Build”.

Open the “AVR Compiler” section and select “Directories”.

Add the include path to the arduino core header files in the Arduino SDK, they are located inside“<arduino_base_path>\hardware\arduino\cores\arduino”

Add the include path to the pins_arduino.h header file, this again depends on the specific board you are working with.

Many version of the pins_arduino.h header file can be found, they are located inside specific folders under“<arduino_base_path>\hardware\arduino\variants\”:
- <arduino_base_path>\hardware\arduino\variants\standard: I used this one for my Arduino Duemilanove
- <arduino_base_path>\hardware\arduino\variants\mega: I guess you should use this one if you are working on an Arduino Mega board
- <arduino_base_path>\hardware\arduino\variants\leonardo: maybe to be used with the new Arduino Leonardo board
- <arduino_base_path>\hardware\arduino\variants\micro: absolutely dunno


Do the same for the “AVR C++ Compiler” section, then click “OK”.


From Eclipse right click on the “ArduinoCore” project, select “Import” and
 then “File System”. Point the file browser to the arduino core files directory inside the Arduino SDK which are located in“<arduino_base_path>\hardware\arduino\cores\arduino”.


Select all the files except main.cpp.


Click on “Advanced” and select “Create
 links in workspace” so the files are not copied into the library but only weak-referenced. It’s better to only reference these files so if you download a newer version of the Arduino SDK you can simply overwrite the existing one and the ArduinoCore
 library is automatically updated.

Click “Finish”.


If everything went well you should now be able to build the ArduinoCore library, right click on the “ArduinoCore”project and do a “Build
 Project”.

#### Create a test project


Let’s now create a test project to check that everything’s fine.

Remember that these steps must be done with every new project that you create.


In Eclipse go to “New” then “C++ Project” and
 then “AVR Cross Target Application” and select “Empty
 Project”.

Set the “Project Name:” to “TestProject” and
 click “Next”.

Deselect the “Debug” configuration and click “Next”.

Set the “MCU Type” and “MCU Frequency” to
 the same values you set in the ArduinoCore library project and hit “Finish”.


Right click on the “ArduinoCore” project, go to “Properties” and
 then “C/C++ Build” and in both “AVR
 Compiler” and “AVR C++ Compiler” add the same include paths that were added in the ArduinoCore library project plus the include path for
 the test project itself which is not added by default and can be added with this magic string: “${workspace_loc:/${ProjName}}” which simply expands
 to your workspace directory followed by the test project directory.


From the “C/C++ Build” section in the “Properties” window
 go to “AVR C++ Linker”.

Set the command line pattern to:

|${COMMAND}--cref-s-Os${OUTPUT_FLAG}${OUTPUT_PREFIX}${OUTPUT}${INPUTS}-lm${FLAGS}|
|----|



Now click on the “Libraries” subsection.


In the “Libraries path” box add the path to the ArduinoCore library by clicking on the “Add” button
 on the top right corner of the box, then select “Workspace” and point it to the “Release” folder
 inside the ArduinoCore project. You can also directly insert this magic string: “${workspace_loc:/ArduinoCore/Release}” which is the same thing.


In the “Libraries” box add the ArduinoCore library by clicking on the “Add” button
 on the top right corner of the box and simply type “ArduinoCore”, this instructs the linker to link against “libArduinoCore.a”
 when linking the final executable.


In the project properties window, click on “Project references” in the left menu and check “ArduinoCore”,
 so the library will be automatically built (and re-built if needed) when building the test project.


In the test project create a new main.cpp file and paste the following code into it.

This is the classic Blink code sample that blinks the led linked on pin 13 of the Arduino board once every second:

|```cpp#include <Arduino.h> int ledPin =  13;    // LED connected to digital pin 13 // The setup() method runs once, when the sketch startsvoid setup()   {  // initialize the digital pin as an output:  pinMode(ledPin, OUTPUT);} // the loop() method runs over and over again,// as long as the Arduino has power void loop(){  digitalWrite(ledPin, HIGH);   // set the LED on  delay(1000);                  // wait for a second  digitalWrite(ledPin, LOW);    // set the LED off  delay(1000);                  // wait for a second}  int main(void) {   init();  setup();   while(true) {    loop();  }}```|
|----|



You’ll notice that there are some difference between this code and the usual code that you write for Arduino, the next post will focus on these differences, for now just take it as it comes. No questions. Or this post will never end.

#### Deploy on the Arduino board


After compiling and linking the binary a tool named AVRDude will deploy it on the Arduino board.

AVRDude must be configured to work with Arduino.

What we are going to do now is stealing the AVRDude configuration from the Arduino SDK.


From Eclipse go to “Window” then “Preferences” and
 select the “AVR” section on the left, then select“AVRDude”.

Check “Use custom configuration file for AVRDude” and set “AVRDude
 config file” to the one provided with the Arduino SDK: it can be found under <arduino_base_path>\hardware\tools\avr\etc\avrdude.conf.

Click “Apply” and then “OK”.


From Eclipse select the test project and open the project properties once more (right click->“Properties”).

Select the “AVR” section and the click on “AVRDude”

You now have to create a new programmer configuration for your board, this must be done only once.

Click “New”, in the window that appears insert these data:
- “Configuration Name” : “Arduino”
- “Programmer Hardware” : “Arduino”
- “Override default baudrate” : 57600


And set “Override default port” to the COM port that you use to work with the Arduino board. If you do not know what this value is, plug your Arduino
 board in and start the Default IDE, the COM port is showed at the bottom of the IDE window, my Arduino IDE says:
Arduino Duemilanove w/ ATMega328 on COM10

My COM port is COM10 so my “Override default port” field is set to:
//./COM10


Don’t ask me what the “//./” prefix means, I don’t know.

Click “OK”


Back in the “AVRDude” window select the “Advanced” tab.

Check “Disable Device signature check” and click “OK”.


加一点：要选择avr-gcc编译，默认的上avr-g++，properties->C/C++ build->settings->AVR C++ Linker中的command改为avr-gcc

#### First deploy


It’s time. Plug your Arduino board in.


Select the test project and build it, then right click on it and select “AVR” and then “Upload
 project to target device”, the deploy will start and if everything’s fine you should have your program running on the Arduino board after a few seconds. Look at the led at PIN 13 and it should be blinking. Mine blinks.

That’s all, hope this guide is useful.

Again, let me know any problem you had so I can update the tutorial with your info.



