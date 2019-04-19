# unity3D与iOS原生工程项目合并以及合并过程中的问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月18日 14:50:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2574
本文来自简书，原文地址：[http://www.jianshu.com/p/f98bcfe09dc7](http://www.jianshu.com/p/f98bcfe09dc7)
由于公司项目的需要,需要将unity3D工程跟原生工程合并, 所以不得不接触unity3D, 本文是将unity3D导出的iOS代码移植到Xcode本地建的工程中, 上代码:
首先,总体来说,总的步骤分为两大块, 第一,你需要从unity3D中导出Xcode第二,你需要将导出的iOS代码部分移植到你想要的本地iOS项目中.
#### 第一部分, unity3D导出Xcode项目
1.打开你的unity工程,然后点击上边菜单栏file中的build settings
![](http://upload-images.jianshu.io/upload_images/2310982-0c1e06512358ab04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
2. 接着会弹出一个如下的框, 点击switch platform 选择iOS平台,
![](http://upload-images.jianshu.io/upload_images/2310982-75fabc84248e52b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3. 点击player settings... 配置参数如下图
![](http://upload-images.jianshu.io/upload_images/2310982-fa3f636331893eb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4. 配置完成后点击build, 等待完成,这个过程可能会时间长点,稍安勿躁, 完成之后是这样的
![](http://upload-images.jianshu.io/upload_images/2310982-4643fc3247cd2a66.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
5.然后打开文件iOS文件夹是这样的,我们需要的重要核心的代码都在画红线的这几个文件夹中,所以,我们下面就需要把这几个画红线的文件夹拷贝到我们需要的iOS原生项目中.
![](http://upload-images.jianshu.io/upload_images/2310982-f80b5e34a0c1b51f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第一部分导出Xcode工程已经完成,这部分很简单, 接着,我们就开始第二大部分,也是很容易出错的地方,尤其是配置需要注意.
#### 第二部分, unity3D导出Xcode项目导入到原生的iOS项目中
1.Unity导出的工程中的bundleId要和你本地iOS项目的id一致
2.修改bit code为NO
3.删除Main.storyboard,代码设置控制器(方便切换window)
4.将Classes,Libraries,MapFileParser.sh拖入到项目(选中Copy items if needed,选中Create groups)
将Data拖入到项目(选中Copy items if needed,选中Create folder references)
#### 注意:文件拖进工程的方式不完全一样,注意勾选的地方
![](http://upload-images.jianshu.io/upload_images/2310982-4b40b1ae5d32f067.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
5.添加framework（注意Optional的三个）
![](http://upload-images.jianshu.io/upload_images/2310982-5b76e0e09bbd29c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2310982-b9e815dfd2298035.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
6.添加Header Search Paths和Library Search Paths
![](http://upload-images.jianshu.io/upload_images/2310982-b76a611baf919456.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
7.other C Flags -> -DINIT_SCRIPTING_BACKEND=1
![](http://upload-images.jianshu.io/upload_images/2310982-1d29b2d59528db1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
8.添加User-Defined (UNITY_RUNTIME_VERSION版本号与导出工程的Unity版本号需要一致)
![](http://upload-images.jianshu.io/upload_images/2310982-0eb3f443dec87e08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
9.添加Run Script
![](http://upload-images.jianshu.io/upload_images/2310982-026ea6d32b554ce7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
10.把Unity/Classes/Prefix.pch的代码复制到新建的pct文件, 并把classes文件夹下的pct文件删除,注意,这里的删除并不是彻底的删除, 而是从列表中删除,选择中间这个, 以下步骤中所说的删除都是这个意思,请牢记哈!
![](http://upload-images.jianshu.io/upload_images/2310982-f3d694cf15a23126.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
11. 将Classes/main.mm全部内容复制到main.m 并把扩展名改为.mm
![](http://upload-images.jianshu.io/upload_images/2310982-2c487b97aa0bcb4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
12. 修改UnityAppController.h
![](http://upload-images.jianshu.io/upload_images/2310982-dfe20efceeab8fe2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
13.在AppDelegate.h .m中添加如下代码
![](http://upload-images.jianshu.io/upload_images/2310982-722d03afcf0b4c4a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2310982-794c65f86be3b615.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/2310982-8f2f85ae0264ccad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
到这里基本上已经完成从本地iOS跳转到Unity项目里了,接下来,是我在做的过程中遇到的问题.
1.Unknown type name 'UnityViewControllerBase'
解决办法就是把它改成UIViewController
2.iOS本地应用跳转unity的时候出现黑屏
原因是你的u3d工程集成了EasyAR插件，而这个插件下有个Assets/Plugins/iOS/EasyARAppController.mm，
@interface EasyARAppController : UnityAppController，这是整个app启动时的第一个controller，自己修改合并这个controller就OK了
或者是将EasyARAppController.mm的内容复制到你的自定义的子类中去，然后删除引用也是OK的
看下图
![](http://upload-images.jianshu.io/upload_images/2310982-b4436a3e8853abb6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在UnityAppController.mm里这个方法中写
![](http://upload-images.jianshu.io/upload_images/2310982-5ccd818963c34b8b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
然后就OK了
以上就是全部了呦,欢迎大家指正, 多多交流!!!
#### // 转载请注明原地址呦!
