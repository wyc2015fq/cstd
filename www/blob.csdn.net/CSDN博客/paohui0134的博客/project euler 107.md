
# project euler 107 - paohui0134的博客 - CSDN博客


2016年09月04日 11:37:15[paohui0134](https://me.csdn.net/paohui0134)阅读数：235


# project euler 107
## 题目
### 原文
Minimal network
The following undirected network consists of seven vertices and twelve edges with a total weight of 243.
![图一](https://img-blog.csdn.net/20160904113339623)[图一](https://img-blog.csdn.net/20160904113339623)
The same network can be represented by the matrix below.
![图二](https://img-blog.csdn.net/20160904113414537)[图二](https://img-blog.csdn.net/20160904113414537)
However, it is possible to optimise the network by removing some edges and still ensure that all points on the network remain connected. The network which achieves the maximum saving is shown below. It has a weight of 93, representing a saving of 243 − 93 = 150 from the original network.
![图三](https://img-blog.csdn.net/20160904113502492)[图三](https://img-blog.csdn.net/20160904113502492)
Using network.txt (right click and ‘Save Link/Target As…’), a 6K text file containing a network with forty vertices, and given in matrix form, find the maximum saving which can be achieved by removing redundant edges whilst ensuring that the network remains connected.
### 简单翻译
最小连通网
如图一所示的无向网包含七个顶点和12条边，所有边的权重之和为243
相同的图可以用图二所示的矩阵表示
然而，可以通过移除一些不必要的边来优化图，使图仍然连通。对于上面的例子，最好的优化如图三所示。优化后的边的权重和为93，与原图相比，节省了150
使用[network.txt](https://projecteuler.net/project/resources/p107_network.txt)，一个6k大小包含40个顶点的图，以及连接的边的权重值，找到最优的优化，并计算最优的优化节省了多少权重值
## 程序
使用最小连通图的Kruskal算法，python代码如下
```python
def
```
```python
problem107
```
```python
()
```
```python
:
```
```python
connectedList = []
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
40
```
```python
):
```
```python
#初始化连通网
```
```python
connectedList.append([])
```
```python
def
```
```python
connected
```
```python
(parameters )
```
```python
:
```
```python
#判断当前边是否已经连通了
```
```python
curempty = -
```
```python
1
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
40
```
```python
):
```
```python
if
```
```python
(len(connectedList[i]) ==
```
```python
0
```
```python
):
                curempty = i
```
```python
break
```
```python
key = parameters[
```
```python
0
```
```python
]
        finda =
```
```python
False
```
```python
findb =
```
```python
False
```
```python
aindex = -
```
```python
1
```
```python
bindex = -
```
```python
2
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
,
```
```python
40
```
```python
):
```
```python
if
```
```python
(finda ==
```
```python
False
```
```python
):
                finda = parameters[
```
```python
0
```
```python
]
```
```python
in
```
```python
connectedList[i]
```
```python
if
```
```python
(finda):
                    aindex = i
```
```python
if
```
```python
(findb ==
```
```python
False
```
```python
):
                findb = parameters[
```
```python
1
```
```python
]
```
```python
in
```
```python
connectedList[i]
```
```python
if
```
```python
(findb):
                    bindex = i
```
```python
if
```
```python
(finda
```
```python
and
```
```python
findb):
```
```python
break
```
```python
if
```
```python
(aindex == bindex):
```
```python
#已经连通，跳过
```
```python
return
```
```python
True
```
```python
if
```
```python
(aindex <
```
```python
0
```
```python
and
```
```python
bindex <
```
```python
0
```
```python
):
```
```python
#没有记录，添加连通区域
```
```python
connectedList[curempty].append(parameters[
```
```python
0
```
```python
])
            connectedList[curempty].append(parameters[
```
```python
1
```
```python
])
```
```python
elif
```
```python
(aindex > -
```
```python
1
```
```python
and
```
```python
bindex > -
```
```python
1
```
```python
):
```
```python
#边的两个端点分别属于两个连通区域，将两个区域合并
```
```python
connectedList[aindex] += connectedList[bindex]
            connectedList[bindex] = []
```
```python
elif
```
```python
(aindex > -
```
```python
1
```
```python
):
```
```python
#只找到一个定点所在的区域，将另一个定点添加到该区域
```
```python
connectedList[aindex].append(parameters[
```
```python
1
```
```python
])
```
```python
elif
```
```python
(bindex > -
```
```python
1
```
```python
):
            connectedList[bindex].append(parameters[
```
```python
0
```
```python
])
```
```python
return
```
```python
False
```
```python
f = open(
```
```python
'p107_network.txt'
```
```python
)
    curline = f.readline()
```
```python
#跳过第一行
```
```python
curline = f.readline()
    alllist = []
    cur =
```
```python
1
```
```python
total =
```
```python
0
```
```python
while
```
```python
curline:
```
```python
#从第二行开始解析
```
```python
temp = curline.split(
```
```python
','
```
```python
)
        templist = []
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
, cur):
```
```python
if
```
```python
temp[i] ==
```
```python
'-'
```
```python
:
```
```python
continue
```
```python
else
```
```python
:
                tempvalue = int(temp[i])
                total += tempvalue
                templist = [cur, i, tempvalue]
                alllist.append(templist)
        cur +=
```
```python
1
```
```python
curline = f.readline()
    alllist.sort(key=
```
```python
lambda
```
```python
data : data[
```
```python
2
```
```python
])
```
```python
#Kruskal
```
```python
resultList = []
    cur =
```
```python
0
```
```python
ret =
```
```python
0
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
, len(alllist)):
```
```python
if
```
```python
(connected(alllist[i]) ==
```
```python
False
```
```python
):
            resultList.append(alllist[i])
            ret += alllist[i][
```
```python
2
```
```python
]
            cur +=
```
```python
1
```
```python
if
```
```python
(cur ==
```
```python
39
```
```python
):
```
```python
#需要39条边来连通40个顶点
```
```python
break
```
```python
print
```
```python
total - ret
```

