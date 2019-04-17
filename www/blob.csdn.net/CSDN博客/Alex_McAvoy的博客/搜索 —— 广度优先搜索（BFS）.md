# 搜索 —— 广度优先搜索（BFS） - Alex_McAvoy的博客 - CSDN博客





2018年03月16日 15:18:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：179








# 【算法原理】

    从初始状态S 开始，利用规则，生成所有可能的状态。

    构成的下一层节点，检查是否出现目标状态G，若未出现，就对该层所有状态节点，分别顺序利用规则。

    生成再下一层的所有状态节点，对这一层的所有状态节点检查是否出现G，若未出现，继续按上面思想生成再下一层的所有状态节点，这样一层一层往下展开。直到出现目标状态为止。

    广度优先搜索可以采用循环队列或动态链表来处理。

![](https://img-blog.csdn.net/20180316153419360)

# 【适用范围】

    最短路寻找、图的遍历等

# 【具体过程】

    广度优先即是要按层数一层一层来遍历，先将一层全部扩展，然后再进行下一层。

    过程：
-  每次取出队列首元素（初始状态），进行拓展。
- 把拓展所得到的可行状态都放到队列里面。
- 将初始状态删除。
- 一直进行以上三步直到队列为空。

# 【过程对应队列情况】

![](https://img-blog.csdn.net/20180316153920544)

每个方块表示一个状态，浅蓝色的表示遍历了该状态

![](https://img-blog.csdn.net/20180316154009102)

对应的队列的情况

# 【空间复杂度】

    因为所有节点都必须被储存，因此BFS的空间复杂度为 O(|V| + |E|)，其中 |V| 是节点的数目，而 |E| 是图中边的数目。

    由于对空间的大量需求，因此BFS并不适合解非常大的问题。

    注：另一种说法称BFS的空间复杂度为 O(BM)，其中 B 是最大分支系数，而 M 是树的最长路径长度。

# 【时间复杂度】

    最差情形下，BFS必须寻找所有到可能节点的所有路径。

    因此其时间复杂度为 O(|V| + |E|)，其中 |V| 是节点的数目，而 |E| 是图中边的数目。

# 【搜索框架】

```cpp
int bfs()
{
    初始化，初始状态存入队列；
    队列首指针head=0；
    队列尾指针tail=1；    
    do
    {
        指针head后移一位，指向待扩展结点；
        for(i=1;i<=max;i++)//max为产生子结点的规则数
        {
            if(子结点符合条件)
            {
                	tail指针+1，将新结点存入列尾；
                    if(新结点与原已产生节点重复)
                        删去该结点（取消入队，tail指针-1）；
                    else if(新结点是目标结点)
                        输出并退出；
            }
        }
    }while(head<tail);//队列为空
}
```

# 【例题】
- A strange lift（HDU-1548）：[点击这里](https://blog.csdn.net/u011815404/article/details/79598745)
- Find The Multiple（POJ-1426）：[点击这里](https://blog.csdn.net/u011815404/article/details/84073671)
- 抓住那头牛（信息学奥赛一本通-T1253）：[点击这里](https://blog.csdn.net/u011815404/article/details/80488217)

	同题：Catch That Cow（POJ-3278）：[点击这里](https://blog.csdn.net/u011815404/article/details/80544641)
- Knight Moves（HDU-1372）：[点击这里](https://blog.csdn.net/u011815404/article/details/79588147)

	同题：Knight Moves（信息学奥赛一本通-T1257）：[点击这里](https://blog.csdn.net/u011815404/article/details/80488212)
- 马的遍历（洛谷-P1443）：[点击这里](https://blog.csdn.net/u011815404/article/details/80300086)
- 细胞（信息学奥赛一本通-T1329）：[点击这里](https://blog.csdn.net/u011815404/article/details/80460808)
- Transform（HDU-5637）**(bfs+异或的应用)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86482330)



