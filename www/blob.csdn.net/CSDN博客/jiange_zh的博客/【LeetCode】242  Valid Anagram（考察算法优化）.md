
# 【LeetCode】242. Valid Anagram（考察算法优化） - jiange_zh的博客 - CSDN博客


2016年01月08日 09:40:39[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：490



## Question
Given two strings s and t, write a function to determine if t is an anagram of s.
For example,
s = “anagram”, t = “nagaram”, return true.
s = “rat”, t = “car”, return false.
Note:
You may assume the string contains only lowercase alphabets.
## My code
```python
bool
```
```python
isAnagram(
```
```python
string
```
```python
s,
```
```python
string
```
```python
t) {
        sort(s.
```
```python
begin
```
```python
(), s.
```
```python
end
```
```python
());
        sort(t.
```
```python
begin
```
```python
(), t.
```
```python
end
```
```python
());
        return s==t;
    }
```
However, my solution costs O(n logn).
O(n) solution:
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
bool
```
```python
isAnagram(
```
```python
string
```
```python
s,
```
```python
string
```
```python
t) {
```
```python
if
```
```python
(s.size() != t.size())
```
```python
return
```
```python
false
```
```python
;
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
vec(
```
```python
26
```
```python
,
```
```python
0
```
```python
);
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
n : s)
            ++vec[n -
```
```python
'a'
```
```python
];
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
n : t)
            --vec[n -
```
```python
'a'
```
```python
];
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
n : vec)
```
```python
if
```
```python
(n !=
```
```python
0
```
```python
)
```
```python
return
```
```python
false
```
```python
;
```
```python
return
```
```python
true
```
```python
;
    }
};
```

