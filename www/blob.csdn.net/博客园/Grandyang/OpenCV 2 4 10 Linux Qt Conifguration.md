# OpenCV 2.4.10  Linux Qt Conifguration - Grandyang - 博客园







# [OpenCV 2.4.10  Linux Qt Conifguration](https://www.cnblogs.com/grandyang/p/4336999.html)







Download CMake 2.8.12

Extract the file, and run "./bootstrap", then "make", then "sudo make install"



Add some dependencies, run "sudo apt-get install build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg62-dev libtiff4-dev cmake libswscale-dev libjasper-dev"



Download OpenCV 2.4.10 for linux

Extract the file, and go into the file, and run "cmake ."

After that, run "make"

Wait about 20 minutes and then run "sudo make install"



Download Qt for Linux

chmod +x qt-opensource-linux-x64-1.6.0-8-online.run

sudo ./qt-opensource-linux-x64-1.6.0-8-online.run



Open Qt Creator, in the .pro file, add

INCLUDEPATH+= /usr/local/include
INCLUDEPATH+=/usr/local/include/opencv
INCLUDEPATH+=/usr/local/include/opencv2
LIBS += -L /usr/local/lib/libopencv*



Build the project, if error "**Cannot find -lGL**" happens, run the following command in the terminal:

sudo apt-get -y install libgl1-mesa-dev














