# python cookbook 1.11对切片命名 - 别说话写代码的博客 - CSDN博客





2019年01月12日 15:11:44[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：44标签：[python cookbook																[python 对切片命名](https://so.csdn.net/so/search/s.do?q=python 对切片命名&t=blog)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```python
#1.11对切片命名，
a='safdhuhgaogoa'
name = slice(4,7)   #编码切片位置，自己也好懂好识别
print(a[name])
name=slice(1,7,2)   #1-7的位置，步长为2
print(a[name])
```

![](https://img-blog.csdnimg.cn/20190112151120823.png)




