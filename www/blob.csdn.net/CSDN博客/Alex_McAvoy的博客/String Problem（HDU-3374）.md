# String Problem（HDU-3374） - Alex_McAvoy的博客 - CSDN博客





2019年03月02日 13:28:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49








> 
# Problem Description

Give you a string with length N, you can generate N strings by left shifts. For example let consider the string “SKYLONG”, we can generate seven strings: 

String Rank 

SKYLONG 1 

KYLONGS 2 

YLONGSK 3 

LONGSKY 4 

ONGSKYL 5 

NGSKYLO 6 

GSKYLON 7 

and lexicographically first of them is GSKYLON, lexicographically last is YLONGSK, both of them appear only once. 

  Your task is easy, calculate the lexicographically fisrt string’s Rank (if there are multiple answers, choose the smallest one), its times, lexicographically last string’s Rank (if there are multiple answers, choose the smallest one), and its times also. 

# **Input**

Each line contains one line the string S with length N (N <= 1000000) formed by lower case letters

# Output

Output four integers separated by one space, lexicographically fisrt string’s Rank (if there are multiple answers, choose the smallest one), the string’s times in the N generated strings, lexicographically last string’s Rank (if there are multiple answers, choose the smallest one), and its times also.

# Sample Input

**abcderaaaaaaababab**

# Sample Output

**1 1 6 11 6 1 61 3 2 3**


题意：给出多组数据，每组数据给出一个字符串，要求输出这个字符串的最小最大表示的起始位置，然后分别求出在同构串中起始位置的字符出现的次数

思路：最小最大的起始位置直接套用模版即可，然后使用 KMP 的 next 数组求循环节，则次数=长度/循环节长度

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int Next[N];
char str[N];
void getNext(char p[]){
    Next[0]=-1;
    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len){
        if(k==-1||p[j]==p[k]){
            k++;
            j++;
            Next[j]=k;
        }
        else{
            k=Next[k];
        }
    }
}
int minmumRepresentation(char *str){//最小表示法
    int len=strlen(str);
    int i=0;
    int j=1;
    int k=0;
    while(i<len&&j<len&&k<len){
        int temp=str[(i+k)%len]-str[(j+k)%len];
        if(temp==0)
            k++;
        else{
            if(temp>0)
                i=i+k+1;
            else
                j=j+k+1;
            if(i==j)
                j++;
            k=0;
        }
    }
    return i<j?i:j;
}
int maxmumRepresentation(char *str){//最大表示法
    int len=strlen(str);
    int i=0;
    int j=1;
    int k=0;
    while(i<len&&j<len&&k<len){
        int temp=str[(i+k)%len]-str[(j+k)%len];
        if(temp==0)
            k++;
        else{
            if(temp>0)
                j=j+k+1;
            else
                i=i+k+1;
            if(i==j)
                j++;
            k=0;
        }
    }
    return i<j?i:j;
}


int main(){
    while(scanf("%s",str)!=EOF){
        getNext(str);

        int n=strlen(str);
        int len=n-Next[n];

        int num=1;//数量
        if(n%len==0)
            num=n/len;

        int minn=minmumRepresentation(str);//最小表示
        int maxx=maxmumRepresentation(str);//最大表示

        printf("%d %d %d %d\n",minn+1,num,maxx+1,num);
    }
    return 0;
}
```





