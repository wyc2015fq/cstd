
# 如何安装多个Python版本以及在Pycharm中切换Python版本 - Magician的博客 - CSDN博客


2018年04月22日 10:32:15[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：8987


[目录](#目录)[前言：](#前言)
[首先要切换不同的版本，你必须先下载不同的Python版本，整个步骤如下所示：](#首先要切换不同的版本你必须先下载不同的python版本整个步骤如下所示)[1、下载Python2.7x和Python3.5x版本](#1下载python27x和python35x版本)
[2、安装Python2.7x和Python3.5x版本](#2安装python27x和python35x版本)
[3、配置环境变量，分别添加如下至path路径](#3配置环境变量分别添加如下至path路径)
[4、只修改Python27（E:\IDES\Python27）文件中的.exe文件（这样系统默认为Python3.5）](#4只修改python27eidespython27文件中的exe文件这样系统默认为python35)
[5、如何在cmd中分别调用Python2.7和Python3.5](#5如何在cmd中分别调用python27和python35)
[其次再说Pycharm如何切换不同的版本：](#其次再说pycharm如何切换不同的版本)[1、打开File菜单，选择Settings：](#1打开file菜单选择settings)
[2、打开Settings窗口后，选择Project: prjectName下面的Porject Interpreter，然后出现下图：](#2打开settings窗口后选择project-prjectname下面的porject-interpreter然后出现下图)


# 目录
## 前言：
最近由于工作需求，要在不同环境下玩Python，所以就有了以上的需求，查了一部分资料后，总结记录下整个问题的解决过程，方便后来者少跳坑。
参考：[https://blog.csdn.net/dream_an/article/details/51248736](https://blog.csdn.net/dream_an/article/details/51248736)
参考：[https://blog.csdn.net/sinat_36246371/article/details/55251854](https://blog.csdn.net/sinat_36246371/article/details/55251854)
# 首先要切换不同的版本，你必须先下载不同的Python版本，整个步骤如下所示：
## 1、下载Python2.7x和Python3.5x版本
此处略去N个字。。。
## 2、安装Python2.7x和Python3.5x版本
很简单，直接点击安装，注意安装位置，我的安装位置为E:\IDES\Python27\和E:\IDES\Python35
![这里写图片描述](https://img-blog.csdn.net/20180422101737502?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3、配置环境变量，分别添加如下至path路径
![这里写图片描述](https://img-blog.csdn.net/20180422101746838?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4、只修改Python27（E:\IDES\Python27）文件中的.exe文件（这样系统默认为Python3.5）
将python.exe修改为python2.exe （声明了环境变量后，输入相关的指令时，就会到对应的环境变量指定的目录下去启动对应的脚本，此时如果Python2与Python3对应的启动脚本名称相同，系统就会报错。所以有必要把两者的启动脚本名称更改的不一样）
![这里写图片描述](https://img-blog.csdn.net/2018042210175629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5、如何在cmd中分别调用Python2.7和Python3.5
**5.1、输入Python2 可调用Python2.7版本**
![这里写图片描述](https://img-blog.csdn.net/20180422101804101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**5.2、输入Python 可调用Python3.5版本**
![这里写图片描述](https://img-blog.csdn.net/20180422101813884?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 其次再说Pycharm如何切换不同的版本：
## 1、打开File菜单，选择Settings：
![这里写图片描述](https://img-blog.csdn.net/20180422102731211?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、打开Settings窗口后，选择Project: prjectName下面的Porject Interpreter，然后出现下图：
![这里写图片描述](https://img-blog.csdn.net/20180422102740305?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后在该窗口的右面Porject Interpreter下拉菜单中选择想要的Python版本号就可以了。（注意：Pycharm环境下可以可以编辑虚拟的Python环境，就类似于一个工程对应一个Python环境，你只要编辑好之后，再次加载该工程时，会将对应的环境一起加载进来，不用再人为的切换环境）

