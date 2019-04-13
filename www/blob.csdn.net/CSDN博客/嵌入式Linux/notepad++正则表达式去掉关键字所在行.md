
# notepad++正则表达式去掉关键字所在行 - 嵌入式Linux - CSDN博客

2018年12月29日 14:54:29[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：133



# 如下图
### 1、选择正则表达式
### 2、选择匹配大小写
### 3、^(.*)KERNEL(.*)$\n   加上\n就是把去除的行删除，不加就算把删除的行替换为空格
![](https://img-blog.csdnimg.cn/20181229145229203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)

