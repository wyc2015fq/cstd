# 【mysql】ERROR 1054 (42S22): Unknown column '‘root‘' in 'where clause' - Big Smile - CSDN博客
2017年08月21日 23:48:22[王啸tr1912](https://me.csdn.net/tr1912)阅读数：13213
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
今天在安装mysql的时候，前面的步骤都没有太大的问题，但是到了想要重置密码的时候出现了问题，ERROR 1054 (42S22): Unknown column '‘root‘' in 'where clause'，不管如何用sql语句进行重置，它都会出现这样的提示语句，我用的语句如下：
`update user set password=password(‘123456‘) where user=‘root‘;`
        但是死活就是报如题中的错误，然后找到了解决的方案，原来是我们在代码和sql工具中写sql语句的时候，字符串用‘’就可以被引用，但是在Xshell环境下，并没有这样的环境，Linux的命令中，似乎“”才是引用字符串的王道，所以只需要把我们想要添加的文字替换成用“”包括就可以了。
`update user set password=password("123456") where user="root";`
然后就可以正常登陆

