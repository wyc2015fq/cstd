# ubuntu安装google test - xcw0754 - 博客园
# [ubuntu安装google test](https://www.cnblogs.com/xcw0754/p/8594946.html)
google test 简称gtest，是一个C/C++的单元测试框架，它的代码在[github仓库](https://github.com/google/googletest)，使用起来还是挺方便的。
### 安装
先确保PC上有安装cmake：
`sudo cmake --version`
如果没有安装cmake，可以安装一下：
`sudo apt-get install cmake`
我的机子14.04安装的是2.8.12.2版本的cmake，注意记一下你的版本号，下面有用。
接下来下载gtest源码包：
`sudo apt-get install libgtest-dev`
下载的只是源码包，切换到`/usr/src/gtest`后用vim查看需要的cmake版本：
`sudo vim CMakeLists.txt`
里面有项`cmake_minimum_required(VERSION 2.6.x)`，只要cmake版本号更高就行了，接着运行：
`sudo cmake CMakeLists.txt`
可以开始用make一下了：
`sudo make`
完成后产生了两个`*.a`库文件，就可以直接用了(拷到你项目里边去)，也可以放在`/usr/local/lib`里。至于静态库文件`*.a`怎么用，请自行百度。能否用动态库呢？答案是可以的，但是需要高版本的cmake，从github上拉代码再编译，主要是弄个高版本cmake太麻烦。

