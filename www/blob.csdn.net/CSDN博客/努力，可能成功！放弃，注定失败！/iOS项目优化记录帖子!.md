# iOS项目优化记录帖子! - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月19日 11:00:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：60
仅作为个人笔记!会持续更新!
### 1:开启Xcode启动时间
通过添加环境变量可以打印出APP的启动时间分析
```
Edit scheme -> Run -> Arguments  
DYLD_PRINT_STATISTICS设置为1
如果需要更详细的信息，那就将DYLD_PRINT_STATISTICS_DETAILS设置为1
```
![](https://upload-images.jianshu.io/upload_images/3633800-325db70c14afba15.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Snip20180724_1.png
项目启动的时候就会打印各种启动app是占用的时间
![](https://upload-images.jianshu.io/upload_images/3633800-ff68a4809a1d2728.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/930/format/webp)
Snip20180724_2.png
### 2:app项目瘦身
用到一个项目文件[LSUnusedResources](https://github.com/tinymind/LSUnusedResources)
在Mac上安装一下 选择项目目录 就可以开始查找没用到的项目
![](https://upload-images.jianshu.io/upload_images/3633800-16659ccc269ec379.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/653/format/webp)
Snip20180724_5.png
### 3:检查项目中不用的类文件 以及没用的代码
这里使用AppCode来检查文件
如果没有 点击下边去安装并且破解
[AppCode 安装](https://www.jianshu.com/p/75d1cb2b1919)
使用
```
打开菜单-code-选择Inspect Code
```
![](https://upload-images.jianshu.io/upload_images/3633800-3b8244c6d50e4611.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Snip20180724_8.png
在Inspection Results窗口中,
找到General => Unused import statement一项, 能找到相应的未使用的import内容. 一一删除即可.
找到General => Unused Code一项, 能找到相应的未使用的代码, 建议在审阅的基础上进行删除.
![](https://upload-images.jianshu.io/upload_images/3633800-6fb933b117676441.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Snip20180724_9.png
### 4:检查项目中文件占用的大小
生成LinkMap文件，可以查看可执行文件的具体组成
> 
生成linkmap文件
XCode -> Project -> Build Settings -> 把Write Link Map File选项设为yes，并指定好linkMap的存储位置
工程编译完成后，在编译目录里找到Link Map文件（txt类型) 默认的文件地址：~/Library/Developer/Xcode/DerivedData/XXX-xxxxxxxxxxxxx/Build/Intermediates/XXX.build/Debug-iphoneos/XXX.build/ \n\
![](https://upload-images.jianshu.io/upload_images/3633800-3c125316afe36904.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/946/format/webp)
Snip20180724_11.png
这里边的文件内容 我看不懂 需要借助一个工具
站在巨人的肩膀上分析问题
就可以看到哪个文件所占用的大小
在这里需要一个工具[LinkMap](https://github.com/huanxsd/LinkMap)来解析linkmap生成的文件
![](https://upload-images.jianshu.io/upload_images/3633800-1b75f519c000f6ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/931/format/webp)
Snip20180724_12.png
作者：因为太有钱
链接：https://www.jianshu.com/p/11727d1fde40
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
