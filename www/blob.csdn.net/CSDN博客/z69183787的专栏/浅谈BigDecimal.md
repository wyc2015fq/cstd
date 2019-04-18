# 浅谈BigDecimal - z69183787的专栏 - CSDN博客
2018年08月01日 10:39:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：354
[https://blog.csdn.net/j754379117/article/details/52238396](https://blog.csdn.net/j754379117/article/details/52238396)
# 看图
![](https://img-blog.csdn.net/20160818102905676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如图
读过effective java的都知道，其中第49条就是：如果需要精确的答案，请避免使用float和double
所以在涉及到货币计算的时候一般使用BigDecimal
# 分析
但是BigDecimal也要讲究使用方法，否则也可能事与愿违，没有获得你想要的效果。
BigDecimal.valueof(0.99)，可以看一下此方法的具体实现：
![](https://img-blog.csdn.net/20160818103100224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到此方法先将参数转成了string类型，然后在执行参数为String类型的BigDecimal的构造函数。这样的结果一般就是我们想要的。
所以在使用BigDecimal的时候，要么使用newBigDecimal(String value)，要么使用BigDecimal.valueof(doublevalue)。
那如果参数为float类型的，比如BigDecimal.valueOf(0.99f)的值是多少呢？
首先此函数会自动进行精度扩展，将float类型的0.99转成double类型的，因为0.99本身就是无法用二进制表示的，也就说无论你的精度是多少位，都无法用二进制来精确表示0.99，或者你用二乘来判断（0.99*2=1.98 0.98*2=1.960.96*2=1.92 。。。永远无法得到一个整数）。这就是二进制计算机的缺点，就如同十进制也也无法表示1/3，1/6一样。
所以在0.99f转成double时，进行了精度扩展，变成了0.9900000095367432，而接着转成字符串，最后转成BigDecimal.
![](https://img-blog.csdn.net/20160818103200021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输出结果如下：
![](https://img-blog.csdn.net/20160818103235241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
至于为什么是这个数，那就涉及到二进制的补码移码的问题，double类型的0.99在计算机中的存储形式。这里你只要记得它绝不是你想要的精确的0.99即可。也为精确计算埋下了隐患。
jdk中已经明确不建议使用new BigDecimal(double value)这种形式的构造函数，这是为什么呢？？？
仍然是因为double类型不够精确，
![](https://img-blog.csdn.net/20160818103324381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输出结果：
![](https://img-blog.csdn.net/20160818103424788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 总结：
总之，在涉及到精确计算的时候，使用浮点型的数据很可能会出现你意想不到的结果。即使在使用BigDecimal的时候，也应该使用new BigDecimal(Stringvalue) 或 BigDecimal.valueof( **double** value)
