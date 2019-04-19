# Java开发Opencv之(一)开发环境安装与配置 - 零度的博客专栏 - CSDN博客
2017年09月27日 15:21:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：873
# 一、下载和安装所需的开发环境
　　使用Java开发程序，当然必不可少的自然是JDK了，关于JDK的安装和环境变量的配置在此不再啰嗦，相信你本地也早已安装好了，本文使用的是JDK1.8、Windows64位开发环境，其他环境暂时没有测试，但理论上应该都是可行的。
　　其次是编辑器的选择，在此还是推荐一下Idea，如果你是Java程序员，一定听说过，如果你是安卓开发者，那你更应该听说，Android Studio就是基于Idea的社区版开发的。扯远了，我这里用的是Idea的最新版 Idea 2016.1 下载[IntelliJ
 IDEA 2016](https://www.jetbrains.com/idea/download/#section=windows)。全平台支持，不管你是Windows还是Mac或者Linux。安装过程也更普通的软件没什么差别。安装好之后建议写个”Hello World"小程序测试一下，如果不成功建议到网上搜索一下安装过程。
　　下面到最重要的OpenCV登场了。目前OpenCV的最新版本是3.1.0，所以后面都是基于OpenCV3.1.0的。使用Java开发程序不像Visual Studio那么复杂的环境配置，因为Java是直接调用OpenCV的jar包中的方法。到[OpenCV主页](http://opencv.org/)下载相应平台的压缩包（虽然是.exe，聪明的你肯定会发现就是个压缩包）。下载完成后解压，我们找到这样一个目录:opencv->build->java
 注意java这个目录文件是我们需要的，其他的暂时都不需要了。
# 二、第一个Java版的OpenCV程序
　　到此为止我们差不多就可以开始第一个Java版本的Opencv程序了。第一个程序我们将读取一张图片，OpenCV的logo如下图，然后将图片的信息使用Mat的格式打印出来。
![](https://img-blog.csdn.net/20160326142813073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
　　运行结果截图：
![](https://img-blog.csdn.net/20160326151315534)
　　首先我们建立一个maven工程，Java开发的应该都知道maven吧，因为该项目都是基于maven和git（后面会提到）来管理的。启动Idea，选择"Create New Project"，在弹出的界面左侧边栏选择"maven"，如果你已经使用Idea测试过"Hello World"程序，那么JDK应该已经设置好了，如果没有，你需要在右边栏上"Project SDK"选择你在本地安装的JDK。界面如图：
![](https://img-blog.csdn.net/20160326143823858?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
　　接着点击"Next"下一步操作，设置maven项目的GroupId和ArtifactId，这里我填的是"GroupId:com.superbool"，"ArtifactId:easy_opencv"，然后点击"Next"设置项目的名称和保存的位置，“Project name:easy_opecv"，"Project location:D:\github\easy_opencv"，然后点击"Finish"就完成了maven项目的创建，以后的代码都是写在该项目中的，就不用再新建工程了。当然，这里的命名和保存的路径等都可以自己修改的。
　　在开始第一个程序之前还得创建几个目录，我们在easy_opencv项目的父目录下创建一个opencv目录，然后将之前解压的OpenCV中的build_>java目录下的内容拷贝过来。然后在src->main->java目录下创建我们第一个类FirstOpenCV.java。
　　创建好的maven工程如下图所示（也可以手动创建这些目录，关于每个目录的作用，在下一篇会做详细解释）:
![](https://img-blog.csdn.net/20160326151147419)
　　如果觉得上面的步骤太繁琐，那么你可以直接在github上clone我的代码[easy_opencv](https://github.com/superbool/easy_opencv.git)，然后切换到day1分支上。
　　终于要开始写代码了，第一个基于Java的OpenCV的代码：
```
public class FirstOpenCV {
      static {
           System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
           //注意程序运行的时候需要在VM option添加该行 指明opencv的dll文件所在路径
           //-Djava.library.path=$PROJECT_DIR$\opencv\x64
      }
  
      public static void main(String[] args) {
           Mat mat = Imgcodecs.imread("C:/opencv.png");
           System.out.println(mat);
     }
}
```
　　等等，"Core.NATIVE_LIBRARY_NAME"以及"Imgcodecs.imread()"方法都标红了，原来是opencv的jar包还没引用进来，所以还得一步操作，导OpenCV的jar包，点击下图的左上角红框标识的图标，弹出"Project Structure"选项界面，依次选择"Modules"->"Dependencies"->"+"->"1 JARs or Dependencies..."选中刚opencv目录下的"opencv-310.jar"，再看代码是不是飘红的线都不见了呢。
![](https://img-blog.csdn.net/20160326152950020)
　　然后使用Ctr+Shift+F10快捷键运行第一个main方法，程序抛出了一个异常：
```
1 Exception in thread "main" java.lang.UnsatisfiedLinkError: no opencv_java310 in java.library.path
 2     at java.lang.ClassLoader.loadLibrary(ClassLoader.java:1864)
 3     at java.lang.Runtime.loadLibrary0(Runtime.java:870)
 4     at java.lang.System.loadLibrary(System.java:1122)
 5     at com.superbool.FirstOpenCV.<clinit>(FirstOpenCV.java:12)
 6     at java.lang.Class.forName0(Native Method)
 7     at java.lang.Class.forName(Class.java:264)
 8     at com.intellij.rt.execution.application.AppMain.main(AppMain.java:122)
 9 
10 Process finished with exit code 1
```
　　由于程序Java程序是调用的本地OpenCV的动态链接库的，所以在这里运行的时候JVM找不到.dll文件的位置，自然就报错了，解决办法也很简单，点击如图中的方框"FirstOpenCV"->"Edit Configurations" 弹出如下的界面，在"VM options"一栏中添加"-Djava.library.path=\$PROJECT_DIR\$\opencv\x64"(注意美元符前面没有斜杠)，如果是32位系统请填"\x32"。
![](https://img-blog.csdn.net/20160326153650049)
　　再使用Ctr+Shift+F10快捷键运行程序，是不是结果终于出来了呢，这里只打印了读取的mat图像的基本信息。
       注意：使用 “-Djava.library.path=\$PROJECT_DIR\$\opencv\x64”经测试还是没有找到dll，可以使用绝对路径：
                 如：”-Djava.library.path=D:\opencv\opencv\build\java\x64“解决。
![](https://img-blog.csdn.net/20160326154321681)
　　不过由于OpenCV官方的jar包还没提供像C++一样的highgui接口，所以暂时还无法将Mat图片显示在界面上，不过在下一节我们将会自己写一个方法来显示这个mat图像，同时解释一下maven各个目录的作用。
