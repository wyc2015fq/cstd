# SVN服务器搭建及使用 - 零度的博客专栏 - CSDN博客
2017年03月24日 11:37:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：176标签：[subversion																[tortoisesvn](https://so.csdn.net/so/search/s.do?q=tortoisesvn&t=blog)](https://so.csdn.net/so/search/s.do?q=subversion&t=blog)
个人分类：[SVN](https://blog.csdn.net/zmx729618/article/category/6814425)
### 1、Subversion安装
#### 1.1 下载安装包
Subversion是优秀的版本控制工具,其具体的的优点和详细介绍,这里就不再多说。首先，来下载和搭建SVN服务器.现在Subversion已经迁移到apache网站上了,下载地址:
[http://subversion.apache.org/packages.html](http://subversion.apache.org/packages.html)
这是二进制文件包的下载地址,你可在左侧的导航栏找到源代码,文档的下载地址。windows操作系统下面的二进制文件包一共有5种,如图：
![](http://pic002.cnblogs.com/images/2012/59020/2012032010475491.png)
**注意：个人认为最好用VisualSVN server 服务端和 TortoiseSVN客户端搭配使用**。
点开上面的VisualSVN连接,下载VisualSVN server，如图：
![](http://pic002.cnblogs.com/images/2012/59020/2012032010512783.png)
然后下载TortoiseSVN客户端,官网下载地址:[http://tortoisesvn.net/downloads.html](http://tortoisesvn.net/downloads.html)
![](http://pic002.cnblogs.com/images/2012/59020/2012032010575396.png)
注意下载跟你电脑匹配的安装包,在页面的下面你还可以找到语言包,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011002690.png)
下载完成后,应该有这些安装包,如图:
TortoiseSVN安装包和简体中文语言包：
![](http://pic002.cnblogs.com/images/2012/59020/2012032011025756.png)
 VisualSVN server安装包：
![](http://pic002.cnblogs.com/images/2012/59020/2012032011030970.png)
#### 1.2 安装VisualSVN 服务端
先安装VisualSVN server的安装包,双击VisualSVN server安装包,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011075915.png)
点Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011092780.png)
点同意,进图下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011104539.png)
选择上面一个选项,点Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011124373.png)
Location是指VisualSVN Server的安装目录,Repositorys是指定你的版本库目录.Server Port指定一个端口,Use secure connection勾山表示使用安全连接,Use Subversion authentication 表示使用Subversion自己的用户认证.点击Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011181538.png)
点Install,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011191323.png)
等待安装完成,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011200288.png)
安装完成后,启动VisualSVN Server Manager,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032011210810.png)
#### 1.3 安装TortoiseSVN客户端
接下来我们安装TortoiseSVN,双击安装包,进入下一步.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013164883.png)
点击Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013173310.png)
选择接受,然后点击Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013182158.png)
选择安装路径,然后点击Next,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013191110.png)
点击Install,开始安装,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013200169.png)
等待安装完成,安装完成后如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013210283.png)
接下来我们安装简体中文语言包,这个非常简单,一路Next就行,就不截图了.语言包安装完成以后在桌面任意空白地方单击鼠标右键,会在右键菜单里找到SVN,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013261385.png)
选择设置,进入下一步,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013324763.png)
在右边的语言里面选择简体中文,然后点击应用,确定,汉化即完成,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013343832.png)
到这里,服务端和客户端都已经安装完毕。
### 2、使用VisualSVN Server建立版本库及TortoiseSVN导出与检出
首先打开VisualSVN Server Manager,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013424241.png)
可以在窗口的右边看到版本库的一些信息,比如状态,日志,用户认证,版本库等.要建立版本库,需要右键单击左边窗口的Repositores,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013460385.png)
在弹出的右键菜单中选择Create New Repository或者新建->Repository,进入下一步:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013482637.png)
输入版本库名称,勾上Create default structure复选框(推荐这么做).点击OK,版本库就创建好了,版本库中会默认建立trunk,branches,tags三个文件夹,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013505340.png)
这时候我们将项目导入到版本库中,找到你的项目文件夹,在项目文件夹上点击鼠标右键,找到SVN菜单,选择导入,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032013561474.png)
在弹出的对话框中填上版本库URL,这个URL可以从VisualSVN Server Manager中获取,在你的版本库上单击右键,选择Copy URL to Clipboard,这样就把版本库URL复制到你的剪贴版了.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014054237.png)
将复制的版本库URL粘贴上,在URL后面加上trunk子路径.然后在导入信息里面填上导入信息"导入项目到版本库".如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014082688.png)
点击确定,所选中的项目就会被导入到版本库中.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014094977.png)
项目导入到版本库以后,不能随便让谁都能够读写版本库,所以需要建立用户组和用户.
在VisualSVN Server Manager窗口的左侧右键单击用户组,选择Create User或者新建->User,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014172032.png)
在弹出的对话框中填写User name和Password,然后点击OK,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014183338.png)
用相同的方式分别创建用户Develpoer1,Develpoer2,Develpoer3,Test1,Test2,Manger六个用户,分别代表3个开发人员,两个测试人员和一个项目经理,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014281495.png)
然后我们建立用户组,在VisualSVN Server Manager窗口的左侧右键单击用户组,选择Create Group或者新建->Group,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014144372.png)
在弹出窗口中填写Group name为Developers,然后点Add按钮,在弹出的窗口中选择三个Developer,加入到这个组,然后点Ok,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014292933.png)
用相同的方式创建组Managers,Testers,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014335217.png)
接下来我们给用户组设置权限,在MyRepository上单击右键,选择属性,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014352997.png)
在弹出的对话框中,选择Security选项卡,点击Add按钮,选中Developers,Managers,Testers三个组,然后添加进来,给Developers,Managers权限设置为Read/Write,Tester权限设置为Read Only,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014371679.png)
到此,服务端就完成了.
接下来,我们用客户端去检出代码,在桌面空白处单击右键,选择SVN检出,在弹出的对话框中填写版本库URL(具体获取方式,上面讲上传项目到版本库的时候讲过),选择检出目录,点击确定.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014541134.png)
开始检出项目,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014544736.png)
检出完成之后,我们打开工作副本文件夹,会看到所有文件和文件夹都有一个绿色的√.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032014563753.png)
至此，创建版本库和使用TortoiseSVN导入项目,检出项目已经介绍完毕。
### 3、使用TortoiseSVN进行文件的修改、提交、冲突解决
添加文件
在检出的工作副本中添加一个Readme.txt文本文件,这时候这个文本文件会显示为没有版本控制的状态,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015060577.png)
这时候,你需要告知TortoiseSVN你的操作,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015080595.png)
加入以后,你的文件会变成这个状态,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015092946.png)
这时候使用TortoiseSVN进行提交.这样别人就能看到你所做的更改了,如图.
![](http://pic002.cnblogs.com/images/2012/59020/2012032015110994.png)
修改文件
使用TortoiseSVN更新,修改工作副本中的Readme.txt文件,加入"hello world!",然后保存,你会发现Readme.txt文件的图标改变了,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015145411.png)
这个红色的叹号代表这个文件被修改了,这时候,提交更改,其他人即可看到你的更改.
重命名文件
使用TortoiseSVN更新,重命名工作副本中的Readme.txt文件为"Readme1.txt',然后保存,你会发现Readme.txt文件的图标改变了,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015180687.png)
更添加文件一个道理,这时候你需要告诉TortoiseSVN你的操作,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015200415.png)
加入以后,提交,这时候版本库中的Readme.txt文件将会被重命名为"Readme1.txt".
删除文件
使用TortoiseSVN更新,使用TortoiseSVN删除工作副本中的Readme.txt文件,然后提交,版本库中的相应文件即被删除掉了,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015263652.png)
强制写注释
为了以后你能更清晰的看到你所做的每一次更改的原因,你在每次提交的时候应该写上注释,而且尽量详细.如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015323124.png)
但是,可能有的人因为觉得太繁琐,而不填写注释,这不利于以后的版本控制,可以将强制在提交的时候写注释,首先单击右键,选择TortoiseSVN->属性,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015353872.png)
在弹出的对话框中,点击新建->日志大小,设置提交日志的最小字符数和锁定日志的最小字符数为20,提交文本框中显示边线的字符位置设置为100,点击确定,如图:
![](http://pic002.cnblogs.com/images/2012/59020/2012032015460294.png)
提交,以后再次提交的时候,如果输入的注释少于20个字符,将无法提交.
冲突解决
冲突问题是最常见的问题,它是这样产生的,A用户check out了一个工作副本A,接着B用户又check out了一个工作副本B.然后A用户对副本A中的文件C做了修改(可以是内容修改,文件删除,重命名,以及位置移动),并且提交.这时候B用户也对文件C的相同部分做了修改,这时候如果B用户进行提交,会先被告知版本过时,要求更新,然后更新的时候会提示冲突了,这时候可以用冲突编辑器进行手动选择。
