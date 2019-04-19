# Android中如何编译运行系统自带桌面Launcher2源码 - xqhrs232的专栏 - CSDN博客
2012年02月09日 11:42:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：784
原文地址::[http://blog.sina.com.cn/s/blog_4b650d650100nstz.html](http://blog.sina.com.cn/s/blog_4b650d650100nstz.html)
Android的源码包,压缩文件大概有3个G左右,要使用其中自带的一些源码需要很多技巧,否则会提示找不到一些库,大量的报错让人心神不定,不知所从.
    我拿桌面代码举个例子吧.
    桌面代码在源码包的packages\apps\Launcher2目录下.当然如果要想在Eclipse中使用,要把Launcher2拷贝到Eclipse工作目录下打开才行.不过一般来讲,Android的Sample以及自身的源码项目需要一些技巧才能打开,大家可以在网上搜一下,我以前记不清是不是曾经讲过了.
    1.将Laucher2拷贝到Eclipse工作目录下.
    2.将虚拟机编译在android_froyo/out/target/common/obj/JAVA_LIBRARIES/framework_in
termediates/下编译的classes.jar改个名字classes-framework拷贝到sdk/platforms/android-8目录下.
    3.点击Eclipse-->Project-->Properties菜单,弹出下面对话框(图1)
![Android中如何编译运行系统自带桌面Launcher2源码](http://s9.sinaimg.cn/middle/4b650d65g997933d209c8&690)
    4.点击Add Library...,选择User Library,点击Next
    5.弹出新对话框"Add Library",点击User Libraries...(图2)
![Android中如何编译运行系统自带桌面Launcher2源码](http://s13.sinaimg.cn/middle/4b650d65g997942b0c22c&690)
    6.选择New...(图3)
![Android中如何编译运行系统自带桌面Launcher2源码](http://s5.sinaimg.cn/middle/4b650d65g99794ebc71c4&690)
    7.填入framework,点击OK
    8.回到图1界面(图4)
![Android中如何编译运行系统自带桌面Launcher2源码](http://s14.sinaimg.cn/middle/4b650d65g997998ae084d&690)
    9.选择Order and Export标签,选中framework,并点击Up,将framework移到到最上面,然后点击OK
    10.运行Launcher2项目,会有两个报错,将报错行注释掉,从新运行.
    11.运行后发现无法加载Launcher2.
    12.点击包名,按F2键,将src目录下包名改为com.eagle.launcher3.
![Android中如何编译运行系统自带桌面Launcher2源码](http://s3.sinaimg.cn/middle/4b650d65g9987f49a3012&690)
    13.之后,对项目进行Build Project....直到没有错误位置,然后再收缩所有文件,将android.laucher2改为eagle.launcher3
    14.接下来对gen目录下的com.android.launcher目录按步骤12的方法将其名字改为com.eagle.launcher4,呵呵,好玩吧.再重复步骤13.如果在build的时候从新生成了com.android.launcher目录,则删掉它.
    15.最后开始运行项目,如有报错,在寻找未必替换的内容.直到成功.
    16.最后,程序就可以运行啦....呵呵.下面的图就是我按这样的方法运行起来的launcher,只是名字不同而已 
![Android中如何编译运行系统自带桌面Launcher2源码](http://s5.sinaimg.cn/middle/4b650d65g998818f119f4&690)
