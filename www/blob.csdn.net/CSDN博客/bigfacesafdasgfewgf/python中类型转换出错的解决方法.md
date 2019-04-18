# python中类型转换出错的解决方法 - bigfacesafdasgfewgf - CSDN博客





2015年04月06日 11:15:01[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3498








python可以使用int()， float()等类型转换函数实现类型转换的功能，特别是string类型转换。 

但是，我们也经常会发现报如下的错误：

ValueError: invalid literal for int() with base 10

出现这种情况的原因是被转换的字符串包含不是数字的字符，例如小数点，连字符，或者其他的字符。

这里提供一种解决小数点的方法：

```
round(float('1.0'))
1.0
int(round(float('1.0')))
1
```

对于其他字符，我们可以事先输出字符看看，是否有非法字符。



