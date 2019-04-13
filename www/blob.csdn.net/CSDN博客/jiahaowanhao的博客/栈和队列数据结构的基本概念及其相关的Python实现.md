
# 栈和队列数据结构的基本概念及其相关的Python实现 - jiahaowanhao的博客 - CSDN博客


2018年04月01日 10:43:37[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：50标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[栈和队列数据结构的基本概念及其相关的Python实现](http://cda.pinggu.org/view/25154.html)
先来回顾一下栈和队列的基本概念：
相同点：从"数据结构"的角度看，它们都是线性结构，即数据元素之间的关系相同。
不同点：栈（Stack）是限定只能在表的一端进行插入和删除操作的线性表。 队列（Queue）是限定只能在表的一端进行插入和在另一端进行删除操作的线性表。它们是完全不同的数据类型。除了它们各自的基本操作集不同外，主要区别是对插入和删除操作的"限定"。
栈必须按"后进先出"的规则进行操作：比如说，小学老师批改学生的作业，如果不打乱作业本的顺序的话，那么老师批改的第一份作业一定是最后那名同学交的那份作业，如果把所有作业本看作是一个栈中的元素，那么最后一个同学交的作业本就是栈顶元素，而第一个同学交的，也就是最低端的作业本，就是栈底元素，这就是对栈的读取规则。
而队列必须按"先进先出"的规则进行操作：打个比方，一些人去银行办理业务，一定是先去排队的最先得到服务，当然他也是第一个走出银行的（假设这些人都在一个窗口排队）。如果把所有这些等候服务的人看作是队的元素，第一个人就是对头元素，相应的，最后一个人就是队尾元素。这是队的读取规则。
用Python实现栈，这是Python核心编程里的一个例子:
'\#!/usr/bin/env python
\#定义一个列表来模拟栈
stack = []
\#进栈,调用列表的append()函数加到列表的末尾,strip()没有参数是去掉首尾的空格
def pushit():
stack.append(raw_input('Enter new string: ').strip())
\#出栈,用到了pop()函数
def popit():
if len(stack) == 0:
print 'Cannot pop from an empty stack!'
else:
print 'Removed [', stack.pop(), ']'
\#编历栈
def viewstack():
print stack
\#CMDs是字典的使用
CMDs = {'u': pushit, 'o': popit, 'v': viewstack}
\#pr为提示字符
def showmenu():
pr = """
p(U)sh
p(O)p
(V)iew
(Q)uit
Enter choice: """
while True:
while True:
try:
\#先用strip()去掉空格,再把第一个字符转换成小写的
choice = raw_input(pr).strip()[0].lower()
except (EOFError, KeyboardInterrupt, IndexError):
choice = 'q'
print '\nYou picked: [%s]' % choice
if choice not in 'uovq':
print 'Invalid option, try again'
else:
break
\#CMDs[]根据输入的choice从字典中对应相应的value,比如说输入u,从字典中得到value为pushit,执行pushit()进栈操作
if choice == 'q':
break
CMDs[choice]()
\#判断是否是从本文件进入,而不是被调用
if __name__ == '__main__':
showmenu()
用Python实现队列:
\#!/usr/bin/env python
queue = []
def enQ():
queue.append(raw_input('Enter new string: ').strip())
\#调用list的列表的pop()函数.pop(0)为列表的第一个元素
def deQ():
if len(queue) == 0:
print 'Cannot pop from an empty queue!'
else:
print 'Removed [', queue.pop(0) ,']'
def viewQ():
print queue
CMDs = {'e': enQ, 'd': deQ, 'v': viewQ}
def showmenu():
pr = """
(E)nqueue
(D)equeue
(V)iew
(Q)uit
Enter choice: """
while True:
while True:
try:
choice = raw_input(pr).strip()[0].lower()
except (EOFError, KeyboardInterrupt, IndexError):
choice = 'q'
print '\nYou picked: [%s]' % choice
if choice not in 'devq':
print 'Invalid option, try again'
else:
break
if choice == 'q':
break
CMDs[choice]()
if __name__ == '__main__':
showmenu()

