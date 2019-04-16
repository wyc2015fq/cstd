# leetcode 684. 冗余链接 802. 找到最终的安全状态 - 彩虹糖的博客 - CSDN博客





2019年02月23日 18:19:53[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：44








在本问题中, 树指的是一个连通且无环的**无向**图。

输入一个图，该图由一个有着N个节点 (节点值不重复1, 2, ..., N) 的树及一条附加的边构成。附加的边的两个顶点包含在1到N中间，这条附加的边不属于树中已存在的边。

结果图是一个以`边`组成的二维数组。每一个`边`的元素是一对`[u, v]` ，满足 `u < v`，表示连接顶点`u` 和`v`的**无向**图的边。

返回一条可以删去的边，使得结果图是一个有着N个节点的树。如果有多个答案，则返回二维数组中最后出现的边。答案边 `[u, v]` 应满足相同的格式 `u < v`。

**示例 1：**
**输入:** [[1,2], [1,3], [2,3]]
**输出:** [2,3]
**解释:** 给定的无向图为:
  1
 / \
2 - 3

**示例 2：**
**输入:** [[1,2], [2,3], [3,4], [1,4], [1,5]]
**输出:** [1,4]
**解释:** 给定的无向图为:
5 - 1 - 2
    |   |
    4 - 3

**注意:**
- 输入的二维数组大小在 3 到 1000。
- 二维数组中的整数在1到N之间，其中N是输入数组的大小。



# 代码

```java
class Solution {
    
    public class WeightedQuickUnionUF {
        private int[] parent;
        private int[] size;
 
        public WeightedQuickUnionUF(int n) {
            parent = new int[n];
            size = new int[n];
            for (int i = 0; i < n; i++) {
                parent[i] = i;
                size[i] = 1;
            }
        }
 
        public int find(int p) {
            while (p != parent[p])
                p = parent[p];
            return p;
        }
 
        public boolean connected(int p, int q) {
            return find(p) == find(q);
        }
        public void union(int p, int q) {
            int rootP = find(p);
            int rootQ = find(q);
            if (rootP == rootQ) return;
 
            if (size[rootP] < size[rootQ]) {
                parent[rootP] = rootQ;
                size[rootQ] += size[rootP];
            }
            else {
                parent[rootQ] = rootP;
                size[rootP] += size[rootQ];
            }
        }
 
    }

    public int[] findRedundantConnection(int[][] edges) {
        WeightedQuickUnionUF uf = new WeightedQuickUnionUF(edges.length);
        int[] result = new int[2];
        for(int i=0;i<edges.length;i++){
            int vertex1 = edges[i][0]-1;
            int vertex2 = edges[i][1]-1;
            if(!uf.connected(vertex1,vertex2)){
                uf.union(vertex1,vertex2);
            }
            else{
                result[0]=vertex1+1;
                result[1]=vertex2+1;
                break;
            }
        }
        return result;
    }
}
```

在有向图中, 我们从某个节点和每个转向处开始, 沿着图的有向边走。 如果我们到达的节点是终点 (即它没有连出的有向边), 我们停止。

现在, 如果我们最后能走到终点，那么我们的起始节点是*最终安全*的。 更具体地说, 存在一个自然数 `K`,  无论选择从哪里开始行走, 我们走了不到 `K` 步后必能停止在一个终点。

哪些节点最终是安全的？ 结果返回一个有序的数组。

该有向图有 `N` 个节点，标签为 `0, 1, ..., N-1`, 其中 `N` 是 `graph` 的节点数.  图以以下的形式给出: `graph[i]` 是节点 `j`的一个列表，满足 `(i, j)` 是图的一条有向边。
**示例：****输入：**graph = [[1,2],[2,3],[5],[0],[5],[],[]]
**输出：**[2,4,5,6]
这里是上图的示意图。


![Illustration of graph](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/03/17/picture1.png)

**提示：**
- `graph` 节点数不超过 `10000`.
- 图的边数不会超过 `32000`.
- 每个 `graph[i]` 被排序为不同的整数列表， 在区间 `[0, graph.length - 1]` 中选取。

# 代码

```java
class Solution {
    public boolean hasCircle = false;
    public void dfs(int i, int[][]graph, int[] visited, int[] onStack){
        visited[i]=1;
        onStack[i]=1;
        for(int j:graph[i]){
            if(visited[j]==0){
                dfs(j,graph,visited,onStack);
            }else if(onStack[j]==1){
                hasCircle = true;
                break;
            }
        }
        onStack[i]=0;
    }
    public List<Integer> eventualSafeNodes(int[][] graph) {
        List<Integer> results = new ArrayList<Integer>();
        for(int i=0;i<graph.length;i++){
            int[] visited = new int[graph.length];
            int[] onStack = new int[graph.length];
            dfs(i,graph,visited,onStack);
            if(!hasCircle){
                results.add(i);
            }
            hasCircle = false;
        }
        return results;
    }
}
```





