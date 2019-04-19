# SQLserver2008数据库备份和还原问题（还原是必须有完整备份） - xqhrs232的专栏 - CSDN博客
2018年07月25日 10:48:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：182
原文地址::[https://www.cnblogs.com/lwcccyingziji/p/4052029.html](https://www.cnblogs.com/lwcccyingziji/p/4052029.html)
相关文章
1、SQLServer2008数据库创建，备份，还原教程----[https://www.2cto.com/database/201711/700826.html](https://www.2cto.com/database/201711/700826.html)
首先，我要说明的是你必须拥有完整的数据库备份，下面的还原教程，才算有用，如果其它问题，请搜索别的大牛的解决办法，本方法只适合菜鸟。
这个连接是站长大人的异常恢复方法，有问题可以自己学习http://www.cnblogs.com/dudu/archive/2011/10/15/sql_server_recover_deleted_records.html
一、先来学习备份数据库，选中需要备份数据库，右击鼠标，如下图：
![](https://images0.cnblogs.com/blog/680336/201410/261502416051189.png)
二、点击备份之后，如下图，点击确定：
![](https://images0.cnblogs.com/blog/680336/201410/261504073084938.png)
三、备份好之后，是还原，选中数据库，右击鼠标。
![](https://images0.cnblogs.com/blog/680336/201410/261505332466618.png)
四、选择源设备，点选右边按钮浏览文件夹，选择备份的文件。
![](https://images0.cnblogs.com/blog/680336/201410/261507124809800.png)
![](https://images0.cnblogs.com/blog/680336/201410/261508162628081.png)
![](https://images0.cnblogs.com/blog/680336/201410/261508477622658.png)
![](https://images0.cnblogs.com/blog/680336/201410/261509032772219.png)
![](https://images0.cnblogs.com/blog/680336/201410/261509200124606.png)
![](https://images0.cnblogs.com/blog/680336/201410/261509367462695.png)
五、点击确定之后，就会进行自动的还原，完成会有提示，要耐心。有时候会遇到错误提示，看看提示是怎么说的，可以先尝试下面的方法，取消其它用户对于数据库的使用。
![](https://images0.cnblogs.com/blog/680336/201410/261509549187309.png)
![](https://images0.cnblogs.com/blog/680336/201410/261512358087389.png)
六、上面的操作完成之后，重复前面的还原操作，再试一次，如果依然还有问题，再来看看下面的方法。首先，删除要还原的数据库（完整的数据备份一定要有哦，不然你就杯具了），不要怪我哦！在数据库目录上右击鼠标，新建数据库进行还原（新建之后重复之前还原的步骤），或者直接还原数据库都可以。
![](https://images0.cnblogs.com/blog/680336/201410/261512510275506.png)
七、名字需要输入，然后重复之前的还原步骤，就可以了。
![](https://images0.cnblogs.com/blog/680336/201410/261517317774669.png)
八、最后，要多多尝试，敢于尝试，很可能过程中遇到各种各样的问题，最后一个删除旧数据库，重新还原一个新的数据库是我使用中最好的方法，不管什么问题都能搞定，但是前提是完整的数据库备份，一定记住，备份，备份，再备份！数据就是命啊！
