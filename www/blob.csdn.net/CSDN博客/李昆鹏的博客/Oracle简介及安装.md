
# Oracle简介及安装 - 李昆鹏的博客 - CSDN博客


2018年05月21日 15:22:57[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：48


--------------------------------Oracle简介及安装-------------------------------
# 1. oracle介绍
ORACLE数据库系统是美国ORACLE公司（甲骨文）提供的以分布式数据库为核心的一组软件产品，是目前最流行的客户/服务器(CLIENT/SERVER)或B/S体系结构的数据库之一。比如SilverStream就是基于数据库的一种中间件。ORACLE数据库是目前世界上使用最为广泛的数据库管理系统，作为一个通用的数据库系统，它具有完整的数据管理功能；作为一个关系数据库，它是一个完备关系的产品；作为分布式数据库它实现了分布式处理功能。但它的所有知识，只要在一种机型上学习了ORACLE知识，便能在各种类型的机器上使用它。
----------------------------------------------------------
**Oracle 10g安装**
1.    解压oracle数据库安装包，如果是win7或者win8系统右键点击setup.exe选择兼容性，以xp方式，并且以管理员方式运行，以及其他所有用户都按着此规则如图
![](https://img-blog.csdn.net/20180521151830158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521151854179?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    并双击解压目录下的setup.exe，出现安装界面，如下：
![](https://img-blog.csdn.net/20180521151936836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.    输入口令和确认口令，如：password，点击下一步，出现如下进度条，
注：此口令即是管理员密码。
![](https://img-blog.csdn.net/20180521151953169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.    检查先决条件，选中红框所示的选择框，如下图：
![](https://img-blog.csdn.net/20180521152016307?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.    点击“下一步”，出现“概要”界面，点击“安装”。
![](https://img-blog.csdn.net/20180521152028261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.    出现安装进度条，等待安装完成，如下图：
![](https://img-blog.csdn.net/20180521152041806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7.    安装完成后，自动运行配置向导，如下图，等待其完成：
![](https://img-blog.csdn.net/20180521152052235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
8.    完成后，出现“口令管理”界面，点击“口令管理”，如下图：
![](https://img-blog.csdn.net/20180521152107644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
9.    将SCOTT和HR用户的沟去掉（解锁这两个账户），如下图所示，点击“确定”：
![](https://img-blog.csdn.net/20180521152118412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
10.  回到“口令管理”界面，点击“确定”，如下图：
![](https://img-blog.csdn.net/2018052115214224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
11.  安装结束，点击“退出”。
![](https://img-blog.csdn.net/20180521152158116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 2. PLSQL Developer安装
傻瓜式安装即可，不建议汉化，最后会提示输入序列号，打开plsql-sn.txt复制序列号，输入即可。重要提示：不要把plsql developer安装在有括号的目录下，否则会出现连不上数据库的情况，安装目录也不要出现中文。
![](https://img-blog.csdn.net/20180521152210768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Plsql developer是最好用也是公司里面大量使用的数据库图形化工具，功能十分强大，在开发人员中广受欢迎，基本上是使用oracle数据库的必备神器。Oracle支持命令行方式操作，在实际的开发中很少使用命令行，绝大多数使用图形化的工具Plsql developer
# 3. PowerDesigner安装
傻瓜式安装即可。
PowerDesigner是数据库模型设计的工具，功能十分强大，不仅能设计数据库模型，还能设计各种UML建模的图形。

