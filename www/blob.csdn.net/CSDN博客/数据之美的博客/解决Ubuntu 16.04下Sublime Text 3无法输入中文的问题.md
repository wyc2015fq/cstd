# 解决Ubuntu 16.04下Sublime Text 3无法输入中文的问题 - 数据之美的博客 - CSDN博客
2018年01月07日 17:34:58[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：3480
## 前言
sublime很好用，但是ubuntu下不能输入中文，这是一个很大的问题。不知道为什么开发着一直也不解决，好在还是有高手在，总能找到方法。网上方法很多，但是也很乱，现在我将自己的经验总结一下。
首先，我参考了好几篇文章，都是蛮不错的，先列出来：
- [sublime-text-imfix](https://link.jianshu.com?t=https://github.com/lyfeyaj/sublime-text-imfix):首先推荐这个方法，最简单，但是在我的系统上有些问题。**可用这个的强烈推荐用这个**
- [完美解决 Linux 下 Sublime Text 中文输入](https://link.jianshu.com?t=https://www.sinosky.org/linux-sublime-text-fcitx.html)：讲的比较明白，也可用参考。
- [Ubuntu下Sublime Text 3解决无法输入中文的方法](https://link.jianshu.com?t=http://jingyan.baidu.com/article/f3ad7d0ff8731609c3345b3b.html): 讲解的最清楚了，建议看看。
## 我的方法
1.下载我们需要的文件，打开终端，输入：
```bash
git clone https://github.com/lyfeyaj/sublime-text-imfix.git
```
2.将`subl`移动到`/usr/bin/`，并且将`sublime-imfix.so`移动到`/opt/sublime_text/`（sublime的安装目录）
终端输入：
```bash
cd ~/sublime-text-imfix
sudo cp ./lib/libsublime-imfix.so /opt/sublime_text/
sudo cp ./src/subl /usr/bin/
```
3.用`subl`命令试试能不能启动sublime，如果成功启动的话，应该就可以输入中文了。
终端输入：
```
LD_PRELOAD=./libsublime-imfix.so subl
```
4.但是这样子只能用上述命令行启动才能输入中文，我们需要更简单的办法，我选择新建一个shell脚本，很简单，就一句话。
sublime新建文件输入：
```bash
#!/bin/bash
LD_PRELOAD=/opt/sublime_text/libsublime-imfix.so subl
```
然后将文件另存为`sublime`（注意：没有后缀），并且放到`/home`文件夹。
5.至此，所有工作完成了。想要启动，直接终端输入：`bash ~/sublime`，就可以启动sublime，并且能够输入中文。
6.如果不需要输入中文，直接按以前的方式打开即可。建脚本文件的目的就是方便打开能输入中文的sublime。
## 说明
这只是一个权宜之计，完美的解决办法还没有，只能等sublime开发者更新。
网上所谓的完美解决，还是有问题，比如如果更改桌面快捷方式，那么sublime的`Browse Packages`功能不能使用，当然，还有其他问题。为了正常使用，我就没改。当然如果你中文输入比较多，还是建议修改桌面快捷方式，毕竟那会方便很多，具体怎么样，自己取舍。
另外，输入法不跟框，暂时无解啊。
作者：Vaayne
链接：https://www.jianshu.com/p/bf05fb3a4709
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
