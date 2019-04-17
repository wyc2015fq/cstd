# svn-clearup 报错的处理（Cleanup failed to process the following paths...） - DoubleLi - 博客园






在使用 svn 客户端执行操作失败后，执行 Clean up 操作也报错：Cleanup failed to process the following paths... ，一直不知道是什么原因。通常的解决方法是，删除所有文件重新 checkout 。文件小的话重新 checkout 可行，但是更新比较大的项目代码出错的话就有些麻烦。

          google 了一下，大致说的原因是：对一些文件操作的时候出现错误，文件加锁。

          对于 svn 的加锁文件，svn 不同版本的加锁方式是不一样的：1. 版本 1.7 之前加锁文件是 lock 的后缀文件，直接在报错目录的.svn目录下删除即可；1.7 版本以后（本人用 2.0 ）加锁信息是存储在隐藏文件夹 .svn 目录下 sqlite 文件中，存储的表名为 wc_lock、work_queue。




- 解决办法：



> 
无论是用 sqlite 命令行环境还是可视化工具，将 wc_lock、work_queue 表中的所有记录删除就好：



delete from wc_lock;

delete from work_queue;

然后，进行其他操作，运行良好![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)。










