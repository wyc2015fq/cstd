
# 3.2 Python while语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:27:49[seven-soft](https://me.csdn.net/softn)阅读数：210个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



while语句是被称为循环语句的一种。只要条件为真(true)，while语句允许您多次执行一个语句块。while语句可以有一个可选的else子句。例如：[复制](http://c.biancheng.net/cpp/html/1821.html#)[纯文本](http://c.biancheng.net/cpp/html/1821.html#)[新窗口](http://c.biancheng.net/cpp/html/1821.html#)
number=23
running=True
whilerunning:
guess=int(input('输入一个整数: '))
ifguess==number:
print('恭喜，你猜对了。')
running=False\# 这使while循环停止
elifguess<number:
print('不对，你猜的有点儿小。')
else:
print('不对，你猜的有点儿大。')
else:
print('while循环结束。')
print('完成')

输出可能有以下三种情况：
输入一个整数 : 50
不对，你猜的有点儿大。
输入一个整数 : 22
不对，你猜的的点儿小。
输入一个整数 : 23
恭喜，你猜对了。
while循环结束。
完成
它是如何工作的：
在这个程序中，我们还是玩猜谜游戏，但优点在于，允许用户一直猜直到他猜对——每次猜测不需要重复运行该程序，正如我们在前一节中所做的。这演示了如何恰当的使用while语句。
我们移动input和if语句到while循环中，在while循环前，设置变量running为True。首先，我们检测变量running是否为True，然后往下执行相应的while块。在这个块执行完后，再检测条件，在这里是变量running，为真，我们再次执行while块，否则，我们执行可选的else块，然后执行下面的语句。
当while循环的条件变为False时--这也可能发生在条件检测时的第一次，执行else块。如果在while循环中有else子句，它将一直执行，除非你使用break语句打破循环。
在这里True和False被称为布尔类型，你可以认为它们分别相当于值1和0。
C/C++程序员注意：记住， while循环可以的else子句。

