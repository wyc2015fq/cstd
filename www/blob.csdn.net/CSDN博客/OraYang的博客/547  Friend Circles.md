
# 547. Friend Circles - OraYang的博客 - CSDN博客

2018年01月28日 11:04:22[OraYang](https://me.csdn.net/u010665216)阅读数：333所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
There are**N**students in a class. Some of them are friends, while some are not. Their friendship is transitive in nature. For example, if A is a**direct**friend of B, and B is a**direct**friend of C, then A is an**indirect**friend of C. And we defined a friend circle is a group of students who are direct or indirect friends.
Given a**N*N**matrix**M**representing the friend relationship between students in the class. If M[i][j] = 1, then the ithand jthstudents are**direct**friends with each other, otherwise not. And you have to output the total number of friend circles among all the students.
**Example 1:**
**Input:**[[1,1,0],
 [1,1,0],
 [0,0,1]]**Output:**2**Explanation:**The 0thand 1ststudents are direct friends, so they are in a friend circle.
The 2ndstudent himself is in a friend circle. So return 2.
**Example 2:**
**Input:**[[1,1,0],
 [1,1,1],
 [0,1,1]]**Output:**1**Explanation:**The 0thand 1ststudents are direct friends, the 1stand 2ndstudents are direct friends,
so the 0thand 2ndstudents are indirect friends. All of them are in the same friend circle, so return 1.
**Note:**
N is in range [1,200].
M[i][i] = 1 for all students.
If M[i][j] = 1, then M[j][i] = 1.


# 思路
本题是个典型的union-find类型的题目。在《算法第四版》第一章的1.5中给出了union-find三种不同实现：
> quick-find

> quick-union

> 加权quick-union
本文实现了第二种方法，即quick-union算法。
思路如下：
首先我们设计一份API来封装所需的操作：连接两个索引并统计连通数、判断两个索引是否在一个连接分量中、找到根节点
API如下所示：
|API|解释|
|---|---|
|bool connected(int p,int q)|判断两个索引是否在一个连接分量中|
|void unionnumber(int p, int q)|连接两个索引并统计连通数|
|int find(int p)|找到根节点|
我们的解题步骤就是，先初始化变量，然后遍历矩阵右半角，遇到值为1先判断两个索引是否有相同的根节点，如果没有则连接索引，如果有则跳过继续循环。
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
int
```
```python
count;
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
id;
```
```python
bool
```
```python
connected(
```
```python
int
```
```python
p,
```
```python
int
```
```python
q){
```
```python
return
```
```python
find(p)==find(q);
    }
```
```python
void
```
```python
unionnumber(
```
```python
int
```
```python
p,
```
```python
int
```
```python
q){
```
```python
int
```
```python
pRoot = find(p);
```
```python
int
```
```python
qRoot = find(q);
```
```python
if
```
```python
(pRoot == qRoot)
```
```python
return
```
```python
;
        id[pRoot] = qRoot;
        count--;
```
```python
//cout<<pRoot<<endl;
```
```python
}
```
```python
int
```
```python
findCircleNum(
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
& M) {
```
```python
//将坐标作为索引
```
```python
count = M.size();
```
```python
int
```
```python
N = M.size();
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
;i<count;i++)
            id.push_back(i);
```
```python
//遍历M矩阵
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
p=
```
```python
0
```
```python
;p<N;p++)
        {
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
q=p;q<N;q++)
            {
```
```python
if
```
```python
(M[p][q]==
```
```python
1
```
```python
)
                {
```
```python
if
```
```python
(connected(p,q))
```
```python
continue
```
```python
;
                    unionnumber(p,q);
                }   
            }
        }
```
```python
return
```
```python
count;
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
find(
```
```python
int
```
```python
p){
```
```python
//找出根节点的名称
```
```python
while
```
```python
(p!=id[p])
                p = id[p];
```
```python
return
```
```python
p;
        }
};
```

