
# shell自动生成的文件有一个问号的后缀 - 嵌入式Linux - CSDN博客

2018年10月16日 11:01:24[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：283


写了一个脚本，自动处理一个文件。
rm -f session.log
rm -f link
wget ftp://hostname/f:/ddn/session.log
egrep '^N[[:digit:]]|^D[1-4]' session.log >>link
egrep -c '^N[[:digit:]]|^D[1-4]' session.log >>link
egrep -v 'ACT/UP' link>>link
ls -l session.log >>link
gedit link
结果下载下来的session.log的文件名变成了加了一个奇怪字符，link变成了link？如下图。
![](http://blog.51cto.com/attachment/201204/153343750.jpg)
后续对link session.log文件的处理就会提示无法找到相应的文件。
搜了半天没有找到相应的中文资料。
这里这种情况的出现是因为，脚本是在windows下编写的，然后传到linux上运行，而windows下的断行符是^M$  linux下的断行符是$linux运行的时候无法识别$之前的^M,所以显示了奇怪的字符。
如下图所示
![](http://blog.51cto.com/attachment/201204/164655437.jpg)
![](http://blog.51cto.com/attachment/201204/164646543.jpg)

---
要将a.txt里的^M去掉并写入b.txt，则使用如下指令cat a.txt | tr -d "^M" > b.txt
注意：语句中的^M是通过ctrl+V, ctrl+M输入的。特指/r字符
1.windows中的换行符是\r\n，
2. linux/unix下的换行符是\n。
其中:
回车符：\r=0x0d  (13)          return； \#回车（carriage return）
换行符：\n=0x0a (10)           newline。\#换行（newline）

