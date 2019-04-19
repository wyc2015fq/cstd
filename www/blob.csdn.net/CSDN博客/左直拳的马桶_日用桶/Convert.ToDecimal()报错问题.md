# Convert.ToDecimal()报错问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年08月15日 12:10:18[左直拳](https://me.csdn.net/leftfist)阅读数：1626
将字符串形式的数值转换回数值，很正常的要求吧。
```
//s_args_value是字符串
Decimal args_value = Convert.ToDecimal(s_args_value);
```
可是，如何知道这个字符串的内容真的是数值，或者没有含有什么别的非法字符？否则的话，有可能会报错。
总不能下下都依赖try catch吧。
可以酱紫：
```
Decimal tmp;
//s_args_value是字符串
Decimal? args_value = Decimal.TryParse(s_args_value,out tmp)
    ? tmp
    : null;
```
其他类型以此类推
