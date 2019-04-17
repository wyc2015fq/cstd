# The Water Bowls（POJ-3185） - Alex_McAvoy的博客 - CSDN博客





2019年03月29日 19:06:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[矩阵——高斯消元法																[POJ](https://blog.csdn.net/u011815404/article/category/7504500)](https://blog.csdn.net/u011815404/article/category/8807958)








> 
# Problem Description

The cows have a line of 20 water bowls from which they drink. The bowls can be either right-side-up (properly oriented to serve refreshing cool water) or upside-down (a position which holds no water). They want all 20 water bowls to be right-side-up and thus use their wide snouts to flip bowls. 

Their snouts, though, are so wide that they flip not only one bowl but also the bowls on either side of that bowl (a total of three or -- in the case of either end bowl -- two bowls). 

Given the initial state of the bowls (1=undrinkable, 0=drinkable -- it even looks like a bowl), what is the minimum number of bowl flips necessary to turn all the bowls right-side-up?

# **Input**

Line 1: A single line with 20 space-separated integers

# Output

Line 1: The minimum number of bowl flips necessary to flip all the bowls right-side-up (i.e., to 0). For the inputs given, it will always be possible to find some combination of flips that will manipulate the bowls to 20 0's.

# Sample Input

**0 0 1 1 1 0 0 1 1 0 1 1 0 0 0 0 0 0 0 0**

# Sample Output

**3**


题意：有 20 个开关，改变一个开关状态时，相邻开关的状态也会改变，给出初始状态，求最小的改变次数使得所有开关均为 0

思路：

以每个开关的状态构造一个异或方程，最终构造一个 20 个方程的异或线性方程组

考虑系数矩阵 a 的取值，由于每个开关 i 的开启、关闭影响自身，那么有：a[i][i]=1，即第 i 个开关受第 i 个开关的影响，又由于每个开关受相邻开关影响，那么对于大于第 1 个的开关有 **a[i-1][i]=1**，对于小于第 20 个的开关有 **a[i+1][i]=1**，每个开关的终止状态即为矩阵的 a[i][n]

然后进行高斯消元求矩阵的自由元，由于要求最小的次数，那么就需要对所有的自由元进行二进制枚举，寻找有解情况下 1 的最小数量

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=6;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;


int a[N][N];//增广矩阵
int x[N];//解集
int freeX[N];//自由变元
int Gauss(int equ,int var){//返回自由变元个数
    /*初始化*/
    for(int i=0;i<=var;i++){
        x[i]=0;
        freeX[i]=0;
    }

    /*转换为阶梯阵*/
    int col=0;//当前处理的列
    int num=0;//自由变元的序号
    int k;//当前处理的行
    for(k=0;k<equ&&col<var;k++,col++){//枚举当前处理的行
        int maxRow=k;//当前列绝对值最大的行
        for(int i=k+1;i<equ;i++){//寻找当前列绝对值最大的行
            if(abs(a[i][col])>abs(a[maxRow][col]))
                maxRow=i;
        }
        if(maxRow!=k){//与第k行交换
            for(int j=k;j<var+1;j++)
                swap(a[k][j],a[maxRow][j]);
        }
        if(a[k][col]==0){//col列第k行以下全是0，处理当前行的下一列
            freeX[num++]=col;//记录自由变元
            k--;
            continue;
        }

        for(int i=k+1;i<equ;i++){
            if(a[i][col]!=0){
                for(int j=col;j<var+1;j++){//对于下面出现该列中有1的行，需要把1消掉
                    a[i][j]^=a[k][j];
                }
            }
        }
    }

    /*求解*/
    //无解：化简的增广阵中存在(0,0,...,a)这样的行，且a!=0
    for(int i=k;i<equ;i++)
        if(a[i][col]!=0)
            return -1;

    //无穷解: 在var*(var+1)的增广阵中出现(0,0,...,0)这样的行
    int temp=var-k;//自由变元有var-k个
    if(k<var)//返回自由变元数
        return temp;

    //唯一解: 在var*(var+1)的增广阵中形成严格的上三角阵
    for(int i=var-1;i>=0;i--){//计算解集
        x[i]=a[i][var];
        for(int j=i+1;j<var;j++)
            x[i]^=(a[i][j]&&x[j]);
    }
    return 0;
}
int enumFreeX(int freeNum,int var){//枚举自由元，统计有解情况下1最少的个数
    int sta=(1<<(freeNum));//自由元的状态总数
    int res=INF;
    for(int i=0;i<sta;++i){//枚举状态
        int cnt=0;
        for(int j=0;j<freeNum;j++){//枚举自由元
            if(i&(1<<j)){
                cnt++;
                x[freeX[j]]=1;
            }else
                x[freeX[j]]=0;
        }
        for(int k=var-freeNum-1;k>=0;k--){//没有自由元的最下面一行
            int index=0;
            for(index=k;k<var;index++){//在当前行找到第一个非0自由元
                if(a[k][index])
                    break;
            }
            x[index]=a[k][var];
            for(int j=index+1;j<var;++j){//向后依次计算出结果
                if(a[k][j])
                    x[index]^=x[j];
            }
            cnt+=x[index];//若结果为1，则进行统计
        }
        res=min(res,cnt);
    }
    return res;
}
int main(){
    int n=20;
    memset(a,0,sizeof(a));

    for(int i=0;i<n;i++){//开始状态
        a[i][i]=1;//第i个开关
        if(i<n-1)//第1到第19个开关
            a[i+1][i]=1;
        if(i>0)//第2到第20个开关
            a[i-1][i]=1;
    }
    for(int i=0;i<n;i++)//最终状态
        scanf("%d",&a[i][n]);

    int freeNum=Gauss(n,n);//获取自由元
    if(freeNum==-1)//无解
        printf("inf\n");
    else if(freeNum==0){//唯一解
        int res=0;
        for(int i=0;i<n;i++)
            res+=x[i];
        printf("%d\n",res);
    }
    else{//多个解
        int res=enumFreeX(freeNum,n);
        printf("%d\n",res);
    }
    return 0;
}
```






