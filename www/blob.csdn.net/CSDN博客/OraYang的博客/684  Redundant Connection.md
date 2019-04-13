
# 684. Redundant Connection - OraYang的博客 - CSDN博客

2018年01月29日 16:01:27[OraYang](https://me.csdn.net/u010665216)阅读数：242所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
In this problem, a tree is an**undirected**graph that is connected and has no cycles.
The given input is a graph that started as a tree with N nodes (with distinct values 1, 2, …, N), with one additional edge added.  The added edge has two different vertices chosen from 1 to N, and was not an edge that already existed.
The resulting graph is given as a 2D-array of`edges`.  Each element of`edges`is a pair`[u, v]`with`u < v`, that represents an**undirected**edge connecting nodes`u`and`v`.
Return an edge that can be removed so that the resulting graph is a tree of N nodes.  If there are multiple answers, return the answer that occurs last in the given 2D-array.  The answer edge`[u, v]`should be in the same format, with`u < v`.
**Example 1:**
**Input:**[[1,2], [1,3], [2,3]]**Output:**[2,3]**Explanation:**The given undirected graph will be like this:
  1
 / \
2 - 3
**Example 2:**
**Input:**[[1,2], [2,3], [3,4], [1,4], [1,5]]**Output:**[1,4]**Explanation:**The given undirected graph will be like this:
5 - 1 - 2
    |   |
    4 - 3
**Note:**
- The size of the input 2D-array will be between 3 and 1000.
- Every integer represented in the 2D-array will be between 1 and N, where N is the size of the input array.

**Update (2017-09-26):**
We have overhauled the problem description + test cases and specified clearly the graph is an**undirected**graph. For the**directed**graph follow up please see**[Redundant Connection II](https://leetcode.com/problems/redundant-connection-ii/description/)**). We apologize for any inconvenience caused.

# 思路
这道题目是道典型的union-find问题，这个动态连通性问题，大家可以参考我以前分享的一篇[解题报告](http://blog.csdn.net/u010665216/article/details/79185303)，基本好多问题都是从那个问题衍生而来。
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
findRedundantConnection(
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
& edges) {
```
```python
int
```
```python
M= edges.size();
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
id(M+
```
```python
1
```
```python
);
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
res;
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
;i<=M;i++)
            id[i] = i;
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
edge:edges){
```
```python
int
```
```python
p = edge[
```
```python
0
```
```python
];
```
```python
int
```
```python
q = edge[
```
```python
1
```
```python
];
```
```python
while
```
```python
(p!=id[p])
                p = id[p];
```
```python
while
```
```python
(q!=id[q])
                q = id[q];
```
```python
if
```
```python
(p==q)
                res = edge;
```
```python
else
```
```python
id[q] = p;
        }
```
```python
return
```
```python
res;
    }
};
```

