# 数据结构 —— RMQ - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:51:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[—————数据结构—————																[数据结构——RMQ](https://blog.csdn.net/u011815404/article/category/8671578)](https://blog.csdn.net/u011815404/article/category/7605501)








# 【概述】

RMQ（Range Minimum/Maximum Query），是对于长度为 n 的数列 A，回答若干次询问 RMQ(i,j)，返回数列 A 中下标在区间 [i,j] 中的最值，即：区间最值查询问题

目前常用于解决 RMQ 问题的方法为 ST 算法（Sparse Table）

# 【ST 算法】

ST 算法可以在 O(nlogn) 时间内进行预处理，然后在 O(1) 时间内回答每个查询，其实际上就是一种数据压缩的思想。

## 1.原理

### 1）预处理

设 A[i] 是要求区间最值的数列，F[i,j] 表示从第 i 个数起连续 2^j 个数中的最大值。**（DP的状态）**

可以看出 F[i,0] = A[i]**（DP的初始值）**

把 F[i,j] 平均分成两段 ( F[i,j] 一定是偶数个数字)，从** i 到 i+2^(j-1)-1** 为一段，**i+2^(j-1) 到 i+2^j-1** 为一段，长度均为** 2^(j-1)**

于是得到：**F[i,j] = max( F[i , j-1] , F[i+2^(j-1) , j-1] ）(状态转移方程****)**

例如：

A 为：3 2 4 5 6 8

F[1,0] 表示第 1 个数起，长度为 2^0=1 的最大值，其实就是 3

F[1,1] = max(3,2) = 3

F[1,2] = max(3,2,4,5) = 5

F[1,3] = max(3,2,4,5,6,8,1,2) = 8

### 2）查询

假设要查询的区间为 (i,j)，那么需要找到覆盖这个闭区间 [i,j] 的最小幂，由于区间长度为 j-i+1，因此可以取 k=log2(j-i+1)，则有：RMQ(i, j) = max{ F[i,k] , F[ j-2^k+1,k] }

例：要求区间 [1,5] 的最大值

有：k = log2(5-1+1) = 2

则：RMQ(1,5) = max( F[1,2] , F[5-2^2+1, 2]) = max(F[1,2] , F[2,2])

## 2.实现

```cpp
int dpMax[N][20];
int dpMin[N][20];
int a[N];
void initMax(int n){//初始化最大值查询
    for(int i=1;i<=n;i++)
        dpMax[i][0]=a[i];
    for(int j=1;(1<<j)<=n;j++)
        for(int i=1;i+(1<<j)-1<=n;i++)
            dpMax[i][j]=max(dpMax[i][j-1],dpMax[i+(1<<(j-1))][j-1]);
}
int getMax(int L,int R){//查询最大值
    //int k = (int)(log10(R-L+1)/log10(2));
    int k=0;
    while((1<<(k+1))<=R-L+1)
        k++;
    return max(dpMax[L][k],dpMax[R-(1<<k)+1][k]);
}

void initMin(int n){//初始化最小值查询
    for(int i=1;i<=n;i++)
        dpMin[i][0]=a[i];
    for(int j=1;(1<<j)<=n;j++)
        for(int i=1;i+(1<<j)-1<=n;i++)
            dpMin[i][j]=min(dpMin[i][j-1],dpMin[i+(1<<(j-1))][j-1]);
}
int getMin(int L,int R){//查询最小值
    int k=0;
    while((1<<(k+1))<=R-L+1)
        k++;
    return min(dpMin[L][k],dpMin[R-(1<<k)+1][k]);
}
```

# 【二维 RMQ】

一维 RMQ 问题是求一个数列 A 中的最值，而二维 RMQ 问题是求一个 n*m 的矩阵中，某个子矩阵内的最值

## 1.原理

### 1）初始化

设 F[i][j][ii][jj] = x 表示以 (i, j) 为左上角，以 (i+(1<<ii)-1, j+(1<<jj)-1) 为右下角的矩阵内的最大值**（DP的状态）**

易知：F[i][j][0][0] = G[i][j]**（DP的初始值）**

假设 F[i][j][ii][jj] 中的** ii 不为 0**，那么 F[i][j][ii][jj] = max(F[i][j][ii-1][jj], F[i+(1<<ii)][j][ii-1][jj] ) **(状态转移方程****)**

如果 **ii 为 0**，那么就按 jj 来求，即 F[i][j][ii][jj] = max(F[i][j][ii][jj-1] , F[i][j+(1<<(jj-1))][ii][jj-1]) **(状态转移方程****)**

简单来说，就是将二维问题转变为一维问题求解

### 2）查询

对于一个以 (x1,y1) 为左上角，以 (x2,y2) 为右下角的矩形，将其分成四小块，这四小块可能有重合部分，但他们共同构成了目标矩形：
- F[x1][y1][ii][jj]
- F[x1][y2-(1<<jj)+1][ii][jj]
- F[x2-(1<<ii)+1][y1][ii][jj]
- F[x2-(1<<ii)+1][y2-(1<<jj)+1][ii][jj]

那么，可先求解第一、二小块的最值，第三、四小块的最值，最后再求整体最值

即：
- temp1 = max(F[x1][y1][ii][jj] , F[x1][y2-(1<<jj)+1][ii][jj])
- temp2 = max(F[x2-(1<<ii)+1][y1][ii][jj] ,F[x2-(1<<ii)+1][y2-(1<<jj)+1][ii][jj] )

最终结果为：res = max(temp1,temp2)

## 2.实现

```cpp
int G[N][N];
int dpMin[N][N][10][10];
int dpMax[N][N][10][10];
void initRMQ(int n,int m){//对n*m的矩阵初始化RMQ且矩阵下标从1开始

    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            dpMin[i][j][0][0]=dpMax[i][j][0][0]=G[i][j];

    for(int ii=0;(1<<ii)<=n;ii++){
        for(int jj=0;(1<<jj)<=m;jj++){
            if(ii+jj){
                for(int i=1;i+(1<<ii)-1<=n;i++){
                    for(int j=1;j+(1<<jj)-1<=m;j++){
                        if(ii){
                            dpMin[i][j][ii][jj] = min(dpMin[i][j][ii-1][jj] , dpMin[i+(1<<(ii-1))][j][ii-1][jj]);
                            dpMax[i][j][ii][jj] = max(dpMax[i][j][ii-1][jj] , dpMax[i+(1<<(ii-1))][j][ii-1][jj]);
                        }
                        else{
                            dpMin[i][j][ii][jj] = min(dpMin[i][j][ii][jj-1] , dpMin[i][j+(1<<(jj-1))][ii][jj-1]);
                            dpMax[i][j][ii][jj] = max(dpMax[i][j][ii][jj-1] , dpMax[i][j+(1<<(jj-1))][ii][jj-1]);
                        }
                    }
                }
            }
        }
    }
}
int getMax(int x1,int y1,int x2,int y2){//RMQ查询最大值
    int k1=0,k2=0;
    while((1<<(k1+1))<=x2-x1+1)
        k1++;
    while((1<<(k2+1))<=y2-y1+1)
        k2++;

    x2=x2-(1<<k1)+1;
    y2=y2-(1<<k2)+1;
    int temp1=max(dpMax[x1][y1][k1][k2],dpMax[x1][y2][k1][k2]);
    int temp2=max(dpMax[x2][y1][k1][k2],dpMax[x2][y2][k1][k2]);

    return max(temp1,temp2);
}
int getMin(int x1,int y1,int x2,int y2){//RMQ查询最小值
    int k1=0,k2=0;
    while((1<<(k1+1))<=x2-x1+1)
        k1++;
    while((1<<(k2+1))<=y2-y1+1)
        k2++;

    x2=x2-(1<<k1)+1;
    y2=y2-(1<<k2)+1;
    int temp1=min(dpMin[x1][y1][k1][k2],dpMin[x1][y2][k1][k2]);
    int temp2=min(dpMin[x2][y1][k1][k2],dpMin[x2][y2][k1][k2]);

    return min(temp1,temp2);
}
```

# 【例题】
- Find the hotel（HDU-3193）**(RMQ 模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/87368522)
- Check Corners（HDU-2888）**(二维 RMQ 模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/87360981)
- A Magic Lamp（HDU-3183）**(RMQ +模拟 )**：[点击这里](https://blog.csdn.net/u011815404/article/details/87375205)




