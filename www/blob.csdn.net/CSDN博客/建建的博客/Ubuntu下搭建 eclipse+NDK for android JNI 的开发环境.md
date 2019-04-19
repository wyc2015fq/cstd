# Ubuntu下搭建 eclipse+NDK for android JNI 的开发环境 - 建建的博客 - CSDN博客
2017年06月08日 11:33:32[纪建](https://me.csdn.net/u013898698)阅读数：570
一、准备安装包
1、去官网下载adt-bundle-[Linux](http://lib.csdn.net/base/linux) 和 [Android](http://lib.csdn.net/base/android)-ndk，我的环境是Ubuntu64位，所以我下载的版本是：
（1）adt-bundle-linux-x86_64-20131030.zip
（2）android-ndk-r9b-linux-x86_64 .tar.bz2
下载完后解压，用命令行进入，执行  "  ./eclipse "，就可以启动eclipse了。
我当时遇到了android项目的R.[Java](http://lib.csdn.net/base/java) 不能自动生成的现象，后来修改了adt-bundle 目录下的sdk目录权限就好了，执行 chmod 777 sdk -R，。
二、环境配置
因为我们下载的是adt-bundle，所以软件包中集成了CDT不需要我们再手动安装了。
现在要做的只是给eclipse添加一些路径：
1、给Eclipse设置NDK路径： window --> preference --> NDK, 然后在配置窗口的左侧输入NDK路径
![](https://img-blog.csdn.net/20131228164655921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、下面的工作都是针对项目的配置。我们通过  右键项目   -->  properties  来修改
2.1、设置Android API版本，如图
![](https://img-blog.csdn.net/20131228165224328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.2、选择项目的Builders，我这里默认的就是CDT Builders，就用他所以不需要改了。
![](https://img-blog.csdn.net/20131228165721812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.3、配置Builder的编译命令，选择的是 Use default build command
![](https://img-blog.csdn.net/20131228170001656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.4、配置Builder的交叉编译工具，我们使用 “ Android GCC”  和  “Android Builder”
![](https://img-blog.csdn.net/20131228170121343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.5、最后一步，添加  jni 、c、c++ 寻找头文件的路径，如图，需要我们添加的（图片中黑体部分）主要有：
（1）[Java](http://lib.csdn.net/base/java)jdk  -->  include 目录路径
（2）jdk --> include --> linux （好像eclipse不会寻找子目录，我没[测试](http://lib.csdn.net/base/softwaretest)，反正加上了）
（3） $NDK_HOME_DIR --> platforms --> android-xx --> arch-arm / usr / include
（4）$NDK_HOME_DIR --> toolchains --> arm-linux-androideabi-4.8 --> prebuilt --> linux-x86_64 --> lib -->  gcc / arm-linux-androideabi/4.8/include
![](https://img-blog.csdn.net/20131228170428875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hhbmNlX3lpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
