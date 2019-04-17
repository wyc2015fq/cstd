# 不用第3个值交换a,b两个元素 - 别说话写代码的博客 - CSDN博客





2019年03月27日 16:18:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：5标签：[不用第三个值交换ab](https://so.csdn.net/so/search/s.do?q=不用第三个值交换ab&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)









第一种：基于加减法

```
a=a+b;
b=a-b;  //其实就是原来的a
a=a-b;  //换为原来的b
```

第二种：基于异或运算

```
a=a^b;
b=a^b; //相同为0，相异为1
a=a^b;
```





