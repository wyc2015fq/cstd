# C#的float,double和decimal - 左直拳的马桶_日用桶 - CSDN博客
2017年08月20日 14:46:09[左直拳](https://me.csdn.net/leftfist)阅读数：552
C#里面处理小数的类型大致有：
> 
float :0.1f  32位 
  double :0.1d 64位 
  decimal:0.1m 128位
decimal的精度比double高，但数值范围比double小。decimal虽然是128位，但这只是因为它的小数位可以很多，但整体数值范围却没有double大。
但是在C#中，给人感觉，好像double在精度兼容方面比decimal要好，比如
```
decimal m;
string value = Decimal.TryParse("3.3900000000000005E-4",out m) ? m.ToString() : "";// value == ""
```
结果value就是空的。
```
double d;
string value = Double.TryParse("3.3900000000000005E-4",out d) ? d.ToString() : "";// value == "0.000339"
```
我就不知道是Decimal.TryParse不支持科学计数法呢还是怎么样
具体见拙作： 
[SQL SERVER的浮点数类型及与C#的对应关系](http://blog.csdn.net/leftfist/article/details/9129039)
也可以参考 
[c#中decimal ,double,float的区别](http://lj.soft.blog.163.com/blog/static/79402481201032210173381/)
