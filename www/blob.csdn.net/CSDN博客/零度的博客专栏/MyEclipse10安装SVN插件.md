# MyEclipse10安装SVN插件 - 零度的博客专栏 - CSDN博客
2016年05月17日 11:42:02[零度anngle](https://me.csdn.net/zmx729618)阅读数：297
个人分类：[Eclipse/MyEclipse](https://blog.csdn.net/zmx729618/article/category/6220621)
## 一、下载SVN插件subclipse
下载地址：[http://subclipse.tigris.org/servlets/ProjectDocumentList?folderID=2240](http://subclipse.tigris.org/servlets/ProjectDocumentList?folderID=2240)
在打开的网站中找到subclipse的最新版本，如下图所示：
![](http://images.cnitblog.com/blog/289233/201312/29234725-57ebeeb0b06244f2aab27b771cb82b76.png)
下载完成之后，得到的是一个压缩包文件，如下图所示：
![](http://images.cnitblog.com/blog/289233/201312/29234904-be1f490bcb7f4e9ea5fec3280ba4da20.png)
## 二、在MyEclipse10中安装SVN插件subclipse
步骤如下：
1.找到MyEclipse10 在安装目录下的dropins文件夹
![](http://images.cnitblog.com/blog/289233/201312/29235011-8da8fb35a29e4e4880d0a8f9c05ff882.png)
2.打开dropins文件夹，新建一个svn文件夹
![](http://images.cnitblog.com/blog/289233/201312/29235105-b7798331f78245b6aa7c94ee98db84d7.png)
3.解压缩下载好的![](http://images.cnitblog.com/blog/289233/201312/29234904-be1f490bcb7f4e9ea5fec3280ba4da20.png)压缩包
![](http://images.cnitblog.com/blog/289233/201312/29235217-469d90d363304aed9dd854f0a58418b8.png)
解压好之后得到一个![](http://images.cnitblog.com/blog/289233/201312/29235316-5b4ce7c271d64a94bd107da723d75a25.png)文件夹，打开文件夹可以看到里面有如下文件
![](http://images.cnitblog.com/blog/289233/201312/29235401-7972bfad665f43279798b4d8a2f92734.png)
把features和plugins文件夹拷贝到dropins文件夹下的SVN文件夹中
![](http://images.cnitblog.com/blog/289233/201312/29235501-4a72cee52fb048f0ba37dbe522f3f59a.png)
![](http://images.cnitblog.com/blog/289233/201312/29235543-1b23b262e7f844329acc50bf91b109a2.png)
这样就可以在MyEclipse10中安装好SVN插件了。
## 三、在MyEclipse10中使用SVN插件
安装好SVN插件后，启动MyEclipse，这时候启动会比较慢，因为MyEclipse要去下载一些相关连的Jar文件，当进入到MyEclipse启动完之后进入到workspace时，就会自动弹出相关联文件已经自动下载完成的提示对话框，如下图所示
![](http://images.cnitblog.com/blog/289233/201312/29235709-5accc25fb4f24931ba4c7a4d94505bbe.png)
### 3.1查看安装好的SVN
点击菜单上的【Window】----->【Show View】------>【Other…】，如下图所示：
![](http://images.cnitblog.com/blog/289233/201312/29235805-2e002af958434b2ba9ad2eb04d92ce63.png)
在弹出的【Show View】对话框中就可以看到已经安装好的SVN插件，如下图所示：
![](http://images.cnitblog.com/blog/289233/201312/29235911-d7da438da7484b84abfaad9df28067db.png)![](http://images.cnitblog.com/blog/289233/201312/29235950-43b42e8de6dc4b9d9dc5bc8364165805.png)
![](http://images.cnitblog.com/blog/289233/201312/30000033-66c9451955c74228bf47841ae79c9755.png)
![](http://images.cnitblog.com/blog/289233/201312/30000133-610a26d9568e4f75a9cb107e515c7152.png)
### 3.2在MyEclipse中通过SVN客户端连接SVN服务器
![](http://images.cnitblog.com/blog/289233/201312/30000228-664e160a41384da7b9feac2c9a4237c2.png)
此时将弹出如下画面
![](http://images.cnitblog.com/blog/289233/201312/30000306-59ee607ccec74f9a944928c70b7bfe7c.png)
点击【Finish】，然后弹出一个要求输入用户名和密码的对话框，输入正确的用户名和密码就可以连接到指定的SVN服务器了，如下图所示：
![](http://images.cnitblog.com/blog/289233/201312/30000349-bb2c15b896e44636aafc89be5dba0044.png)
