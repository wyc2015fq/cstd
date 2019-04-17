# Fibsieve`s Fantabulous Birthday（LightOJ-1008） - Alex_McAvoy的博客 - CSDN博客





2019年01月22日 16:49:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41








> 
# Problem Description

Fibsieve had a fantabulous (yes, it's an actual word) birthday party this year. He had so many gifts that he was actually thinking of not having a party next year.

Among these gifts there was an N x N glass chessboard that had a light in each of its cells. When the board was turned on a distinct cell would light up every second, and then go dark.

The cells would light up in the sequence shown in the diagram. Each cell is marked with the second in which it would light up.

![](https://img-blog.csdnimg.cn/20190122160224253.jpg)

(The numbers in the grids stand for the time when the corresponding cell lights up)

In the first second the light at cell (1, 1) would be on. And in the 5th second the cell (3, 1) would be on. Now, Fibsieve is trying to predict which cell will light up at a certain time (given in seconds). Assume that N is large enough.

# Input

Input starts with an integer T (≤ 200), denoting the number of test cases.

Each case will contain an integer S (1 ≤ S ≤ 1015) which stands for the time.

# Output

For each case you have to print the case number and two numbers (x, y), the column and the row number.

# **Sample Input**

**382025**

# Sample Output

**Case 1: 2 3Case 2: 5 4Case 3: 1 5**


题意：数字按照如上表中的规律在表中排列，现在给出一个数 n，输出这个数在表中的坐标

思路：用 Excel 做了个表，找了找规律

![](https://img-blog.csdnimg.cn/20190122161816734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

从表中可以看出，蓝色的是偶数的平方，黄色的是奇数的平方，绿色是对角线，满足 x(x-1)+1，x 为行数

通过给出的数开根后强转为整型，即为这个数的行/列数，首先判断这个数是否为奇数方或偶数方，若不是，则根据行列数枚举圈数从而确定具体位置，注意使用 long long

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
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n;
        scanf("%lld",&n);

        LL t=(LL)sqrt(n);//n的行/列数

        printf("Case %d: ",Case++);
        if(t*t==n){//平方关系
            if(t%2)//奇数时横坐标为1
                printf ("1 %lld\n",t);
            else//偶数时纵坐标为1
                printf ("%lld 1\n",t);

        }
        else{//不为平方关系时
            LL circle=t+1;//圈数
            LL minn=(circle-1)*(circle-1)+1;//一圈中最小的数
            if(circle%2){//奇数圈
                if(minn+circle-1>=n)
                    printf("%lld %lld\n",circle,n-minn+1);
                else
                    printf("%lld %lld\n",circle*circle-n+1,circle);
            }
            else{//偶数圈
                if(minn+circle-1>=n)
                    printf("%lld %lld\n",n-minn+1,circle);
                else
                    printf("%lld %lld\n",circle,circle*circle-n+1);
            }
        }
    }
    return 0;
}
```





