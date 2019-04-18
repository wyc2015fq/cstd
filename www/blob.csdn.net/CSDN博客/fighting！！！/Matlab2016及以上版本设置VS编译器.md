# Matlab2016及以上版本设置VS编译器 - fighting！！！ - CSDN博客
2018年06月14日 22:30:02[dujiahei](https://me.csdn.net/dujiahei)阅读数：149
个人分类：[编程软件学习																[matlab](https://blog.csdn.net/dujiahei/article/category/7619771)](https://blog.csdn.net/dujiahei/article/category/7390103)

源地址：https://blog.csdn.net/cztqwan/article/details/78902530
# Matlab2016及以上版本设置VS编译器
在Matlab中使用mex可以编译c/c++ ，但是首先要设置好编译器，可以使用mex -setup命令来设置。如果电脑上是先安装的Visual Studio，然后安装MATLAB，那么一般情况下，只需要在命令行输入mex -setup按照提示选择即可完成编译器的设置。
但是，假如是先安装的matlab后安装的Visual Studio，或者其它一些原因，很可能mex -setup命令无法找到安装好的Visual Studio编译器，那么就需要进行一些修改才能成功设置编译器了。
由于Matlab从2016版本以后使用xml文件来保存配置文件，所以如果使用的Matlab是2016以前的版本，方法可能和本文有所区别。（本文以Matlab2016a和Visual Studio2012为例）
在Matlab的安装目录\bin\win64\mexopts目录中有各种编译器的配置文件，Matlab就是依靠这些xml文件和编译器建立关联：
![](https://img-blog.csdn.net/20171226151227206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对应编译器的xml文件会到windows系统的注册表中读取编译器和SDK的路径信息，之所以mex -setup命令无法找到编译器就是因为注册表中缺少编译器和SDK的路径信息。
对于Matlab2016以前的版本，在\bin\win64\mexopts和mbuildopts目录下是一些stp和bat文件，这些文件中也保存着SDK和不同版本编译器的路径信息。
为了清楚Matlab2016搜索编译器和SDK的过程，使用mex -setup -v命令可以看到搜索过程（下图是已经修改好注册表后的搜索过程）：
![](https://img-blog.csdn.net/20171226152450592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以Visual studio2012为例，Matlab主要从注册表中读取SDK和Visual studio2012的安装路径，出现否代表注册表或环境变量中没有相应的信息或者信息错误。可以根据具体情况修改注册表和环境变量。主要包括以下的路径信息：
- Microsoft\VisualStudio\SxS\VS7：Visual studio的安装路径
- Microsoft\VisualStudio\SxS\VC7：Visual studio的安装路径下的VC目录
- Microsoft\Microsoft SDKs\Windows\v8.0：Microsoft SDKs的v8.0 目录
- VS110COMNTOOLS：Visual studio的安装路径下的Common7\Tools\目录
其中，VS110COMNTOOLS是环境变量，可以在：我的电脑-->属性-->高级系统设置-->环境变量-->系统变量中修改或添加：
![](https://img-blog.csdn.net/20171226151239601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Win+R打开运行窗口，输入regedit命令打开注册表编辑器：
![](https://img-blog.csdn.net/20171226151235475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在注册表编辑器中，HKEY_CURRENT_USER和HKEY_LOCAL_MACHINE分别对应Matlab中的HKCU和HKLM：
![](https://img-blog.csdn.net/20171226151248038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在对应目录下添加项（子目录）和字符串值（用于保存路径），例如在HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SDKs\Windows\下新建“v8.0”项，然后在该项中新建字符串值，根据Matlab搜索过程中的提示命名为“InstallationFolder”，并填入数值数据，Microsoft SDKs一般位于C:\Program Files (x86)\Windows Kits\下。
![](https://img-blog.csdn.net/20171226151248038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171226151252924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3p0cXdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
依次把搜索过程中出现否的地方修正，如果环境变量设置后不起作用，需要重启电脑，然后进行mex -setup即可完成编译器的设置。
