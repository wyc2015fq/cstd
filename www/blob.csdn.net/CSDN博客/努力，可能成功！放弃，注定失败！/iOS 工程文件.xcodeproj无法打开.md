# iOS 工程文件.xcodeproj无法打开 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:40:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：146
![](https://upload-images.jianshu.io/upload_images/1708447-0de3137193c5d1b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400/format/webp)
image.png
Bug回放：
> 
本小猿刚提交了一些图片，继续激情的敲代码中，然后另一小媛一脸怀笑的出现在背后，“我刚提交了一些图片，你更新一下”，“好的”，then。。。
![](https://upload-images.jianshu.io/upload_images/1708447-56dca5c3b8f8cdd0.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/150/format/webp)
纳尼？
![](https://upload-images.jianshu.io/upload_images/1708447-e2b788586b7e8b2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/256/format/webp)
workspace中主工程文件GovCn目录没了
> 
吓得我赶紧打开主GovCn.xcodeproj文件，结果。。。
![](https://upload-images.jianshu.io/upload_images/1708447-c44aa93d482495ae.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/150/format/webp)
惊吓
![](https://upload-images.jianshu.io/upload_images/1708447-4fdc9de2838a2d0e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/442/format/webp)
GovCn.xcodeproj损坏了
> 
翻译：项目“GOVCN”损坏，由于解析错误无法打开。检查项目文件是否有无效编辑或未解决的源代码冲突。
### 暂停Bug
> 
知识点：Xcode的工程文件是 工程名.xcodeproj，而它其实是个package目录，通过显示包内容，可以查看到它内部主要有project.pbxproj 和 xcuserdata。其中，xcuserdata 一般是跟用户相关的一些设置，如断点 记录等，一般不用放到版本管理中。而project.pbxproj 是工程描述文件，描述了工程里的源码文件、schema设置等。它的格式是文本类型的plist(Info.plist是binary plist)，里面是一个一个的object。 ------- 摘自[iOS 开发 xcode中的project.pbxproj--深入剖析](https://blog.csdn.net/kuangdacaikuang/article/details/52987132)
![](https://upload-images.jianshu.io/upload_images/1708447-e0fed3d938d5f0c5.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/452/format/webp)
容我冷静一下
> - 冷静想了一下，应该是因为我俩同时提交了图片，工程 [GovCn.xcodeproj](https://www.jianshu.com/p/dd58925dfa5d) 文件冲突了，然后还是会强制更新，内部配置文件 [project.pbxproj](https://www.jianshu.com/p/e82ec6a56fc2) 出现了冲突，然后就解析不了了。这时，如果是svn就会强制在 [project.pbxproj](https://www.jianshu.com/p/e82ec6a56fc2) 文件中给你加上<<<<< .mine ...... ======= ...... >>>>> .r33299 来区分谁谁做的修改，只要把这个文件用编辑器打开，把这些<>标记的冲突内容都删除，然后工程就能正常打开显示了。打开之后，工程里关于图片的配置信息都没了，但图片源文件还存在源目录下，重新导入一下就OK了。
### 修理Bug
![](https://upload-images.jianshu.io/upload_images/1708447-99d14c36090e89b4.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/360/format/webp)
> 
好了，开始修理Bug，除了修改 [project.pbxproj](https://www.jianshu.com/p/e82ec6a56fc2) ，我们还可以粗暴的用上一个版本的project.pbxproj直接把它覆盖掉，太粗暴了O(∩_∩)O
![](https://upload-images.jianshu.io/upload_images/1708447-8fec81b31311123e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/497/format/webp)
显示包内容，找到project.pbxproj
![](https://upload-images.jianshu.io/upload_images/1708447-8ce99f493fc7f4dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/809/format/webp)
删除冲突标记内容
![](https://upload-images.jianshu.io/upload_images/1708447-e223e0a9e21e9e0a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/300/format/webp)
赞赞.gif
感谢以下大佬：
[https://www.jianshu.com/p/e82ec6a56fc2](https://www.jianshu.com/p/e82ec6a56fc2)
[https://blog.csdn.net/kuangdacaikuang/article/details/52987132](https://blog.csdn.net/kuangdacaikuang/article/details/52987132)
[https://www.jianshu.com/p/dd58925dfa5d![sabo.png]](https://www.jianshu.com/p/dd58925dfa5d!%5Bsabo.png%5D)
作者：且行且珍惜_iOS
链接：https://www.jianshu.com/p/a0762b67e657
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
