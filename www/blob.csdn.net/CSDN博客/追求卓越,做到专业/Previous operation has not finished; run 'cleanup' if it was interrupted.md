
# Previous operation has not finished; run 'cleanup' if it was interrupted - 追求卓越,做到专业 - CSDN博客


2016年04月18日 16:17:42[Waldenz](https://me.csdn.net/enter89)阅读数：577标签：[svn clean up																](https://so.csdn.net/so/search/s.do?q=svn clean up&t=blog)个人分类：[其他																](https://blog.csdn.net/enter89/article/category/953774)


svn执行clean up命令时报错“Previous
 operation has not finished; run 'cleanup' if it was interrupted”。无论你到那个父层次的目录执行“clean up“，都是报一样的错。执行cleanup时候，提示要cleanup。

Svn的operation是存放在“work
 queue’“里的。而“work queue’是在内嵌数据库wc.db的work_queue表中的。看看work_queue表中放了些什么，再做处理。
1.内嵌数据库一般是用sqlite进行轻量级管理的。[下载sqlite](http://download.csdn.net/download/enter89/9668724)
2.为了方便命令行执行，将sqlite3.exe放到svn项目的主目录下，和.svn目录同级下。
3.   在该目录下执行sqlite3 .svn/wc.db "select * from work_queue".看到有多条记录，这些记录就是刚才执行的操作。
226539|(************)
226540|(************)
226541|(************)
226542|(************)
4.执行sqlite3 .svn/wc.db "delete from work_queue".把队列清空。
5.执行sqlite3 .svn/wc.db "select * from work_queue".确认一下是否已经清空队列，发现已经没有记录显示，说明已经清空了。
6.最后再试一下，看是否可以clean up了。果然成功了。

