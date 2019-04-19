# create groups 与 create folder references选哪个？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:43:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：53
      当我们xcode开发中，如何正确添加文件等资源？
      可能很多人只是拖入文件资源，但是忘记勾选【create groups】这时候出现错误，找不到文件，或者.h头文件无法引用？
一：案例介绍演示
![](https://upload-images.jianshu.io/upload_images/2707674-053173eb3c243088.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/973/format/webp)
图：示范
**1、步骤提示：**
> 
1、拖入第三方文件，选项为【create folder references】时候，error报错显示，找不到引用的第三方文件【“ ‘XXXXX.h’ file not found”】。
2、删除此第三方（【move  to trash】完全删除，删除所有的引用）
3、重新拖入第三方文件，选项为【create groups】时候，success啦！
**2正确截图提示：**
![](https://upload-images.jianshu.io/upload_images/2707674-b41291db2cebadb2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
> 
所以：
**拖入新的文件等资源时候记得勾选：【Copy items if needed】和【Create Groups】**
二、浅析：【Create groups】与【create folder references】区别与联系？
> 
1、如果选择了Create groups方式添加了一个文件，我们会发现被添加进来的文件，文件夹是黄色的
2、选择了 Create folder references方式添加进来的文件的文件夹是蓝色的
如图：
![](https://upload-images.jianshu.io/upload_images/2707674-948f930405aca623.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/426/format/webp)
图：2者文件夹效果图
**三、深析【create groups】 与 【create folder reference】及【copy item if needed】区别与联系？**
**1.使用Create groups**
![](https://upload-images.jianshu.io/upload_images/2707674-861561d5c48a1ab1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/518/format/webp)
图1.使用Create groups
> 
为任何新增加的文件夹创建组，组的概念也许我们并不陌生，我们在项目中完全可以手动添加一个groups（右键点击选择New Group），但是手动添加的groups实际上并不会存在于项目的目录中，你会发现被添加进groups中的文件仍在位于它原来所在的位置，但是从外部引入进来的groups并不会如此，正如我们刚才那样。groups一旦被创建或添加，都是以黄色文件夹的形式存在的，当你想要使用文件夹中的某个类的头文件时，你可以直接添加它的引用，例如
**#include xxx.h**
因为groups下的cpp文件是会被编译的。
**所以：建议✔️使用【Create groups】**
**2.使用Create folder references方法**
![](https://upload-images.jianshu.io/upload_images/2707674-4bd9496a9fef6bb8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/582/format/webp)
图2.使用Create folder references方法
> 
    只是将文件单纯的创建了引用，这些文件不会被编译，所以在使用的时候需要加入其路径，比如在我们想要使用上图中的myScripts文件夹下面有某个头文件，则需要按照下面的方法添加声明：
**#include myScripts/xxx.h**
否则编译器就会告诉你找不到xxx.h文件。
这就是项目中黄色文件夹与蓝色文件夹的区别以及使用方法
**所以：不建议❌使用【Create folder references】**
**3、至于Destination对应【copy item if needed】 选项,**
![](https://upload-images.jianshu.io/upload_images/2707674-95920676b0d56f73.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554/format/webp)
图3、至于Destination对应【copy item if needed】 选项,
> 
1、当你选择了这个选项时，如果你想要引入的文件不在你的项目文件的目录下，Xcode则会自动将这个文件复制一份拷贝到你的项目的目录下
2、如果你不选择，并且这个文件也并不在你的项目目录下，那么这个文件仍然可以被使用，但是一旦你将项目拷贝到了别的地方时，这个文件就会找不到了。因为这个文件在你的电脑中，而不在你的项目中。
**所以：建议✔️使用【copy item if needed】**
### 四、最后总结：
**1、建议**
> 
**所以：建议✔️使用【Create groups】**
**所以：不建议❌使用【Create folder references】**
**所以：建议✔️使用【copy item if needed】**
**  2、 如图：**
![](https://upload-images.jianshu.io/upload_images/2707674-022b1ce20254f8d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
参考链接：[Create groups 与 Create folder references](https://link.jianshu.com?t=http://blog.csdn.net/yqmfly/article/details/45718157)
作者：iOS开发学习与分享zwj
链接：https://www.jianshu.com/p/7bdf90d9f966
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
