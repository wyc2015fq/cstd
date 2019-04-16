# Leetcode 785. 判断二分图 - 彩虹糖的博客 - CSDN博客





2019年02月22日 20:39:02[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：22








# 题目描述

给定一个无向图`graph`，当这个图为二分图时返回`true`。

如果我们能将一个图的节点集合分割成两个独立的子集A和B，并使图中的每一条边的两个节点一个来自A集合，一个来自B集合，我们就将这个图称为二分图。

`graph`将会以邻接表方式给出，`graph[i]`表示图中与节点`i`相连的所有节点。每个节点都是一个在`0`到`graph.length-1`之间的整数。这图中没有自环和平行边： `graph[i]` 中不存在`i`，并且`graph[i]`中没有重复的值。

```
**示例 1:**
输入**:** [[1,3], [0,2], [1,3], [0,2]]
**输出:** true
**解释:** 
无向图如下:
0----1
|    |
|    |
3----2
我们可以将节点分成两组: {0, 2} 和 {1, 3}。
```

```
**示例 2:**
**输入:** [[1,2,3], [0,2], [0,1,3], [0,2]]
**输出:** false
**解释:** 
无向图如下:
0----1
| \  |
|  \ |
3----2
我们不能将节点分割成两个独立的子集。
```

**注意:**
- `graph` 的长度范围为 `[1, 100]`。
- `graph[i]` 中的元素的范围为 `[0, graph.length - 1]`。
- `graph[i]` 不会包含 `i` 或者有重复的值。
- 图是无向的: 如果`j` 在 `graph[i]`里边, 那么 `i` 也会在 `graph[j]`里边。



# 题目分析

这其实是一道挺简单的题目的，使用深度优先搜索遍历图，并在遍历过程中为图涂色。如果涂色始终满足同样颜色的点不邻接，那么着就是一个二分图。

写的有点复杂，对于图的遍历要熟练掌握。

# 代码

```java
class Solution {
    public boolean isConnected(int[] elements, int target) {
        if (elements.length == 0){
            return true;

        }
        for (int i : elements) {
            if (i == target)
                return true;
        }
        return false;
    }

    public boolean isValid(int[] elements, int target, int[] visited) {
        boolean flag = true;
        for (int i : elements) {
            if (visited[i] != -visited[target] && visited[i] != 0) {
                flag = false;
                break;
            }

        }
        return flag;
    }

    public int chooseStart(int[] visited) {
        for (int i = 0; i < visited.length; i++) {
            if (visited[i] == 0)
                return i;
        }
        return -1;
    }

    public boolean isBipartite(int[][] graph) {
        int visitCounter = 1;
        int counter = 1;
        Stack<Integer> stack = new Stack<Integer>();
        int[] visited = new int[graph.length];
        stack.push(0);
        visited[0] = 1;
        for (int i = 0; ; i++) {
            int currentElement = 0;
            if (stack.size() > 0)
                currentElement = stack.peek();
            else {
                int unVisited = chooseStart(visited);
                if (unVisited == -1)
                    break;
                else {
                    currentElement = unVisited;
                    visited[currentElement]=1;
                    visitCounter++;
                    counter++;
                    stack.push(currentElement);
                }
                i = 0;
            }
            if (isConnected(graph[currentElement], i)&&i < visited.length && visited[i] == 0 ) {


                    visited[i] = -visited[currentElement];
                    visitCounter++;
                    if (!isValid(graph[i], i, visited))
                        break;
                    counter++;
                    if (visitCounter == graph.length)//if(stack.peek()==0)
                        break;
                    stack.push(i);
                    i = 0;

            }
            if (i == graph.length) {
                i = 0;
                if (visitCounter == graph.length)//if(stack.peek()==0)
                    break;
                else
                    stack.pop();
            }
        }
        if (counter == graph.length) {
            return true;
        } else {
            return false;
        }
    }


}
```





