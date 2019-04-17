# python之ini配置文件（configparser） - zhusongziye的博客 - CSDN博客





2018年04月20日 21:49:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：5316








# 前言

使用配置文件来灵活的配置一些参数是一件很常见的事情，配置文件的解析并不复杂，在python里更是如此，在官方发布的库中就包含有做这件事情的库，那就是configParser

configParser解析的配置文件的格式比较象ini的配置文件格式，就是文件中由多个section构成，每个section下又有多个配置项

# ini

1.ini配置文件格式如下：

```
;这里是注释

[section0]

key0 = value0
key1 = value1

[section1]

key2 = value2
key3 = value3
```

2.section不能重复，里面数据通过section去查找，每个seletion下可以有多个key和vlaue的键值对，注释用英文分号(;)

# configparser

1.python3里面自带configparser模块来读取ini文件

```
# python3
import configParser
```

敲黑板：python2的版本是Configparser

```
# python2
import ConfigParser
```

2.在pycharm里面，新建一个ini文件：右键New->File, 输入框直接写一个.ini后缀文件就行了，然后写数据

![](https://img-blog.csdn.net/20180420214756318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.注释里面有中文的话，这里代码跟python2是有点区别的，python2里面直接conf.read(cfgpath)就可以了，python3需要加个参数：encoding=”utf-8”

![](https://img-blog.csdn.net/20180420214808443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

敲黑板：如果ini文件里面写的是数字，读出来默认是字符串

```
# coding:utf-8
# 作者：上海-悠悠
# QQ群：588402570

import configparser
import os
curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")
print(cfgpath)  # cfg.ini的路径

# 创建管理对象
conf = configparser.ConfigParser()

# 读ini文件
conf.read(cfgpath, encoding="utf-8")  # python3

# conf.read(cfgpath)  # python2

# 获取所有的section
sections = conf.sections()

print(sections)  # 返回list

items = conf.items('email_163')
print(items)  # list里面对象是元祖
```

运行结果：

```
D:\debug_p3\cfg\cfg.ini
['email_qq', 'email_163']
[('sender', 'yoyo@163.com'), 
('psw', '123456'),
('port', '465'), 
('smtp_server', 'smtp.163.com'), 
('receiver', 'xxx@qq.com')]
```

# remove

1.如果想删除section中的一项，比如我想删除[email_163]下的port 这一行

```
# 删除一个 section中的一个 item（以键值KEY为标识）
conf.remove_option('email_163', "port")
```

2.删除整个section这一项
`conf.remove_section('email_163')`
3.参考代码：

```
# coding:utf-8
import configparser
import os
# 作者：上海-悠悠
# QQ群：588402570

curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")
print(cfgpath)  # cfg.ini的路径

# 创建管理对象
conf = configparser.ConfigParser()

# 删除一个 section中的一个 item（以键值KEY为标识）
conf.remove_option('email_163', "port")

items = conf.items('email_163')
print(items)  # list里面对象是元祖

# 删除一个 section
conf.remove_section('email_163')
sects = conf.sections()
print(sects)  # list里面对象是元祖
```

运行结果：

```
D:\debug_p3\cfg\cfg.ini
[('sender', 'yoyo@163.com'),
 ('psw', '123456'), 
 ('smtp_server', 'smtp.163.com'),
 ('receiver', 'xxx@qq.com')]
['email_qq']
```

# add

1.新增一个section

```
# 添加一个select
conf.add_section("emali_tel")
print(conf.sections())
```

2.section里面新增key和value

```
# 往select添加key和value
conf.set("emali_tel", "sender", "yoyo1@tel.com")
conf.set("emali_tel", "port", "265")
```

# write写入

1.write写入有两种方式，一种是删除原文件内容，重新写入:w

> 
conf.write(open(cfgpath, “w”))  # 删除原文件重新写入


另外一种是在原文件基础上继续写入内容，追加模式写入：a

> 
conf.write(open(cfgpath, “a”))  # 追加模式写入


2.前面讲的remove和set方法并没有真正的修改ini文件内容，只有当执行conf.write()方法的时候，才会修改ini文件内容，举个例子：在ini文件上追加写入一项section内容

```
# coding:utf-8
import configparser
import os
curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")
print(cfgpath)  # cfg.ini的路径

# 创建管理对象
conf = configparser.ConfigParser()

# 添加一个select
conf.add_section("emali_tel")
print(conf.sections())

# 往select添加key和value
conf.set("emali_tel", "sender", "yoyo1@tel.com")
conf.set("emali_tel", "port", "265")
items = conf.items('emali_tel')
print(items)  # list里面对象是元祖

conf.write(open(cfgpath, "a"))  # 追加模式写入
```

运行后会发现ini文件最后新增了写入的内容了




**set修改ini文件**

1.比如我想把前面ini文件里面的[email_163]下的port改成”中文”
敲黑板：这里写入中文的话，需要加上写入的编码格式:encoding=”utf-8”

```
# coding:utf-8
import configparser
import os

# 作者：上海-悠悠
# QQ群：588402570

curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "cfg.ini")

# 创建管理对象
conf = configparser.ConfigParser()

# 先读出来
conf.read(cfgpath, encoding="utf-8")

# 修改section里面的值
conf.set("email_163", "port", "中文")  # 写入中文

conf.write(open(cfgpath, "r+", encoding="utf-8"))  # r+模式
```


![](https://img-blog.csdn.net/20180420214854228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


