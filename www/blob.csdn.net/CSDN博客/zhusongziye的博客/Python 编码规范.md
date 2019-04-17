# Python 编码规范 - zhusongziye的博客 - CSDN博客





2018年10月26日 21:15:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：88








软件行业现在基本上是协同工作，不再是以前个人单打独斗的年代了。在一个团队中，每个人可能负责一个或者多个模块。如果团队没有统一的编程规范，会增加代码理解难度，从而增加维护成本。所以遵循良好的编码风格，可以有效的提高代码的可读性，降低出错几率和维护难度。另外，使用（尽量）统一的编码风格，还可以降低沟通成本。



网上有许多的编码规范，我介绍分享几个知名编码规范给大家参考学习。



**PEP 8**

PEP 8 可以算是 Python 官方的标准编码规范。它是用于规范 Python 主发行版中的标准库的代码。所以这个编码规范是值得一看。



文档地址：

https://www.python.org/dev/peps/pep-0008/



**Google 的 Python 风格指南**

总所周知，Google 是开源大户。Google 会将项目托管到 Github 上面，任何人都可以 fork、 修改、提交。如果代码贡献者的编程风格与 Google 的不一致, 会给代码阅读者和其他代码提交者造成不小的困扰。Google 因此发布了这份自己的编程风格指南, 使所有提交代码的人都能获知 Google 的编程风格。Google 的 Python 风格也是遵循 PEP8 规范。



文档地址：

http://zh-google-styleguide.readthedocs.io/en/latest/google-python-styleguide/contents/



**Pocoo 风格指南**

估计大家对 Pocoo 比较陌生，但大家一定对小型 Web 开发框架 Flask 很熟悉。没错，Flask 是 Pocoo 团队开发的项目。 除了 Flask 之外，Pocoo 团队还有开发出很多广受欢迎的项目，例如 Jinja2（模板引擎）、Pygments（语法高亮包）、Sphinx（文档处理器）、Werzeug（WSGI工具集）。Poco o团队编码风格指南适用于所有 Pocoo 团队的项目。总体来说，Pocoo 团队编码风格指南严格遵循了 PEP8 的要求，但略有一些不同之处，并进行了一定的扩展延伸。



文档地址：

http://dormousehole.readthedocs.io/en/latest/styleguide.html



PyCharm 目前开发 Python 的主流 IDE 工具，我介绍下如何在 PyCharm 配置 PEP 8 代码提示、将代码格式化符合 PEP 8 规范。



## 配置 PEP 8 代码提示



一般安装 PyCharm 都默认配置了规范提示。直接在右下角调整 Highlighting Level 为 Inspections 就能自动 PEP 8 提示。

![](https://img-blog.csdnimg.cn/20181026210846980.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



在我之前 Django 学习笔记系列的第一个 demo 中，有不符合规范的地方。在代码编辑框的右边会有一个浅黄色的标记，你将鼠标悬停在光标上，PyCharm 会发现有提示。

![](https://img-blog.csdnimg.cn/20181026210947327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



对于这种提示，只要在第 7 行增加一个回车就搞定了，之后PyCharm 也没有提示。

![](https://img-blog.csdnimg.cn/20181026211020973.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



当然，你也可以修改提示框的配色。

![](https://img-blog.csdnimg.cn/2018102621113815.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



## 将代码格式化符合 PEP 8 规范

这里我们需要使用到一个第三方库 **Autopep8**。Autopep8 是一个将 Python 代码自动排版为 PEP 8 风格的小工具。它使用 PEP 8 工具来决定代码中的哪部分需要被排版。Autopep8 可以修复大部分PEP 8 工具中报告的排版问题。



打开终端，使用 pip 命令来安装 Autopep8：
`pip install autopep8`


autopep8 是一个命令行工具，所以我们在终端下对某个文件进行格式化。命令行如下：
`autopep8 --in-place --aggressive --aggressive <filename>`


Pycharm 配置 Autopep8 方法如下：

1）**选择菜单「File」–>「Settings」–>「Tools」–>「External Tools」–>点击加号添加工具**

![](https://img-blog.csdnimg.cn/20181026211347666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



2）**填写如下配置项，点击「OK」保存**

![](https://img-blog.csdnimg.cn/20181026211422848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



图片中需要配置信息

Name：Autopep8 (可随意填写)

Tools settings:

    Programs：autopep8

    Parameters：--in-place --aggressive --ignore=E123,E133,E50 $FilePath$

    Working directory：$ProjectFileDir$



3）** 选择菜单「Tool」–>「Extern Tools」–>「Autopep8」或在某个文件中右键选择「Extern Tools」–>「Autopep8」，即可使用autopep8自动格式化你的python代码了。**

![](https://img-blog.csdnimg.cn/20181026211445316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



或

![](https://img-blog.csdnimg.cn/20181026211500718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



