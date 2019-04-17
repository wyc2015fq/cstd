# python读取yaml配置文件 - zhusongziye的博客 - CSDN博客





2018年04月20日 21:43:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：7848








# yaml简介

1.yaml [ˈjæməl]: Yet Another Markup Language ：另一种标记语言。yaml 是专门用来写配置文件的语言，非常简洁和强大,之前用ini也能写配置文件，看了yaml后，发现这个更直观，更方便，有点类似于json格式

2.yaml基本语法规则：
- 
大小写敏感

- 
使用缩进表示层级关系

- 
缩进时不允许使用Tab键，只允许使用空格。

- 
缩进的空格数目不重要，只要相同层级的元素左侧对齐即可

- 
#表示注释，从这个字符一直到行尾，都会被解析器忽略，这个和python的注释一样


3.yaml支持的数据结构有三种：
- 
对象：键值对的集合，又称为映射（mapping）/ 哈希（hashes） / 字典（dictionary）

- 
数组：一组按次序排列的值，又称为序列（sequence） / 列表（list）

- 
纯量（scalars）：单个的、不可再分的值。字符串、布尔值、整数、浮点数、Null、时间、日期


# 安装yaml

1.使用pip安装pyyaml模块

> 
pip install pyyaml


![](https://img-blog.csdn.net/20180420213928852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 键值对(dict)

1.yaml里面的键值对，也就是python里面的字典（dict）数据类型，比如python里面的字典，用yaml如何去写

```
# python3.6
{
"user": "admin",
"psw": "123456,
}
```

在yaml文件里可以这样写：

```
# yaml
user: admin
psw: 123456
```

2.字典嵌套字典

```
# python3.6
"nb1": {
        "user": "admin",
        "psw": "123456,
        }
```

3.yaml文件里可以这样写：

```
# yaml
nb1:
    user: admin
    psw: 123456
```

4.用python读取yaml文件案例如下，先用open方法读取文件数据，再通过load方法转成字典，这个load跟json里面的load是相似的。

```
# coding:utf-8
import yaml
import os
# 作者：上海-悠悠 交流QQ群：588402570

# 获取当前脚本所在文件夹路径
curPath = os.path.dirname(os.path.realpath(__file__))
# 获取yaml文件路径
yamlPath = os.path.join(curPath, "cfgyaml.yaml")

# open方法打开直接读出来
f = open(yamlPath, 'r', encoding='utf-8')
cfg = f.read()
print(type(cfg))  # 读出来是字符串
print(cfg)

d = yaml.load(cfg)  # 用load方法转字典
print(d)
print(type(d))
```

运行结果如下

![](https://img-blog.csdn.net/20180420214228538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 序列（list）

1.yaml里面写一个数组，前面加一个‘-’符号，如下

```
- admin1: 123456
- admin2: 111111
- admin3: 222222
```

对应python里面的list

```
[{'admin1': 123456},
{'admin2': 111111}, 
{'admin3': 222222}]
```

注意：数字读出来的是int或float类型

# 纯量（str）

1.int和float类型的数字
`n1: 12.30`
对应python
`{'n1': 12.3}`
2.布尔值用true和false表示

```
n2: true 
n3: false
```

对应python
`{'n2': True, 'n3': True}`
3.None用~表示
`n4: ~`
对应python
`{'n4': None}`
4.时间采用 ISO8601 格式。
`time1: 2001-12-14t21:59:43.10-05:00`
对应python
`{'time1': datetime.datetime(2001, 12, 15, 2, 59, 43, 100000)}`
5.日期采用复合 iso8601 格式的年、月、日表示。
`date1: 2017-07-31`
对应python
`{'date1': datetime.date(2017, 7, 31)}`
6.使用两个感叹号，强制转换数据类型。

```
# int转str
n6: !!str 123
```

对应python
`{'n6': '123'}`
```
# bool值转str
n7: !!str true
```

对应python
`{'n7': 'true'}`
7.在yaml文件写入以下内容：

```
n1: 12.30
n2: true
n3: false
n4: ~
time1: 2018-04-18t21:59:43.10+08:00
date1: 2018-04-18
n6: !!str 123
n7: !!str true
```

python读取结果：

```
{'n1': 12.3, 
'n2': True, 
'n3': False, 
'n4': None, 
'time1': datetime.datetime(2018, 4, 18, 13, 59, 43, 100000), 
'date1': datetime.date(2018, 4, 18), 
'n6': '123', 
'n7': 'true'}
```

# 混合使用

1.list嵌套dict，在yaml里面写入如下内容：

```
- user: admin1
  psw: '123456'

- user: admin2
  psw: '111111'

- user: admin3
  psw: '222222'
```

用python读取出来的结果：

```
[{'user': 'admin1', 'psw': '123456'},
{'user': 'admin2', 'psw': '111111'},
{'user': 'admin3', 'psw': '222222'}]
```

2.dict嵌套list，在yaml里面写入如下内容：

```
nub1:
    - admin1
    - '123456'

nb2:
    - admin2
    - '111111'

nb3:
    - admin3
    - '222222'
```

用python读取出来的结果：

```
{'nub1': ['admin1', '123456'],
'nb2': ['admin2', '111111'], 
'nb3': ['admin3', '222222']}
```



