# MyEclipse安装插件的几种方法 - 深之JohnChen的专栏 - CSDN博客

2017年03月07日 21:49:55[byxdaz](https://me.csdn.net/byxdaz)阅读数：11633
个人分类：[JAVA](https://blog.csdn.net/byxdaz/article/category/628883)


MyEclipse安装svn插件的几种方法

　　一、通过MyEclipse Configuration Center在线安装

　　　　1. 打开MyEclipse10，在菜单栏选择MyEclipse→MyEclipse Configuration Center，即可进入到MyEclipse Configuration Center。

　　　　2. 在MyEclipse Configuration Center界面中点击Software选项卡，在Software界面中点击add site，在弹出框输入信息。比如svn插件安装。

　　　　　　Name : SVN

　　　　　　URL : http://subclipse.tigris.org/update_1.8.x

　　　　3. 在左边栏中找到Personal Site中找到SVN展开。将Core SVNKit Library和Optional JNA

Library添加(右键Add to Profile)，Subclipse下面的Subclipse Integration for Mylyn

3.0可以不添加（特别注意，此处不要添加）。
4. 在右下角窗格(Pending Changes )中点击Apply。安装重启后MyEclipse即可。

　　二、解压安装

　　　　下载SVN的zip文件 site-1.8.4.zip，直接把文件下的features目录和plugins目录解压到MyEclipse安装目录下的dropins目录即可(MyEclipse→MyEclipse 10→dropins)，重启即可。 site-1.8.4.zip解压目录中的其它*.xml文件不要。

　　三、创建link文件指向插件位置（推荐）

　　　　1. 同样下载SVN的zip文件 site-1.8.4.zip，把文件下的features目录和plugins目录解压到任意位置。

　　　　2. 在MyEclipse安装目录下的dropins目录创建.link文件，如svn.link。

　　　　3. 编辑svn.link文件，输入 path=第一步解压的features文件夹和plugins文件夹所在的目录，如 path=D:/Plugin/SVN。重启即可。

     安装其他插件（比如：FindBugs、Mybatis Editor、Regex Util等），可以使用上面的方法，但需要匹配插件要求的Eclipse的版本号。

如何查看MyEclipse包含的Eclipse的版本号？

在myeclipse的安装目录中搜索readme_eclipse.html文件，它在readme文件夹下面

双击打开，版本号就显示了myeclipse中的eclipse版本号：

如果你的安装目录是D:/MyEclipse ， 那么你进入到这个目录，按ctrl + F快捷键 ， 输入名字：

readme_eclipse.html ，按enter进行搜索，双击搜索到的文件就能显示eclipse版本号。

