# 使用Cygwin在Windows下取得Android源代码 - xqhrs232的专栏 - CSDN博客
2012年04月25日 10:39:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1078
原文地址::[http://dev.10086.cn/cmdn/wiki/index.php?doc-view-3681.html](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-3681.html)
相关网帖
1.在cygwin下设置repo和同步----[http://yandonghua.blogspot.com/2010/08/cygwinrepo.html](http://yandonghua.blogspot.com/2010/08/cygwinrepo.html)
2.[使用Repo在Windows平台下載Android原始碼](http://blog.csdn.net/zhenwenxian/article/details/5780412)----[http://blog.csdn.net/zhenwenxian/article/details/5780412](http://blog.csdn.net/zhenwenxian/article/details/5780412)
3.[**Cygwin下获取Android源代码**](http://www.cnblogs.com/watsonlong/archive/2011/05/16/2047991.html) ----[http://www.cnblogs.com/watsonlong/archive/2011/05/16/2047991.html](http://www.cnblogs.com/watsonlong/archive/2011/05/16/2047991.html)
1、**准备Cygwin环境** ，其中要有curl、wget、python等基本工具。
2、**准备源码存放目录** (如：c:\myeclair)，进入Cygwin Shell环境，执行以下命令：
[Java](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1306)代码 
- // 改变目录位置到 C:
- cd /cygdrive/c/  
- 
- // 创建存放源代码的目录、及repo命令存放的目录
- mkdir myeclair myeclair/bin  
- 
- //进入源代码存放目录
- cd myeclair  
3、**安装repo工具** ，repo是python语言编写的用于获取[Android](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1789)源代码的命令行工具，执行以下命令进行安装：
Java代码 
- // 将repo文件保存到 bin目录下
- curl http://android.git.kernel.org/repo >bin/repo
- 
- //修改repo文件为可执行文件
- chmod a+x bin/repo  
打开repo文件，并修改首行内容 ：
  把其中REPO_URL=**git://**修改成 **http://**(这主要是因为git://协议不能穿过代理服务/防火墙)。
4、**初始化版本****库** ，执行repo命令完成版本库初始化：
Java代码 
- // 注意:此处和官方文档所述方法的不同点在于，git://  --->  http://
- // 我们加上了eclair标记
- bin/repo init -u http://android.git.kernel.org/platform/manifest.git -b eclair
5、**同步版本****库** ，即真正开始源代码的下载。首先修改 **.repo/manifest.xml**文件，将其中的 **git://**修改成 **http://**。然后再执行以下命令：
Java代码 
- // 同步版本库
- bin/repo sync  
视网络情况而定，也许你会经过一个漫长的等待，但最终总会获得了eclair的完整源代码。
