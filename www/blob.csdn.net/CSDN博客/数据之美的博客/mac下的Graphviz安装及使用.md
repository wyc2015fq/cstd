# mac下的Graphviz安装及使用 - 数据之美的博客 - CSDN博客
2018年05月07日 09:37:55[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：236
个人分类：[计算机系统](https://blog.csdn.net/oppo62258801/article/category/7154103)
一.安装 
Graphviz [http://www.graphviz.org/](http://www.graphviz.org/)
mac用户建议直接用homebrew来安装，官网上版本比较旧
1.安装homebrew 
打开终端复制、粘贴以下命令：
```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
- 1
2.安装Graphviz 
homebrew安装完毕后运行 brew install graphviz即可
注：运行第2步时可能会提示-bash: brew: command not found 
解决办法： 
终端输入`sudo vim .bash_profile`， 
输入`i`进入输入模式， 
添加`export PATH="/usr/local/bin:$PATH"` , 
按esc键退出输入模式， 
输入`:wq`退出并保存 
最后输入`source .bash_profile`使配置生效。 
（注意`:` 和空格也要输入，bash_profile保存时可能会警告，`:w!`强制保存即可，更多vim命令参考[http://www.cnblogs.com/usergaojie/p/4583796.html](http://www.cnblogs.com/usergaojie/p/4583796.html)）
二.使用 
随便找的一个例子： 
1.使用文本编辑器（这里用的是sublime ）输入以下内容：
```
digraph pic { 
  Hello -> World
}
```
- 1
- 2
- 3
2.保存为pic.dot文件 
3.打开终端，切换到dot文件所在目录，输入`dot pic.dot -T png -o pic.png`
4.可以看到在dot文件目录下生成了一个pic.png文件 
![这里写图片描述](https://img-blog.csdn.net/20171013133022687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY4NDc2NDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三.进阶 
目的：流程图较复杂时，能够边写代码边看效果 
目前找了两个教程，还没试，有问题了再更新。 
[https://blog.zengrong.net/post/2294.html](https://blog.zengrong.net/post/2294.html)
[https://zhuanlan.zhihu.com/p/22820399](https://zhuanlan.zhihu.com/p/22820399)
