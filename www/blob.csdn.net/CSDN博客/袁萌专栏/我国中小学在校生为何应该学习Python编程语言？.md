# 我国中小学在校生为何应该学习Python编程语言？ - 袁萌专栏 - CSDN博客
2012年04月12日 13:39:53[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：2091
    有人会问，从BASIC语言转向Python编程一定很难吧？非也。不仅不难，而且还要更简单易于学习。为什么？
当今，我国中小学的信息技术课程还在教授BASIC编程语言（无用的老古董）。为什么不教授现代而流行的Python编程语言呢？后者既简单，又通用，不知为何受到如此这般”冷落“，我们要为其鸣冤叫屈也。以下，对BASIC与Python语句进行逐一”对比“，看看情况究竟如何？
一、在BASIC语言中，语句
`PRINT``"HelloWorld"`
`而在Python``语言中，则如下表示：`
`**print**````"HelloWorld"`
两者只是命令行里面的字母，其大、小写不同而已。
二、在BASIC语句中，语句
a = 4.5
b% = 3c$ = "Hello World"
变成相应的Python语句，则是：
a = 4.5b = 3c = "Hello World"
      Python变量可以随意赋值。
三、关于循环，BASIC语句如下：
10 Finished% = FALSE
20 WHILE NOT Finished%30   REM Stuff...
40   IF FNerror THEN Finished% = TRUE:ENDWHILE:GOTO  7050   REM More stuff...
60 ENDWHILE
70 PRINT "Done"转向相应的Python语句，则是：finished = False**while****not** finished:*# Stuff...***if** error(): **break***# More stuff...***print**"Done"
    在Python语言中，# 等于BASIC语句中的REM，而且Python语言没有GOTO语句。四、BASIC的REPEAT语句
REPEATREM StuffUNTIL FNfinished
变为：
**while**True:*# Stuff***       if** finished(): **break**
五、BASIC的IF...THEN...ELSE...ENDIF语句
**if** x > 0**and** x < 10: **print**"OK"**else**: **print**"Out of range!"
变为：
>>> **if**"hello": 
**print**"yes"...yes>>> **if**None: **print**"yes"...
在Python语言中，空字符串为“假”，非空字符串为”真“。
六、......（省略）
从以上几点”对比“可以看出，Python编程语言确实要比传统的BASIC语言简单，准确，易于掌握与使用。我们应该在中小学大力推广应用Python编程语言，老师先期Python集训不是问题。
