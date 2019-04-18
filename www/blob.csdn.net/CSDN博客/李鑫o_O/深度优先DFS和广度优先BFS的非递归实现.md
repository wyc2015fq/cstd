# 深度优先DFS和广度优先BFS的非递归实现 - 李鑫o_O - CSDN博客

2016年08月09日 16:32:58[hustlx](https://me.csdn.net/HUSTLX)阅读数：3377


```cpp
void DFS(Node root)   //非递归实现
{
    stack<Node> s;
    root.visited = true;
    printf("%d ", root.val);     //访问
    s.push(root);              //入栈
    while (!s.empty())
    {
        Node pre= s.top();          //取栈顶顶点
        int j;
        for (j = 0; j<pre.adjacent.size(); j++)  //访问与顶点i相邻的顶点
        {
            Node cur = pre.adjacent[j];
            if (cur.visited == false)
            {
                printf("%d ", cur.val);     //访问
                cur.visited = true;
                s.push(cur);           //访问完后入栈
                break;               //找到一个相邻未访问的顶点，访问之后则跳出循环
            }
        }
        //对于节点4，找完所有节点发现都已访问过或者没有临边，所以j此时=节点总数，然后把这个4给弹出来
        //直到弹出1，之前的深度搜索的值都已弹出，有半部分还没有遍历，开始遍历有半部分
        if (j == pre.adjacent.size())                   //如果与i相邻的顶点都被访问过，则将顶点i出栈
            s.pop();
    }
}
void BFS(Node root) {
    queue<Node> q;
    root.visited = true;
    printf("%d ", root.val);     //访问
    q.push(root);              //入栈
    while (!q.empty()) {
        Node pre = q.front();
        q.pop();
        for (Node cur : pre.adjacent) {
            if (cur.visited == false) {
                printf("%d ", cur.val);     //访问
                cur.visited = true;
                q.push(cur);
            }
        }
    }
}
```

