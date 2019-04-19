# python2 python3，window共存_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
error  ：  Fatal error in launcher: Unable to create
process using '"'
先安装python2，，在安装python3，
每次安装都让程序设置环境变量。。
这样输入python，默认选择的是python2。因为两个路径在path里面都有，不过先搜索到python2
的路径。也有可能反过来，这个顺序不重要。
这里假设输入python调用的是python2。如果调用的是python3，那么下面的操作反过来就行。
找到python3的安装目录，把python.exe，重新命名为python3.exe。。
这样你以后使用python2就命令行直接输入python，如果你使用python3，就命令行输入python3。
你命令行输入pip默认调用python2的pip，，你输入pip3可以直接调用python3的pip。
但是有个问题，你输入pip3，会报下面的错误：
Fatal error in launcher: Unable to create process using
'"'
这个问题是因为我们之前修改了python.exe为python3.exe。
解决方法也很简单。复制两个python3.exe即可，一个命名为python3.exe，一个为python.exe。
这样pip3就可以正常运行了。

