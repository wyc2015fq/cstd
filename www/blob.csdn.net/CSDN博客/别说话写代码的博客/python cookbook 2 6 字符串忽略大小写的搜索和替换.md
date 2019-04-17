# python cookbook 2.6 字符串忽略大小写的搜索和替换 - 别说话写代码的博客 - CSDN博客





2019年03月30日 14:50:06[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：16
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#问题：需要以忽略大小写的方式搜索与替换 字符串，你需要在使用re模块的时候给这些操作提供re.IGNORECASE标志参数，比如
text = 'UPPER PYTHON,lower python,Mixed Python'
print(re.findall('python',text,flags=re.IGNORECASE))  #设置参数ignorecase忽略大小写
print(re.sub('python','snake',text,flags=re.IGNORECASE)) #将无论大小写的python替换为snake
```

![](https://img-blog.csdnimg.cn/20190330144908845.png)

```
#上面第二个例子有个小缺陷，就是替换字符串并不会在佛那个跟被匹配字符串的大小写保持一致，可以用个辅助函数修复这个：
def matchcase(word):
    def replace(m):
        text = m.group()
        if text.isupper():
            return word.upper()
        elif text.islower():
            return word.lower()
        elif text[0].isupper():
            return word.capitalize()
        else:
            return word
    return replace

print(re.sub('python',matchcase('snake'),text,flags=re.IGNORECASE))
#matchcase('snake')反悔了一个回调函数（参数必须是match对象），前面一节提到过，sub()函数除了接受替换字符串外，还可以接受一个回调函数
#其实这里也就是闭包的意思
```

![](https://img-blog.csdnimg.cn/20190330144925631.png)



