# python cookbook 3.1 数字的四舍五入 - 别说话写代码的博客 - CSDN博客





2019年04月16日 14:31:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#若要对浮点数执行指定精度的舍入运算，使用内置round(value,digits)函数即可
print(round(1.34,1))
print(round(1.35,1))   #四舍五入
print(round(-1.27,1))  #向下舍入
print(round(1.25361,3))
```

![](https://img-blog.csdnimg.cn/20190416143037426.png)

```
#参数digits可以为负数，这种舍入运算会作用在十位，百位千位
a=1627731
print(round(a,-1))
print(round(a,-2))
print(round(a,-3))
```

![](https://img-blog.csdnimg.cn/20190416143053204.png)

```
#注意不要将舍入和格式化搞混了。若只想输出一定宽度的数，只需要在格式化时指定精度
x=1.23456
print(format(x,'.2f'))
print(format(x,'.3f'))   #保留3位小数
print('value is {:0.3f}'.format(x))
```

![](https://img-blog.csdnimg.cn/20190416143113793.png)

```
#不要用舍入浮点值来修正表面看起来正确的问题
a=3.2
b=2.1
c=a+b
print(c)
c=round(c,2)
print(c)
#对于某些行业，比如金融行业，这些浮点值是零容忍
```

![](https://img-blog.csdnimg.cn/20190416143132285.png)



