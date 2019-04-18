# python中list的初始化 - bigfacesafdasgfewgf - CSDN博客





2015年04月06日 11:09:15[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：8972








产生一个递增的list：

```
num_inc_list = range(30)
#will return a list [0,1,2,...,29]
```

产生一个固定初始值的list：

```
initial_value = 0
list_length = 5
sample_list = [ initial_value for i in range(10)]
sample_list = [initial_value]*list_length
# sample_list ==[0,0,0,0,0]
```



