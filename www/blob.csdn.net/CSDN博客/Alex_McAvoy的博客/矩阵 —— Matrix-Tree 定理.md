# 矩阵 —— Matrix-Tree 定理 - Alex_McAvoy的博客 - CSDN博客





2019年04月08日 15:24:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30








# 【概述】

Matrix-Tree 定理又称基尔霍夫矩阵树定理，其用于解决：**给定 n 个点 m 条边的无向图，求图的生成树个数**的问题。

# 【矩阵的行列式】

## 1.行列式

排成 n 阶方阵形式的 n^2 个数所确定的一个数称为 **n 阶方阵 A 的行列式**，记为：det(A) 或 |A|

一个 2x2 的矩阵的行列式可表示为：![det\begin{pmatrix} a&b \\ c&d \end{pmatrix}=ad-bc](https://private.codecogs.com/gif.latex?det%5Cbegin%7Bpmatrix%7D%20a%26b%20%5C%5C%20c%26d%20%5Cend%7Bpmatrix%7D%3Dad-bc)

## 2.余子式与代数余子式

将 n 阶行列式中元素 ![a_{ij }](https://private.codecogs.com/gif.latex?a_%7Bij%20%7D) 的第 i 行和第 j 列划去后，留下的 n-1 阶行列式称为 ![a_{ij }](https://private.codecogs.com/gif.latex?a_%7Bij%20%7D) 的**余子式** ，记作：![M_{ij}](https://private.codecogs.com/gif.latex?M_%7Bij%7D)

将 ![a_{ij }](https://private.codecogs.com/gif.latex?a_%7Bij%20%7D) 的余子式与 -1 的 i+j 的幂的乘积称为**代数余子式**，记作：![A_{ij}=(-1)^{i+j}M_{ij}](https://private.codecogs.com/gif.latex?A_%7Bij%7D%3D%28-1%29%5E%7Bi&plus;j%7DM_%7Bij%7D)

一个 n 阶方阵的行列式等于任意行/列的元素与对应的代数余子式乘积之和，即：![det(A)=\sum_{j=1}^na_{ij}(-1)^{i+j}det(A_{ij})](https://private.codecogs.com/gif.latex?det%28A%29%3D%5Csum_%7Bj%3D1%7D%5Ena_%7Bij%7D%28-1%29%5E%7Bi&plus;j%7Ddet%28A_%7Bij%7D%29)

## 4.主子式与顺序主子式

在 n 阶行列式中，选取 k 个行号，再选取与行号相同的 k 个列号，则有行列均为 k 个的行列式即为** n 阶行列式的 k 阶主子式**，简单来说，即在 n 阶行列式中，选取的 k 个行列号相同的行、列的交汇处的元素组成的行列式

在 k 阶行列式中，由 1~k 行和 1~k 列组成的子式，即为 **n 阶行列式的 k 阶顺序主子式**

## 5.行列式的性质
- 互换矩阵的两行(列)，行列式变号
- 如果矩阵有两行(列)完全相同，则行列式为 0
- 如果矩阵的某一行(列)中的所有元素都乘以同一个数 k，新行列式的值等于原行列式的值乘上数 k

	推论：如果矩阵的某一行(列)中的所有元素都有一个公因子 k，则可以把这个公因子 k 提到行列式求和式的外面
- 如果矩阵有两行(列)成比例(比例系数k)，则行列式的值为 0
- 如果把矩阵的某一行(列)加上另一行(列)的 k 倍，则行列式的值不变

## 5.主子式的值

对于一个主子式的值，可以根据其定义算出：![det(A)=\sum_p((-1)^{\tau (P)}*A_{1,p1}*A_{2,p2}*...*A_{n,pn})](https://private.codecogs.com/gif.latex?det%28A%29%3D%5Csum_p%28%28-1%29%5E%7B%5Ctau%20%28P%29%7D*A_%7B1%2Cp1%7D*A_%7B2%2Cp2%7D*...*A_%7Bn%2Cpn%7D%29)

其中 P 为 1~n 的一个排列，τ(P) 为排列 P 的逆序对数，求和式的每一项可以看做在矩阵中选出 n 个数，使得他们的行列都不重合，显然，求和式共 n！项，根据定义求值的时间复杂度是 O(n!) 阶的，因此必须根据行列式的性质进行优化。

由于对于任意一个上(下)三角矩阵，其行列式的值为对角线的乘积。

因此可根据性质 5，可采用高斯消元的方法，将矩阵消为一个上三角矩阵后，求出对角线的积，即为该矩阵的行列式的值，时间复杂度为 O(n^3)

如果要求的矩阵不允许出现实数，且需要取模，那么采用辗转相除的高斯消元法，时间复杂度多出一个 O(logN)

```cpp
int f[N][N];//n阶矩阵
int det(int n){
    int res=1;
    for(int i=1;i<=n-1;i++){//枚举主对角线上第i个元素
        for(j=i+1;j<=n-1;j++){//枚举剩下的行
            while(f[j][i]){//辗转相除
                t= f[i][i]/f[j][i];
                for(int k=i;k<=n-1;k++)//转为倒三角
                    f[i][k]=(f[i][k]-t*f[j][k]+MOD)%MOD;
                swap(f[i],f[j]);//交换i、j两行
                res=-res;
            }
        }
        res=(res*f[i][i])%MOD;
    }
    return (res+MOD)%MOD;
}
```

# 【基本定义】

1.无向图** G**：给定 n 个点，m 条边的无向图，设点集为 V，边集为 E，则其记为 G(V,E)

2.度数矩阵 **D[G]**：**当 i≠j 时，D[i][j]=0，当 i=j 时，D[i][i]=点 vi 的度数**

3.邻接矩阵 **A[G]**：**当 vi、vj 有边连接时，A[i][j]=1，当 vi、vj 无边连接时，A[i][j]=0**

4.基尔霍夫矩阵(Kirchhoff) **K[G]**：也称拉普拉斯算子，其定义为 **K[G]=D[G]-A[G]**，即：K[i][j]=D[i][j]-A[i][j]

例如：

![](https://img-blog.csdnimg.cn/20190408185124519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【基尔霍夫矩阵的性质】

对于任意一个图 G，其基尔霍夫矩阵 K 具有以下性质：
- 基尔霍夫矩阵 K 的每一行或每一列上的元素和都是 0
- 基尔霍夫矩阵 K 的行列式的值为 0
- 基尔霍夫矩阵 K 的任意一个代数余子式值都相同
- 如果图 G 不连通，基尔霍夫矩阵 K 的任意主子式行列式值为 0
- 如果图 G 是一棵树，基尔霍夫矩阵 K 的任意一个 n-1 阶主子式的行列式为 1

# 【Matrix-Tree 定理】

Matrix-Tree 定理的内容为：**对于已经得出的基尔霍夫矩阵，去掉其随意一行一列得出的矩阵的行列式，其绝对值为生成树的个数**

因此，对于给定的图 G，若要求其生成树个数，可以先求其基尔霍夫矩阵，然后随意取其任意一个 n-1 阶行列式，然后求出行列式的值，其绝对值就是这个图中生成树的个数。

## 1.朴素实现

```cpp
LL K[N][N];
LL det(int n){//求矩阵K的n-1阶顺序主子式
    LL res=1;
    for(int i=1;i<=n-1;i++){//枚举主对角线上第i个元素
        for(int j=i+1;j<=n-1;j++){//枚举剩下的行
            while(K[j][i]){//辗转相除
                int t=K[i][i]/K[j][i];
                for(int k=i;k<=n-1;k++)//转为倒三角
                    K[i][k]=(K[i][k]-t*K[j][k]+MOD)%MOD;
                swap(K[i],K[j]);//交换i、j两行
                res=-res;//取负
            }
        }
        res=(res*K[i][i])%MOD;
    }
    return (res+MOD)%MOD;
}
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    memset(K,0,sizeof(K));
    for(int i=1;i<=m;i++){
        int x,y;
        scanf("%d%d",&x,&y);
        K[x][x]++;
        K[y][y]++;
        K[x][y]--;
        K[y][x]--;
    }
    printf("%lld\n",det(n));
    return 0;
}
```

## 2.高斯消元实现

```cpp
#define E 1e-12
double K[N][N];
int judge(int x){
    if(x<=E||x>=-E)
        return 0;
    return x<0?-1:1;
}
double det(int n){//求矩阵K的n-1阶乘顺序主子式
    double res=1;
    for(int i=1;i<=n-1;i++){
        int big=i;
        for(int j=i+1;j<=n-1;j++)
            if(judge(K[big][i]-K[j][i])<0)
                big=j;
        if(big!=i)
            swap(K[i],K[big]);
        if(!K[i][i])
            return 0;

        for(int j=i+1;j<=n-1;j++){
            double t=K[j][i]/K[i][i];
            for(int k=i;k<=n;k++)
                K[j][k]-=t*K[i][k];
        }
    }
    for(int i=1;i<=n-1;i++)
        res=res*K[i][i];

    return res;
}
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    memset(K,0,sizeof(K));
    for(int i=1;i<=m;i++){
        int x,y;
        scanf("%d%d",&x,&y);
        K[x][x]++;
        K[y][y]++;
        K[x][y]--;
        K[y][x]--;
    }
    printf("%.2lf\n",det(n));
    return 0;
}
```

# 【例题】
- Highways（SPOJ-104）**(矩阵树朴素实现)**：[点击这里](https://blog.csdn.net/u011815404/article/details/89108766)



