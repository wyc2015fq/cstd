
# linux命令行在任意目录下启动任意的脚本的方法 - Magician的博客 - CSDN博客


2018年08月25日 23:37:24[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：2802


[目录：](#目录)[前言：](#前言)
[1、直接在命令行中设置PATH](#1直接在命令行中设置path)
[2、在profile中设置PATH](#2在profile中设置path)
[3、在当前用户的profile中设置PATH](#3在当前用户的profile中设置path)


# 目录：
## 前言：
这应该算是一个常识吧，但是对于许多像我们这样的新手来说，一旦你出点小差错，整个过程就会很曲折。碰巧今天就遇到了这样的问题。
下面是查过资料后总结的解决方法：
简单说PATH就是一组路径的字符串变量，当你输入的命令不带任何路径时，LINUX会在PATH记录的路径中查找该命令。有的话则执行，**不存在则提示命令找不到**，也就是我们经常看到的`-bash: ***: command not found`。比如在根目录/下可以输入命令ls,在/usr目录下也可以输入ls,但其实ls命令根本不在这个两个目录下，当你输入ls命令时LINUX会去/bin,/usr/bin,/sbin等目录寻找该命令。而PATH就是定义/bin:/sbin:/usr/bin等这些路劲的变量，其中冒号为目录间的分割符。使用`export $PATH`命令可以查看环境变量的内容。
如何自定义路径：
假设你新编译安装了一个apache在/usr/local/apache下，你希望每次启动的时候不用敲一大串字符（\# /usr/local/apache/bin/apachectl start）才能使用它，而是直接像ls一样在任何地方都直接输入类似这样（\# apachectl start）的简短命令。这时，你就需要修改环境变量PATH了，准确的说就是给PATH增加一个值/usr/local/apache/bin。将/usr/local/apache/bin添加到PATH中有三种方法：
## 1、直接在命令行中设置PATH
```python
# PATH=$PATH:/usr/local/apache/bin
```
使用这种方法,只对当前会话有效，也就是说每当登出或注销系统以后，PATH设置就会失效。
## 2、在profile中设置PATH
```python
# vi /etc/profile
```
找到export行，在下面新增加一行，内容为：
```python
export
```
```python
PATH
```
```python
=
```
```python
$PATH
```
```python
:/usr/local/apache/bin
```
```python
。
```
**注：＝ 等号两边不能有任何空格。这种方法最好,除非手动强制修改PATH的值,否则将不会被改变。**
编辑/etc/profile后PATH的修改不会立马生效，如果需要立即生效的话，可以执行\#`source profile`命令。
其次对于profile来说一定要注意整个编辑的过程，一旦你输入一些格式不正确的文本（如把windows编码的文本复制过来），就会造成profile文件无法正常解析等错误。
## 3、在当前用户的profile中设置PATH
```python
vi ~/
```
```python
.bash
```
```python
_profile
```
修改PATH行,把/usr/local/apache/bin添加进去,如：
```python
PATH=
```
```python
$PATH
```
```python
:
```
```python
$HOME
```
```python
/bin:/usr/local/apache/bin。
```
```python
# source ~/.bash_profile
```
让这次的修改生效。
注：这种方法只对当前用户起作用的,其他用户该修改无效。
去除自定义路径：
当你发现新增路径/usr/local/apache/bin没用或不需要时，你可以在以前修改的/etc/profile或~/.bash_profile文件中删除你曾今自定义的路径。
除此之外，打开prifile文件，你会发现内部逻辑是先对几个路径下的变量进行检索如/bin:/sbin:/usr/bin，如下图所示
![这里写图片描述](https://img-blog.csdn.net/20180825220330939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180825220330939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是一旦你在编辑profile文件的时候，一不小心，你就会发现很多“可怕”的错误，比如linux的基本指令都用不了等等。所以对于有些你已经配置了Path变量，执行相关命令却未生效时，你就需要留意下，是不是你的profile文件出了问题，今天就莫名中招了，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180825234803171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180825234803171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在PATH下声明了HIVE，但是最后输出环境变量的时候却发现没有包含对应的Hive的地址，导致最终执行hive命令时无效，最后将Hive单独声明出来问题就解决了，我也是很郁闷这是什么情况…
[
](https://img-blog.csdn.net/20180825234803171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)参考：[https://www.cnblogs.com/leibg/p/4479921.html](https://www.cnblogs.com/leibg/p/4479921.html)
[            ](https://img-blog.csdn.net/20180825234803171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
  ](https://img-blog.csdn.net/20180825220330939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)