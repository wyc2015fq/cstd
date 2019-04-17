# python cookbook 3.4 二八十六进制整数，二进制八进制十六进制十进制准换 - 别说话写代码的博客 - CSDN博客





2019年04月16日 14:38:10[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：4
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#需要转换或者输出使用二进制，八进制或者十六进制表示的整数。可以分别使用bin(),oct(),hex()函数
x=1234
print(bin(x))
print(oct(x))
print(hex(x))
```

![](https://img-blog.csdnimg.cn/20190416143648903.png)

```
#整数有符号，若处理负数，输出结果会包含一个负号。
x=-1234
print(format(x,'b'))
print(format(x,'x'))
```

![](https://img-blog.csdnimg.cn/20190416143703114.png)

```
#若想产生一个无符号值，需要增加一个指示最大位长度的值。比如为了显示32位值，可以这样写：
x=-1234
print(format(2**32+x,'b'))
print(format(2**32+x,'x'))
```

![](https://img-blog.csdnimg.cn/20190416143716923.png)

```
#为了以不同进制转换整数字符串，简单的使用带有进制的int(0函数即可
print(int('4d2',16))
print(int('10011010010',2))
```

![](https://img-blog.csdnimg.cn/20190416143732331.png)

如要指定8进制，注意前缀是0o



