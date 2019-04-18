# 基于Visual Studio 2015的SVN简单使用记录 - weixin_33985507的博客 - CSDN博客
2017年07月28日 18:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：62
### 1.首先安装visualsvn和tortoisesvn
下载地址：https://www.visualsvn.com/visualsvn/download/
我安装好visualsvn之后自动提示我没有tortoisesvn，然后自动选择安装了tortoisesvn。
![4264767-f4ddd55d338aa37b.png](https://upload-images.jianshu.io/upload_images/4264767-f4ddd55d338aa37b.png)
### 2.安装好之后，首先在本地建个仓库（比如E:\demo）
先建立路径：在E盘下新建demo文件夹，右键使用tortoisesvn  然后选择创建仓库。
![4264767-53a88629a430be4c.png](https://upload-images.jianshu.io/upload_images/4264767-53a88629a430be4c.png)
### 3.新建一个VS工程文件（我这里设置的工作路径是D盘目录下），然后添加进subversion，进行如下操作。
![4264767-46ab4b6616203c35.png](https://upload-images.jianshu.io/upload_images/4264767-46ab4b6616203c35.png)
![4264767-d6c0532d28cb89d8.png](https://upload-images.jianshu.io/upload_images/4264767-d6c0532d28cb89d8.png)
选择仓库
![4264767-df2e882da68c4fe8.png](https://upload-images.jianshu.io/upload_images/4264767-df2e882da68c4fe8.png)
输入仓库url地址。
![4264767-5b5391e978177d89.png](https://upload-images.jianshu.io/upload_images/4264767-5b5391e978177d89.png)
![4264767-31f4b1ec0e36130e.png](https://upload-images.jianshu.io/upload_images/4264767-31f4b1ec0e36130e.png)
![4264767-53591a398c360e61.png](https://upload-images.jianshu.io/upload_images/4264767-53591a398c360e61.png)
### 4.添加进subscribe之后，观察之后的界面，发现文件变色了
![4264767-13e9ac7d95a8e5f1.png](https://upload-images.jianshu.io/upload_images/4264767-13e9ac7d95a8e5f1.png)
### 5.选择commit提交版本，将这个版本保存。
![4264767-2ff55b5536d6952c.png](https://upload-images.jianshu.io/upload_images/4264767-2ff55b5536d6952c.png)
### 6.这里可以看到文件变绿了。
![4264767-cf8d37086d4b268c.png](https://upload-images.jianshu.io/upload_images/4264767-cf8d37086d4b268c.png)
要想生活过得去头上总得有点绿。
是时候祭出这张图了。
![4264767-3c19fef37cf81937.jpg](https://upload-images.jianshu.io/upload_images/4264767-3c19fef37cf81937.jpg)
### 7.随便修改代码：比如将main中的return 0改为return 1。
![4264767-72d2adfae323de45.png](https://upload-images.jianshu.io/upload_images/4264767-72d2adfae323de45.png)
### 8.这就是改变后的代码了，那么选择如何还原呢？选择如下图所示处。
![4264767-6798a7b8ed326bcd.png](https://upload-images.jianshu.io/upload_images/4264767-6798a7b8ed326bcd.png)
弹出如下窗口，选中文件，点ok。
![4264767-f5bf069712dc7517.png](https://upload-images.jianshu.io/upload_images/4264767-f5bf069712dc7517.png)
![4264767-adf27807071a9113.png](https://upload-images.jianshu.io/upload_images/4264767-adf27807071a9113.png)
点完确认之后就发现，代码恢复到了修改之前的版本。
![4264767-af6b4a14edb8f31c.png](https://upload-images.jianshu.io/upload_images/4264767-af6b4a14edb8f31c.png)
大致流程就是这个样子了~
