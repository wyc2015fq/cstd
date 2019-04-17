# python实现字符串转浮点型，str2float - 别说话写代码的博客 - CSDN博客





2019年01月26日 11:03:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：40标签：[python str2float																[python字符串转浮点数](https://so.csdn.net/so/search/s.do?q=python字符串转浮点数&t=blog)](https://so.csdn.net/so/search/s.do?q=python str2float&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)








```python
DIGITS={'0':0,'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9}
def str2float(s):
    s=s.split('.')
    if s[0]==0:
        return 0+reduce(lambda x,y:x/10+y , map(lambda x:DIGITS[x],s[1][::-1]))/10
    else:
        return reduce(lambda x,y:x*10+y,map(lambda x:DIGITS[x],s[0]))+reduce(lambda x,y:x/10+y , map(lambda x:DIGITS[x],s[1][::-1]))/10
print(str2float('123.0456'))
print(str2float('0.0456'))
```

主要为了学习lambda，map，reduce等操作。注意s[::-1]是将所有元素翻转




