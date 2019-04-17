# 【拓扑排序】包含拓扑深度的拓扑排序 LeetCode 329 - CD's Coding - CSDN博客





2018年03月05日 16:00:37[糖果天王](https://me.csdn.net/okcd00)阅读数：238








## 0x01 题面

> 
LeetCode 329. Longest Increasing Path in a Matrix [传送门](https://leetcode.com/problems/longest-increasing-path-in-a-matrix/description/)


Given an integer matrix, find the length of the longest increasing path.

From each cell, you can either move to four directions: left, right, up or down. You may NOT move diagonally or move outside of the boundary (i.e. wrap-around is not allowed).

```
# Example 1
nums = [
  [9,9,4],
  [6,6,8],
  [2,1,1]
]
# Return 4
# The longest increasing path is [1, 2, 6, 9].

# Example 2:
nums = [
  [3,4,5],
  [3,2,6],
  [2,2,1]
]
# Return 4
# The longest increasing path is [3, 4, 5, 6]. Moving diagonally is not allowed.
```

Credits: 

Special thanks to @dietpepsi for adding this problem and creating all test cases.

## 0x02 思路提示

第一眼很自然的反应应该是DFS或者BFS， 

前者每次到达终点时记录路径长度，遍历完所有路线后取最大， 

后者记录遍历深度，深度即最大长度， 

但是，肯定会倒在一个递归深度的测试数据面前， 

就算不刻意卡深度，上由于多种编译器在栈深度的限制都会导致stack overflow， 

所以寻找非递归的解决方案才是关键，那么就是**拓扑排序**了， 

这道题难就难在想到这一点（吹，接着吹，这题的Tag里就写了拓扑排序的好吧）， 

那么，怎么把这个矩阵走格子的问题变成图，来使用图论里的拓扑排序呢？ 

矩阵本身就是一张**邻接图**呀， 

那么拓扑排序之后该如何获取最长路径的**长度**呢？ 

这题只需要长度，如果需要返回值是路径经过的**节点**又该怎么处理呢？ 

提示就到这里，去试试看吧。
## 0x03 解题源码

> 
自写代码 Accepted


```python
class Solution:
    def longestIncreasingPath(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: int
        """

        def topsort(G):
            in_degrees = dict((u, 0) for u in G)
            topo_level = dict((u, 0) for u in G)
            for u in G:
                for v in G[u]:
                    in_degrees[v] += 1  # each node's indegree
            S, Q = [], [u for u in G if in_degrees[u] == 0]  # node with indegree 0, get level=0
            while Q:
                u = Q.pop()  # Default remove the end
                S.append(u)
                for v in G[u]:
                    in_degrees[v] -= 1  # Remove this link
                    topo_level[v] = max(topo_level[v], topo_level[u]+1)
                    if in_degrees[v] == 0:
                        Q.append(v)
            return S, list(map(lambda x: topo_level[x], S))

        G = {}  # G means graph
        nodes, links = [], []  # prepare nodes and links for toposort
        row = matrix.__len__()
        if not row: return 0
        column = matrix[0].__len__()
        for j in range(column)[::-1]:
            for i in range(row)[::-1]:
                node_value = matrix[i][j]
                cur_node = "{},{}".format(i,j)
                down_node = "{},{}".format(i+1,j) if i+1<row else None
                right_node = "{},{}".format(i,j+1) if j+1<column else None
                # print(cur_node, down_node, right_node)

                # add nodes
                G[cur_node] = []
                nodes.append(cur_node)

                # then add links
                if down_node:
                    down_value = matrix[i+1][j]
                    if node_value > down_value:
                        links.append((down_node, cur_node))
                        G[down_node].append(cur_node)
                    if down_value > node_value:
                        links.append((cur_node, down_node))
                        G[cur_node].append(down_node)
                if right_node:
                    right_value = matrix[i][j+1]
                    if node_value > right_value:
                        links.append((right_node, cur_node))
                        G[right_node].append(cur_node)
                    if right_value > node_value:
                        links.append((cur_node, right_node))
                        G[cur_node].append(right_node)

        result, topo_level = topsort(G)
        return max(topo_level)+1


s = Solution()
nums = [
  [9,9,4],
  [6,6,8],
  [2,1,1]
]
print(s.longestIncreasingPath(nums))
```

## 0x04 解题报告

### 获取图节点

```
node_value = matrix[i][j]
cur_node = "{},{}".format(i,j)
down_node = "{},{}".format(i+1,j) if i+1<row else None
right_node = "{},{}".format(i,j+1) if j+1<column else None
# print(cur_node, down_node, right_node)

# add nodes
G[cur_node] = []
nodes.append(cur_node)
```

### 获取有向边

这里我采用了从右下向左上遍历节点，每个节点只和右方、下方连边的方式， 

来保证不遗漏、不重复：

```
# then add links
if down_node:
    down_value = matrix[i+1][j]
    if node_value > down_value:
        links.append((down_node, cur_node))
        G[down_node].append(cur_node)
    if down_value > node_value:
        links.append((cur_node, down_node))
        G[cur_node].append(down_node)
if right_node:
    right_value = matrix[i][j+1]
    if node_value > right_value:
        links.append((right_node, cur_node))
        G[right_node].append(cur_node)
    if right_value > node_value:
        links.append((cur_node, right_node))
        G[cur_node].append(right_node)
```

### 实现拓扑排序

并在其基础上魔改两行，增加深度标记：

```python
def topsort(G):
      in_degrees = dict((u, 0) for u in G)
      topo_level = dict((u, 0) for u in G)  # depth level for toposort
      for u in G:
          for v in G[u]:
              in_degrees[v] += 1  # each node's indegree
      S, Q = [], [u for u in G if in_degrees[u] == 0]  # node with indegree 0, get level=0
      while Q:
          u = Q.pop()  # Default remove the end
          S.append(u)
          for v in G[u]:
              in_degrees[v] -= 1  # Remove this link
              topo_level[v] = max(topo_level[v], topo_level[u]+1)  # IMPORTANT MODIFIED POINT
              if in_degrees[v] == 0:
                  Q.append(v)
      return S, list(map(lambda x: topo_level[x], S))
```

### 调用

获得拓扑序及深度标记，取最大深度， 

由于我的深度是从0开始的所以需要加1：

```
result, topo_level = topsort(G)
return max(topo_level)+1
```

## 0x05 后记

这里我特意只使用了拓扑深度来解决题目中需要的长度需求， 

但输出的时候我仅仅使用了第二个返回值， 

那么第一个返回值（拓扑序节点列表）有什么用处呢？ 

是否可以与第二个返回值（节点拓扑深度列表）合作， 

从而得到最长路径经历的节点顺序呢？ 

试试看吧~
## 0x06 应用场景下拓扑排序的使用案例

> 
对于具有单向连边关系的节点，需要按照拓扑排序生成满足`非严格拓扑升序`和`唯一性`的节点序列（并重排节点名称）


```python
def sort_elements_in_answer(dic):
    """
    sort relations in one candidate_answer
    :param dic: candidate_answer as a dict {'M0':{...}, 'M1':{...}}
    :return: a dict with relations ordered
    """

    def cd_topo_sort(G):
        """
        topo_sort implement recently as
        https://blog.csdn.net/okcd00/article/details/79446956
        """
        in_degrees = dict((u, 0) for u in G)
        topo_level = dict((u, 0) for u in G)  # depth level for toposort
        for u in G:
            for v in G[u]:
                in_degrees[v] += 1  # each node's indegree
        S, Q = [], [u for u in G if in_degrees[u] == 0]  # node with indegree 0, get level=0
        while Q:
            u = Q.pop()  # Default remove the end
            S.append(u)
            for v in G[u]:
                in_degrees[v] -= 1  # Remove this link
                topo_level[v] = max(topo_level[v], topo_level[u] + 1)  # IMPORTANT MODIFIED POINT
                if in_degrees[v] == 0:
                    Q.append(v)

        _order = lambda x: topo_level[x]
        return S, list(map(lambda x: (_order(x), x), S))

    if 'Data' in dic:
        dic['Data'] = sort_elements_in_answer(dic.get('Data'))
        return dic

    dic = dict(map(
        lambda x: (x.get('Name'), x),
        sorted(dic.values(), key=lambda x: x.get('Name'))))

    graph = dict([(x, []) for x in dic.keys()])
    for k, v in sorted(dic.items()):
        cause = v.get('C', None)
        if cause and cause.startswith('M'):
            graph[cause].append(k)

    _, topo_list = cd_topo_sort(graph)

    def order(x):
        _node = dic[x]
        _ret = _node.get('R')
        while _node.get('C').startswith('M'):
            _node = dic[_node.get('C')]
            _ret = _ret + _node.get('R')
        # print _node.get('C') + _ret
        return _node.get('C') + _ret

    topo_list.sort(key=lambda x: (x[0], order(x[1])))

    n_len = topo_list.__len__()
    reorder_dict = dict(zip(  # old_node -> new_node
        [topo_list[idx][1] for idx in range(n_len)],
        ["M{}".format(idx) for idx in range(n_len)]))
    reflect_dict = dict([(v, k) for (k, v) in reorder_dict.items()])

    def gen_new_node(idx):
        new_node = "M{}".format(idx)
        old_node = reflect_dict.get(new_node)
        _ret = dic[old_node]
        if _ret.get('C') is None:
            return None
        _ret['C'] = reorder_dict.get(_ret['C'], _ret['C'])
        _ret['Name'] = new_node
        return _ret

    return dict(zip(
        ["M{}".format(idx) for idx in range(n_len)],
        [gen_new_node(idx) for idx in range(n_len)]))


def sort_elements_in_answers(answers):
    return [sort_elements_in_answer(each) for each in
            map(lambda x: x.get('answer', {}), answers)]
```

## 0xFF 呱代码

> 
Accepted @ 2018-03-05


```python
class Solution:
    def longestIncreasingPath(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: int
        """
        def getIndegree(matrix, i, j):
            result = 0
            if i - 1 >= 0 and matrix[i][j] < matrix[i - 1][j]:
                result += 1

            if i + 1 < row and matrix[i][j] < matrix[i + 1][j]:
                result += 1

            if j - 1 >= 0 and matrix[i][j] < matrix[i][j - 1]:
                result += 1

            if j + 1 < col and matrix[i][j] < matrix[i][j + 1]:
                result += 1

            return result

        def delete(point, matrix, shadow_matrix):
            i = point[0]
            j = point[1]
            result = []
            if i - 1 >= 0 and matrix[i][j] > matrix[i - 1][j]:
                shadow_matrix[i-1][j] -= 1
                if shadow_matrix[i-1][j] == 0:
                    result.append([i-1, j])

            if i + 1 < row and matrix[i][j] > matrix[i + 1][j]:
                shadow_matrix[i+1][j] -= 1
                if shadow_matrix[i+1][j] == 0:
                    result.append([i+1, j])

            if j - 1 >= 0 and matrix[i][j] > matrix[i][j - 1]:
                shadow_matrix[i][j-1] -= 1
                if shadow_matrix[i][j-1] == 0:
                    result.append([i, j-1])

            if j + 1 < col and matrix[i][j] > matrix[i][j + 1]:
                shadow_matrix[i][j+1] -= 1
                if shadow_matrix[i][j+1] == 0:
                    result.append([i, j+1])
            return result

        if len(matrix) == 0 or len(matrix[0]) == 0:
            return 0

        shadow_matrix = []
        queue = []
        paths = {}
        row = len(matrix)
        col = len(matrix[0])

        # compute the indegree of each point in the matrix
        for i in range(row):
            shadow_matrix.append([])
            for j in range(col):
                indegree = getIndegree(matrix, i, j)
                shadow_matrix[i].append(indegree)
                if indegree == 0:
                    queue.append([i, j])
                    paths[str([i, j])] = 1

        # topsort:
        # - delete the points of zere indegree and the out edge
        # - add the points effected by delete operation into the queue
        # - repeate until the queue is empty
        while len(queue) != 0:
            point = queue.pop(0)
            effected_points = delete(point, matrix, shadow_matrix)
            if len(effected_points) != 0:
                queue += effected_points
                for ep in effected_points:
                    paths[str(ep)] = paths[str(point)] + 1


        # find maxium path len
        max = 1
        for p in paths:
            if paths[p] > max:
                max = paths[p]
        return max
```





