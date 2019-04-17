# Python通过字符串调用函数 - zhusongziye的博客 - CSDN博客





2018年04月26日 21:53:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：971








有时候我们想通过字符串调用已经定义好的函数，一开始的思路及实现方式如下：

```
def chinese(text):
    print('jieba分词')

def english(text):
    print('nltk处理对文本进行分词')

def textprocess(file, language):
    text = open(file).read()
    
    #字符串调用chinese()或english()函数。
    language(text)
    
#调用函数
```
`file = ``'``data.txt`
```
'
```
`language = `
```
'english'
```
`textprocess(file, language)`
但是

```
textprocess(file='data.txt', language='english')
language(text)
TypeError: 'str' object is not callable
```




百度谷歌一番，我查到以下几种方式

### 1-字典

最简单，易上手的方式，使用字典配对。

```
def chinese(text):
    print('jieba分词')

def english(text):
    print('nltk处理对文本进行分词')

str_func_pair = {'chinese': chinese,
                 'english': english}
                 
def textprocess(file, language):
    text = open(file).read()

    #字典调用出函数名
    str_func_pair[language](text)
   
file =
```
`'data.txt'`
```
language =
```
`'english'`
```
textprocess(file, language)
```

### 2- globals()

使用globals()函数获取全局变量,其实实现方式跟字典差不多。我们先看看代码当前的变量们有哪些

```
def chinese(text):
    print('jieba分词')

def english(text):
    print('nltk处理对文本进行分词')
    
#查看全局变量
print(globals())
```

我们发现这不就是字典吗？

```
{'__name__': '__main__', 
'__doc__': None, 
此处省略一部分变量，方便展示

'chinese': <function chinese at 0x101f62e18>, 

'english': <function english at 0x10453ca60>}
```

于是

```
def chinese(text):
    print('jieba分词')

def english(text):
    print('nltk处理对文本进行分词')
                 
def textprocess(file, language):
    text = open(file).read()
   
    #globals()调用出函数名
    globals()[language](text)
file =
```
`'data.txt'`
```
language =
```
`'english' `
```
textprocess(file, language)
```

### 3-eval()

eval英文名翻译过来大概是等同的意思。

```
data = "[1,2,3,4]"

print(type(data))

print(type(eval(data)))
```

我们发现，eval可以将字符串转化为我们想要的列表对象。

```
<class 'str'>
<class 'list'>
```

据此，我们用eval也可以用字符串调用函数

```
def chinese(text):
    print('jieba分词')

def english(text):
    print('nltk处理对文本进行分词')

def textprocess(file, language):
    text = open(file).read()
    eval(language)(text)

file =
```
`'data.txt'`
```
language =
```

```
'english'
```
`textprocess(file, language)`




