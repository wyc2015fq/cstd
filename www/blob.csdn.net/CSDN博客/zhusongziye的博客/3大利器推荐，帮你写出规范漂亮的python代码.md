# 3大利器推荐，帮你写出规范漂亮的python代码 - zhusongziye的博客 - CSDN博客





2018年11月24日 10:49:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：140








*新建Python软件开发测试技术交友群QQ:952490269（加群备注software）*

Python学了好久，但是拿出来review的代码好像总是长的**不够俊美，不够工整**！因此标准化的代码规范就显得尤为重要。**今天就来推荐3个利器，python界广泛认同的代码风格规范PEP8和两个超牛的工具pylint和black**，分别用于代码风格规范检测和自动优化。



**1、代码风格规范PEP8**

首先讲一讲为什么要使用PEP8，我们先来看下面这段代码，在相关函数定义后，它是可以正常编译执行的，**但是 这段代码的怪异风格无论是让别人阅读还是自己阅读都会感到很难受**。这就需要一种普遍认同的代码风格规范，对行长度、缩进、多行表达式、变量 命名约定等内容进行统一，这就是PEP8的意义所在。

![](https://img-blog.csdnimg.cn/20181124104454114.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

*需要说明的是，PEP 8中有一些规范是为了方便阅读，而有一些规范实实在在地影响着代码的性能、容错率或者重 构难度。*



![](https://img-blog.csdnimg.cn/20181124104514148.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

比如上面这段代码，foo函数仅在满足条件的情况下有返回值、bar函数仅在不满足条件的情况下有返回值，**这样的代码兼容性会比较差，正确的做法是保持代码一致性**，函数中的返回语句都应该返回一个表达式，或者都不返回:

![](https://img-blog.csdnimg.cn/20181124104532785.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

关于PEP8就简单介绍到这里，有兴趣的朋友可以自行查阅文档。当我们打开文档的时候会发现PEPE 8规范的内容非常多、非常细。

作为一名新手如果要一条条解读、一条条记住这些规定实在不是件容易的事情。然而养成良好的 代码编写习惯对新手来说又是十分重要的。

**下面我们就正式进入主题，介绍两个简单又实用的代码自动化检测和优化工具，可以帮助我们快速规范自己的代码风格。**



2、自动检测工具Pylint

**Pylint 是一个检查违反 PEP8 规范和常见错误的库**，它会自动查找不符合代码风格标准和有潜在问题的代码，并在控制台输出代码中违反规范和出现问题的相关信息。

**1.安装与使用**

与python的其他库一样，直接 pip install pylint 即可完成安装，另外anaconda自带pylint，所以如果安装过anaconda不必再单独安装此库。

pylint的使用也非常简单，最基本的用法直接在控制台输入 pylint 路径/模块名 即可对相关模块的代码风格规范 进行检查，检查结果会在控制台输出。

建议结合 pylint --help 的提示进行 学习和检索。介绍完pylint的基本情况，我们来结合一个实例进行详细说明。



**2).实例演示说明**

这里我找了自己刚学python时写的一段代码进行测试:

```
import pandas as pd

data = []
char_replace_dict = {':':'\t', '（':'(', '）':')', '，':','}

with open('xmq_survey.txt', 'r', encoding = 'utf-8') as file:
 for line in file.readlines():
   for key, value in char_replace_dict.items():
     line = line.replace(key, value)#原来这个是深度引用
     #这条代码比自己写的简介的多，也更python
   data.append(line)

with open('survey.txt', 'w', encoding = 'utf-8') as file:
 for line in data:
   file.write(line)

raw_data = pd.read_table('survey.txt', delimiter = '\t', header = None) #查看read_table函数的用法
raw_data.columns = ['Name', 'Raw Info']
raw_data.count()
print('successful')
```

乍一看好像没什么大问题，但是经过pylint检查后却给出了一堆问题提示(下图)，我们来看检查结果，**每行以大 写字母+冒号开头的信息都是一处反馈提示**。

![](https://img-blog.csdnimg.cn/20181124104651270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

其中开头的大写字母表示错误类型(**主要有C\R\W\E\F几类**);以逗号间隔的两个数字表示发现问题的位置(行和 列);其后是对问题的具体描述，括号里的内容称为message id，可以简单理解为错误类型的详细分类，通过

pylint --help-msg=<msg-id> 指令可以查看这个问题的的详细信息 。

C——违反代码风格标准;

R——代码结构较差;

W——关于细节的警告;

E——代码中存在错误;

F——导致Pylint无法继续运行的错误。

例如，我们执行 pylint --help-msg=trailing-newlines 指令，会在控制台输出对 trailing-newlines 这种问题的详细描述:

![](https://img-blog.csdnimg.cn/20181124104722962.png)

pylint输出的最后一行是其对本次检测的评分，满分为10分，可以看到我的代码本次评分为0分T_T。



既然知道了自 己的代码哪里不规范，就去针对性地改正吧，根据提示结果首先将函数参数赋值 = 两边的空格去掉，再将多余的空 行去掉，然后运行一下pylint再次进行检测，得到如下结果:

![](https://img-blog.csdnimg.cn/20181124104740692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到，刚才修改过的代码相关问题提示已经没有了，**评分也从0分提高到了3.33分**。但是仍然有很多问题，**这里就要注意了，PEP8并不是要百分百遵守的，当遵循PEP 8规范会使代码可读性变差、会跟周围代码风格不一致的时候，还是要遵循自己的判断。**

这种情况下，pylint也提供了一种操作，可以手动屏蔽某些问题提示，以刚才的代码为例，剩下的几个问题主要是因为使用了Tab键、变量命名不规范、缺少文档说明造成的，我们可以使用

pylint --disable=mixex-indentation,invalid-name,missing-docstring 模块名称 命令对相关规范进行屏蔽重新检 测，发现问题提示全部消除，评分也提升到了10分。

*一段20行的代码就检测到如此之多的问题提示，虽然手动修改代码有助于对PEP 8规范的学习，但当项目文件比较 多、脚本代码很长的时候，实在是一个不小的工作量，因此就出现了能够自动优化代码风格的工具。*



**3、自动优化工具Black**

**在众多代码格式化工具中，Black算是比较新的一个**，它最大的特点是可配置项比较少，个人认为这对于新手来说是件好事，因为我们不必过多考虑如何设置Black，让 Black 自己做决定就好。

### **1).安装与使用**

与pylint类似，直接pip install black即可完成该模块的安装，不过black依赖于Python 3.6+，但它仍然可以格式化Python2的代码。

在使用方面black默认读取指定python文件并对其进行代码规范格式化，然后输出到原文件。

```
l = [1,
    2,
    3,
]
```

例如，我们将上面这段代码保存为test.py，然后在控制台执行 black test.py 指令，再次打开test.py，发现其中 的代码变成了这个样子:
`l = [1, 2, 3]`
当然，Black的封装程度再高也是有自定义配置项的，例如使用--version查看版本、使用--help查看帮助信息、使用--diff将修改信息输出到控制台而不更改原文件，下面我们还是结合一个实例来进行演示说明。



### **2).实例演示说明**

这里我们仍然使用pylint部分的代码进行演示。通过上面的操作我们知道，对这段代码直接使用pylint进行测试会输出很多问题提示，并给出一个评分0。现在我们首先使用black对其进行格式化，得到以下代码：

```
# -*- coding:utf-8 -*-

import pandas as pd

data = []
char_replace_dict = {':':'\t', '（':'(', '）':')', '，':','}

with open('xmq_survey.txt', 'r', encoding = 'utf-8') as file:
 for line in file.readlines():
   for key, value in char_replace_dict.items():
     line = line.replace(key, value)#原来这个是深度引用
     #这条代码比自己写的简介的多，也更python
   data.append(line)

with open('survey.txt', 'w', encoding = 'utf-8') as file:
 for line in data:
   file.write(line)

raw_data = pd.read_table('survey.txt', delimiter = '\t', header = None) #查看read_table函数的用法
raw_data.columns = ['Name', 'Raw Info']
raw_data.count()
print('successful')
```

**可能看起来修改前后的代码差异并不十分明显，实质上black已经对代码中参数赋值 = 两端的空格、注释的格式、 制表符等进行了替换和修改**，我们使用pylint来进行验证，执行 pylint 模块名称 命令，得到如下结果:

![](https://img-blog.csdnimg.cn/20181124104838409.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**可以看到，相对于最初的文件，评分从0分提高到7.3分，输出的问题提示少了很多**，剩余的问题主要是缺少说明文档、变量命名不规范.black对于提高我们代码规范性价比也是非常高的。

如果不想black直接对原文件进行修改，而是想看看它对代码中的哪些地方进行了改动的话，**可以使用--diff参数**，执行black --diff 文件名称，black会将相关信息输出到控制台（下图，其中-表示源代码，+表示建议修改后的代码），而不会对原文件进行修改。

![](https://img-blog.csdnimg.cn/2018112410485712.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**总之，black真的是一个非常好用的库，尤其对于新手来说，可以很方便地规范自己的代码风格。**



