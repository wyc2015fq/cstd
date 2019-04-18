# linux 环境变量设置（临时 + 永久） - z69183787的专栏 - CSDN博客
2017年10月09日 11:25:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6116
一、环境变量
环境变量是具有特殊名字的一个特定对象，包含了一个或多个应用程序运行所需的信息。（例如PATH，可执行程序的搜索路径，当要求系统运行一个程序，而没告诉系统它的具体路径时，系统就要在PTAH值的路径中寻找此程序，找到去执行）
环境变量不仅有PATH，系统的环境变量还有：
C_INCLUDE_PATH 头文件的搜索路径
LIBRARY_PATH静态库搜索路径（编译时包含）
LD_LIBRARY_PATH动态库搜索路径（链接时包含）
...
此外，还可以根据具体程序的要求，为某程序单独设置其所需的环境变量。
例如tesseract，运行时需要traindata信息：
1、它默认去/usr/local/share目录下去找tessdata目录，使用其中的数据。
2、tesseract提供一个机会，让我们可以设置TESSDATA_PREFIX环境变量，指定tessdata的搜索路径，
可以设置 TESSDATA_PREFIX=/home/xyz/Tesseract/share      就可以在此路径的tessdata目录下放自己的训练数据了，tesseract运行时，会去这查找。
二、环境变量的设置（临时 + 永久）
举例：tesseract安装在当前用户某个目录下，其依赖库lept也安装在当前目录下
（非系统及程序默认的/usr/local/lib     /usr/local/include/    /usr/local/share）
-------------------------------------------------------------------------------------------------------------------------------------------
临时设置：
1、直接用export命令：
#export PATH=$PATH:/home/xyz/Tesseract/bintesseract可执行文件目录
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH://home/xyz/Tesseract/lib其依赖库lept的路径
#export TESSDATA_PREFIX=/home/xyz/Tesseract/share其依赖的训练数据文件所在路径
查看是否已经设好，可用命令export查看：
# export 或者#echo $PATH
2、若crontab后台运行程序，也可在sh脚本进行临时设置
#! /bin/sh
#export PATH=$PATH:/home/xyz/Tesseract/bintesseract可执行文件目录
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH://home/xyz/Tesseract/lib其依赖库lept的路径
#export TESSDATA_PREFIX=/home/xyz/Tesseract/share其依赖的训练数据文件所在路径
。。。其他操作语句
---------------------------------------------------------------------------------------------------------------------------------------------
永久设置
1、修改profile文件：
修 改/etc/profile（对所有用户都是有效的）
#vi /etc/profile
在里面加入:
export PATH="$PATH:/home/xyz/Tesseract/bin"
2、 修改.bashrc文件：
修改~/.bashrc文件。  （每个用户目录下都有，ls -all，单独用户有效）
cd ~
vi .bashrc
在里面加入：
export PATH="$PATH:/opt/au1200_rm/build_tools/bin"
永久设置方法一般需要重新注销系统才能生效，注销重登陆后，可以通过echo命令[测试](http://lib.csdn.net/base/softwaretest)一下：
-------------------------------------------------------------------------------------------------------------------------------------------
三、设置、取消
         1、用set命令查看当前用户本地shell设置的所有变量
                    #set
          2、用unset取消变量的设置
                   #unset $TESSDATA_PREFIX
---------------------------------------------------------------------------------------------------------------------------------------------
　“/bin”、“/sbin”、“/usr/bin”、“/usr/sbin”、“/usr/local/bin”等路径已经在系统环境变量中了，如果可执行文件在这几个标准位置，在终端命令行输入该软件可执行文件的文件名和参数(如果需要参数)，回车即可。
　　如果不在标准位置，文件名前面需要加上完整的路径。不过每次都这样跑就太麻烦了，一个“一劳永逸”的办法是把这个路径加入环境变量。命令 “PATH=$PATH:路径”可以把这个路径加入环境变量，但是退出这个命令行就失效了。要想永久生效，需要把这行添加到环境变量文件里。有两个文件可选：“/etc/profile”和用户主目录下的“.bash_profile”，“/etc/profile”对系统里所有用户都有效，用户主目录下的“.bash_profile”只对这个用户有效。
　　“PATH=$PATH:路径1:路径2:...:路径n”，意思是可执行文件的路径包括原先设定的路径，也包括从“路径1”到“路径n”的所有路径。当用户输入一个一串字符并按回车后，shell会依次在这些路径里找对应的可执行文件并交给系统核心执行。那个“$PATH”表示原先设定的路径仍然有效，注意不要漏掉。某些软件可能还有“PATH”以外类型的环境变量需要添加，但方法与此相同，并且也需要注意“$”。
　　注意，与DOS/Window不同，UNIX类系统环境变量中路径名用冒号分隔，不是分号。另外，软件越装越多，环境变量越添越多，为了避免造成混乱，建议所有语句都添加在文件结尾，按软件的安装顺序添加。
　　格式如下()：
　　# 软件名-版本号
　　PATH=$PATH:路径1:路径2:...:路径n
　　其他环境变量=$其他环境变量:...
　　在“profile”和“.bash_profile”中，“#”是注释符号，写在这里除了视觉分隔外没有任何效果。
　　设置完毕，注销并重新登录，设置就生效了。如果不注销，直接在shell里执行这些语句，也能生效，但是作用范围只限于执行了这些语句的shell。
　　相关的环境变量生效后，就不必老跑到软件的可执行文件目录
