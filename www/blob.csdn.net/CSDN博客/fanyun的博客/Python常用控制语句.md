# Python常用控制语句 - fanyun的博客 - CSDN博客
2017年04月04日 19:30:54[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1003
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
　Python常用控制语句跟其它语言差不多，用法也类似，现在总结如下：
1. if 语句
```python
i = 10
n = int(raw_input("enter anumber:"))
 
if n == i:
   print "equal"
elif n < i:
   print "lower"
else:
   print "higher"
```
2. while语句
```python
while True:
   pass
else:
   pass
#else语句可选，当while为False时，else语句被执行。 pass是空语句。
```
3. for 循环 for..in
```python
for i in range(0, 5):
   print i
else:
   pass
# 打印0到4
```
　　注：当for循环结束后执行else语句；
 range(a, b)返回一个序列，从a开始到b为止，但不包括b，range默认步长为1，可以指定步长，range(0,10,2)；
4. break语句
　终止循环语句，如果从for或while中终止，任何对应循环的else将不执行。
5. continue语句
　continue语句用来调过当前循环的剩余语句，然后继续下一轮循环。
