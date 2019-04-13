
# 71. Simplify Path - OraYang的博客 - CSDN博客

2017年09月23日 11:32:07[OraYang](https://me.csdn.net/u010665216)阅读数：154个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an absolute path for a file (Unix-style), simplify it.
For example,
path = “/home/”, => “/home”
path = “/a/./b/../../c/”, => “/c”
## 思路
本题如果会用getline这个C++库函数，那么问题就简单了
## 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
string
```
```python
simplifyPath(
```
```python
string
```
```python
path) {
```
```python
string
```
```python
res, tmp;
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
stk;
```
```python
stringstream
```
```python
ss(path);
```
```python
while
```
```python
(getline(ss,tmp,
```
```python
'/'
```
```python
)) {
```
```python
if
```
```python
(tmp ==
```
```python
""
```
```python
or tmp ==
```
```python
"."
```
```python
)
```
```python
continue
```
```python
;
```
```python
if
```
```python
(tmp ==
```
```python
".."
```
```python
and !stk.empty()) stk.pop_back();
```
```python
else
```
```python
if
```
```python
(tmp !=
```
```python
".."
```
```python
) stk.push_back(tmp);
        }
```
```python
for
```
```python
(
```
```python
auto
```
```python
str : stk) res +=
```
```python
"/"
```
```python
+str;
```
```python
return
```
```python
res.empty() ?
```
```python
"/"
```
```python
: res;
}
};
```

