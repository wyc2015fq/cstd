# osgOcean+VS2010+Win7编译方法 - L_Andy的专栏 - CSDN博客

2016年05月20日 15:27:57[卡哥](https://me.csdn.net/L_Andy)阅读数：1901



1、首先要下载osgOcean-Source-1.0.1包，以及依赖包osgOcean-Resources-1.0.1、fftss-3.0-20071031，可在：链接：http://pan.baidu.com/s/1bo8h8iN 密码：bneu，下载

2、下载好之后，在D盘的osg3.2文件夹下创建一个osgOcean文件夹，并将下载的源码放到该文件夹下，并解压，如下图所示：

![](https://img-blog.csdn.net/20151224201121442)

3、首先要编译fftss-3.0-20071031依赖库，打开fftss-3.0-20071031\win32目录，该目录下有一个fftss.sln文件，如下图所示：

![](https://img-blog.csdn.net/20151224201541725)

4、使用vs2010打开进行编译，在打开时可能需要转换，依次选择转换即可，最后使用vs2010打开，如下图所示：

![](https://img-blog.csdn.net/20151224201722545)

5、使用批处理进行编译，批生成时选择Release和Debug两个版本，如下图所示：

![](https://img-blog.csdn.net/20151224201912746)

6、点击生成按钮，即可进入到编译状态，最后编译完成，成功2个，如下图所示：

![](https://img-blog.csdn.net/20151224202015097)

7、那么此时就会在win32目录生成了Debug和Release目录，里边有一个lib文件就是所需要的依赖库，如下图所示：

![](https://img-blog.csdn.net/20151224202158613)

8、在osgOcean/osgOcean中新建一个build文件夹，将生成的工程以及编译都在此文件中进行，然后打开CMake，进入到osgOcean文件夹中，将CMakeLists.txt拖放到CMake中，然后将Where to build the binaries:的路径改到build下，如下图所示：

![](https://img-blog.csdn.net/20151224202630949)

9、配置好路径之后点击Configure然后选择vs2010，如下图所示：

![](https://img-blog.csdn.net/20151224202840638)

10、选择好之后点击完成，就会生成一些信息，不过是需要进行路径配置，配置CMAKE_INSTALL_PREFIX表示编译路径，即最后bin和lib安装到哪里(这里安装好像有些问题，此文中在编译时就不INSTALL了，到时直接拷贝build下的bin文件夹即可)，配置OSG_INCLUDE_DIR目录，这是osg的include目录，配置OSG_LIBRARY文件，即osg的lib文件osg.lib，OSG_LIBRARY_DEBUG配置osgd.lib，如下图所示：

![](https://img-blog.csdn.net/20151224202923230)

11、点击Configure按钮，出现以下界面，按照上述方法配置osgDB.lib和osgDBd.lib，如下图所示：

![](https://img-blog.csdn.net/20151224203324996)

12、点击Configure按钮，出现以下界面，配置osgGA.lib,osgGAd.lib，如下图所示：

![](https://img-blog.csdn.net/20151224203425201)

13、点击Configure按钮，出现以下界面，配置osgViewer.lib和osgViewerd.lib文件，如下图所示：

![](https://img-blog.csdn.net/20151224203526835)

14、点击Configure按钮，出现以下界面，配置osgUtil.lib和osgUtild.lib文件，如下图所示：

![](https://img-blog.csdn.net/20151224203629762)

15、点击Configure按钮，出现以下界面，配置osgText.lib和osgTextd.lib文件，如下图所示：

![](https://img-blog.csdn.net/20151224203706749)

16、点击Configure按钮，出现以下界面，配置OpenThreads.lib和OpenThreadsd.lib文件，如下图所示：

![](https://img-blog.csdn.net/20151224203806432)

17、点击Configure按钮，出现以下界面，配置fftss.lib文件，此文件就是刚刚编译的依赖库，以及包含目录为：D:\osg3.2\osgOcean\fftss-3.0-20071031\include，如下图所示：

![](https://img-blog.csdn.net/20151224203826058)

18、点击Configure按钮，出现以下界面，没有红色的提示，那么配置就完成了。

![](https://img-blog.csdn.net/20151224203855256)

19、点击Generate按钮生成工程，如下界面表示生成成功，如下图所示：

![](https://img-blog.csdn.net/20151224203915892)

20、打开build就会出现相应的信息，如下图所示：

![](https://img-blog.csdn.net/20151224203934086)

21、打开osgOcean.sln，进行批生成，选择ALL_BUILD的Debug和Release，如下图所示：

![](https://img-blog.csdn.net/20151224203955605)

22、点击生成即可生成相应的文件，以下是生成成功8个，如下图所示：

![](https://img-blog.csdn.net/20151224204017045)

23、我们来把最后的文件进行归总，在D:\osg3.2\osg3.2.1目录中新建一个osgOcean文件夹，并在osgOcean文件夹中新建一个bin和lib文件夹，如下图所示：

![](https://img-blog.csdn.net/20151224205235082)

24、将D:\osg3.2\osgOcean\osgOcean\build\bin\Release路径下的oceanExample.exe和osgOcean.dll拷贝到D:\osg3.2\osg3.2.1\osgOcean\bin目录，将D:\osg3.2\osgOcean\osgOcean\build\bin\Debug路径下的osgOceanD.dll拷贝到D:\osg3.2\osg3.2.1\osgOcean\bin目录

25、将D:\osg3.2\osgOcean\osgOcean\build\lib\Release中的所有文件和D:\osg3.2\osgOcean\osgOcean\build\lib\Debug所有文件拷贝到D:\osg3.2\osg3.2.1\osgOcean\lib文件夹中。

26、将D:\osg3.2\osgOcean\osgOcean文件夹中的include拷贝到D:\osg3.2\osg3.2.1\osgOcean中，最后D:\osg3.2\osg3.2.1\osgOcean中有三个文件夹，分别是include、lib、bin目录，如下图所示：

![](https://img-blog.csdn.net/20151224205710241)

27、最后拷贝资源文件，将D:\osg3.2\osgOcean\osgOcean中的resources文件夹拷贝到D:\osg3.2\osg3.2.1\osgOcean\bin目录中，然后将osgOcean-Resources-1.0.1中的boat、island、textures拷贝到D:\osg3.2\osg3.2.1\osgOcean\bin\resources目录中，如果有重复的则覆盖或者合并文件夹

28、D:\osg3.2\osg3.2.1\osgOcean\bin\resources文件夹的内容如下图所示：

![](https://img-blog.csdn.net/20151224210027490)

29、回到bin目录，然后双击oceanExample.exe文件，出现以下界面，说明osgOcean编译正确，如下图所示：

![](https://img-blog.csdn.net/20151224210223441)

[](http://blog.csdn.net/sunxiaoju/article/details/50396593#)[](http://blog.csdn.net/sunxiaoju/article/details/50396593#)[](http://blog.csdn.net/sunxiaoju/article/details/50396593#)

