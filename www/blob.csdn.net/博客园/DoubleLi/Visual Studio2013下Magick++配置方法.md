# Visual Studio2013下Magick++配置方法 - DoubleLi - 博客园







声明：本文系作者原创，如需转载请保持文章完整并注明出处（[http://blog.csdn.net/u010281174/article/details/52224829](http://blog.csdn.net/u010281174/article/details/52224829)）。

ImageMagick是一个图片编辑的跨平台开源库，可以读写各种类型的图片（包括svg格式的矢量图）并对其进行处理。本文将介绍Win10平台下用源码编译ImageMagick库的流程，主要支持Visual Studio 2013的调用。

# 一、下载源码

首先前往官网下载源码。注意是下载源码，不是可执行文件。（[http://imagemagick.org/script/install-source.php#windows](http://imagemagick.org/script/install-source.php#windows)）选择Install from Windows Source下的download。

# 二、配置configure.exe



下载后解压，找到\ImageMagick-7.0.2-9\VisualMagick\configure文件夹（注意，只有windows平台下的源码才包含VisualMagick文件夹，如果找不到这个文件夹可能是下载错了），如果该文件夹下没有configure.exe文件，则用VS打开configure.sln生成configure.exe文件。

# 三、用configure.exe生成.sln文件



打开configure.exe，
![](http://img.blog.csdn.net/20160816204034086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



单击下一步。
![](http://img.blog.csdn.net/20160816204037391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



来到配置页面，这里需要注意：


- 第一栏build type setup，根据需要选择动态链接库还是静态链接库；
- 第二栏里面，如果是生成x64平台下的库文件，就勾选Build 64-bit distribution；选择正确的VS版本（Visual Studio Version），我是用的是VS2013，因此选择了2013；建议勾选Include all demo, test, appl, and contrib director。



然后完成，生成项目。

# 四、编译项目。



接下来前往父目录（\ImageMagick-7.0.2-9\VisualMagick）打开Visual*.sln，编译生成项目。

生成的过程中可能会因为编码问题遇到几个error：


- 项目CORE_pango，提示常量中包含换行符，找到错误的位置，在字符串最后添加一个空格即可；这是因为非ASCII码后面直接跟的字符被忽略掉了。
- afstyles.h文件中line100有个错误，这个错误同样是因为编码问题，但是这个问题的根源来自于afscript.h文件，其中很多/**/注释中间是非ASCII字符，将这些注释后面（*/前面）添加一个空格即可解决问题。这个bug隐藏的很深，思路来源自[http://savannah.nongnu.org/bugs/?48126](http://savannah.nongnu.org/bugs/?48126)，感谢这位作者！



弄完这几个bug就差不多可以编译成功了。

注意：最好将debug和release都编译一遍，这两者生成的.lib和.dll文件分别为：*DB*./*RL*.

# 五、使用



编译好之后，将\VisualMagick\bin目录下的.dll动态链接库拷贝到C:\Windows\System32目录下，新建一个Visual Studio项目，添加

包含目录：





**[plain]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- $(ImageMagickPath)\ImageMagick  
- $(ImageMagickPath)\ImageMagick\Magick++\lib  




库目录：





**[plain]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- $(ImageMagickPath)\VisualMagick\lib  




debug下添加附加依赖项：





**[plain]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- CORE_DB_Magick++_.lib  
- CORE_DB_MagickCore_.lib  
- CORE_DB_MagickWand_.lib  




release下添加附加依赖项：





**[plain]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- CORE_RL_Magick++_.lib  
- CORE_RL_MagickCore_.lib  
- CORE_RL_MagickWand_.lib  




添加一个cpp文件，用官网给的示例（[http://imagemagick.org/script/magick++.php](http://imagemagick.org/script/magick++.php)）：



**[cpp]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- #include <Magick++.h>   
- #include <iostream>   
- 
- using namespace std;   
- using namespace Magick;   
- 
- int main(int argc,char **argv)   
- {   
-   InitializeMagick(*argv);  
- 
- // Construct the image object. Seperating image construction from the   
- // the read operation ensures that a failure to read the image file   
- // doesn't render the image object useless.   
-   Image image;  
- try {   
- // Read a file into image object   
-     image.read( "logo:" );  
- 
- // Crop the image to specified size (width, height, xOffset, yOffset)  
-     image.crop( Geometry(100,100, 100, 100) );  
- 
- // Write the image to a file   
-     image.write( "logo.png" );   
-   }   
- catch( Exception &error_ )   
-     {   
-       cout << "Caught exception: " << error_.what() << endl;   
- return 1;   
-     }   
- return 0;   
- }  




如果这个项目运行后提示



**[plain]**[view plain](http://blog.csdn.net/u010281174/article/details/52224829#)[copy](http://blog.csdn.net/u010281174/article/details/52224829#)



- UnableToOpenConfigureFile `magic.xml'  


的话，可以把\bin\magic.xml复制到exe文件所在的文件夹。这个问题貌似是源码的一个bug，加载*.xml的时候路径错误。尚未找到其他解决方案。如果有更好的办法欢迎留言。



到此，就把ImageMagick配置好了。



参考：



[http://blog.csdn.net/fksec/article/details/36008343](http://blog.csdn.net/fksec/article/details/36008343)

[http://qingqingzjin.blog.163.com/blog/static/1881032672013917103716344/](http://qingqingzjin.blog.163.com/blog/static/1881032672013917103716344/)




- 顶
0- 踩










