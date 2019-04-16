# Linux 运维分析中的常用命令一点通 - 我相信...... - CSDN博客





2014年12月07日 13:17:14[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1513
个人分类：[Linux](https://blog.csdn.net/wireless_com/article/category/2553345)









除了万能的vi, 一般要记下下面的命令

查看文件内容：cat 
分页显示文件：more，less
显示文件头尾：tail head
内容排序： sort
字符统计：wc
查看重复出现的行：uniq
字符串查找：grep
文件查找：find
归档：tar
URL访问工具：curl




以上命令熟练了，就可以使用组合拳了，例如查看最耗时的页面：
cat  文件名 |sort -k 2 -n -r（对栏目排序 ）|head -10

再进一步，就要使用sed和awk了，从而进行 shell编程。            


