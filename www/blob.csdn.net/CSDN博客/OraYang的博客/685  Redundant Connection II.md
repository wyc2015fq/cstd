
# 685. Redundant Connection II - OraYang的博客 - CSDN博客

2018年01月30日 15:23:52[OraYang](https://me.csdn.net/u010665216)阅读数：261所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
In this problem, a rooted tree is a**directed**graph such that, there is exactly one node (the root) for which all other nodes are descendants of this node, plus every node has exactly one parent, except for the root node which has no parents.
The given input is a directed graph that started as a rooted tree with N nodes (with distinct values 1, 2, …, N), with one additional directed edge added.  The added edge has two different vertices chosen from 1 to N, and was not an edge that already existed.
The resulting graph is given as a 2D-array of`edges`.  Each element of`edges`is a pair`[u, v]`that represents a**directed**edge connecting nodes`u`and`v`, where`u`is a parent of child`v`.
Return an edge that can be removed so that the resulting graph is a rooted tree of N nodes.  If there are multiple answers, return the answer that occurs last in the given 2D-array.
**Example 1:**
**Input:**[[1,2], [1,3], [2,3]]**Output:**[2,3]**Explanation:**The given directed graph will be like this:
  1
 / \
v   v
2-->3
**Example 2:**
**Input:**[[1,2], [2,3], [3,4], [4,1], [1,5]]**Output:**[4,1]**Explanation:**The given directed graph will be like this:
5 <- 1 -> 2
     ^    |
     |    v
     4 <- 3
**Note:**
- The size of the input 2D-array will be between 3 and 1000.
- Every integer represented in the 2D-array will be between 1 and N, where N is the size of the input array.


# 思路
本题与前面一道题目：[684. Redundant Connection](http://blog.csdn.net/u010665216/article/details/79196453)十分类似，唯一的区别就是无项图转换成了有向图。具体思路看代码
# 代码：
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
findRedundantDirectedConnection(
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
n = edges.size();
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
parent(n+
```
```python
1
```
```python
,
```
```python
0
```
```python
), candA, candB;
```
```python
// step 1, check whether there is a node with two parents
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
&edge:edges) {
```
```python
if
```
```python
(parent[edge[
```
```python
1
```
```python
]] ==
```
```python
0
```
```python
)
                parent[edge[
```
```python
1
```
```python
]] = edge[
```
```python
0
```
```python
];
```
```python
else
```
```python
{
                candA = {parent[edge[
```
```python
1
```
```python
]], edge[
```
```python
1
```
```python
]};
                candB = edge;
                edge[
```
```python
1
```
```python
] =
```
```python
0
```
```python
;
            }
        }
```
```python
// step 2, union find
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
i =
```
```python
1
```
```python
; i <= n; i++) parent[i] = i;
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
&edge:edges) {
```
```python
if
```
```python
(edge[
```
```python
1
```
```python
] ==
```
```python
0
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
int
```
```python
u = edge[
```
```python
0
```
```python
], v = edge[
```
```python
1
```
```python
], pu = root(parent, u);
```
```python
// Now every node only has 1 parent, so root of v is implicitly v
```
```python
if
```
```python
(pu == v) {
```
```python
if
```
```python
(candA.empty())
```
```python
return
```
```python
edge;
```
```python
return
```
```python
candA;
            }
            parent[v] = pu;
        }
```
```python
return
```
```python
candB;
    }
```
```python
private
```
```python
:
```
```python
int
```
```python
root(
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
& parent,
```
```python
int
```
```python
k) {
```
```python
if
```
```python
(parent[k] != k) 
            parent[k] = root(parent, parent[k]);
```
```python
return
```
```python
parent[k];
    }
};
```

