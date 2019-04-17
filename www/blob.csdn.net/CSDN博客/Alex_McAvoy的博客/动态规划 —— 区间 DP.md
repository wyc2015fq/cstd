# 动态规划 ——  区间 DP - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 10:14:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：128
个人分类：[—————动态规划—————																[动态规划——区间 DP](https://blog.csdn.net/u011815404/article/category/7813197)](https://blog.csdn.net/u011815404/article/category/7540247)








# 【概述】

区间型动态规划，又称为合并类动态规划，是线性动态规划的扩展，它在分阶段地划分问题时，与阶段中元素出现的顺序和由前一阶段的区间中哪些元素合并而来有很大的关系。

实质上就是在一个区间上进行的动态规划，不仅要满足 DP 问题的最优子结构，还要符合在区间上操作的特点。

其主要思想就是在小区间进行 DP 得到最优解，然后再利用小区间的最优解合并求大区间的最优解。

往往会枚举区间，将区间分成左右两部分，再求出左右区间进行合并操作。这样一来，如果要得知一个大区间的情况，由于它必定是由从多个长度不一的小区间转移而来，那么可以通过求得多个小区间的情况，从而合并信息，得到大区间。

即：已知区间长度为 len-1 的所有状态，然后可以通过小于 len 的状态转移到区间长度为 len 的状态，一般是在外层循环遍历 len，内层循环遍历起点来解决

# 【过程】

```cpp
for k:=1 to n-1 do           //区间长度
  for i:=1 to n-k do         //区间起点
    for j:=i to i+k-1 do     //区间中任意点
      dp[i,i+k]:=max{dp[i,j] + dp[j+1,i+k] + a[i,j] + a[j+1,i+k]};
```

状态转移方程：寻找区间 dp[i,i+k] 的一种合并方式 dp[i,j] + dp[j+1,i+k]，使得其值最大或最小

区间长度 k 必须要放到第一层循环，来保证方程中状态 dp[i,j]、dp[j+1,i+k] 值在 dp[i,i+k] 之前就已计算出来

其中 a[i,j]+a[j+1,i+k] 可以灵活多变，其指的是合并区间时产生的附加值

# 【平行四边形不等式优化】

对于一个长度为 n 的区间，根据子区间的划分，时间复杂度会达到 O(n^3)，但根据平行四边形不等式优化，时间复杂度会达到 O(n^2)

## 1.性质

1）区间包含的单调性：如果对于 i≤i'<j≤j'，有：w(i',j)≤w(i,j')，那么说明 w 具有区间包含的单调性。（可以理解为如果小区间包含于大区间中，那么小区间的w值不超过大区间的w值）

2）四边形不等式：如果对于 i≤i'<j≤j'，有：w(i,j)+w(i',j')≤w(i',j)+w(i,j')，我们称函数 w 满足四边形不等式。（可以理解为两个交错区间的w的和不超过小区间与大区间的w的和）

## 2.定理

假设现有一个函数：m(i,j)=min｛m(i,k-1),m(k,j)｝+w(i,j)（i≤k≤j），定义 s(i,j) 表示 m(i,j) 取得最优值时对应的下标（即：i≤k≤j 时，k处的w值最大，则s(i,j)=k）

1）如果上述的 w 函数同时满足区间包含单调性和四边形不等式性质，那么函数 m 也满足四边形不等式性质。

2）假如 m(i,j) 满足四边形不等式，那么 s(i,j) 单调，即：s(i,j)≤s(i,j+1)≤s(i+1,j+1)

## 3.实现

如何一个 w 函数满足区间包含单调性与四边形不等式性质，那么就可以使用优化，此时有：s(i,j-1)≤s(i,j)≤s(i+1,j)。

简单来说，就是：交叉小于包含，即交叉的两个区间，a 到 c 和 b 到 d 的值满足小于等于包含的两个区间 [bc包含于ad]

由于状态转移时是三重循环的，我们可以对枚举分割点一重循环进行优化，所以可以把这个点找到后保存下来，即：通过记录子区间的最优决策来减少当前的决策量。

如果状态转移方程为 m 且决策代价 w 满足四边形不等式的单调函数，则可利用四边形不等式推出最优决策 s 的单调函数性，从而减少每个状态的状态数，用 s[i][j] 表示区间 [i,j] 中的最优分割点，那么第三重循环可以从 [i,j-1) 优化到 ｛ s[i][j-1],s[i+1][j] ｝

即：s[i][j]=max{k | ma[i][j] = m[i][k-1] + m[k][j] + w[i][j]}（ (s(i,j-1)≤k≤s(i+1,j) ）

此时，小区间 s[i][j-1] 和 s[i+1][j] 的值已经求出来了，然后通过这个循环又可以得到 s[i][j] 的值

# 【模板】

```cpp
int dp[N][N];

for(int i=1;i<=n;i++)
    dp[i][i]=初始值

for(int len=2;len<=n;len++)      //区间长度
{
    for(int i=1;i<=n;i++)        //枚举起点
    {
        int j=i+len-1;           //区间终点
        if(j>n) break;           //越界结束
        for(int k=i;k<j;k++)     //枚举分割点，构造状态转移方程
            dp[i][j]=max(dp[i][j],dp[i][k]+dp[k+1][j]+w[i][j]);
    }
}
```

# 【例题】

## 1.石子合并
- 合并石子（信息学奥赛一本通-T1274）**(直线合并求最大)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80573797)
- 石子合并问题--直线版（Hrbust-1818）**(直线合并求最小)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82285641)
- 石子合并（洛谷-P1880）**(环形合并)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81773222)
- 能量项链（NOIP-2006 提高组）**(环形合并，较难)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82320603)

扩展：[石子合并三讲](https://blog.csdn.net/u011815404/article/details/81773570)

## 2.整数划分
- Multiplication Puzzle（POJ-1651）**(模板题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81779345)
- 传球游戏（洛谷-P1057）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81808364)
- 释放囚犯（洛谷-P1622）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81775439)
- Halloween Costumes（LightOJ-1422）**(入门题，较难)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82151693)
- Running（POJ-3661）**(跑和休息分别进行状态转移)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80501392)
- You Are the One（HDU-4283）**(栈过程的模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81778835)

## 3.字串处理
- Palindrome subsequence（HDU-4632）**(求回文串数目)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81775720)
- Two Rabbits（HDU-4757）**(非连续最长回文子序列)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81776231)
- String painter（HDU-2476）**(两次DP)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82285361)
- Cheapest Palindrome（POJ-3280）**(存在递推关系，加上减去字符成本是等价的)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80609165)
- The Cow Lexicon（POJ-3267）**(字符匹配)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80795155)

## 4.括号匹配
- Brackets（POJ-2955）**(数字和的形式匹配)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81805860)
- Brackets Sequence（POJ-1141）**(递归输出)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81805648)




