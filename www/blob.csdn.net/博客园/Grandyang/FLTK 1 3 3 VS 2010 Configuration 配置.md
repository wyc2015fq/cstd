# FLTK 1.3.3 VS 2010 Configuration 配置 - Grandyang - 博客园







# [FLTK 1.3.3 VS 2010 Configuration 配置](https://www.cnblogs.com/grandyang/p/5140999.html)







Download FLTK 1.3.3

Download VS2010



I assume you've already installed VS2010 correctly.



**Compile the FLTK:**

Go to your FLTK 1.3.3 folder (**C:\FLTK\fltk-1.3.3\ide\VisualC2010**), open **fltk.sln**, choose **Release** mode, right click **Solution 'fltk' (79 projects)**, choose **Rebuild**.

**Wait a long time for make process, have a cup of coffee :)**



**Now, the configuration is done, enjoy it :)**



Create a new empty project,



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\FLTK\fltk-1.3.3



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\FLTK\fltk-1.3.3\lib



**Project->Project Property->Configuration Properties->Linker->Input:**

wsock32.lib
comctl32.lib
fltk.lib
fltkforms.lib
fltk_forms.lib
fltkgl.lib
fltk_gl.lib
fltkimages.lib
fltk_images.lib
fltkjpeg.lib
fltk_jpeg.lib
fltkpng.lib
fltk_png.lib
fltkzlib.lib
fltk_zlib.lib


****Create a test.cpp file as below:****

```
#define WIN32
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














