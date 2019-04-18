# windows中jupyter（原ipython notebook）中文件工作路径的设置 - fighting！！！ - CSDN博客
2019年02月24日 13:53:50[dujiahei](https://me.csdn.net/dujiahei)阅读数：40
[https://www.zhihu.com/question/31600197/answer/90214029](https://link.jianshu.com/?t=https%3A%2F%2Fwww.zhihu.com%2Fquestion%2F31600197%2Fanswer%2F90214029)
作者：乌匠
链接：https://www.jianshu.com/p/67aa3a68cb69
楼主应该用的是最新版的 jupyter notebook
根据官方文档的说明最近版的ipython 和 jupyter 的配置文件是分开的，
因此要分别配置文件
![](https://upload-images.jianshu.io/upload_images/4316111-1e85bed987948b98.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)
image
打开 cmd 输入命令 jupyter notebook --generate-config
![](https://upload-images.jianshu.io/upload_images/4316111-f8b7d6464b3b4e6c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/635/format/webp)
image
可以看到生成文件的路径
![](https://upload-images.jianshu.io/upload_images/4316111-d7d54a7f0997e443.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/593/format/webp)
image
如上图可以看到，这个目录
![](https://upload-images.jianshu.io/upload_images/4316111-dbd7dfd132d90724.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/630/format/webp)
image
这个就是生成的配置文件，打开后找到 这个地方 删除前面的’#‘ 然后填写自己的路径，保存
![](https://upload-images.jianshu.io/upload_images/4316111-5ad93489b8adf4f6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/531/format/webp)
image
启动 jupyter notebook
![](https://upload-images.jianshu.io/upload_images/4316111-a13f09fa6e89ce5a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/720/format/webp)
