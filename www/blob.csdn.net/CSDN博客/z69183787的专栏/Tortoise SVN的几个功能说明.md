# Tortoise SVN的几个功能说明 - z69183787的专栏 - CSDN博客
2014年01月13日 20:17:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：971
1. 多人修改同一份代码
Lock/Unlock
为了防止多人同时修改一份代码，就加把锁吧！
右击文件或目录->TortoiseSVN->Get Lock加锁
右击文件或目录->TortoiseSVN->Release Lock加锁
2. 统计功能(Statistics)
![](https://img-my.csdn.net/uploads/201206/19/1340121247_5145.png)
以此查看开发者提交频率。
3. 为工程指定忽略文件列表
![](https://img-my.csdn.net/uploads/201206/19/1340121337_3118.png)
也可以通过修改目录属性的方式查看和修改，如下面两图所示方式
![](https://img-my.csdn.net/uploads/201206/19/1340121370_2832.png)
![](https://img-my.csdn.net/uploads/201206/19/1340121397_2811.png)
在Properties中直接可以查看到svn::ignore的值。
在Project Properities内可以设定tsvn:logminsize,以指定在提交时最少要输入的Log Message字节数。以后新建项目均将加入此项限制,以避免空Log信息的提交。
4. 哪些人变更了哪些行(Blame)
在查看文件时，可以逐行查看到一段时间内哪些人做了什么样的修改。
![](https://img-my.csdn.net/uploads/201206/19/1340121456_2896.png)
![](https://img-my.csdn.net/uploads/201206/19/1340121491_2669.png)
5. 分支和标签 (Branching & Tagging)
在代码管理中，以下情形会使用到Branching:
   a.需要回溯到某个版本进行再开发。 如Project A要求基于1.10版再发新的项目程序。
   b. 对现有项目进行改进，但不影响现有工作。此时一般的做法会建立新的工程，但如果利用TortoiseSVN我们就可以直接基于已有代码建立新的工程。
如以前对已有程序的改进，原本的做法是导出原来代码建立新工程。 以后如果有相应的需求也可以参考建立Branch的办法。
有关Merge:
        参考以下事例：
        Project B按要求基于Project A,添加某个的功能,为避免影响正常的发布，创建了一个Branch进行开发，待开发完成就需要将新完成的功能合并到Project A里，Merge就是用于完成这样的工作。
 有关Tag, 相关于branch, tag仅是做一个标识作用，比如为KIMO RC33建一个节点，这样，以后可以很方便的获取RC32时的源代码。
TortoiseSVN原则上不允许对Tag进行提交操作，也就是视之为一个只读项，不过用户仍然可以强制提交。
综上，在SVN里对一个工程最完整的定义需要包括三个基本目录：
 /trunk
 /branches
 /tags
(完)
