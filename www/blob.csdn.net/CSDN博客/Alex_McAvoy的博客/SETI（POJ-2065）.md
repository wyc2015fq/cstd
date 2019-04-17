# SETI（POJ-2065） - Alex_McAvoy的博客 - CSDN博客





2019年03月30日 20:40:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：9
个人分类：[POJ																[矩阵——高斯消元法](https://blog.csdn.net/u011815404/article/category/8807958)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

For some years, quite a lot of work has been put into listening to electromagnetic radio signals received from space, in order to understand what civilizations in distant galaxies might be trying to tell us. One signal source that has been of particular interest to the scientists at Universit´e de Technologie Spatiale is the Nebula Stupidicus. 

Recently, it was discovered that if each message is assumed to be transmitted as a sequence of integers a0, a1, ...a n-1 the function f (k) = ∑ 0<=i<=n-1a ik i (mod p) always evaluates to values 0 <= f (k) <= 26 for 1 <= k <= n, provided that the correct value of p is used. n is of course the length of the transmitted message, and the ai denote integers such that 0 <= a i < p. p is a prime number that is guaranteed to be larger than n as well as larger than 26. It is, however, known to never exceed 30 000. 

These relationships altogether have been considered too peculiar for being pure coincidences, which calls for further investigation. 

The linguists at the faculty of Langues et Cultures Extraterrestres transcribe these messages to strings in the English alphabet to make the messages easier to handle while trying to interpret their meanings. The transcription procedure simply assigns the letters a..z to the values 1..26 that f (k) might evaluate to, such that 1 = a, 2 = b etc. The value 0 is transcribed to '*' (an asterisk). While transcribing messages, the linguists simply loop from k = 1 to n, and append the character corresponding to the value of f (k) at the end of the string. 

The backward transcription procedure, has however, turned out to be too complex for the linguists to handle by themselves. You are therefore assigned the task of writing a program that converts a set of strings to their corresponding Extra Terrestial number sequences.

# **Input**

On the first line of the input there is a single positive integer N, telling the number of test cases to follow. Each case consists of one line containing the value of p to use during the transcription of the string, followed by the actual string to be transcribed. The only allowed characters in the string are the lower case letters 'a'..'z' and '*' (asterisk). No string will be longer than 70 characters.

# Output

For each transcribed string, output a line with the corresponding list of integers, separated by space, with each integer given in the order of ascending values of i.

# **Sample Input**

**331 aaa37 abc29 hello*earth**

# **Sample output**

**1 0 00 1 08 13 9 13 4 27 18 10 12 24 15**


题意：

t 组数据，每组给出一个素数 p 以及一个字符串，字符串中每个字符对应一个数字，a~z 对于 1~26，* 对应 0

并定义一个函数：

![\left\{\begin{matrix}a_0*1^0+a_1*1^1+a_2*1^2+...+a_{n-1}*1^{n-1}= f(1)(mod \:\:p) \\ a_0*2^0 + a_1*2^1+a_2*2^2+...+a_{n-1}*2^{n-1} = f(2)(mod\:\:p) \\ ... \\ a_0*n^0 + a_1*n^1+a_2*n^2+........+a_{n-1}*n^{n-1} = f(n)(mod\:\:p)\end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7Da_0*1%5E0&plus;a_1*1%5E1&plus;a_2*1%5E2&plus;...&plus;a_%7Bn-1%7D*1%5E%7Bn-1%7D%3D%20f%281%29%28mod%20%5C%3A%5C%3Ap%29%20%5C%5C%20a_0*2%5E0%20&plus;%20a_1*2%5E1&plus;a_2*2%5E2&plus;...&plus;a_%7Bn-1%7D*2%5E%7Bn-1%7D%20%3D%20f%282%29%28mod%5C%3A%5C%3Ap%29%20%5C%5C%20...%20%5C%5C%20a_0*n%5E0%20&plus;%20a_1*n%5E1&plus;a_2*n%5E2&plus;........&plus;a_%7Bn-1%7D*n%5E%7Bn-1%7D%20%3D%20f%28n%29%28mod%5C%3A%5C%3Ap%29%5Cend%7Bmatrix%7D%5Cright.)

求 a0、a1、a2、...、an-1

思路：若字符串长度为 n，则是一个含 n 个方程 n 个位置数的线性方程组，将相应的系数转成矩阵

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
const int MOD=7;
const int N=100+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int mod;
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
                    a[i][j]=((a[i][j]*ta-a[k][j]*tb)%mod+mod)%mod;
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
            temp=(temp%mod+mod)%mod;//取模
        }
        while(temp%a[i][i]!=0)//外层每次循环都是求a[i][i]，它是每个方程中唯一一个未知的变量
            temp+=mod;//a[i][i]必须为整数，加上周期MOD
        x[i]=(temp/a[i][i])%mod;//取模
    }
    return 0;
}

char str[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){

        scanf("%d",&mod);
        scanf("%s",str);

        int n=strlen(str);
        for(int i=0;i<n;i++){//计算系数
            if(str[i]=='*')//将字符串转为系数
                a[i][n]=0;
            else
                a[i][n]=str[i]-'a'+1;
            a[i][0]=1;
            for(int j=1;j<n;j++)
                a[i][j]=(a[i][j-1]*(i+1))%mod;
        }

        Gauss(n,n);
        for(int i=0;i<n;i++)
            printf("%d ",x[i]);
        printf("\n");
    }
    return 0;
}
```






