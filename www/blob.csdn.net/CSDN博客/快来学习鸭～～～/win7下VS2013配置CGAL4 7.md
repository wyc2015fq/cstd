# win7下VS2013配置CGAL4.7 - 快来学习鸭～～～ - CSDN博客





2016年09月27日 21:46:24[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：954








环境：win7 64位、vs2013 professional、CGAL4.7

安装CGAL过程比较繁琐，怕以后忘记，因此开启了我人生的第一篇博客。

直入主题…
**第一步：Boost安装（[Boost下载地址](http://www.boost.org/)）**

我下载的是Boost_59_0-msvc-11.0-32.exe，不过各个版本都可以（我的电脑是64的，但是装的是32的，这个无所谓的）。

1.双击运行.，新建个目录用于安装Boost，选择默认路径也可以，我只是为了以后方便管理。我的安装地址：D:\Program Files\boost_1_59_0

2.新建两个系统环境变量，右键我的电脑->属性->高级系统设置->环境变量。在系统变量里面新建

变量名：`BOOST_LIBRARYDIR`

变量值：`D:\Program Files\boost_1_59_0\lib32-msvc-12.0`
![环境变量设置](https://img-blog.csdn.net/20160927213002840)

变量值中的地址是我的地址，选择自己的Boost安装地址，lib32-msvc-12.0是32位的，版本为12.0，选择自己boost文件下的lib32或者lib64.

继续新建：

变量名：`BOOST_INCLUDEDIR`

变量名：`D:\Program Files\boost_1_59_0`
![环境变量设置](https://img-blog.csdn.net/20160927213039529)

同上，注意修改路径，和boost的版本。

3.将设置的第一个环境变量D:\Program Files\boost_1_59_0\lib32-msvc-12.0（根据自己的地址和版本改地址）加入系统环境变量PATH中。
**第二步：CGAL安装（[CGAL官网下载地址](http://www.cgal.org/download.html)）**

1.双击运行下载的CGAL，可以在官网下载，任何版本都可以，我的是4.7。新建一个安装目录，我的安装在D:\Program Files\CGAL

运行过程中有可能会报错：unable download，然后下面一大串，什么包没安装上什么的，十有八九是你的网不行，我就是这个问题一直不能安装，出现这样的问题，我用的最笨的方法，直接取消安装，再到控制面板里的程序中，删除刚安装的CGAL，大胆删，没事。然后继续重新安装CGAL。

**第三步：Cmake安装，自己搜吧，链接我就不找了**

1.安装CMake，随便安装那里都行，这只是用来编译CGAL文件的。

2.运行Cmake，打开图形化界面，可以看到下图界面，其中：

where is the source code是我们CGAl前面的安装目录，我的是D:/Program Files/CGAL

where to build the binaries是我们编译后文件的存放位置，我们在CGAl安装目录下先新建一个build文件夹，然后选择这个文件夹，如下所示：
![文件路径选择](https://img-blog.csdn.net/20160927213417373)

选择好文件夹地址

3.点击左下角的*Configure*，让程序运行，如果不报错，OK

4.点击*Generate*

报错了的话，去检查一下前面设置的环境变量是否正确，安装是否正确。成功后如下图：
![运行成功结果](https://img-blog.csdn.net/20160927213524201)

5.进入上一步新建的build文件夹，即D:/Program Files/CGAL/build，找到CGAL.sln，双击打开vs2013程序，分别用Debug和Release运行编译主程序ALL_BUILD，打开后如下图，直接点击运行即可：
![运行ALL_BUILD](https://img-blog.csdn.net/20160927213719874)

6.看看有没有报错，报错的话返回前面进行检查一遍，没有错误的话，将*D:\Program Files\CGAL\build\bin*添加到系统环境变量*PATH*中。
**第四步：CGAL配置**

1.打开vs2013，新建控制台程序，选择空项目。

2.点击菜单栏中的视图->其他窗口->属性管理器->右键项目名->添加新项目属性表。
![添加新项目属性表](https://img-blog.csdn.net/20160927213915406)

进去后，找到VC++目录

选择包含目录，根据自己的安装目录添加：
```
D:\Program Files\boost_1_59_0
D:\Program Files\CGAL\auxiliary\gmp\include
D:\Program Files\CGAL\build\include
D:\Program FIles\CGAL\include
```

VC++目录下的库目录：

```
D:\Program FIles\boost_1_59_0\lib32-msvc-12.0
D:\Program Files\CGAL\auxiliary\gmp\lib
D:\Program FIles\CGAL\build\lib\Debug
D:\Program FIles\CGAL\build\lib
```

VC++目录下的源目录：

```
D:\Program Files\CGAL\build\src
D:\Program FIles\CGAL\src
```

链接器->输入->附加依赖项：

```
libgmp-10.lib
libmpfr-4.lib
```

配置好后右键保存

CGAL配置到这就差不多了，不过很多时候CGAL是和openGL配套使用的，建议也装下openGL我相信你会用到的，还有就是我在运行demo的时候最后出现过几个错误，好像是什么*_CRT_SECURE_NO_WARNING*…什么的，这好像是版本兼容问题，这应该是我选的demo版本太旧的问题，要在链接器->命令行下加一些代码，这个问题百度下很常见。







