
# 91. Decode Ways - OraYang的博客 - CSDN博客

2018年03月13日 10:42:53[OraYang](https://me.csdn.net/u010665216)阅读数：341所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
A message containing letters from`A-Z`is being encoded to numbers using the following mapping:
'A' -> 1
'B' -> 2
...
'Z' -> 26Given an encoded message containing digits, determine the total number of ways to decode it.
For example,
Given encoded message`"12"`,
it could be decoded as`"AB"`(1 2) or`"L"`(12).
The number of ways decoding`"12"`is 2.

# 思路
本题题目和爬梯子很相似（每次只能爬一级或两级），这是个很典型的动态规划问题。我们维护一个数组dp[i]表示从索引i开始的字符串，转码的种类。那么我们可以写出状态转移方程：
```python
dp[i] = Integer.
```
```python
.parseInt
```
```python
(s
```
```python
.substring
```
```python
(i,i+
```
```python
2
```
```python
))<=
```
```python
26
```
```python
?dp[i+
```
```python
1
```
```python
]+dp[i+
```
```python
2
```
```python
]:dp[i+
```
```python
1
```
```python
]
```
# 代码
```python
class Solution {
```
```python
public
```
```python
int
```
```python
numDecodings
```
```python
(String s) {
```
```python
int
```
```python
n = s.length();
```
```python
if
```
```python
(n==
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
0
```
```python
;
```
```python
int
```
```python
[] nums =
```
```python
new
```
```python
int
```
```python
[n+
```
```python
1
```
```python
];
        nums[n] =
```
```python
1
```
```python
;
        nums[n-
```
```python
1
```
```python
] = s.charAt(n-
```
```python
1
```
```python
)==
```
```python
'0'
```
```python
?
```
```python
0
```
```python
:
```
```python
1
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=n-
```
```python
2
```
```python
;i>=
```
```python
0
```
```python
;i--)
        {
```
```python
if
```
```python
(s.charAt(i)==
```
```python
'0'
```
```python
)
```
```python
continue
```
```python
;
            nums[i] = Integer.parseInt(s.substring(i,i+
```
```python
2
```
```python
))<=
```
```python
26
```
```python
?nums[i+
```
```python
1
```
```python
]+nums[i+
```
```python
2
```
```python
]:nums[i+
```
```python
1
```
```python
];
        }
```
```python
return
```
```python
nums[
```
```python
0
```
```python
];
    }
}
```

