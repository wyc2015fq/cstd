# iOS 生成静态库.a文件 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月29日 22:29:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：920
做iOS开发的同学们都知道，我们在开发的过程中经常用到一些第三方的库，我们可以调用，但是看不见他们的源码。
有时我们不想别人看见我们的源码，也需要生成静态库，那怎么生成呢？
首先，我们要创建一个CoCoa Touch Static Library工程
![](http://upload-images.jianshu.io/upload_images/228486-9711d15d90dc892a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
创建一个CoCoa Touch Static Library工程
这样我们就会得到一个.h和.m文件，我在.h文件声明两个方法（加密，解密），还要在.m文件写它们的实现方法。
![](http://upload-images.jianshu.io/upload_images/228486-0b1387a58eb4b5fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
写入代码
这样我们就可以进行编译了 （记得用模拟器和真机分别编译一次）
> 
快捷键：command+B
两次编译之后，找到StaticLibrary.a所在目录
![](http://upload-images.jianshu.io/upload_images/228486-4ba56fba767e4212.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
找到StaticLibrary.a所在目录
在它的上一级目录中我们可以看见`Debug-iphoneos`和`Debug-iphonesimulator`两个文件，如图：
如果你的工程是release模式，文件名为：`Release-iphoneos`和`Release-iphonesimulator`
![](http://upload-images.jianshu.io/upload_images/228486-8ce97b63ad6d03f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
编译文件
**Debug-iphoneos：应用于真机的静态库文件。Debug-iphonesimulator：应用于模拟器调试的静态库文件。**
我们可以通过`lipo -info`命令来查看这个.a文件的属性。
![](http://upload-images.jianshu.io/upload_images/228486-a3618140d6f3ac6d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
查看属性
现在我们就可以用了 ，但是现在模拟器和真机编译的.a文件是分开的，我们在不同的环境上使用.a文件，需要导入不同的.a文件，比较麻烦，我们可以把它们进行合并成一个文件来使用。（只是体积会变大）
合并命令：`lipo -create`/路径/Debug-iphoneos/libStaticLibrary.a /路径/Debug-iphonesimulator/libStaticLibrary.a
`-output`
/Users/。。。。/Desktop/libStaticLibraryFile.a
![](http://upload-images.jianshu.io/upload_images/228486-6a685bf993d5fe83.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
合成文件
合成完成了，就可以使用了。
把.h文件和合成的.a文件放入你的工程中测试一下吧~
![](http://upload-images.jianshu.io/upload_images/228486-ce2753dc4eab17fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
测试结果
**娱乐一下：**
![](http://upload-images.jianshu.io/upload_images/228486-e6a6fb6a5e67ee68.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240/format/jpg)
文／青楼（简书作者）
原文链接：http://www.jianshu.com/p/9b06dc6acdec
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
