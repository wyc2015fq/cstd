# VS2012+OpenCV3.0+CMake3.8环境的OpenCV安装/配置/新建工程 - 战斗蜗牛的专栏 - CSDN博客





2017年03月10日 22:26:52[vbskj](https://me.csdn.net/vbskj)阅读数：2798








        OpenCV3.0版本相比2.x版本，不仅提供了新的特性，而且将众多核心/依赖库（opencv_core249.lib,opencv_highgui249d等）合成为opencv_world300/300d.lib, 并将一些性能测试功能（如CUDA等）合成为opencv_ts300/300d.lib，在进行程序开发时设置链接库环节方便了许多。



        由于OpenCV3.0只提供了vs2012（v11)和vs2013（v12）版本，因此这里选择如题所示环境进行安装配置介绍。




        OpenCV是一个跨平台的开发包，提供了总计 六十六多万行的源代码，为了便于学习和调试，需要CMake工具将各个sample工程转化为vs2012解决方案，进行载入和运行。需要注意的是，如果你不需要学习OpenCV自带的例子，则不需要使用CMake工具进行转化，直接使用OpenCV开发包中编译好的lib库即可。



        因此下面分两个方面进行安装配置介绍，第一个：直接利用OpenCV编程；第二个：学习OpenCV自带例子。









一、OpenCV的SDK安装配置+新建工程HelloCV（VS2012+OpenCV3.0 SDK）

1）安装vs2012，解压缩OpenCV3.0；

2）OpenCV解压缩后，里面有两个目录，分别是build和sources，build目录为编译好的库文件以及包含头文件；

3）主要是环境变量设置部分：在用户变量中新增open_cv变量，值设置为你解压缩的OpenCV目录下build所在的这一层，如C:\Users\fanz\Desktop\opencv\build；然后，将OpenCV的bin目录添加入系统变量PATH中，C:\Users\fanz\Desktop\opencv\build\x64\vc11\bin（对应vs2012的x64编译方式）。

4）主要是vs2012的属性设置问题。新建一个win32控制台程序，比如叫hello_cv，双击其属性管理器中对应的编译方式的配置项，如下图所示，采用的是x64 Debug方式编译，然后将对应的bin目录、include、lib加入属性中，如下图所示。注意：包含目录需要添加三个C:\Users\fanz\Desktop\opencv\build\include 、C:\Users\fanz\Desktop\opencv\build\include\opencv、C:\Users\fanz\Desktop\opencv\build\include\opencv2;
 当然，如果你的编程习惯是这样包含头文件的#include <opencv2/opencv.hpp>，后两个也可以不加.




对于库文件，开始已经说明，3.0版本之后，只需要包含world和ts两个lib，所以库目录只包含C:\Users\fanz\Desktop\opencv\build\x64\vc11\lib 即可

![](https://img-blog.csdn.net/20170310194345180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





4）-1：如果为了以后新建工程方便，可以新建一个专门针对OpenCV的属性配置项，如下图所示。保存后，在当前工程目录会有一个props后缀的文件，新建工程时把这个文件导入即可避免重复性的bin/include/lib设置。




![](https://img-blog.csdn.net/20170310221520986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








5）在项目属性下面的“连接器”-〉“输入”项，插入opencv_ts300d.lib;opencv_world300d.lib;




ctrl+F5, 程序就可以正常运行了。







二、学习OpenCV的自带例子源代码（CMake+OpenCV sourcecode）

1、在OpenCV目录下建立一个新目录，比如叫output。然后如下图所示，将sources目录（源代码）和output目录（源代码编译输出目录）进行选定，点击“configure”按钮即可，后面选择vs2012对应的选项，编译器设置就选择第一个默认的native 编译器即可。

![](https://img-blog.csdn.net/20170310221809112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





2、经过configure，出现下图红色背景的，要对编译的设置选项进行删减。删除的选项包括：IPP，CUDA等可能你的机器环境不具备的；增加的选项包括：test例程/example例程等等。

![](https://img-blog.csdn.net/20170310222205816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170310222211353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





3、再次点击configure按钮，背景就正常了，然后点击Generate，就会在output目录产生对应的vs2012工程文件。

![](https://img-blog.csdn.net/20170310222334786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





4、使用vs2012打开output目录下的opencv.sln，即可打开所有相关的源代码目录。

![](https://img-blog.csdn.net/20170310222459271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170310222508528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








至此，OpenCV的源代码就全在掌握之中，重点针对samples, applications，tests中的相关例子进行学习即可。



