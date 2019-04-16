# python中，list转str - lyx的专栏 - CSDN博客





2016年05月31日 19:44:56[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1116
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









      在python中，经常会有需要把list转成一个字符串的情况。比如['you','are','freak']。怎么把它变成“you are freak”呢？

      python的str类型有一个方法，叫做join，作用就是把原str作为间隔，插入到list中的每一个str元素中。所以，



```python
' '.join(li)
```


      返回的就是'you are freak'。这个过程可以很好的被用到list创建csv文件中，毕竟csv文件通常是用逗号分隔的。






