# windows下编译 libjpeg以及libjpeg-turbo - yixianfeng41的专栏 - CSDN博客
2016年07月19日 18:57:29[第2梦](https://me.csdn.net/yixianfeng41)阅读数：5955

## **一、编译libjpeg**
1、下载源代码下载地址：[http://www.ijg.org/files/](http://www.ijg.org/files/)，
    选择最新版本的windows版本压缩包，进行下载。
2、使用vs2012的visual studio tools下的本机工具命令提示，进行编译。
      Microsoft Visual Studio 2012
      ----Visual Studio Tools
      -------Visual Studio 本机工具命令提示(2012)
3、打开VS2010的命令行窗口，将目录转到libjpeg所在文件下。
![](https://img-blog.csdn.net/20160719171428023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4，此时 “nmake -f makefile.vc”编译会失败，需要修改两个地方：
    1）将makefile.vc里的12行，运来是！incldue<win32.mak>替换为!include <C:\Program Files\Microsoft            
         SDKs\Windows\v6.0A\Include\win32.mak>
    2）新建一个文件名叫jconfig.h，将jconfig.vc中的内容全部复制过来。
5、命令行窗口，编译jpeglib，生成libjpeg.lib
     nmake -f makefile.vc
## **二、编译libjpeg-turbo：**
1、  从[https://sourceforge.net/projects/libjpeg-turbo/](https://sourceforge.net/projects/libjpeg-turbo/)下载libjpeg-turbo最新源代码，将其保存到E:\Desktop\libjpeg-
        turbo-master文件夹下；
2、  从[http://www.cmake.org/](http://www.cmake.org/)下载最新版的CMake，并安装；
3、  在E:\Desktop\libjpeg-turbo-master文件夹下手动创建一个build文件夹；
4、  打开CMake，其中where is the source code选项，选择E:\Desktop\libjpeg-turbo-master；where to
 build the               binaries选项，选择E:\Desktop\libjpeg-turbo-master\build；
![](https://img-blog.csdn.net/20160719144359518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、点击Configure，在弹出的对话框中选择你电脑上的VS，我装的是vs2012，因此选择visual stdio 11。其它默认， 
     点击Finish；
6、点击Generate，此时会在build文件夹中看到很多vcxproj项目文件，以及一个libjpeg-turbo.sln文件；此时打开这个       sln进行编译是不会成功的，它不会生成各种lib。
7、从[https://sourceforge.net/projects/nasm/](https://sourceforge.net/projects/nasm/)下载最新版的nasm；
8、将nasm安装到电脑上任意文件夹下，并将其中的nasm.exe和ndisasm.exe两个文件拷贝到                 
      C:\ProgramFiles\Microsoft Visual Studio 10.0\VC\bin下(如果是64位，则拷贝到C:\ProgramFiles (x86)\
      Microsoft Visual Studio 10.0\VC\bin)；
9、打开libjpeg-turbo.sln，分别在Debug和Release下，选择Solution Explorer里的Solution libjpeg-turbo，点击右             键， 运行”Rebuild Solution”，然后选中INSTALL,
 build；
10、全部完成后会在build文件夹下会生成很多文件夹，编译成功，我们所需要的.exe以及lib文件在debug或者release         中；
![](https://img-blog.csdn.net/20160719151025023)
## 三、注意
    libjpeg.lib是用c语言开发的，
    如果在C++程序里使用，需要用extern "C" { }包含一下。
    如下：
    extern "C"
    {
          #include "jpeglib.h"
    }
## 四、包
   为了方便大家，我将编译好（32位）的包传上来，大家可以直接下载，[libjpeg.lib](http://download.csdn.net/detail/yixianfeng41/9580434)!!
