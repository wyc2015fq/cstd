# python cookbook 2.2 字符串开头或结尾匹配 - 别说话写代码的博客 - CSDN博客





2019年03月29日 21:38:41[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#检查字符串开头或者结尾简单方法是str.startswith()或者str.endswith()方法
filename = "spam.txt"
print(filename.endswith('.txt'))
print(filename.startswith('file:'))
url='http://www.python.org'
print(url.startswith('http'))
```

![](https://img-blog.csdnimg.cn/20190329213651329.png)

```python
#若想检查多种匹配可能，只需要将所有的匹配项放入到一个元组中去，然后传给startswith()或者endswith()
import  os
filenames = os.listdir('.')  #获取当前目录下所有目录信息
print(filenames)
allname = [name for name in filenames if name.endswith(('.pdf','.jpg'))]#判断多个要放入元组中
print(allname)  #获取当前目录下所有后缀为pdf和jpg的文件
print(any(name.endswith('.py') for name in filenames))  #判断当前文件夹下是否存在.py结尾的文件
```

```python
#其实也可以自己实现startswith()和endswith()操作
filename = 'spam.txt'
print(filename[-4:]=='.txt')
url = 'http://www.python.org'
print(url[:5]=='http:' or url[:6]=='https:' or url[:4] == 'ftp:')
```

![](https://img-blog.csdnimg.cn/20190329213730115.png)

```
#也可使用正则表达式去实现
import re
url = 'http://www.python.org'
print(re.match('http:|https:|ftp:',url))
```

![](https://img-blog.csdnimg.cn/2019032921375291.png)

```python
#下面这个语句可以检查某个文件夹中是否存在指定的文件类型
if any(name.endswith(('.c','.h')) for name in os.listdir(dirname)):
    pass
```





