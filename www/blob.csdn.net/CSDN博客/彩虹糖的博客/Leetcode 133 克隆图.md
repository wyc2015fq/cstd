# Leetcode 133. 克隆图 - 彩虹糖的博客 - CSDN博客





2019年02月25日 20:47:26[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：25标签：[图																[深度优先																[leetcode																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[数据结构与算法](https://blog.csdn.net/caozixuan98724/article/category/7297182)





给定无向[**连通**](https://baike.baidu.com/item/%E8%BF%9E%E9%80%9A%E5%9B%BE/6460995?fr=aladdin)图中一个节点的引用，返回该图的[**深拷贝**](https://baike.baidu.com/item/%E6%B7%B1%E6%8B%B7%E8%B4%9D/22785317?fr=aladdin)（克隆）。图中的每个节点都包含它的值 `val`（`Int`） 和其邻居的列表（`list[Node]`）。

**示例：**

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/23/113_sample.png)
**输入：**{"$id":"1","neighbors":[{"$id":"2","neighbors":[{"$ref":"1"},{"$id":"3","neighbors":[{"$ref":"2"},{"$id":"4","neighbors":[{"$ref":"3"},{"$ref":"1"}],"val":4}],"val":3}],"val":2},{"$ref":"4"}],"val":1}

**解释：**
节点 1 的值是 1，它有两个邻居：节点 2 和 4 。
节点 2 的值是 2，它有两个邻居：节点 1 和 3 。
节点 3 的值是 3，它有两个邻居：节点 2 和 4 。
节点 4 的值是 4，它有两个邻居：节点 1 和 3 。



**提示：**
- 节点数介于 1 到 100 之间。
- 无向图是一个[简单图](https://baike.baidu.com/item/%E7%AE%80%E5%8D%95%E5%9B%BE/1680528?fr=aladdin)，这意味着图中没有重复的边，也没有自环。
- 由于图是无向的，如果节点 *p* 是节点 *q* 的邻居，那么节点 *q* 也必须是节点 *p* 的邻居。
- 必须将**给定节点的拷贝**作为对克隆图的引用返回。

# 题目分析

我的解法比较糟糕。首先进行了一遍DFS，构造一个数组，里面包含所有节点，值已经确定，但是neighbors为空。

再对这个数组进行遍历，把点与点之间的连接信息加入数组中。

将目标节点返回。

# 代码

```java
class Solution {
    public ArrayList<Node> nodes = new ArrayList<Node>();
    public ArrayList<Integer> visited = new ArrayList<Integer>();
    
    public boolean isVisited(int val){
       
        for(Integer i:visited)
        {
            if(i==val){
                return true;
            }
        }
        return false;
    }
    
    public void dfs(Node node){
        for(Node n:node.neighbors)
        {
            if(!visited.contains(node.val)){
                visited.add(node.val);
                nodes.add(node);
            }
            if(!isVisited(n.val))
            {
                dfs(n);
            }
                
        }

    }

    
    public Node getNode(Node[] nodes, int val)
    {
        for(Node n:nodes)
        {
            if(n.val==val){
                return n;
            }
        }
        return null;
    }
    public Node cloneGraph(Node node) {
        visited.add(node.val);
        nodes.add(node);
        dfs(node);
        int totalNum = visited.size();
        Node[] newNodes = new Node[totalNum];

        for(int i=0;i<totalNum;i++)
        {
            ArrayList<Node> neighbors = new  ArrayList<Node>();
            newNodes[i] = new Node(visited.get(i),neighbors);
        }
        
        Node[] nodes_array = new Node[nodes.size()];
        for(int i=0;i<nodes.size();i++)
        {
            nodes_array[i]=nodes.get(i);
        }
        
        for(Node n:newNodes)
        {
            Node originNode = getNode(nodes_array, n.val);
            for(Node x:originNode.neighbors)
            {
                n.neighbors.add(getNode(newNodes, x.val));
            }
        }

        Node result = getNode(newNodes,node.val);
        return result;
    }
}
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=深度优先&t=blog)](https://so.csdn.net/so/search/s.do?q=图&t=blog)




