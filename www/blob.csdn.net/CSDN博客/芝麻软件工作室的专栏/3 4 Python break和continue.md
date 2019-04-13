
# 3.4 Python break和continue -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:30:26[seven-soft](https://me.csdn.net/softn)阅读数：345


break语句和continue语句都用来跳出循环。
## break语句
break语句是用来跳出一个循环语句，即停止执行一个循环语句，即使循环条件还没有成为False或序列的项目没有被完全遍历。
很重要的一点是，如果你跳出for或while循环，任何相应的循环else块是不执行的。例如：whileTrue:
s=input('输入一些东西 : ')
ifs=='quit':
break
print('字符串的长度是',len(s))
print('完成')

输出结果类似下面：
输入一些东西: Programming is fun
字符串的长度是 18
输入一些东西: When the work is done
字符串的长度是 21
输入一些东西 : if you wanna make your work also fun:
字符串的长度是 37
输入一些东西 : use Python!
字符串的长度是 12
输入一些东西 : quit
完成
它是如何工作的：
在这个程序中，我们重复获取用户输入的东西并打印每次输入字符串的长度。我们提供一个特殊的条件--通过检查用户输入是否是quit来结束程序。我们通过跳出循环停止程序，达到该程序的结束位置。
输入字符串的长度可以使用内建的len函数。
记住， break语句同样适用于for循环。
## continue语句
continue语句是用来告诉Python跳过当前循环块中其余的语句，继续循环的下一次迭代。举例：whileTrue:
s=input('输入一些东西: ')
ifs=='quit':
break
iflen(s)<3:
print('太小')
continue
print('输入的东西有足够的长度')
\# 在这做其它各种处理...

输出结果类似下面：
输入一些东西: a
太小
输入一些东西: 12
太小
输入一些东西: abc
输入的东西有足够的长度
输入一些东西: quit
它是如何工作的：
在这个程序中，我们接受来自用户的输入，但是只有在输入的字符串至少有3个字符时才处理。因此，我们使用内建的len函数来获得长度，如果长度小于3，通过使用continue句，我们跳过块中的其余语句。否则，在循环中的其余语句将被执行--在这做其它各种处理。
注意，continue语句同样适用于for循环。

