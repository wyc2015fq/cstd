# python cookbook 3.3 数字的格式化输出 - 别说话写代码的博客 - CSDN博客





2019年04月16日 14:35:56[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：7
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#需要将数字格式化后输出，并控制数字的位数、对其、千位分隔符和其他的细节
#格式化输出单个数字的时候，可以使用内置的format()函数
x = 1234.56789
print(format(x,'.3f'))
print(format(x,':>10.1f'))   #共10位，保留小数点后一位，向后对其，左边空余填充：
print(format(x,'.<10.1f'))
print(format(x,'.^10.1f'))   #共10位，保留小数点后1位，居中，剩下部分填充.
print(format(x,','))   #千位分隔
print(format(x,'0,.1f'))   #千位分隔，保留小数点后一位
```

![](https://img-blog.csdnimg.cn/20190416143410845.png)

```
#若想使用指数记法，将f改成e或者E
print(format(x,'e'))
print(format(x,'0.2E'))  #保留两位，用指数记法
```

![](https://img-blog.csdnimg.cn/20190416143426522.png)

```
#同时指定宽度和精度的一般形式是'[<>^]?width[,]?(.digits)?'，其中width和digits为整数，？表示可选部分
print('the value is {:0,.2f}'.format(x))
```

![](https://img-blog.csdnimg.cn/20190416143441608.png)

```
#上面技术同时适用于浮点数和decimal模块的Decimal数字对象。当指定位数后，结果值会根据round(0函数同样的规则进行四舍五入后返回
print(x)
print(format(x,'0.1f'))
print(format(-x,'0.1f'))
```

![](https://img-blog.csdnimg.cn/20190416143455142.png)

```
#也可以使用字符串translate()方法来交换千位符
swap_separators={ord('.'):',',ord('.'):','}
print(format(x,',').translate(swap_separators))
```

![](https://img-blog.csdnimg.cn/20190416143513212.png)

```
#python中也有用%来格式化数字
print('%0.2f'%x)
print('%10.1f'%x)
print('%-10.1f'%x)
```

![](https://img-blog.csdnimg.cn/20190416143530269.png)



