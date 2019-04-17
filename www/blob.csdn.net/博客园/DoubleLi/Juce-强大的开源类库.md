# Juce-强大的开源类库 - DoubleLi - 博客园






**介绍**

Juce是一个完全围绕C++语言的类库，用来开发跨平台的应用程序。

完整的用doxgen生成的html形式的API手册可以在[这里](http://www.rawmaterialsoftware.com/juce/api/index.html)下到。或者可以从[下载页面](http://www.rawmaterialsoftware.com/juce.php)下载预编译的windows帮助文件。



想获取更多的帮助或信息，请访问JUCE的[网站](http://www.rawmaterialsoftware.com/juce.php)。



**许可**

Juce发布遵循[Gnu Public License](http://www.gnu.org/copyleft/gpl.html)约定，该约定允许Juce可以被自用拷贝或发布，免费用于开源的软件代码中。

如果你想采用Juce开发发布一个非开源的应用，商业许可需要付费-点击[这里](http://www.rawmaterialsoftware.com/jucelicense.php)获取更多关于价格和条款的信息。



**安装**

Juce的源文件全部放在一个名为juce的文件夹下，你可以解压并且拷贝它到你系统的任何地方。

在juce文件下的子文件下“juce/build”里包含用于不同的系统和编译器的工程文件，你可以编译juce库。



**编译实例代码**

在juce文件夹里面有例子程序，可以展示Juce的一些特性，在文件夹juce/extras/juce demo/build里包含适用于各种平台和编译器的工程文件，可以用于编译实例程序。



**“混合（amalgamated）”版Juce**

Juce的一个特性是可以把Juce作为一个独立c++文件链接进你的工程，而不是静态的链接库。也就是你实际上不用事先构建Juce库就把可以编写Juce应用程序，该特性需要在你的工程中添加“juce_amalgamated.cpp”文件，并且包含“juce_amalagamated.h”而不是“juce.h”.所有的例子程序都采用这种方法，因为这样对于一个刚使用Juce的新手来说不需要配置太多的信息。但是有些编译器需要和调试器可能引入巨大的文件，所以建议你采用的传统的方式，将Juce最为一个独立的库使用。

（备注：我[测试](http://lib.csdn.net/base/softwaretest)过，如果采用混合版本，编译器会把所有的Juce库编译进程序中，一个Debug版本helloworld程序都要5M，很吓人，所以文档建议采用传统方式，以库的形式加载）



这中方法的一个变化就是需要包含“juce_amalgamated_template.cpp”在你的应用中，该文件和普通的混合文件一样，但区别是它是通过#include语句包含进来的，而不是把所有文件堆积到一个文件中。这样可以是调试简单一些。



**创建一个新的Juce应用程序**





**在Microsoft Visual Studio中编译Juce程序**

最快的方法就是尝试编译demo应用程序，在juce/extras/juce下有个VisualStudio的项目文件demo/build/win32_vc8/jucedemo.sln。该文件可以在vs2005以后包括免费版的VisualExpress2009都可以编译运行而无需额外的配置。



需要确认的是你是否熟悉VisualStudio，因为需要把jucedemo设置成你的启动工程(可以右键点击解决方案面板中jucedemo工程条目将会看到这个选项)。还有当前激活的选项应该被设置为“Debug”或者“Release”.(如果第一次加载工程，vc会选择一个默认的配置，通常选择“Debug DLL”，原因只有它自己知道)



创建你自己的应用程序链接Juce：

1 要么拷贝一份“juce/projects”下的例子工程，重命名和自定义配置一下，或者创建一个新的空的win32工程-不要选择MFC或者其他的项目因为VisualStuido可能污染了你的应用程序(因为vc会添加一些额外的代码进去)



2 在你的源文件中包含“juce.h”文件（最好放在预编译头文件中）



3 确保链接库的搜索路径包含“juce/bin”目录，这个路径可以设置在全局配置中或添加在你工程链接（linker）设置中.



4.是否选择“Multithreaded”或“Debug Multithreaded“运行时库，取决你做的Debug还是release版本编译。在VC6中该属性可以在”工程设置（Project Setting）->C/C++->代码(Code)->通用（General）选项“面板。在Visual Studio，这在"工程属性(Project properites)"面板设置。



5 确保你的工程开启了异常处理和运行时类型信息选项(RTTI)



6 看看"HellowWorld"工程，例子工程或者API文档关于JUCEApplication类的用法，从而知道怎么创建应用程序启动代码。



此外，你也可以使用Juce的混合形式（看下面的注释）。使用这个方法，需要你在你的工程中添加”juce_amalagamated.cpp“文件，包含”juce_amalagamated.h“头文件而不是"juce.h"文件。这个会将整个库拉进你的工程而无需单独去链接库。所以你可以跳过上述关于设置链接路径的步骤，等等。大多的例子程序都是采用这种方法编写，使用混合版本，所以参考这些例子看它怎么做的。





**在Microsoft Visual Stuido6中编译Juce程序**

由于VC6已经过时，此章节没有翻译



To compile the JUCE .lib files from the source code:
Install the latest Platform SDK from Microsoft.
Set up your include and library search paths. The first few items on your include path should look like this (obviously you might have things installed in different places, but the order is important!):
C:/Program Files/Microsoft Platform SDK/include
C:/Program Files/Microsoft Platform SDK/include/crt
C:/Program Files/Microsoft Platform SDK/include/mfc
C:/mycode/juce
...
And the library search path should begin like this:
C:/Program Files/Microsoft Visual Studio/VC98/LIB
C:/Program Files/Microsoft Platform SDK/lib
C:/mycode/juce/bin
...
Open the juce.dsp project file in juce/build/win32/vc6
There are several configurations: debug, release, debug-unicode, and release-unicode. You can build all or some of these, and the resultant .lib files should end up in the "juce/bin" folder.
Note that there's a rather lame bug in VC6 that causes an internal compiler error if you include filenames that are too long. This can get triggered if you put the juce folder in a deeply-nested directory (such as your user home directory). Unfortunately I think the only workaround for this is to move the source tree to a shallower directory.
For info on how to create an application that uses Juce, see the VC2005 notes above.



**在Microsoft Visual Studio7中编译Juce程序**

对于VC7，你可以导入VC6的配置应该可以工作。可能需要稍微调整一下VC8工程的版本信息，就可以在VC7中打开，但是这不是一个可靠的方法。



**在MacOSX平台上的XCode环境中编译Juce程序**

To compile the JUCE binaries from the source code:
Open the Juce.xcodeproj file in juce/build/macosx
This project has "debug" and "release" configurations, and the library files it creates are libjuce.a (release) and libjucedebug.a (debug), which will appear in the juce/bin directory.
Then, to create and build an application:
Either make a copy of the example project in juce/extras/example projects and rename/customise it, or create a new "Carbon Application" project.
Include the header file juce.h in all your source files.
Get rid of any main() functions that XCode might have generated for you, and instead use the JUCEApplication class as your application launcher - see the API documentation for this class for more details, or have a look at the example projects, or demos.
Drag-and-drop the juce.xcodeproj file into the project's "External Frameworks and Libraries" list.
Expand this item in the treeview, and inside there'll be an item "libjuce.a" or "libjucedebug.a" - drag-and-drop this into the "link binary with libraries" phase inside the xcode target. When you select either a debug or release juce build these entries will (usually) update themselves to show the correct debug or release library name. If you want your project to automatically rebuild Juce when you make changes to a juce file, you can also add Juce to your target's "Direct Dependency" list (show information for the target, and this is on the "general" tab).
Alternative ways of linking to juce would be to add the libjuce.a or libjucedebug.a library to your "External Frameworks and Libraries" list, or to add switch to the linker's command-line of either "-ljuce" or "-ljucedebug".
You'll also need to add some or all of the following OSX frameworks to your "External Frameworks and Libraries" list, depending on what features your application uses:
Cocoa.framework
Carbon.framework
IOKit.framework
CoreAudio.framework
CoreMIDI.framework
WebKit.framework
DiscRecording.framework
QTKit.framework
QuickTime.framework
QuartzCore.framework
AudioUnit.framework
AudioToolbox.framework
OpenGL.framework
AppKit.framework
CoreAudioKit.framework
CoreFoundation.framework
In future there may be other frameworks that you'll need to link with to support new JUCE features. (It should be pretty obvious from the link-time error when one of these is missing).
If all this seems too complicated, you can make things slightly easier by using the amalgamated form of Juce (see earlier note). To do this, all you need to do is to add juce_amalagamated.cpp to your project, and include juce_amalagamated.h instead of juce.h. This pulls the entire library into your project without needing to link to it separately, so you can skip the steps above that involve compiling the library, setting up the link paths, etc. Most of the demo apps are written using the amalgamated version, so have a look through their source code for examples of how to do this.



**使用Code::Blocks和MinGW创建Juce应用程序**

open the Juce project: juce/build/win32/codeblocks/juce.cbp
open the demo app project: juce/extras/juce demo/build/win32_codeblocks/JuceDemo.cbp
Build first the "Juce Library" project, and then the "Juce Demo App" project. If your build environment is set up correctly, these should just work and the demo app should run.
To create your own application:
Create a new project, as a "win32 GUI".
Either copy the example main.cpp from the Juce example project, or write your own based around the JUCEApplication class
In your project's build settings, you'll need to make sure the linker uses the following libraries:
libjuce.a or libjucedebug.a (these should be created in the juce/bin/codeblocks directory)
libshell32.a
libole32.a
libvfw32.a
libwinmm.a
libwininet.a
libdsound.a
libwsock32.a
libopengl32.a
libglu32.a
libuuid.a
librpcrt4.a (these are all in the MinGW libraries folder)



**在[Linux](http://lib.csdn.net/base/linux)平台利用Gcc创建Juce应用程序**
Creating a JUCE application on Linux with GCC

Most linux distros should come with the tools you need, although you might want to get hold of premake, which is used to automatically generate the juce makefile. (This isn't necessary if you're just going to use the makefile that's provided).
Get a command prompt and [Go](http://lib.csdn.net/base/go) into /juce/build/linux
To build the debug version, use "make CONFIG=Debug", or use "make CONFIG=Release" to build the release version. You can also use "make clean" to delete the intermediate files.
Then, to create and build an application:
Building the library will have produced the library files /juce/bin/libjuce.a and /juce/bin/libjuce_debug.a. You'll need to link to one of these in your app, and you'll also need to link to these libraries:
freetype
pthread
X11
If you've set the JUCE_USE_XINERAMA flag in juce_Config.h, you'll also need to link to the xinerama library. And you'll need the GL and GLU libraries if you've enabled JUCE_OPENGL
***









