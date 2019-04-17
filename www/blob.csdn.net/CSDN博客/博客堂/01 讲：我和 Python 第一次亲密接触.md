# 01  讲：我和 Python 第一次亲密接触 - 博客堂 - CSDN博客





2016年12月19日 11:23:54[最小森林](https://me.csdn.net/u012052268)阅读数：588








今天开始学习 鱼C论坛的鱼C工作室出品的《零基础入门学习Python》的视频教程（网易云课堂）。

学习笔记下载：http://download.csdn.net/detail/u012052268/9715280

课后习题：http://bbs.fishc.com/forum-243-1.html

这个系列的博客是我看视频以及课本（Python使用教程/清华大学出版社）的读书笔记。名字就以视频的名字命名。




1. Python 即可用作解释其他高级语言的脚本语言，也是一门面向对象的高级语言；较之 C 语言和 java 编程效率要高

得多。




2. 推荐在 Python 官网下载 3.0 以上版本并安装，下载地址见 www.python.org/downloads/



3. IDLE 是 Python 程序自带的辅助开发工具，也就是一个能与程序进行交互的文本框，在开始菜单下的输入框内输入

“IDLE”并回车， 便能找到并打开 IDLE 界面，初学者可以靠它进行交互式学习。




4. 字符串类型在 Python 中被定义为以双引号或单引号引起来的字符，例如，




在 Python 中只有字符串变量，没有像 C 语音一样字符变量的概念，例如




5. print()函数用于将字符串或数字等在屏幕上显示，例如



>>> print('小甲鱼')

小甲鱼

>>> print(3+5)

8




6. 在 Python 中“*”连接一个字符串和数字时表示将字符串重复多少次，例如

>>> "赞"*5

'赞赞赞赞赞'

而两个字符串之间以“+”号连接表示将两个字符串拼接，例如

>>> '小甲鱼'+'醉酒青牛'

'小甲鱼醉酒青牛'

另 外“+”号两边必须是同样的数据类型，如果是一个字符串和一个数字则报错，例如

>>> '小甲鱼'+5

Traceback (most recent call last):

File "<pyshell#4>", line 1, in <module>

'小甲鱼'+5

TypeError: Can't convert 'int' object to str implicitly



7. 在 Widnwos 下 ，在 IDLE 下的快捷键小知识：

1）组合 Alt + P 表示上一个命令，Alt + N 表示下一条命令




