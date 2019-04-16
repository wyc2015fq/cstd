# python的一些细节（3） - lyx的专栏 - CSDN博客





2017年07月29日 23:55:38[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：363
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









1.python逻辑判断式的连续判断



```python
x = 2
if 3 > x > 1:
   print x

if 1 < x > 0:
   print x
```


2.list转dict，其实是字典的推导式



```python
teams = ["Packers", "49ers", "Ravens", "Patriots"]
print {key: value for key, value in enumerate(teams)}
```




3.字典的正确访问方法



```python
data = {'user': 1, 'name': 'Max', 'three': 4}
is_admin = data.get('admin', False)
```




4.选择的所有可能性



```python
from itertools import combinations
teams = ["Packers", "49ers", "Ravens", "Patriots"]
for game in combinations(teams, 2):
    print game
```




5.计数器



```python
from collections import Counter
print Counter("hello")
```




6.循环的优化方法



```python
lowerlist = ['this', 'is', 'lowercase']
upper = str.upper
upperlist = []
append = upperlist.append
for word in lowerlist:
    append(upper(word))
    print(upperlist)
    #Output = ['THIS', 'IS', 'LOWERCASE']
```
这里，我们要尽可能在在for循环里面避免使用.号，也就是，‘点’出方法。我们可以向上面的代码一样，先在外面把这些方法赋值给一个变量。








