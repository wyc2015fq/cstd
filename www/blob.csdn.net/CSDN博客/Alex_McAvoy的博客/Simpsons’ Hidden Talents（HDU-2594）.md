# Simpsons’ Hidden Talents（HDU-2594） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 18:39:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37








> 
# Problem Description

Homer: Marge, I just figured out a way to discover some of the talents we weren’t aware we had. 

Marge: Yeah, what is it? 

Homer: Take me for example. I want to find out if I have a talent in politics, OK? 

Marge: OK. 

Homer: So I take some politician’s name, say Clinton, and try to find the length of the longest prefix 

in Clinton’s name that is a suffix in my name. That’s how close I am to being a politician like Clinton 

Marge: Why on earth choose the longest prefix that is a suffix??? 

Homer: Well, our talents are deeply hidden within ourselves, Marge. 

Marge: So how close are you? 

Homer: 0! 

Marge: I’m not surprised. 

Homer: But you know, you must have some real math talent hidden deep in you. 

Marge: How come? 

Homer: Riemann and Marjorie gives 3!!! 

Marge: Who the heck is Riemann? 

Homer: Never mind. 

Write a program that, when given strings s1 and s2, finds the longest prefix of s1 that is a suffix of s2.

# **Input**

Input consists of two lines. The first line contains s1 and the second line contains s2. You may assume all letters are in lowercase.

# Output

Output consists of a single line that contains the longest string that is a prefix of s1 and a suffix of s2, followed by the length of that prefix. If the longest such string is the empty string, then the output should be 0. 

The lengths of s1 and s2 will be at most 50000.

# Sample Input

**clintonhomerriemannmarjorie**

# Sample Output

**0rie 3**


题意：每组数据给出两个字符串 s1、s2，判断是否存在 s1 的前缀是 s2 的后缀的情况，若没有则输出 0，若有则输出这个前缀/后缀与其长度

思路：KMP 的应用

本质上就是找前一个字符串的前缀和后一个字符串的后缀相同的串，使用 KMP 进行匹配时，注意当模式串 s1 到达结尾时，需要更新指针位置，返回值也需要返回前缀位置

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
const int MOD=20091226;
const int N=50001;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int Next[N];
char str1[N],str2[N];
void getNext(char p[]){
    Next[0]=-1;
    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len){
        if(k==-1||p[j]==p[k]) {
            k++;
            j++;
            Next[j]=k;
        }else{
            k=Next[k];
        }
    }
}
int KMP(char t[],char p[]){
    int tLen=strlen(t);
    int pLen=strlen(p);

    int i=0;
    int j=0;

    while(i<tLen&&j<pLen){
        if(j==-1||t[i]==p[j]){
            i++;
            j++;
            if(j==pLen&&i!=tLen){//当模式串到达结尾时，回到指定位置
                j=Next[j];
            }
        }
        else{
           j=Next[j];
        }
    }

    return j;//返回前缀的位置
}

int main(){
    while(scanf("%s",str1)!=EOF){
        scanf("%s",str2);
        getNext(str1);
        int res=KMP(str2,str1);
        if(res!=0){
            str1[res]='\0';//截取前缀
            printf("%s %d\n",str1,res);
        }
        else
            printf("0\n");
    }
    return 0;
}
```





