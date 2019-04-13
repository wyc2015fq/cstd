
# 717. 1-bit and 2-bit Characters - OraYang的博客 - CSDN博客

2018年03月19日 16:01:18[OraYang](https://me.csdn.net/u010665216)阅读数：224所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
We have two special characters. The first character can be represented by one bit`0`. The second character can be represented by two bits (`10`or`11`).
Now given a string represented by several bits. Return whether the last character must be a one-bit character or not. The given string will always end with a zero.
**Example 1:**
**Input:**bits = [1, 0, 0]**Output:**True**Explanation:**The only way to decode it is two-bit character and one-bit character. So the last character is one-bit character.
**Example 2:**
**Input:**bits = [1, 1, 1, 0]**Output:**False**Explanation:**The only way to decode it is two-bit character and two-bit character. So the last character is NOT one-bit character.
**Note:**
- 1 <= len(bits) <= 1000- .
- bits[i]- is always- 0- or- 1- .


# 思路
这道题目很简单，遍历数组，当前数字为1，则移动两格，当前数字是0，则移动一格。在制定了上述移动规则后，我们就能得出如果索引i=N-1，并且nums[i]==0;那么我们必然可以确定最后一个字符是一位字符。
# 代码
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
isOneBitCharacter(
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
& bits) {
```
```python
int
```
```python
N = bits.size();
```
```python
if
```
```python
(N==
```
```python
0
```
```python
||bits[N-
```
```python
1
```
```python
]==
```
```python
1
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
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<N;i++)
        {
```
```python
if
```
```python
(i==(N-
```
```python
1
```
```python
)&&bits[i]==
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
true
```
```python
;
```
```python
if
```
```python
(bits[i]==
```
```python
1
```
```python
)
                i++;
        }
```
```python
return
```
```python
false
```
```python
;
    }
};
```

