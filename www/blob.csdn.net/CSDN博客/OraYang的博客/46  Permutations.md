
# 46. Permutations - OraYang的博客 - CSDN博客

2017年09月18日 10:21:25[OraYang](https://me.csdn.net/u010665216)阅读数：466个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a collection of distinct numbers, return all possible permutations.
For example,
[1,2,3] have the following permutations:
[
[1,2,3],
[1,3,2],
[2,1,3],
[2,3,1],
[3,1,2],
[3,2,1]
]
## 思路
本题有两种思路，一种是DFS,每次两两交换，代码如下：
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
vector
```
```python
<
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
>
```
```python
permute(
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
&num) {
```
```python
vector
```
```python
<
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
>
```
```python
res;
        permuteDFS(num,
```
```python
0
```
```python
, res);
```
```python
return
```
```python
res;
    }
```
```python
void
```
```python
permuteDFS(
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
&num,
```
```python
int
```
```python
start,
```
```python
vector
```
```python
<
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
>
```
```python
&res) {
```
```python
if
```
```python
(start >= num.size()) res.push_back(num);
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
i = start; i < num.size(); ++i) {
            swap(num[start], num[i]);
            permuteDFS(num, start +
```
```python
1
```
```python
, res);
            swap(num[start], num[i]);
        }
    }
};
```
一种是网上公开的思路，思路如下：
> 当n=1时，数组中只有一个数a1，其全排列只有一种，即为a1

> 当n=2时，数组中此时有a1a2，其全排列有两种，a1a2和a2a1，那么此时我们考虑和上面那种情况的关系，我们发现，其实就是在a1的前后两个位置分别加入了a2

> 当n=3时，数组中有a1a2a3，此时全排列有六种，分别为a1a2a3, a1a3a2, a2a1a3, a2a3a1, a3a1a2, 和 a3a2a1。那么根据上面的结论，实际上是在a1a2和a2a1的基础上在不同的位置上加入a3而得到的。

> _ a1 _ a2 _ : a3a1a2, a1a3a2, a1a2a3

> _ a2 _ a1 _ : a3a2a1, a2a3a1, a2a1a3

> 代码如下：

> class
> Solution {
> public
> :
> vector
> <
> vector
> <
> int
> >
> >
> permute(
> vector
> <
> int
> >
> &num) {
> if
> (num.empty())
> return
> vector
> <
> vector
> <
> int
> >
> >
> (
> 1
> ,
> vector
> <
> int
> >
> ());
> vector
> <
> vector
> <
> int
> >
> >
> res;
> int
> first = num[
> 0
> ];
        num.erase(num.begin());
> vector
> <
> vector
> <
> int
> >
> >
> words = permute(num);
> for
> (
> auto
> &a : words) {
> for
> (
> int
> i =
> 0
> ; i <= a.size(); ++i) {
                a.insert(a.begin() + i, first);
                res.push_back(a);
                a.erase(a.begin() + i);
            }
        }
> return
> res;
    }
};

