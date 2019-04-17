# 如何优雅的升级自己Ubuntu的CMake - 站在巨人的肩膀上coding - CSDN博客





2018年04月10日 09:56:02[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：6341








有时候在编译东西的时候，会发现自己的CMake版本太低，比如下面这种情况：

![](https://img-blog.csdn.net/20180410005915925)

然后就上网搜各种教程，搜到：sudo apt-get autoremove cmake，然后得到这种情况：

![](https://img-blog.csdn.net/20180410010026543)

你如果不细看的话，就将你之前编译过的ROS等其他包都删掉了。。。细思极恐。。。。万一看都不看直接y了，可有的你后悔。。。

好了，那我们来说说，如何在线直接升级CMake的版本



1、Installation by a PPA (Upgrade to 3.2)

```
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
```
- 
如果之前没有装CMake，执行

```
sudo apt-get install cmake
```

- 
如果之前已经装过CMake，执行（这步非常要时间，可以在吃饭或者睡觉的点来做，会更新之前cmake编译过的一些东西，自动重新安装或者升级）

`sudo apt-get upgrade`

再次查看cmake的版本:cmake --version

![](https://img-blog.csdn.net/20180410095513900)

大功告成！！！


参考链接：https://askubuntu.com/questions/610291/how-to-install-cmake-3-2-on-ubuntu







