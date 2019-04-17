# 异常检测(Anomaly Detection)导论 - Hxyue2018's Blog - CSDN博客





2018年09月14日 07:26:15[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：119








> 
除了深度学习之外，在实际业务中，我们通常会遇到异常检测的任务，需要我们运用不同的算法实现对异常值的检测。该系列是自己对异常值检测的整理以及反思。





### 文章目录
- [1.异常检测的定义和应用](#1_19)
- [a.](#a_20)
- [Markdown及扩展](#Markdown_26)
- [表格](#_34)
- [代码块](#_67)
- [目录](#_86)
- [数学公式](#_91)
- [UML 图:](#UML__101)
- [离线写博客](#_129)



- **Markdown和扩展Markdown简洁的语法**
- **代码块高亮**
- **图片链接和图片上传**
- ***LaTex*数学公式**
- **UML序列图和流程图**
- **离线写博客**
- **导入导出Markdown文件**
- **丰富的快捷键**

![](https://img-blog.csdn.net/20180914071455629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1N1cGVyX0pzb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

c

### 1.异常检测的定义和应用

#### a.

异常检测（Anomaly Detection），也叫做异常分析（outlier analysis），相关的

异常检测就是从茫茫数据中找到那些“长得不一样“的数据。但检测异常过程一般都比较复杂，而且实际请鲁昂下数据一般都没有标签（label），我们并不知道哪些数据是异常点，所以一般很难直接用简单的监督学习。异常值检测还有很多困难，如极端的类别不平衡、多样的异常表达形式，复杂的异常原因分析等。

对于异常检测而言，有着许多衍生的应用场景。

## Markdown及扩展

使用简单的符号标识不同的标题，将某些文字标记为**粗体**或者*斜体*，创建一个[链接](http://www.csdn.net)等，详细语法参考帮助？。

本编辑器支持 **Markdown Extra** , 　扩展了很多好用的功能。具体请参考[Github](https://github.com/jmcmanus/pagedown-extra).

### 表格

**Markdown　Extra**　表格语法：
|项目|价格|
|----|----|
|Computer|$1600|
|Phone|$12|
|Pipe|$1|

可以使用冒号来定义对齐方式：
|项目|价格|数量|
|----|----|----|
|Computer|1600 元|5|
|Phone|12 元|12|
|Pipe|1 元|234|

###定义列表

**Markdown　Extra**　定义列表语法：

项目１

项目２

:   定义 A

:   定义 B- 项目３

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

###脚注

生成一个脚注[1](#fn1).

### 目录

用 `[TOC]`来生成目录：

### 数学公式

使用MathJax渲染*LaTex* 数学公式，详见[math.stackexchange.com](http://math.stackexchange.com/).
- 行内公式，数学公式为：$\Gamma(n) = (n-1)!\quad\forall n\in\mathbb N$。
- 块级公式：

$$	x = \dfrac{-b \pm \sqrt{b^2 - 4ac}}{2a} $$

更多LaTex语法请参考 [这儿](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).

### UML 图:

可以渲染序列图：

张三李四嘿，小四儿, 写博客了没?李四愣了一下，说：忙得吐血，哪有时间写。张三李四

或者流程图：

Created with Raphaël 2.2.0开始我的操作确认？结束yesno
- 关于 **序列图** 语法，参考 [这儿](http://bramp.github.io/js-sequence-diagrams/),
- 关于 **流程图** 语法，参考 [这儿](http://adrai.github.io/flowchart.js/).

## 离线写博客

即使用户在没有网络的情况下，也可以通过本编辑器离线写博客（直接在曾经使用过的浏览器中输入[write.blog.csdn.net/mdeditor](http://write.blog.csdn.net/mdeditor)即可。**Markdown编辑器**使用浏览器离线存储将内容保存在本地。

用户写博客的过程中，内容实时保存在浏览器缓存中，在用户关闭浏览器或者其它异常情况下，内容不会丢失。用户再次打开浏览器时，会显示上次用户正在编辑的没有发表的内容。

博客发表后，本地缓存将被删除。

用户可以选择  把正在写的博客保存到服务器草稿箱，即使换浏览器或者清除缓存，内容也不会丢失。

> 
**注意：**虽然浏览器存储大部分时候都比较可靠，但为了您的数据安全，在联网后，**请务必及时发表或者保存到服务器草稿箱**。


##浏览器兼容
- 目前，本编辑器对Chrome浏览器支持最为完整。建议大家使用较新版本的Chrome。
- IE９以下不支持
- IE９，１０，１１存在以下问题
- 不支持离线功能
- IE9不支持文件导入导出
- IE10不支持拖拽文件导入

- 
这里是 **脚注** 的 *内容*. [↩︎](#fnref1)





