
# Python实现字符串匹配算法代码示例 - jiahaowanhao的博客 - CSDN博客


2018年02月10日 21:39:28[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：495


[Python实现字符串匹配算法代码示例](http://cda.pinggu.org/view/24700.html)
字符串匹配存在的问题
Python中在一个长字符串中查找子串是否存在可以用两种方法：一是str的find()函数，find()函数只返回子串匹配到的起始位置，若没有，则返回-1；二是re模块的findall函数，可以返回所有匹配到的子串。
但是如果用findall函数时需要注意字符串中存在的特殊字符
蛮力法字符串匹配：
将模式对准文本的前m（模式长度）个字符，然后从左到右匹配每一对对应的字符，直到全部匹配或遇到一个不匹配的字符。后一种情况下，模式向右移一位。
代码如下：
def string_match(string, sub_str):
\# 蛮力法字符串匹配
for i in range(len(string)-len(sub_str)+1):
index = i  \# index指向下一个待比较的字符
for j in range(len(sub_str)):
if string[index] == sub_str[j]:
index += 1
else:
break
if index-i == len(sub_str):
return i
return -1
if __name__ == "__main__":
print(string_match("adbcbdc", "dc"))
最坏情况下，该算法属于Θ(nm)，事实上，该算法的平均效率比最差效率好得多。事实上在查找随机文本的时候，其属于线性的效率Θ(n)。
Horspool算法：
Horsepool算法是Boyer-Moore算法的简化版本，这也是一个空间换时间的典型例子。算法把模式P和文本T的开头字符对齐，从模式的最后一个字符开始比较，如果尝试比较失败了，它把模式向后移。每次尝试过程中比较是从右到左的。
在蛮力算法中，模式的每一次移动都是一个字符，Horspool算法的核心思想是利用空间来换取时间，提升模式匹配窗口的移动幅度。与蛮力算法不同的是，其模式的匹配是从右到左的，通过预先算出每次移动的距离并存于表中。
代码如下：
__author__ = 'Wang'
from collections import defaultdict
def shift_table(pattern):
\# 生成 Horspool 算法的移动表
\# 当前检测字符为c，模式长度为m
\# 如果当前c不包含在模式的前m-1个字符中，移动模式的长度m
\# 其他情况下移动最右边的的c到模式最后一个字符的距离
table = defaultdict(lambda: len(pattern))
for index in range(0, len(pattern)-1):
table[pattern[index]] = len(pattern) - 1 - index
return table
def horspool_match(pattern, text):
\# 实现 horspool 字符串匹配算法
\# 匹配成功，返回模式在text中的开始部分；否则返回 -1
table = shift_table(pattern)
index = len(pattern) - 1
while index <= len(text) - 1:
print("start matching at", index)
match_count = 0
while match_count < len(pattern) and pattern[len(pattern)-1-match_count] == text[index-match_count]:
match_count += 1
if match_count == len(pattern):
return index-match_count+1
else:
index += table[text[index]]
return -1
if __name__ == "__main__":
print(horspool_match("barber", "jim_saw_me_in_a_barbershopp"))
显然，Horspool算法的最差效率属于属于Θ(nm)。在查找随机文本的时候，其属于线性的效率Θ(n)。虽然效率类型相同，但平均来说，Horspool算法比蛮力算法快很多。

