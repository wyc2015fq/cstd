# Widget Factor（POJ-2947） - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 16:09:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[POJ																[矩阵——高斯消元法](https://blog.csdn.net/u011815404/article/category/8807958)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The widget factory produces several different kinds of widgets. Each widget is carefully built by a skilled widgeteer. The time required to build a widget depends on its type: the simple widgets need only 3 days, but the most complex ones may need as many as 9 days. 

The factory is currently in a state of complete chaos: recently, the factory has been bought by a new owner, and the new director has fired almost everyone. The new staff know almost nothing about building widgets, and it seems that no one remembers how many days are required to build each diofferent type of widget. This is very embarrassing when a client orders widgets and the factory cannot tell the client how many days are needed to produce the required goods. Fortunately, there are records that say for each widgeteer the date when he started working at the factory, the date when he was fired and what types of widgets he built. The problem is that the record does not say the exact date of starting and leaving the job, only the day of the week. Nevertheless, even this information might be helpful in certain cases: for example, if a widgeteer started working on a Tuesday, built a Type 41 widget, and was fired on a Friday,then we know that it takes 4 days to build a Type 41 widget. Your task is to figure out from these records (if possible) the number of days that are required to build the different types of widgets. 

# **Input**

The input contains several blocks of test cases. Each case begins with a line containing two integers: the number 1 ≤ n ≤ 300 of the different types, and the number 1 ≤ m ≤ 300 of the records. This line is followed by a description of the m records. Each record is described by two lines. The first line contains the total number 1 ≤ k ≤ 10000 of widgets built by this widgeteer, followed by the day of week when he/she started working and the day of the week he/she was fired. The days of the week are given bythe strings `MON', `TUE', `WED', `THU', `FRI', `SAT' and `SUN'. The second line contains k integers separated by spaces. These numbers are between 1 and n , and they describe the diofferent types of widgets that the widgeteer built. For example, the following two lines mean that the widgeteer started working on a Wednesday, built a Type 13 widget, a Type 18 widget, a Type 1 widget, again a Type 13 widget,and was fired on a Sunday. 

4 WED SUN 

13 18 1 13 

Note that the widgeteers work 7 days a week, and they were working on every day between their first and last day at the factory (if you like weekends and holidays, then do not become a widgeteer!). 

The input is terminated by a test case with n = m = 0 .

# Output

For each test case, you have to output a single line containing n integers separated by spaces: the number of days required to build the different types of widgets. There should be no space before the first number or after the last number, and there should be exactly one space between two numbers. If there is more than one possible solution for the problem, then write `Multiple solutions.' (without the quotes). If you are sure that there is no solution consistent with the input, then write `Inconsistent data.'(without the quotes).

# **Sample Input**

**2 32 MON THU1 23 MON FRI1 1 23 MON SUN1 2 210 21 MON TUE 31 MON WED30 0**

# **Sample output**

**8 3Inconsistent data.**


题意：有 n 个工具，每种工具的加工时间是 3-9 天，但由于老员工被炒了因此不知道具体的加工时间，对于一些老员工，存有他的记录，记录着他开始工作的时间、被炒的时间，但时间只给出了是星期几，具体的星期并不知道，在他工作的这段时间中，正好加工了 k 个物品，并给出这 k 个物品的编号，现在要通过这些编号来确定每种工具的加工时间

思路：

对于每个记录，建立一个方程，那么 m 条记录就有 m 个方程：n 个物品，编号从 0 到 n-1，以 x0、x1、...、xn-1 代表第 i 种工具的加工时间，a[i][j] 代表第 i 个方程编号为 j 的物品加工的个数，a[i][n] 代表第 i 个方程中加工完所欲种类工具所需的时间，由于不知道开始、结束时间是第几个星期，因此需要 %7

即建立出方程组：

