# Android测试时常用到的指令 - zhusongziye的博客 - CSDN博客





2018年05月08日 22:48:31[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：352








在面试app测试的时候，很多面试官会问一下Android的常用指令，你要是知道的话，肯定是加分，所以今天就来普及下安卓常用指令。

##### 1、adb的常用指令有：

(1)查看当前运行的设备和获得root权限的指令：

     adb -devices和adb root

(2)把电脑上的文件复制拷贝到手机设备上的指令：


   adb push<1><2>

    <1>：为电脑上的磁盘路径,如e:/xx.png; 

    <2>：为手机上的目录,如/sdcard/

(3)把手机设备上的文件复制拷贝到电脑上的指令：

     adb pull

 ：手机设备上的目录,如：/sdcard/xx.png; 

  ：为电脑磁盘目录,如：e:/

(4)启动shell窗口，可以执行一些常用的Linux的指令(如ls/mkdir/rm等等)

  adb shell

(5)安装APK的指令：

adb install [-r] [-s]

[-r]为重新安装;

[-s]为安装到SD卡上,默认是安装到内部存储器上;

为apk包所在目录下的文件

(6)卸载APK的指令：

adb uninstall [-k]

 [-k]只删除应用程序,不删除数据和缓存目录;

为删除指定的包名

2、对apk安装包进行优化的指令：

       zipalign -f -v 4

         -f：为强制覆盖已存在的文件;

         -v：生成详细的输出;

3、指令创建秘钥库(JDK的bin目录下提供的keytool.exe)

       keytool -genkeypair -alias -keyalg RSA -validity 500 -keystore

    1）-genkeypair：为生成的是数字证书;

    2）-alias：为生成数字证书的别名, 如为test.keystore|test.jks;

    3）-keyalg：为生成数字证书的算法, 使用的是RSA算法;

    4）-validity：为生成数字证书的有效期;

    5）-keystore：为生成数字证书的保存路径, 如为        d:/test.keystore|test.jks.

4、使用jarsigner对未签名的apk进行签名的指令：

   jarsigner -verbose -keystore -signedjar

    1）-verbose：为生成详细的输出;

    2）-keystore：为数字证书的存储路径, 如为        d:/test.keystore|test.jks;

    3）-signedjar：后面三个参数: 为签名后的APK文件,为未签名的APK文件, a为数字证书的别名, 如test.keystore|test.jks.






