# PreScan打开MATLAB时，“Error:Matlab ||和&&运算符的操作数必须能够转换为逻辑标量值”的解决方案 - 心纯净，行致远 - CSDN博客





2019年03月13日 12:57:23[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：178标签：[&&																[&																[MATLAB																[PreScan](https://so.csdn.net/so/search/s.do?q=PreScan&t=blog)
个人分类：[【MatLab/Simulink】																[【ADAS与ICV】](https://blog.csdn.net/zhanshen112/article/category/7655573)](https://blog.csdn.net/zhanshen112/article/category/7184373)





在使用PreScan软件invoke MATLAB时，出现了标题所示问题：
`Error:Matlab ||和&&运算符的操作数必须能够转换为逻辑标量值`
搜索了相关方案，网上给出的解决方案如下：

**||和&&为值运算， |与&为向量运算，改为|与&**

但是如何更改呢，本来我的想法是打开脚本文件进行对应的修改，但是找到该文件之后发现该文件是p文件，这是一种加密文件。P文件为了保护知识产权设计的一种加密文件，是不能查看的。之前，我还查找对应的能打开p文件的软件，但是毫无所获。后来网上看到了对应的实际措施。

**在编译的时候出现了问题，你需要电脑里有一个编译器，下一个VS2013问题就解决啦**

**现在正在尝试，，，**](https://so.csdn.net/so/search/s.do?q=MATLAB&t=blog)](https://so.csdn.net/so/search/s.do?q=&&t=blog)](https://so.csdn.net/so/search/s.do?q=&&&t=blog)




