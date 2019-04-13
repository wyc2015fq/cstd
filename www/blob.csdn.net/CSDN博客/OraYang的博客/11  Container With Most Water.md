
# 11. Container With Most Water - OraYang的博客 - CSDN博客

2017年09月07日 10:47:40[OraYang](https://me.csdn.net/u010665216)阅读数：128个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given n non-negative integers a1, a2, …, an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.
Note: You may not slant the container and n is at least 2.
## 思路
> 维护两个下标i=0,j=height.size()-1,从数组两侧遍历，求面积的最大值，然后判断i,j对应的数值大小，height[i]

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
int
```
```python
maxArea(
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
& height) {
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
,j=height.size()-
```
```python
1
```
```python
,mx=
```
```python
0
```
```python
;
```
```python
while
```
```python
(i<j)
        {
            mx = max((j-i)*min(height[i],height[j]),mx);
```
```python
if
```
```python
(height[i]<height[j])
                i++;
```
```python
else
```
```python
j--;
        }
```
```python
return
```
```python
mx;
    }
};
```

