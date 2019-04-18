# python类型转换、数值操作 - wishchinYang的专栏 - CSDN博客
2013年07月30日 20:42:28[wishchin](https://me.csdn.net/wishchin)阅读数：592

我去，Python也有类型转换！！！因为Python作为动态语言，有延迟定义的功能，但作为强类型语言，类型一旦确定便固定，类型转换需要显示的类型转换操作。
原文：[http://www.cnblogs.com/jenry/archive/2010/05/27/1744861.html](http://www.cnblogs.com/jenry/archive/2010/05/27/1744861.html)
类型转换 
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码
 1 函数                      描述
 2 int(x [,base ])         将x转换为一个整数
 3 long(x [,base ])        将x转换为一个长整数
 4 float(x )               将x转换到一个浮点数
 5 complex(real [,imag ])  创建一个复数
 6 str(x )                 将对象 x 转换为字符串
 7 repr(x )                将对象 x 转换为表达式字符串
 8 eval(str )              用来计算在字符串中的有效Python表达式,并返回一个对象
 9 tuple(s )               将序列 s 转换为一个元组
10 list(s )                将序列 s 转换为一个列表
11 chr(x )                 将一个整数转换为一个字符
12 unichr(x )              将一个整数转换为Unicode字符
13 ord(x )                 将一个字符转换为它的整数值
14 hex(x )                 将一个整数转换为一个十六进制字符串
15 oct(x )                 将一个整数转换为一个八进制字符串
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
序列操作
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码
 1 操作                      描述
 2 s + r                   序列连接
 3 s * n , n * s           s的 n 次拷贝,n为整数
 4 s % d                   字符串格式化(仅字符串)
 5 s[i]                    索引
 6 s[i :j ]                切片
 7 x in s , x not in s     从属关系
 8 for x in s :            迭代
 9 len(s)                  长度
10 min(s)                  最小元素
11 max(s)                  最大元素
12 s[i ] = x               为s[i]重新赋值
13 s[i :j ] = r            将列表片段重新赋值
14 del s[i ]               删除列表中一个元素
15 del s[i :j ]            删除列表中一个片段
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
数值操作
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码
 1 x << y                  左移
 2 x >> y                  右移
 3 x & y                   按位与
 4 x | y                   按位或
 5 x ^ y                   按位异或 (exclusive or)
 6 ~x                      按位翻转
 7 x + y                   加
 8 x - y                   减
 9 x * y                   乘
10 x / y                   常规除
11 x // y                  地板除
12 x ** y                  乘方 (xy )
13 x % y                   取模 (x mod y )
14 -x                      改变操作数的符号位
15 +x                      什么也不做
16 ~x                      ~x=-(x+1)
17 abs(x )                 绝对值
18 divmod(x ,y )           返回 (int(x / y ), x % y )
19 pow(x ,y [,modulo ])    返回 (x ** y ) x % modulo
20 round(x ,[n])           四舍五入，n为小数点位数
21 x < y                   小于
22 x > y                   大于
23 x == y                  等于
24 x != y                  不等于(与<>相同)
25 x >= y                  大于等于
26 x <= y                  小于等于
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
