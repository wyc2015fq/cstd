# python cookbook 3.2 执行精确的浮点数运算 - 别说话写代码的博客 - CSDN博客





2019年04月16日 14:33:40[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9标签：[python执行精确的浮点数运算																[python cookbook](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)](https://so.csdn.net/so/search/s.do?q=python执行精确的浮点数运算&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








![](https://img-blog.csdnimg.cn/20190416143233194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```
#这是由于浮点数底层的误差引起的。可以使用decimal模块来计算
from decimal import localcontext,Decimal
a=Decimal('1.3')
b=Decimal('1.7')
print(a/b)
with localcontext() as ctx:
    ctx.prec=3
    print(a/b)
with localcontext() as ctx:
    ctx.prec=40   #保留小数点后40位
    print(a/b)
```

![](https://img-blog.csdnimg.cn/20190416143249839.png)

```
#有些错误仍然难以避免
nums=[1.23e+18,1,-1.23e+18]
print(sum(nums))
```

![](https://img-blog.csdnimg.cn/20190416143304996.png)

```
#上面错误可以利用math.fsum()提供的更精确计算能力来解决：
import math
print(math.fsum(nums))
```

![](https://img-blog.csdnimg.cn/20190416143317191.png)