![\left\{\begin{matrix}(a[0][0]*x_0 + a[0][1] *x_1 + ...+a[0][n-1]*x_{n-1})mod\:\:7= a[0][n] \\ (a[1][0]*x_0 + a[1][1] *x_1 +...+a[1][n-1]*x_{n-1})mod\:\:7= a[1][n] \\ ... \\ (a[m-1][0]*x_0 + a[m-1][1] *x_1 + ...+a[m-1][n-1]*x_{n-1}mod\:\:7= a[m-1][n] \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D%28a%5B0%5D%5B0%5D*x_0%20&plus;%20a%5B0%5D%5B1%5D%20*x_1%20&plus;%20...&plus;a%5B0%5D%5Bn-1%5D*x_%7Bn-1%7D%29mod%5C%3A%5C%3A7%3D%20a%5B0%5D%5Bn%5D%20%5C%5C%20%28a%5B1%5D%5B0%5D*x_0%20&plus;%20a%5B1%5D%5B1%5D%20*x_1%20&plus;...&plus;a%5B1%5D%5Bn-1%5D*x_%7Bn-1%7D%29mod%5C%3A%5C%3A7%3D%20a%5B1%5D%5Bn%5D%20%5C%5C%20...%20%5C%5C%20%28a%5Bm-1%5D%5B0%5D*x_0%20&plus;%20a%5Bm-1%5D%5B1%5D%20*x_1%20&plus;%20...&plus;a%5Bm-1%5D%5Bn-1%5D*x_%7Bn-1%7Dmod%5C%3A%5C%3A7%3D%20a%5Bm-1%5D%5Bn%5D%20%5Cend%7Bmatrix%7D%5Cright.)

建立好方程后，使用高斯消元法即可求出解

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
const int MOD=7;//一周7天，模7
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;


int a[N][N];//增广矩阵
int x[N];//解集
bool freeX[N];//标记是否为自由变元
int GCD(int a,int b){
    return !b?a:GCD(b,a%b);
}
int LCM(int a,int b){
    return a/GCD(a,b)*b;
}
int Gauss(int equ,int var){//返回自由变元个数
    /*初始化*/
    for(int i=0;i<=var;i++){
        x[i]=0;
        freeX[i]=true;
    }

    /*转换为阶梯阵*/
    int col=0;//当前处理的列
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
            k--;
            continue;
        }

        for(int i=k+1;i<equ;i++){//枚举要删去的行
            if(a[i][col]!=0){
                int lcm=LCM(abs(a[i][col]),abs(a[k][col]));
                int ta=lcm/abs(a[i][col]);
                int tb=lcm/abs(a[k][col]);
                if(a[i][col]*a[k][col]<0)//异号情况相加
                    tb=-tb;
                for(int j=col;j<var+1;j++) {
                    a[i][j]=((a[i][j]*ta-a[k][j]*tb)%MOD+MOD)%MOD;
                }
            }
        }
    }

    /*求解*/
    //无解：化简的增广阵中存在(0,0,...,a)这样的行，且a!=0
    for(int i=k;i<equ;i++)
        if (a[i][col]!=0)
            return -1;

    //无穷解: 在var*(var+1)的增广阵中出现(0,0,...,0)这样的行
    int temp=var-k;//自由变元有var-k个
    if(k<var)//返回自由变元数
        return temp;

    //唯一解: 在var*(var+1)的增广阵中形成严格的上三角阵
    for(int i=var-1;i>=0;i--){//计算解集
        int temp=a[i][var];
        for(int j=i+1;j<var;j++){
            if(a[i][j]!=0)
                temp-=a[i][j]*x[j];
            temp=(temp%MOD+MOD)%MOD;//取模
        }
        while(temp%a[i][i]!=0)//外层每次循环都是求a[i][i]，它是每个方程中唯一一个未知的变量
            temp+=MOD;//a[i][i]必须为整数，加上周期MOD
        x[i]=(temp/a[i][i])%7;//取模
    }
    return 0;
}
int getDay(char s[]){//将星期字符转为具体天
    if(s[0]=='M'&&s[1]=='O'&&s[2]=='N')
        return 1;
    else if(s[0]=='T'&&s[1]=='U'&&s[2]=='E')
        return 2;
    else if(s[0]=='W'&&s[1]=='E'&&s[2]=='D')
        return 3;
    else if(s[0]=='T'&&s[1]=='H'&&s[2]=='U')
        return 4;
    else if(s[0]=='F'&&s[1]=='R'&&s[2]=='I')
        return 5;
    else if(s[0]=='S'&&s[1]=='A'&&s[2]=='T')
        return 6;
    else if(s[0]=='S'&&s[1]=='U'&&s[2]=='N')
        return 7;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n||m)){
        memset(a,0,sizeof(a));
        for(int i=0;i<m;i++){
            int k;
            char start[5],endd[5];

            scanf("%d",&k);
            scanf("%s%s",start,endd);
            a[i][n]=((getDay(endd)-getDay(start)+1)%MOD+MOD)%MOD;//完成第i件物品的时间

            for(int j=1;j<=k;j++){//打造的数量
                int num;
                scanf("%d",&num);//可能是相同的数
                num--;
                a[i][num]++;//统计系数
                a[i][num]%=MOD;//去重
            }
        }

        int freeNum=Gauss(m,n);//m个方程n个变量
        if(freeNum==0){
            for(int i=0;i<n;i++)
                if(x[i]<=2)//每个零件加工时间在3-9，若小于3天，加上一个周期
                    x[i]+=MOD;
            for(int i=0;i<n;i++)
                printf("%d ",x[i]);
            printf("\n");
        }
        else if(freeNum==-1)//无解
            printf("Inconsistent data.\n");
        else//多个解
            printf("Multiple solutions.\n");
    }
    return 0;
}
```






