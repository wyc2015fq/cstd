# bat输出log日志操作 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年09月15日 10:56:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：26878标签：[dos																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=dos&t=blog)
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)








1.在C:/盘符下写bat文件，假设命名文件为ping.bat，代码如下：
   :a
   @time /t 
   @ping 172.0.0.1/n 100
   goto a

2.cmd打开dos窗口，在C:/盘符下执行bat文件，并输入log，如：

  c:/>ping >ping.log



3.在C:/盘符下找到ping.log就可以查看ping的行。




