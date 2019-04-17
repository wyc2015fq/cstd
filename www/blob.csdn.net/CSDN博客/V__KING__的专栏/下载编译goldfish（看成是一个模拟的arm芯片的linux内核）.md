# 下载编译goldfish（看成是一个模拟的arm芯片的linux内核） - V__KING__的专栏 - CSDN博客





2013年10月25日 17:14:11[v__king__](https://me.csdn.net/V__KING__)阅读数：1115









关于如何下载Android4.0源码，请查看我的博客内另一篇文章(同样是图文教程):


[http://blog.csdn.net/flydream0/article/details/7036156](http://blog.csdn.net/flydream0/article/details/7036156)


如何编译Android4.0源码请看:

[http://blog.csdn.net/flydream0/article/details/7046612](http://blog.csdn.net/flydream0/article/details/7046612)

[](http://blog.csdn.net/flydream0/article/details/7046612)


下面进入正题:



第一步:下载goldfish源码


在Android源码根目录下新建kernel文件夹





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $mkdir kernel  
- $cd kernel  

下载源码:





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $git clone http://android.googlesource.com/kernel/goldfish.git   

![](http://hi.csdn.net/attachment/201112/14/0_1323845106Z19M.gif)
下载完毕如下图:


![](http://hi.csdn.net/attachment/201112/14/0_1323845296R9Eo.gif)


此时在kernel目录下会生成一个goldfish文件夹。进入此目录:





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $cd goldfish  


此目录下有一个隐藏的目录.git,通过






**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ls -al  


可看到此目录:


![](http://hi.csdn.net/attachment/201112/14/0_1323845573upel.gif)



查看所有分支:





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- git  branch -a  

如下图:


![](http://hi.csdn.net/attachment/201112/14/0_1323845730NX0v.gif)



check out:






**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $git checkout remotes/origin/android-goldfish-2.6.29  





![](http://hi.csdn.net/attachment/201112/14/0_1323845943FAXM.gif)



此时你会看到goldfish目录下会出现很多文件:





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ls  


![](http://hi.csdn.net/attachment/201112/14/0_1323846010Z3Jd.gif)



 这个时候goldfish源码就已经下下来了，接下来的事情就是编译了.





第二步:编译goldfish

导出交叉编译工具目录到$PATH环境变量中去.





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- export PATH=$PATH:~/WORKING_DIRECTORY/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin  


我们将使用上述这个目录下的交叉编译器**arm-eabi-**gcc



然后在glodfish目录下用gedit打开Makefile文件，找到这两行文字:

**# ARCH****?= (SUBARCH)**

** # CROSS_COMPILE****?=修改为:**ARCH?= arm****
**CROSS_COMPILE    ?= arm-eabi-**


**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $gedit Makefile  

![](http://hi.csdn.net/attachment/201112/15/0_132391563915xa.gif)

注意:ARTH ?=arm,的arm后边不要有空格，不然就会出现如下类似错误:

Make:...../kernel/goldfish/arch/arm: Is a directory.  Stop.

害我白白浪费了几个小时.


上述操作的目的是为了指定目标设备的体系架构和交叉编译器，其实也可以通过以下指令来完成:



**[plain]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ export ARCH=arm  
- $ export SUBARCH=arm  
- $ export CROSS_COMPILE=arm-eabi-  




为了确保环境参数正确，接下来执行下面两条指令,否则有可能不能正常启动模拟器.(注:以下两条指令是在Android源码根目录下执行)



**[cpp]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ source build/envsetup.sh  //缺少这条指令，可能会导致无法编译通过
- $ lunch full-eng            //缺少这条指令，可能会导致无法启动模块器，系统报无法找到AVD，并要求你创建AVD。



关闭gedit,接下来就开始make了,执行如下指令:



**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ make goldfish_armv7_defconfig  
- $ make  

注：用$make goldfish_defconfig这样配置也可以编译通过，模拟器也可以启动，但是Android的开机画机就显示不了，$adb shell也死活连不上，原因就是这个goldfish_defconfig这个配置文件问题.


提示:

$make goldfish_armv7_defconfig指令的意思是将目录WORKING_DIRECTORY/kernel/goldfish/arch/arm/configs/下的goldfish_armv7_defconfig文件内的Kconfig配置内容复制到WORKING_DIRECTORY/kernel/goldfish/目录下的.config文件中，.config文件是一个隐藏目录，保存着各个目录下Kconfig文件的配置.

最终结果如下图所示:
![](http://hi.csdn.net/attachment/201112/14/0_1323848396tlxf.gif)


这样就表示编译成功了.





**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ ls arch/arm/boot/  


可以看到zImage文件.
![](http://hi.csdn.net/attachment/201112/14/0_13238486137t03.gif)


第三步:在模拟器中启动编译好的内核


按下来再运行其下指令:



**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $ export PATH=$PATH:~/WORKING_DIRECTORY/out/host/linux-x86/bin  
- $ export ANDROID_PRODUCT_OUT=~/WORKING_DIRECTORY/out/target/product/generic  
- $ emulator -kernel ~/WORKING_DIRECTORY/kernel/goldfish/arch/arm/boot/zImage &   


模拟器启动界面:
![](http://hi.csdn.net/attachment/201112/15/0_1323916450wMir.gif)





进入模拟器从设置里看版本信息:

![](http://hi.csdn.net/attachment/201112/15/0_1323916639t0p7.gif)


从上图可以看出当前Android版本是4.0.1,内核版本是2.6.29,说明成功了.


同样也可以通过adb shell来查看内核版本信息,如下图:






**[html]**[view
 plain](http://blog.csdn.net/flydream0/article/details/7070392#)[copy](http://blog.csdn.net/flydream0/article/details/7070392#)



- $adb shell  
- #cd proc  
- #cat version  

![](http://hi.csdn.net/attachment/201112/15/0_1323917505xoxn.gif)






完!



