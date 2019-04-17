# 测试前奏 之 dos字符页面执行 monkeyrunner 命令报错 - DEVELOPER - CSDN博客





2018年09月14日 12:50:45[学术袁](https://me.csdn.net/u012827205)阅读数：51
个人分类：[测试																[Android](https://blog.csdn.net/u012827205/article/category/1804657)](https://blog.csdn.net/u012827205/article/category/8057114)








借着中午休息的时间，记录下在使用**monkeyruuner命令**的时候的出错问题。
|电脑系统|执行窗口|执行命令|
|----|----|----|
|win10企业版|管理员身份dos字符窗口|monkeyrunner|

*展示下执行命令出错的样式*

![这里写图片描述](https://img-blog.csdn.net/20180914114128241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**错误内容也简单，但是也需要踩坑、填坑的过程。**

```
SWT folder `..\framework\x86_64` does not exist .
Please set ANDROID_SWT to point to the folder containing swt.jar for you platform .
```

报错的提示原因，在dos字符命令窗口执行**monkeyrunner命令**，在`sdk/tools目录`找不到`framework/x86_64目录`。那么处理问题的方式就是”供其所需”。 

接下来看步骤：

`1)`在**sdk/tools目录**下创建一个文件夹并命名为**framework**
![这里写图片描述](https://img-blog.csdn.net/20180914124526696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`2)`拷贝当前**sdk/tools目录**下的bin目录下的所有内容到**framework文件夹**中 
`3)`拷贝**sdk/platform-tool目录下**的**adb.exe**到**framework**文件夹中 
![这里写图片描述](https://img-blog.csdn.net/20180914124754135?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
到这里就已经配置操作完成，接下来重新执行**monkeyrunner命令**，成功！哦耶

![这里写图片描述](https://img-blog.csdn.net/20180914124900500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






