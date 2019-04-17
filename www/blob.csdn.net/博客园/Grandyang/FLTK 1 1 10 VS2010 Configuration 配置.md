# FLTK 1.1.10 VS2010 Configuration 配置 - Grandyang - 博客园







# [FLTK 1.1.10 VS2010 Configuration 配置](https://www.cnblogs.com/grandyang/p/5126116.html)






Download FLTK 1.1.10 at [here](http://www.fltk.org/software.php?VERSION=1.3.0&FILE=fltk/1.1.10/fltk-1.1.10-source.zip).

Download VS2010

Download CMake 2.8.12



I assume you've already installed VS2010 and CMake 2.8.12 correctly.



**Compile the VTK:**

Start CMake 2.8.12, fill the source and destination:

source:** C:/FLTK/fltk-1.1.10**

destination: **C:/FLTK/VS2010**

Click **Configure** and use **Visual Studio 10 2010** to complie.



When first configure is done, select **Grouped** and **Advanced**.

```
BUILD_EXAMPLES          [ON]  
BUILD_TEST              [ON]  
CMAKE_INSTALL_PREFIX    [C:/FLTK/VS2010_install]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open **FLTK.sln**, right click **ALL_BUILD**, choose **Rebuild**.

**Wait a long time for make process, have a cup of coffee :)**

After rebuild is done, right click **INSTALL**, choose **Build**.



**Now, the configuration is done, enjoy it :)**



Create a new empty project,

Open VS2010, create an empty project, go to Properties -> Linker -> Input -> Additional Dependencies, copy the following items:

**fltk.libfltk_forms.libfltk_gl.libfltk_images.libfltk_jpeg.libfltk_png.libfltk_zlib.lib**



Create a new .cpp file, and try the following test code:

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














