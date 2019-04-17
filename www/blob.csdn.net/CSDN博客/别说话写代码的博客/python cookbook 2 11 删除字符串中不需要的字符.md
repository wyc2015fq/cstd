# python cookbook 2.11 删除字符串中不需要的字符 - 别说话写代码的博客 - CSDN博客





2019年04月12日 10:02:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#若想去掉文本字符串开头，结尾或者中间不想要的字符，比如空白。
#strip()能用于删除开始或者结尾的字符。lstrip() ,rstrip()分别从左和右执行删除操作。
#默认这些方法会去除空白字符，当然你也可以指定其他字符
s=' hello world \n'
print(s.strip())
print(s.lstrip())
print(s.rstrip())   #可以自动去除空格和换行

t='---hello===='
print(t.lstrip('-'))   #去掉左边的-
print(t.strip('-='))   #去掉字符串所有-=
```

![](https://img-blog.csdnimg.cn/20190412100154106.png)

```
#strip()，可以去除空格，注意去除操作不会对中间的文本产生任何影响。比如：
s=' hell0    world \n'
print(s.strip())
```

![](https://img-blog.csdnimg.cn/2019041210020745.png)

```
#若想处理中间空格，可以使用replace()方法或者用正则表达式替换
print(s.replace(' ',''))
import re
print(re.sub('\s+','',s))
```

![](https://img-blog.csdnimg.cn/20190412100221220.png)

```
#通常情况下你想将字符串strip操作和其他迭代操作相结合，比如文件中读取多行数据。这样的话
#生成器可以派上用场
with open(filename) as f：
    lines = (line.strip() for line in f)   #高效执行数据准换操作，不需预先读取所有数据放到一个临时列表，只是创建生成器
    for line in lines:
        print(line)
#对于更高阶strip,你可能需要使用translate()方法。
```





