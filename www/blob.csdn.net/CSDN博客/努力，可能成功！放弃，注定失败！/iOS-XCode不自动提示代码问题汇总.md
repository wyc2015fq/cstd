# iOS-XCode不自动提示代码问题汇总 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月14日 13:29:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：274
1.代码提示功能失效
> 
解决方法：
在Xcode->Window->Projects选中你的项目，点击Derived Data右侧的Delete按钮
![](https://upload-images.jianshu.io/upload_images/3069532-84311c76a2e91cff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
> 
用命令行解决，效果同上方
- cd到Xcode目录下：Xcode~/Library/Developer/Xcode/DerivedData
- ls（显示当前路径下的文件）
- 找到你项目名开头的目录
- cd 目录名
- rm -r Index 删除掉你的项目所用的索引文件夹
如果上面两种方法都不管用
> 
1.找到DerivedData 文件夹 删除 (路径: ~/Library/Developer/Xcode/DerivedData)
2.删除com.apple.dt.Xcode 文件 (路径: ~/Library/Caches/com.apple.dt.Xcode)
3.如果不让删除，就退出Xcode，重启电脑即可
4.运行code就可以了
2.使用cocoapods导入第三方库，使用import不提示第三方库的头文件
> 
解决方法：
选择target -> BuildSettings -> search Paths 下的 User Header Search Paths, 添加 $(PODS_ROOT)，并设置为“recursive”
![](https://upload-images.jianshu.io/upload_images/3069532-e16b97ab17fc9e44.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
3.Xcode7.3 添加自己创建的类，import类名不提示
> 
解决方法：
buildingSetting —> 将Weak References in Manual Retain Release 设置为Yes
![](https://upload-images.jianshu.io/upload_images/3069532-10b31c4a9a78e8e4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
作者：iOS安年
链接：https://www.jianshu.com/p/dbf38fb0a18b
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
