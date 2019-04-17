# 动态规划 ——  状压 DP - Alex_McAvoy的博客 - CSDN博客





2018年08月21日 09:53:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：115
个人分类：[—————动态规划—————																[动态规划——状压 DP](https://blog.csdn.net/u011815404/article/category/7955244)](https://blog.csdn.net/u011815404/article/category/7540247)








# 【概述】

通常将以一个集合内的元素信息作为状态且状态总数为指数级别的动态规划称为状态压缩动态规划。

其是一类以集合信息为状态的特殊的动态规划问题，主要有传统集合动态规划与基于连通性状态压缩的动态规划两种。

其原理是通过二进制位运算将状态压缩（用整数表示集合）作为动态规划的状态来解决问题。

通常具备以下两个特点：
- 数据规模的某一维或几维特别小
- 需要具备动态规划问题的两个基本性质：最优性原理、无后效性原则

为更好的理解状压 DP，首先要有相关的位运算知识，关于位运算：[点击这里](https://blog.csdn.net/u011815404/article/details/81902087)。

# 【使用条件】

在任意时刻，已经求出最优解的状态与尚未求出最优解的状态在各维度上的分界点组成了 DP 扩展的轮廓，对于某些问题，需要在动态规划的状态中记录一个集合，保存这个轮廓的详细信息，以便进行状态转移。

若集合大小不超过 n，集合中每个元素都是小于 k 的自然数，则可以把这个集合看作一个 n 位 k 进制数，以一个 ![[0,k^n-1]](https://private.codecogs.com/gif.latex?%5B0%2Ck%5En-1%5D) 之间的十进制整数的形式作为 DP 状态的一维。

# 【使用技巧】

1.状压 DP 题型

1）数据 n≤16 ，但状态总数可达指数级

2）满足无后效性原则，通过前面的状态知道后面的怎么选，用 1、0 来记录状态是否存在

2.一般定义二维数组

第一维是第几排（如：铺砖块等）

第二维标识状态，0 或 1

3.数组初始化

根据题设设置初值，一般为 memset(dp,127,sizeof(dp));

再进行状态更新：

```cpp
for(int i=0;i<n;i++)   
    dp[i][1<<i]=0;
```

表示 i 在 state 中存在，这里的 1<<i 就是一种只选 i 的 state

4.枚举状态循环求值

```cpp
for(int i=1;i<=n;i++)
{
    for(int j=0;j<(1<<m);j++)
    {
        if(check(i,j))//判断当前行满足条件的state
        {
            for(int k=0;k<(1<<m);k++)//枚举上一行的pre_state进行更新
            {
                ...
            }
        }
    }
}
```

5.判断状态中的每一个是否符合要求

```cpp
for(int i=sum;i!=0;i=(i-1)&sum)
{
    ...
}
```

# 【例题】

## 1.入门题
- Corn Fields（POJ-3254）：[点击这里](https://blog.csdn.net/u011815404/article/details/81915330)
- 最小总代价（洛谷-U17433）：[点击这里](https://blog.csdn.net/u011815404/article/details/81905372)
- Necklace（HDU-3091）：[点击这里](https://blog.csdn.net/u011815404/article/details/82935235)
- Most Powerful（ZOJ-3471）：[点击这里](https://blog.csdn.net/u011815404/article/details/81940941)

## 2.TSP 问题
- Hie with the Pie（POJ-3311）**(状压+Floyd求最短路)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81915329)
- Traveling（HDU-3011）**(三进制的压缩)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81943079)
- Tourism Planning（HDU-4096）**(注意满意度的计算)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82927687)

## 3.其他
- 炮兵阵地（POJ-1185）**(三层状态的判断)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81906430)
- Pieces（HDU-4628）**(回文字串的判断)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82936564)
- Mondriaan's Dream（POJ-2411）**(贴砖块问题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81904893)
- Doing Homework（HDU-1074）**(状压+递归输出)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82934722)
- 方格取数(1)（HDU-1565）**(需进行预处理，计算合法数据个数)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82934766)
- Riddle（2018 CCPC (秦皇岛站) I 题）**(互斥的状态)**：[点击这里](https://blog.csdn.net/u011815404/article/details/85610072)




