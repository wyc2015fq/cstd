# top+jstack分析cpu过高原因 - z69183787的专栏 - CSDN博客
2018年08月03日 17:59:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：676
1、用ps -ef | grep tomcat-v3 查出tomcat运行的进程id 
![这里写图片描述](https://img-blog.csdn.net/20160711175137284)
2、用top -Hp pid 查询进程下所有线程的运行情况（shift+p 按cpu排序，shift+m 按内存排序） 
top -Hp 30316 
![这里写图片描述](https://img-blog.csdn.net/20160711175601506)
3、找到cpu最高的pid，用printf ‘%x\n’ pid 转换为16进制 
printf ‘%x\n’ 30506 
![这里写图片描述](https://img-blog.csdn.net/20160711180026533)
4、用jstack 进程id | grep 16进制线程id 找到线程信息 
jstack 30316 | grep -A 20 772a 
![这里写图片描述](https://img-blog.csdn.net/20160711180311022)
这里说不定能看到一些有用的信息(准备定位我也还没弄明白) 
我知道的好像只能解决线程死锁之类的问题，网上没有查到准备定位cpu高的代码的例子
