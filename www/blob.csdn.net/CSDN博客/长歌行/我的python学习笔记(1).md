# 我的python学习笔记(1) - 长歌行 - CSDN博客





2015年09月20日 22:15:53[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1786
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









最近经常听到一句话，“珍爱生命，我用python”。故而觉得有必要学习了解一下。

### 瞥一眼

脚本语言（script language） 

胶水语言（glue language） 

科研/支撑
### 语法

**入门时参考资料**： 
[http://learnpythonthehardway.org/book/](http://learnpythonthehardway.org/book/)
[http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000](http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000)
[http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000](http://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)
[http://learnpythonthehardway.org/book/](http://learnpythonthehardway.org/book/)
本科GIS专业学C，用MFC。所以刚接触到python时，觉得这语言，方便是方便，但是一点都不严谨，类型和内存都太散了。从一个C程序员来看感觉比较常用的有：

**1. 变量与类型**

弱类型，无需申明类型

**2. list**

```
animals = ['bear', 'tiger', 'penguin', 'zebra'] 

bear = animals[0]
```

**3. 函数**

`def cheese_and_crackers(cheese_count, boxes_of_crackers):`

```
`print "You have %d cheeses!" % cheese_count`
```

python没有花括号，取而代之的是缩进和冒号，所以有笑话说，看python代码需要买一个游标卡尺。

**4. 类**

`class MyStuff(object):`

```
def __init__(self):
    self.tangerine = "And now a thousand years between"

  def apple(self):
    print "I AM CLASSY APPLES!"
```

注意，成员函数第一个参数是self。 

   self可以指向自己的成员变量。并没有私有，保护，公有的区别。

### 工具篇

**PyCharm**

很好的集成环境，语法高亮，安装库，支持各种插件，python各版本。个人觉得是最好的IDE了。

主界面

![PyCharm](https://img-blog.csdn.net/20150920215631958)

各种配置 
![Setting](https://img-blog.csdn.net/20150920215720973)
**IDLE**

**VIM**

推荐用spf13 
[https://github.com/spf13/spf13-vim](https://github.com/spf13/spf13-vim)
spf13对于c/c++等各种编程语言，markdown等文本标记都支持。

![这里写图片描述](https://img-blog.csdn.net/20150920220507273)

PyCharm也可以使用插件，使用vim的配置 

目前我VS， Chrome也使用vim插件。 

真心挺好用的。
### 简单比较
- 有很多支撑库 

毕竟可以调用c/c++
- 语法简洁 

C++: 

```
std::shared_ptr<std::vector<std::shared_ptr<Object>>>  

= std::make_share<<std::vector<std::shared_ptr<Object>>>
```


Python: 
`object_array = []`

果然是珍爱生命啊
字符编码解码，与数字转换就不说了啊。

详细的情况以后深入了解了再写吧。










