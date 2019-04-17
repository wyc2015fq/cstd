# Palindrome（POJ-3974） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 10:59:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：24
个人分类：[POJ																[字符串处理——回文串相关](https://blog.csdn.net/u011815404/article/category/8650935)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Andy the smart computer science student was attending an algorithms class when the professor asked the students a simple question, "Can you propose an efficient algorithm to find the length of the largest palindrome in a string?" 

A string is said to be a palindrome if it reads the same both forwards and backwards, for example "madam" is a palindrome while "acm" is not. 

The students recognized that this is a classical problem but couldn't come up with a solution better than iterating over all substrings and checking whether they are palindrome or not, obviously this algorithm is not efficient at all, after a while Andy raised his hand and said "Okay, I've a better algorithm" and before he starts to explain his idea he stopped for a moment and then said "Well, I've an even better algorithm!". 

If you think you know Andy's final solution then prove it! Given a string of at most 1000000 characters find and print the length of the largest palindrome inside this string.

# **Input**

Your program will be tested on at most 30 test cases, each test case is given as a string of at most 1000000 lowercase characters on a line by itself. The input is terminated by a line that starts with the string "END" (quotes for clarity).

# Output

For each test case in the input print the test case number and the length of the largest palindrome. 

# Sample Input

**abcbabcbabcbaabacacbaaaabEND**

# Sample Output

**Case 1: 13Case 2: 6**


题意：给出多组数据，每组给出一个字符串，要求找出字符串中最长回文子串的长度，数据以 END 为结尾

思路：马拉车算法模版题

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
#define N 1000001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
char str[N];//原字符串
char newStr[N*2];//预处理后的字符串
int p[N*2];//辅助数组
int init(){//对原字符进行预处理
    newStr[0]='$';
    newStr[1]='#';

    int j=2;
    int len=strlen(str);
    for (int i=0;i<len;i++){
        newStr[j++]=str[i];
        newStr[j++]='#';
    }
    newStr[j] ='\0'; //字符串结束标记

    return j;//返回newStr的长度
}

int manacher(){
    int len=init();//取得新字符串长度并完成字符串的预处理
    int res=-1;//最长回文长度

    int id;
    int mx=0;
    for(int i=1;i<len;i++){
        int j=2*id-i;//与i相对称的位置
        if(i<mx)
            p[i]=min(p[j], mx-i);
        else
            p[i]=1;

        //由于左有'$',右有'\0'，不需边界判断
        while(newStr[i-p[i]] == newStr[i+p[i]])//p[i]的扩大
            p[i]++;

        if(mx<i+p[i]){//由于希望mx尽可能的远，因此要不断进行比较更新
            id=i;
            mx=i+p[i];
        }
        res=max(res,p[i]-1);
    }
    return res;
}

int main(){
    int Case=1;
    while(scanf("%s",str)!=EOF){
        if(str[0]=='E')
            break;
        printf("Case %d: %d\n",Case++,manacher());
    }
    return 0;
}
```






