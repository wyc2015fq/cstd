# Markdown编辑器写博客 - 心纯净，行致远 - CSDN博客





2018年05月24日 15:23:15[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：89








# 欢迎使用Markdown编辑器写博客

本Markdown编辑器使用[StackEdit](https://github.com/benweet/stackedit)修改而来，用它写博客，将会带来全新的体验哦：
- **Markdown和扩展Markdown简洁的语法**
- **代码块高亮**
- **图片链接和图片上传**
- ***LaTex*数学公式**
- **UML序列图和流程图**
- **离线写博客**
- **导入导出Markdown文件**
- **丰富的快捷键**

## 快捷键
- 加粗 `Ctrl + B`
- 斜体 `Ctrl + I`
- 引用 `Ctrl + Q`
- 插入链接 `Ctrl + L`
- 插入代码 `Ctrl + K`
- 插入图片 `Ctrl + G`
- 提升标题 `Ctrl + H`
- 有序列表 `Ctrl + O`
- 无序列表 `Ctrl + U`
- 横线 `Ctrl + R`
- 撤销 `Ctrl + Z`
- 重做 `Ctrl + Y`

## Markdown及扩展

> 
Markdown 是一种轻量级标记语言，它允许人们使用易读易写的纯文本格式编写文档，然后转换成格式丰富的HTML页面。    —— [[ 维基百科 ]](https://zh.wikipedia.org/wiki/Markdown)


使用简单的符号标识不同的标题，将某些文字标记为**粗体**或者*斜体*，创建一个[链接](http://www.csdn.net)等，详细语法参考帮助？。

本编辑器支持 **Markdown Extra** , 扩展了很多好用的功能。具体请参考[Github](https://github.com/jmcmanus/pagedown-extra).

### 表格

**Markdown Extra** 表格语法：
|项目|价格|
|----|----|
|Computer|$1600|
|Phone|$12|
|Pipe|$1|
可以使用冒号来定义对齐方式：|项目|价格|数量|
|----|----|----|
|Computer|1600 元|5|
|Phone|12 元|12|
|Pipe|1 元|234|
定义列表- **Markdown Extra** 定义列表语法：
- 项目１
- 项目２
定义 A定义 B- 项目３
定义 C
定义 D

> 
定义D内容


### 代码块

代码块语法遵循标准markdown代码，例如：

```python
@requires_authorization
def somefunc(param1='', param2=0):
    '''A docstring'''
    if param1 > param2: # interesting
        print 'Greater'
    return (param2 - param1 + 1) or None
class SomeClass:
    pass
>>> message = '''interpreter
... prompt'''
```

### 脚注

生成一个脚注[1](https://blog.csdn.net/u010916338/article/details/77484632#fn:footnote).

### 目录

用 `[TOC]`来生成目录：

- [欢迎使用Markdown编辑器写博客](https://blog.csdn.net/u010916338/article/details/77484632#%E6%AC%A2%E8%BF%8E%E4%BD%BF%E7%94%A8markdown%E7%BC%96%E8%BE%91%E5%99%A8%E5%86%99%E5%8D%9A%E5%AE%A2)- [快捷键](https://blog.csdn.net/u010916338/article/details/77484632#%E5%BF%AB%E6%8D%B7%E9%94%AE)
- [Markdown及扩展](https://blog.csdn.net/u010916338/article/details/77484632#markdown%E5%8F%8A%E6%89%A9%E5%B1%95)- [表格](https://blog.csdn.net/u010916338/article/details/77484632#%E8%A1%A8%E6%A0%BC)
- [定义列表](https://blog.csdn.net/u010916338/article/details/77484632#%E5%AE%9A%E4%B9%89%E5%88%97%E8%A1%A8)
- [代码块](https://blog.csdn.net/u010916338/article/details/77484632#%E4%BB%A3%E7%A0%81%E5%9D%97)
- [脚注](https://blog.csdn.net/u010916338/article/details/77484632#%E8%84%9A%E6%B3%A8)
- [目录](https://blog.csdn.net/u010916338/article/details/77484632#%E7%9B%AE%E5%BD%95)
- [数学公式](https://blog.csdn.net/u010916338/article/details/77484632#%E6%95%B0%E5%AD%A6%E5%85%AC%E5%BC%8F)
- [UML 图](https://blog.csdn.net/u010916338/article/details/77484632#uml-%E5%9B%BE)

- [离线写博客](https://blog.csdn.net/u010916338/article/details/77484632#%E7%A6%BB%E7%BA%BF%E5%86%99%E5%8D%9A%E5%AE%A2)
- [浏览器兼容](https://blog.csdn.net/u010916338/article/details/77484632#%E6%B5%8F%E8%A7%88%E5%99%A8%E5%85%BC%E5%AE%B9)



### 数学公式

使用MathJax渲染*LaTex* 数学公式，详见[math.stackexchange.com](http://math.stackexchange.com/).
- 行内公式，数学公式为：1
- 块级公式：

24−−−−−−−√2

更多LaTex语法请参考 [这儿](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).

### UML 图:

可以渲染序列图：

张三张三李四李四嘿，小四儿, 写博客了没?李四愣了一下，说：忙得吐血，哪有时间写。

或者流程图：

开始我的操作确认？结束yesno
- 关于 **序列图** 语法，参考 [这儿](http://bramp.github.io/js-sequence-diagrams/),
- 关于 **流程图** 语法，参考 [这儿](http://adrai.github.io/flowchart.js/).

## 离线写博客

即使用户在没有网络的情况下，也可以通过本编辑器离线写博客（直接在曾经使用过的浏览器中输入[write.blog.csdn.net/mdeditor](http://write.blog.csdn.net/mdeditor)即可。**Markdown编辑器**使用浏览器离线存储将内容保存在本地。

用户写博客的过程中，内容实时保存在浏览器缓存中，在用户关闭浏览器或者其它异常情况下，内容不会丢失。用户再次打开浏览器时，会显示上次用户正在编辑的没有发表的内容。

博客发表后，本地缓存将被删除。

用户可以选择 把正在写的博客保存到服务器草稿箱，即使换浏览器或者清除缓存，内容也不会丢失。

> 
**注意：**虽然浏览器存储大部分时候都比较可靠，但为了您的数据安全，在联网后，**请务必及时发表或者保存到服务器草稿箱**。


## 浏览器兼容
- 目前，本编辑器对Chrome浏览器支持最为完整。建议大家使用较新版本的Chrome。
- IE９以下不支持
- IE９，１０，１１存在以下问题 
- 不支持离线功能
- IE9不支持文件导入导出
- IE10不支持拖拽文件导入

转载自：[https://blog.csdn.net/u010916338/article/details/77484632](https://blog.csdn.net/u010916338/article/details/77484632)



