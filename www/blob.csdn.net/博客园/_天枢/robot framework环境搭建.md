# robot framework环境搭建 - _天枢 - 博客园
## [robot framework环境搭建](https://www.cnblogs.com/yhleng/p/8807703.html)
2018-04-12 14:51 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8807703)
**Robot FrameWork环境搭建:**
Robot Framework是一款python编写的功能自动化测试框架。具备良好的可扩展性，支持关键字驱动，可以同时测试多种类型的客户端或者接口，可以进行分布式测试执行。主要用于轮次很多的验收测试和验收测试驱动开发（ATDD）。
**环境支持:**
1.robotframework框架
2.robotframeword-ride框架界面
3.wxpython这是一个gui库,robotframeword-ride基于这个库开发的.
4.robotframework-selenium2library这个是selenium库
5.安装selenium
1.安装robot framework命令:**pip install robotframework**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180412142658476-202601337.png)
2.安装robot framework-ride命令:**pip install robotframework-ride**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180412143241445-1521295226.png)
3.WXpython
[下载地址1](https://sourceforge.net/projects/wxpython/files/wxPython/2.8.12.1/)
[下载地址2](https://pypi.python.org/pypi/wxPython)
 地址2下载注意事项,正确选择,python版本,与对应操作系统版本.
whl格式,是一下压缩包,安装方法:
cmd命令行下,进行到你压缩包所在的目录,然后执行以下命令进行安装.
命令:**pip install wxpython-4.0.1-cp27-cp27m-win_amd64.whl**
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180412150929128-241512771.png)
安装完成:
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180412151233047-892357883.png)
4.安装robotframework-selenium2library:命令**pip install robotframework-selenium2library**
**![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180412143817216-949774985.png)**
5.安装selenium命令:**pip install selenium**
**[小结]:**
**以上各组件安装完成后,在python/scripts/目录下会有一个ride.py,执行,打开robot framework界面.**
**[安装中可能遇到的问题]:**
**如果不是用pip安装的,需要注意各组件版本问题.**
**双击ride.py之后，一闪而过，并没有打开robotframework界面，有可能是wxpython版本不正确。在cmd下进入python命令行，输入from robotide import main  如果版本有问题会提示你。**
