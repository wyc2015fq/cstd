# 做SLAM实验时g2o的安装问题cmake的版本问题及git下载历史版本代码方法总结 - 站在巨人的肩膀上coding - CSDN博客





2018年04月10日 11:47:23[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：782








博主之前跑了ORBSLAM2做了相关实验，半年没碰之后，又重新来弄发现，WTF，g2o用不了？？cmake版本过低无法编译？？直接修改CMakeLists.txt文件还不行？？

好吧，我屈服了，升级cmake吧。。。升级到了3.2爽歪歪腰不酸了腿也利索了。。。

注意：之前没搞清楚g2o的版本，用了最新版，事实证明，你只需要找到对应的老版本的g2o，下载安装，并不需要更新cmake的版本，更新cmake版本后会带来哪些影响，这个还没有验证过。不过可以降级回去（以后用的时候出现问题再说吧，感觉也没什么问题，一般是最低版本要求，高版本也没有什么负面影响）


### **怎么优雅的升级cmake请参考我的上一篇博客https://blog.csdn.net/love1055259415/article/details/79875113**

**好了，现在我们来说说g2o的问题。**

cd g2o

mkdir build

cd build

cmake..

make

sudo make install

![](https://img-blog.csdn.net/20180410105105238)


想着，这下应该可以了把，结果是未找到文件



# g2o_viewer 无法链接到 libqglviewer.so

原因：

在默认情况下，编译器只会使用/lib和/usr/lib这两个目录下的库文件，通过源码包进行安装时，如果不指定--prefix会将库安装在/usr/local目录下，而又没有在文件/etc/ld.so.conf中添加 /usr/local/lib这个目录。这样虽然安装了源码包，但是使用时仍然找不到相关的.so库，就会报错。也就是说系统不知道安装了源码包。

解决方法是在/etc/ld.so.conf.d/里新建g2o.config文件，在其中添加绝对路径/usr/local/lib；【在文件/etc/ld.so.conf中添加 /usr/local/lib这个目录也是可以的】

sudo gedit /etc/ld.so.conf.d/g2o.config

即在/etc/ld.so.conf.d/目录下添加一个g2o.config文件，把/usr/local/lib复制进去。然后，sudo ldconfig，就可以了。

因为博主在做这个实验https://www.cnblogs.com/gaoxiang12/p/5041142.html，所以，接下来再次回到octomap_tutor目录下，执行g2o_viewer data/result_after.g2o

![](https://img-blog.csdn.net/20180410105826511)


什么？？居然Segmentation fault (core dumped)。。。。




# g2o_viewer 运行 Segmentation fault

运行 g2o_viewer 会报错`Segmentation fault (core dumped)`，Google 找了很多都没有解决问题。最后还是回到 Github 这个 repo 的 issues 才找到答案。

链接 [https://github.com/RainerKuemmerle/g2o/issues/151](https://github.com/RainerKuemmerle/g2o/issues/151) 中说了是因为在 Ubuntu 14.04 下使用的是 Qt4，而当前的 g2o 版本使用的是 Qt5，所以 g2o_viewer 链接了 Qt 两个版本的动态链接库。

所以解决方案是找到原来使用 Qt4 的 g2o 源码进行编译安装。

看链接 [https://github.com/RainerKuemmerle/g2o/commits/master](https://github.com/RainerKuemmerle/g2o/commits/master) 知道在2017年1月13日有一个 commit 是“Switch to Qt5”，所以 git checkout 到之前版本就可以直接正常编译安装。

## **如何使用git checkout下载历史版本的代码**



> 
git上一个项目，都是经过多次迭代产生的，过程从简单到复杂，那么，想要用好git，也不能仅仅局限于自己的版本管理，你很可能需要去看懂别人的代码，这个时候就需要用到下载历史版本了，这个过程中，你能够看到很多别人实现功能的一个思路。

一个小贴士：善用git的搜索功能哟


1、首先找到g2o的位置：https://github.com/RainerKuemmerle/g2o



git clone 下载源码：git clone https://github.com/RainerKuemmerle/g2o.git　


2、点击905commits

![](https://img-blog.csdn.net/20180410110838784)


3、然后就看到历史版本

![](https://img-blog.csdn.net/20180410110939799)

4、下载历史版本代码，使用git checkout命令，找到你想要的历史版本的提交时间及位置

注意到在2017.1.13日提交的版放弃了对qt4的支持，所以下载之前的版本



dbfea7aade1b808b4a7625175aa0dec54931a949dbfea7aade1b808b4a7625175aa0dec54931a949![](https://img-blog.csdn.net/20180410111153720)

5、复制SHA（右边的带箭头的标志点一下）

6、git checkout -b  dbfea7aade1b808b4a7625175aa0dec54931a949

或者可以通过git log查看历史修改的SHA，找到对应的SHA，然后git checkout SHA

![](https://img-blog.csdn.net/20180410112830399)


按q退出


通过git tag查看历史主要修改

![](https://img-blog.csdn.net/20180410112945840)






dbfea7aade1b808b4a7625175aa0dec54931adbfea7aade1b808b4a7625175aa0dec54931a949dbfea7aade1b808b4a7625175aa0dec54931a949
<刚复制的SHA>

然后查看你的文件夹，已经更改为历史版本了

# 删除之前编译过安装过的版本

（1）删除/usr/local/include/g2o，指令为sudo rm -rf /usr/local/include/g2o

（2）删除/usr/local/lib下有关libg2o_*.so的库文件，先进入目录cd /usr/local/lib，然后挨个（可多个同时）删除sudo rm -rf libg2o_*

# **编译下载的历史版本**

回到之前的下载的g2o文件夹：

cd g2o

mkdir build&&cd build

cmake ..

make 


sudo make install

![](https://img-blog.csdn.net/20180410113902859)

接下来，sudo gedit /etc/ld.so.conf.d/g2o.config


即在/etc/ld.so.conf.d/目录下添加一个g2o.config文件，把/usr/local/lib复制进去。

然后，sudo ldconfig更新动态链接库，就可以了。

执行g2o_viewer查看是否正常启动

![](https://img-blog.csdn.net/20180410114301137)


大功告成！！！


# **参考链接：以下链接可能解决你的问题**

一起做的解决g2o的问题：https://www.cnblogs.com/gaoxiang12/p/4770813.html     


git checkout:https://segmentfault.com/a/1190000010771974

g2o core dumped:https://blog.csdn.net/u011306452/article/details/54943956









*************************************我是萌萌哒分割线******************************************




今天在用的时候发现，还是出现段错误的情况。不能加载g2o文件。重新在高翔的github上下载了g2o版本https://github.com/gaoxiang12/rgbd-slam-tutorial-gx

make时候出现错误

![](https://img-blog.csdn.net/20180410150126272)

解决办法为：

libqglviewer 安装 
[http://libqglviewer.com/installUnix.html](http://libqglviewer.com/installUnix.html)


我之前不是装过这个吗？怎么。。仔细检查一下

由于libqglviewer库与编译源文件不匹配，好吧，参考libqglviewer [http://libqglviewer.com/installUnix.html](http://libqglviewer.com/installUnix.html)安装一下吧。

最后出现的问题是



# error: No rule to make target libqglviewer.so
`No rule to make target /usr/lib/libqglviewer.so, needed by ../lib/libg2o_viewer.so`
到这里下载g2o，里面一个是我编译好的一个是源码包，可以用源码包来安装，下载地址https://download.csdn.net/download/love1055259415/10339455



cd g2o

mkdir build

cd build

cmake ..

make


![](https://img-blog.csdn.net/20180410174212005)


sudo make install

![](https://img-blog.csdn.net/20180410174403572)


参考链接

https://blog.csdn.net/yangziluomu/article/details/72622225










