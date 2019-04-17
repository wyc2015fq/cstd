# python cookbook 2.3 用shell通配符匹配字符串 - 别说话写代码的博客 - CSDN博客





2019年03月29日 21:54:35[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：10
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#fnmatch模块提供了两个函数--fnmatch()和fnmatchcase()  ,可以用来实现通配符(*.py,Dat[0-9]*.csv等)
from fnmatch import fnmatch,fnmatchcase
print(fnmatch('foo.txt','*.txt'))
print(fnmatch('Dat45.csv','Dat[0-9]*'))
names = ['Dat1.csv','Dat2.csv','config.ini','foo.py']
print([name for name in names if fnmatch(name,'Dat*.csv')])
#fnmatch()函数使用底层操作系统的大小写敏感规则（不同的系统是不一样的）；来匹配模式
```

![](https://img-blog.csdnimg.cn/20190329215328221.png)

```
#如果对大小写匹配区别比较在意，使用fnmatchcase()来代替。它完全使用你的模式进行大小写匹配
print(fnmatchcase('foo.txt','*.TXT'))
```

![](https://img-blog.csdnimg.cn/20190329215344774.png)

```python
#这两个函数通常会被忽略的一个特性是在处理非文件名的字符串时候他们也很有用。假如有个街道地址列表数据
addresses = [
'5412 N CLARK ST',
'1060 W ADDISON ST',
'1039 W GRANVILLE AVE',
'2122 N CLARK ST',
'4802 N BROADWAY',
]
from fnmatch import fnmatchcase
print([addr for addr in addresses if fnmatchcase(addr,'*ST')])  #选出所有ST结尾的
print([addr for addr in addresses if fnmatchcase(addr,'54[0-9][0-9] *CLARK*')])
#逐个通配
```

![](https://img-blog.csdnimg.cn/20190329215405566.png)



