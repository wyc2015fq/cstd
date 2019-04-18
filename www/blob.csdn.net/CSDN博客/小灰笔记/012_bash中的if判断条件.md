# 012_bash中的if判断条件 - 小灰笔记 - CSDN博客





2018年11月07日 23:24:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：110
个人分类：[bash](https://blog.csdn.net/grey_csdn/article/category/7113865)









这个使用方式跟常见的语言差不多，但是在表达方式上稍微细节化了一些。简单做几个示范例子：
- 数字的比较判断（是否相等）

![](https://img-blog.csdnimg.cn/20181107232338161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

执行结果：

![](https://img-blog.csdnimg.cn/20181107232340205.png)

         这里条件表达式涉及到了两种模式，其中第二种模式跟我之前接触的编程语言比较相似。但是，值得注意的一点事这个括号使用的时候必须与里面的参数存在一定的空白，否则会报错。同样的判断还是ge、le、gt、lt、ne等，与Perl在一定程度上有相似之处。
- 字符串的比较

测试代码：

![](https://img-blog.csdnimg.cn/2018110723234256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

执行结果：

![](https://img-blog.csdnimg.cn/2018110723234170.png)



         值得注意的是，变量应该注意要加上$来做引用。其次，还是中括号的间隔问题。除此之外，还有判断字符串是否为空、字符串长度大于零、字符串长度等于零等判断。感觉上，总体的使用简便程度上似乎是比不上Perl，也有可能是我对Perl熟悉而对这个不熟悉导致。



