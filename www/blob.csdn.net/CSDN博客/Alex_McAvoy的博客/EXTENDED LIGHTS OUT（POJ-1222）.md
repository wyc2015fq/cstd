# EXTENDED LIGHTS OUT（POJ-1222） - Alex_McAvoy的博客 - CSDN博客





2019年03月29日 16:53:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：23
个人分类：[矩阵——高斯消元法																[POJ](https://blog.csdn.net/u011815404/article/category/7504500)](https://blog.csdn.net/u011815404/article/category/8807958)








> 
# Problem Description

In an extended version of the game Lights Out, is a puzzle with 5 rows of 6 buttons each (the actual puzzle has 5 rows of 5 buttons each). Each button has a light. When a button is pressed, that button and each of its (up to four) neighbors above, below, right and left, has the state of its light reversed. (If on, the light is turned off; if off, the light is turned on.) Buttons in the corners change the state of 3 buttons; buttons on an edge change the state of 4 buttons and other buttons change the state of 5. For example, if the buttons marked X on the left below were to be pressed,the display would change to the image on the right. 

![](https://img-blog.csdnimg.cn/20190329155353118.jpg)

The aim of the game is, starting from any initial set of lights on in the display, to press buttons to get the display to a state where all lights are off. When adjacent buttons are pressed, the action of one button can undo the effect of another. For instance, in the display below, pressing buttons marked X in the left display results in the right display.Note that the buttons in row 2 column 3 and row 2 column 5 both change the state of the button in row 2 column 4,so that, in the end, its state is unchanged. 

![](https://img-blog.csdnimg.cn/20190329155405109.jpg)

Note: 

1. It does not matter what order the buttons are pressed. 

2. If a button is pressed a second time, it exactly cancels the effect of the first press, so no button ever need be pressed more than once. 

3. As illustrated in the second diagram, all the lights in the first row may be turned off, by pressing the corresponding buttons in the second row. By repeating this process in each row, all the lights in the first 

four rows may be turned out. Similarly, by pressing buttons in columns 2, 3 ?, all lights in the first 5 columns may be turned off. 

Write a program to solve the puzzle.

# **Input**

The first line of the input is a positive integer n which is the number of puzzles that follow. Each puzzle will be five lines, each of which has six 0 or 1 separated by one or more spaces. A 0 indicates that the light is off, while a 1 indicates that the light is on initially.

# Output

For each puzzle, the output consists of a line with the string: "PUZZLE #m", where m is the index of the puzzle in the input file. Following that line, is a puzzle-like display (in the same format as the input) . In this case, 1's indicate buttons that must be pressed to solve the puzzle, while 0 indicate buttons, which are not pressed. There should be exactly one space between each 0 or 1 in the output puzzle-like display.

# Sample Input

**20 1 1 0 1 01 0 0 1 1 10 0 1 0 0 11 0 0 1 0 10 1 1 1 0 00 0 1 0 1 01 0 1 0 1 10 0 1 0 1 11 0 1 1 0 00 1 0 1 0 0**

# Sample Output

**PUZZLE #11 0 1 0 0 11 1 0 1 0 10 0 1 0 1 11 0 0 1 0 00 1 0 0 0 0PUZZLE #21 0 0 1 1 11 1 0 0 0 00 0 0 1 0 01 1 0 1 0 11 0 1 1 0 1**


题意：有一个 6*5 的矩形，每个单元格都有两种状态 1、0，改变一个单元格的状态会使得其周围 4 个单元格状态也改变，现在给出每个单元格的状态，要求输出一种能将所有单元格全变为 0 的方案

思路：

题目要求将所有单元格都变为 0，那么建立线性方程组就是求解如何从全 0 变为给出的状态，由于仅有两种状态，且单元格状态的改变与异或相同，因此使用异或操作，那么问题就转为了用高斯消元法求异或方程组

由于每次操作的下一次操作都与前面的操作相关，那么一共有 5*6=30 个方程，每个单元格 G[i][j] 对应一个线性方程，建立一个 30*31 的增广矩阵，x[i] 为每一个开关的操作次数，也即高斯消元的解，那么就有：

a[0][0]x[0]^a[0][1]x[1]^....^a[0][29]x[29]=a[0][30]

a[1][0]x[0]^a[1][1]^x[1]^...^a[1][29]x[29]=a[1][30]

...

其中 a[i][j] 为矩阵的系数，即第 i 个灯若受第 j 个灯影响即赋值为 1 无影响赋值为 0，a[i][30] 为输入的值

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
    //唯一解: 在var*(var+1)的增广阵中形成严格的上三角阵
    for(int i=var-1;i>=0;i--){//计算解集
        x[i]=a[i][var];
        for(int j=i+1;j<var;j++)
            x[i]^=(a[i][j]&&x[j]);
    }
    return 0;
}
void init(){//由于经过求解后增广矩阵a发生了改变，因此每次都要对a初始化
    memset(a,0,sizeof(a));
    memset(x,0,sizeof(x));//初始时，x全为0
    
    //构造增广矩阵
    int n=5,m=6;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            int t=i*m+j;
            a[t][t]=1;
            if(i>0)
                a[(i-1)*m+j][t]=1;
            if(i<n-1)
                a[(i+1)*m+j][t]=1;
            if(j>0)
                a[i*m+j-1][t]=1;
            if(j<m-1)
                a[i*m+j+1][t]=1;
        }
    }
}
int main(){
    int t;
    scanf("%d",&t);
    
    int Case=1;
    while(t--){
        init();
        
        for(int i=0;i<5*6;i++)//输入a[i][30]
            scanf("%d",&a[i][30]);
        Gauss(30,30);

        printf("PUZZLE #%d\n", Case++);

        int n=5,m=6;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                printf("%d ",x[i*m+j]);
            }
            printf("\n");
        }
    }
    return 0;
}
```






