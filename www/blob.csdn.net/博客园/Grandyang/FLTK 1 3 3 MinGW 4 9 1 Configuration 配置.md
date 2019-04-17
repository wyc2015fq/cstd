# FLTK 1.3.3 MinGW 4.9.1 Configuration 配置 - Grandyang - 博客园







# [FLTK 1.3.3 MinGW 4.9.1 Configuration 配置](https://www.cnblogs.com/grandyang/p/5129035.html)







Download FLTK 1.3.3

Download CMake 3.2.0



Start CMake 3.2.0, fill the source and destination:

source: **C:/FLTK/fltk-1.1.10**

destination: **C:/FLTK/build**

Click **Configure** and use **MinGW Makefiles** to complie.



Change the following item:

```
CMAKE_INSTALL_PREFIX    [C:/FLTK/MinGW]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open command line, type: **mingw32-make**

**Wait a long time for make process, have a cup of coffee :)**

After make is done, type **mingw32-make install**



**Now, the configuration is done, enjoy it :)**



Let's test our installation. First create a test.cpp:

```
#include "FL/Fl.h"
#include "FL/Fl_Box.h"
#include "FL/Fl_Window.h"

int main()
{
    Fl_Window window(500,500, "Test");
    Fl_Box box(0,0,500,500,"Hello World!");
    window.show();
    return Fl::run();
}
```



Then create the CMakeLists.txt:

```
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(TestFLTK)

SET(FLTK_DIR C:\\FLTK\\MinGW)
SET(FLTK_FLUID_EXECUTABLE C:\\FLTK\\MinGW\\bin)
SET(FLTK_INCLUDE_DIR C:\\FLTK\\MinGW\\include)
SET(FLTK_LIBRARIES C:\\FLTK\\MinGW\\lib)

FIND_PACKAGE(FLTK)
INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})

set(Sources
    test.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${Sources})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${FLTK_LIBRARIES})
```



I use Qt Creator to compile and run the example code on Windows 7.














