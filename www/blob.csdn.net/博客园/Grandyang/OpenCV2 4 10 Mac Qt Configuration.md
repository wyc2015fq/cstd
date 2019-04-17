# OpenCV2.4.10 Mac Qt Configuration - Grandyang - 博客园







# [OpenCV2.4.10 Mac Qt Configuration](https://www.cnblogs.com/grandyang/p/4336964.html)







Download OpenCV 2.4.10

Download CMake 2.8



Open CMake and choose the source code directory and build binary directory



Then click "Configure", and "Generate"



Go to the build binary directory, open terminal, and run the following command:

make



After "make" is done, then type

sudo make install



Download Qt for Mac

Open Qt Creator, in the .pro file, add

```
INCLUDEPATH+= /usr/local/include
INCLUDEPATH+=/usr/local/include/opencv
INCLUDEPATH+=/usr/local/include/opencv2

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_highgui \
-lopencv_calib3d
```



In the .cpp file or .h file, add the following line:

```
#include <opencv2/opencv.hpp>
```



You can find the code in my another blog to test your configuration, find the code [here](http://www.cnblogs.com/grandyang/p/5002633.html).














