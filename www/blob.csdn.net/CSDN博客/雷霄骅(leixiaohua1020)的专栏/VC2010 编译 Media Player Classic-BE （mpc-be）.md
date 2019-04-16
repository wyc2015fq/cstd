# VC2010 编译 Media Player Classic - BE （mpc-be） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月16日 23:56:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32
个人分类：[MPC-HC](https://blog.csdn.net/leixiaohua1020/article/category/2286581)










Media Player Classic - BE (mpc-be)播放器是基于Media Player Classic - Home Cinema (mpc-hc)的播放器，它是开源的。是俄国人写的。今天尝试着编译了一下它的源代码。发现总体上和Media Player Classic - Home Cinema (mpc-hc) 差别不大。但是界面做了皮肤，更加的美观。

编译后界面截图：

![](https://img-blog.csdn.net/20130916235454296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第一步:：准备

 1. 安装 Visual C++ 2010(不能是Express版本)
 2. 安装Visual Studio 2010 Service Pack 1 ->[http://www.microsoft.com/downloads/en/details.aspx?FamilyID=75568aa6-8107-475d-948a-ef22627e57a5&displaylang=en](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=75568aa6-8107-475d-948a-ef22627e57a5&displaylang=en)
 3. 安装DirectX SDK (June 2010) ->[http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=3021d52b-514e-41d3-ad02-438a3ba730ba](http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=3021d52b-514e-41d3-ad02-438a3ba730ba)

第二步：准备GCC环境（这个主要是编译FFMPEG用的，FFMPEG只能用GCC编译）

1. 下载并解压msys_be_20120911.7z到C:\MSYS\。地址： [http://dev.mpc-next.ru/index.php/topic,706.0.html](http://dev.mpc-next.ru/index.php/topic,706.0.html)

（注意：1.就算自己电脑上有MSYS_MinGW，也建议下载这个。2.下载地址貌似被墙掉了，需要下载，而且下载页面默认是俄文的 = =，需要改成英文）

2. 编辑  C:\MSYS\etc\下的"fstab" 文件设置MinGW路径
 添加: C:\MSYS\mingw\mingw
 注意他们中间有一个Tab

2. 把以下信息加入系统环境变量：



**[plain]**[view plain](http://blog.csdn.net/leixiaohua1020/article/details/11694711)[copy](http://blog.csdn.net/leixiaohua1020/article/details/11694711)



- Variable Value
 MSYS C:\MSYS
 MINGW32 C:\MSYS\mingw
 MINGW64 C:\MSYS\mingw
 YASM C:\MSYS\bin



第三步：编译

1.使用svn下载 MPC-HC's 到 "C:\mpc-be" （其他地方也行）

svn地址：

svn://mpc-next.ru/mpc-be/trunk

2. 打开目录 C:\mpc-be\
 运行update_gcc.bat (有可能要拷贝 在lib 和 lib64文件夹中的libgcc.a 和 libmingwex.a)

3.其他步骤和编译mpc-hc差不多，里面有很多vs工程，可以分别编译生成相应的exe或者dll

注：直接运行"build.bat" 可以编译所有文件




