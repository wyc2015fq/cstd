# 【Linux学习】linux源代码版本控制RCS - 江南烟雨 - CSDN博客
2012年12月24日 21:55:04[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5296标签：[linux																[Linux																[LINUX																[版本控制](https://so.csdn.net/so/search/s.do?q=版本控制&t=blog)](https://so.csdn.net/so/search/s.do?q=LINUX&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/xiajun07061225/article/category/856333)
RCS是在SCCS源代码控制系统之后开发的。SCCS是由AT&T在系统V版本的Unix中引入的最初的源代码控制系统，现在它已经是X/Open标准的一部分了。RCS的功能与SCCS非常类似，但是它有着更加直观的接口和一些其他的选项，所以SCCS基本上已经被RCS所取代。
RCS只保存版本之间的不同之处，因此它非常节省存储空间。
RCS对个人的开发项目来说非常容易使用，因此在这里进行介绍。
下面介绍一下RCS的基本用法。
假设我们进行版本控制的文件是important.c，其内容是一个简单的Hello World C程序。
![](https://img-my.csdn.net/uploads/201212/24/1356354911_4181.png)
**首先需要用rcs命令来初始化该文件的RCS控制：**
![](https://img-my.csdn.net/uploads/201212/24/1356355128_1789.png)
命令rcs -i的作用是初始化RCS控制文件。
我们可以使用多行注释，在结束输入的时候需要在一行中单独使用一个英文句号或输入文件结束字符（通常是组合键CTRL+D）。
执行完这条语句之后，rcs将创建一个新的只读文件，其后缀是,v。如图所示：
![](https://img-my.csdn.net/uploads/201212/24/1356355351_3036.png)
我们开头创建了一个文件夹RCS，就是用来存放RCS文件的。这个系统会自动进行。
**接下来用ci命令将源文件的当前版本签入（check in）到RCS中。**
![](https://img-my.csdn.net/uploads/201212/24/1356355478_7949.png)
这时候发现源文件important.c已经被删除了：
![](https://img-my.csdn.net/uploads/201212/24/1356355551_2899.png)
文件内容及其控制信息都已经被保存到RCS文件important.c,v中了。
要对文件进行修改，需要首先签出（check out）该文件。
如果只是想阅读该文件，可以用co命令重建当前版本的该文件并将其权限设置为只读。
![](https://img-my.csdn.net/uploads/201212/24/1356355822_7067.png)
如果想对其进行修改，必须使用命令co -l锁定该文件。
![](https://img-my.csdn.net/uploads/201212/24/1356356133_5657.png)
现在可以对其进行修改，我们简单的添加一条打印语句：
![](https://img-my.csdn.net/uploads/201212/24/1356356251_7848.png)
然后保存，再次用ci命令保存改动。
![](https://img-my.csdn.net/uploads/201212/24/1356356313_6245.png)
![](https://img-my.csdn.net/uploads/201212/24/1356356891_6450.png)
如果想在签入该文件的时候仍然保留该文件的锁定状态，使得可以继续对该文件进行修改，就需要在调用ci命令的时候加上-l选项，这样在签入该文件的同时它会被自动签出来供同一用户使用。
查看一个文件的改动，使用rlog命令：
![](https://img-my.csdn.net/uploads/201212/24/1356356980_3661.png)
输出结果的第一行给出了对该文件的描述以及rcs使用的选项。接着列出了对该文件的修改情况和签入该文件时输入的注释内容，最近修改的在最前面。
如果现在要取出该文件的第一个版本，可以在调用co命令是指定所需要的版本号：
![](https://img-my.csdn.net/uploads/201212/24/1356357339_5138.png)
ci命令也有一个-r选项，其作用是强制指定版本号。
RCS和SCCS默认都用数字1作为第一个次版本号。
如果只是想了解两个版本之间的区别，使用命令rcsdiff：
![](https://img-my.csdn.net/uploads/201212/24/1356357527_5995.png)
