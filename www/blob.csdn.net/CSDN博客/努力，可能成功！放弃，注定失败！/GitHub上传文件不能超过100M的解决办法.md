# GitHub上传文件不能超过100M的解决办法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月15日 13:22:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：293
![](https://upload-images.jianshu.io/upload_images/4770884-6307057853f1a392.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
一直卡着
      上传项目到GitHub上，当某个文件大小超过100M时，就会上传失败，因为默认的限制了上传文件大小不能超过100M。如果需要上传超过100M的文件，就需要我们自己去修改配置。
首先，打开终端，进入项目所在的文件夹；
输入命令：**git config http.postBuffer 524288000**
之前git中的配置是没有这一项的,执行完以上语句后输入：**git config -l**可以看到配置项的最下面多出了一行我们刚刚配置的内容. (52428000=500×1024×1024,即500M)
![](https://upload-images.jianshu.io/upload_images/4770884-6c051e1dcc197b96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/569)
作者：honey缘木鱼
链接：https://www.jianshu.com/p/1c243e39b323
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
