# android开发环境 - 长歌行 - CSDN博客





2012年02月27日 13:53:10[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：622








Android开发环境搭建全程演示(jdk+eclipse+android sdk)

2010-04-03 13:32

         全程演示android开发环境的搭建过程，无需配置环境变量。所有软件都是写该文章时最新版本

友情广告：**[微美丽](http://weimeili.taobao.com/)**,谢谢支持!


一 相关下载

(1) java JDK下载:

进入该网页: [http://java.sun.com/javase/downloads/index.jsp](http://java.sun.com/javase/downloads/index.jsp) (或者直接[点击下载](http://cds-esd.sun.com/ESD6/JSCDL/jdk/6u19-b04/jdk-6u19-windows-i586.exe?AuthParam=1270206602_6c9f792e352fc522c4403f147158a87e&TicketId=nodzAF4XQ3F%2FkeIukUaZUpmdcw%3D%3D&GroupName=CDS&FilePath=/ESD6/JSCDL/jdk/6u19-b04/jdk-6u19-windows-i586.exe&File=jdk-6u19-windows-i586.exe))如下图:![](http://hiphotos.baidu.com/lvjava/pic/item/41be638222db13940df4d24a.jpg)

选择 Download JDK 只下载JDK，无需下载jre. 

(2)eclipse下载

   进入该网页: [http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/) (或者直接点击下载:[BT下载](http://build.eclipse.org/technology/phoenix/torrents/jee/eclipse-jee-galileo-SR2-win32.zip.torrent)[HTTP下载](http://download.actuatechina.com/eclipse/technology/epp/downloads/release/galileo/SR2/eclipse-jee-galileo-SR2-win32.zip)) 如下图:

![](http://hiphotos.baidu.com/lvjava/pic/item/beecea16f6ac682e962b432e.jpg)

我们选择第一个(即eclipse IDE for java EE Developers)

(3)下载Android SDK

   说明: Android SDK两种下载版本，一种是包含具体版本的SDK的，一种是只有升级工具，而不包含具体的SDK版本，后一种大概20多M，前一种70多M。

[完全版下载](https://dl-ssl.google.com/android/repository/android-2.1_r01-windows.zip) (android sdk 2.1 r01)     [升级版下载](http://dl.google.com/android/android-sdk_r04-windows.zip) (建议使用这个，本例子就是使用这个这里面不包含具体版本，想要什么版本在Eclipse里面升级就行)

二 软件安装

   (1)安装jdk 6u19   安装完成即可，无需配置环境变量

   (2)解压eclipse       eclipse无需安装，解压后，直接打开就行

   (3)解压android sdk     这个也无需安装，解压后供后面使用

   (4)最终有三个文件夹，如下图:

![](http://hiphotos.baidu.com/lvjava/pic/item/d58b1b3f2330bcda55e72364.jpg)


三 Eclipse配置

   1 安装android 开发插件

     (1)打开Eclipse, 在菜单栏上选择 help->Install New SoftWare 出现如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/4566b0132b6ba5e6f6039e6a.jpg)


    点击 Add按钮,出现如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/bba62bdfefa34d2562279875.jpg)


     输入网址: [https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)    (如果出错，请将https改成http)

     名称: Android (这里可以自定义)

   点击OK，将出现如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/95bb3055ab4d2cf2b645ae72.jpg)


点击 Next按钮 ，出现如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/27997354b7741a62574e007d.jpg)


点击Next按钮，出现如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/977e30d1208535e5562c847a.jpg)


选择 I accept the terms of the license agreements   点击Next,进入安装插件界面

![](http://hiphotos.baidu.com/lvjava/pic/item/c17fdafe2f477b075d600840.jpg)

安装完成后，出现如下界面


![](http://hiphotos.baidu.com/lvjava/pic/item/9053b7109981fe37203f2e4e.jpg)


点击Yes按钮，重启Eclipse

2 配置android sdk

     (1)点击菜单window->preferences,进入如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/6b5c1d7faa5cc03e29388a51.jpg)


   选择你的android SDK解压后的目录，选错了就会报错，这个是升级工具，目前还没有一个版本的SDK

（2）升级SDK版本,选择菜单 window->Android sdk and avd manager 出现如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/4a9492d32d9c8602960a1621.jpg)


   选择update all按钮，出现如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/69bfdfdd419957d977c6382e.jpg)


选择左边的某一项，点击accept表示安装，点击reject表示不安装，我这里只选了SDK 2.1 和samples for api 7 , 自己可以任意自定义，确定后，选择install按钮，进入安装界面如下:

![](http://hiphotos.baidu.com/lvjava/pic/item/1a89498d8de2fe24b21bba33.jpg)

安装完成如下:

![](http://hiphotos.baidu.com/lvjava/pic/item/1f548e8f3cc353d8f11f363d.jpg)

(3)新建AVD(android vitural device)    和上面一样，进入android sdk and avd manager,选中Vitural Devices 在点击New按钮![](http://hiphotos.baidu.com/lvjava/pic/item/619a3be9a0a0e609b90e2d07.jpg)


点击New按钮后，进入如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/9e017d105cbb0fcac2ce790f.jpg)

名称可以随便取，target选择你需要的SDK版本，SD卡大小自定义,点击 Create AVD,得到如下结果

![](http://hiphotos.baidu.com/lvjava/pic/item/bba62bdfef5f4d2562279811.jpg)

如上显示创建AVD完毕









3 新建Android项目

(1)选择菜单file->new->other 进入如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/b6259e0f44673cdbaa645739.jpg)

选择新建Android Project项目，点击Next按钮，进入如下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/da5b152e19fe5c664ec226ec.jpg)

名称自定义，应用程序名自定义，报名必须包含一个点以上，min SDK version里面必须输入整数

点击Next出现如下界面:

![](http://hiphotos.baidu.com/lvjava/pic/item/da59ef39ced1fdc53b87cef7.jpg)

注: 若有错误如: Project ... is missing required source folder: 'gen' ,则将gen->Android.Test->R.java这个文件删掉，Eclipse会为我们重新生成这个文件，并且不会报错。

(3)配置运行

   右键项目->Run as -> Run Configuration 进入如下界面:![](http://hiphotos.baidu.com/lvjava/pic/item/ab9a83122e1304f8c2fd78d5.jpg)

该界面，点击Browse 按钮，选择你要运行的项目

选择Target切换到以下界面

![](http://hiphotos.baidu.com/lvjava/pic/item/3d9949f08eef9d99a50f52ca.jpg)

该界面选择运行的AVD，将AVD前面的方框设置为选择状态。



(4)测试项目运行

   右键项目名称->run as ->Android Application 即可启动运行该Android程序，如下所示:

![](http://hiphotos.baidu.com/lvjava/pic/item/2f44f9fd14005a73d6887da1.jpg)


正在进入

![](http://hiphotos.baidu.com/lvjava/pic/item/5e7179163924877c20a4e9ac.jpg)

测试程序运行结果

四 结束语

       至此，android开发环境搭建完毕，有问题请留言。在这里要注意，我这里只是下载了android sdk r4升级工具，没有下载具体的SDK，而是通过在Eclipse里面的Android Sdk管理工具升级的，你也可以直接下载具体的SDK版本，如: Android sdk 2.1 r1 上面有这个的下载链接，但我任务用升级工具更好。




引用地址

http://hi.baidu.com/lvjava/blog/item/74a8480ee120e0c67acbe10f.html





