# Codeforces1029A——Many Equal Substrings - westbrook1998的博客 - CSDN博客





2018年08月25日 12:29:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：102








> 
You are given a string t consisting of n lowercase Latin letters and an integer number k. 

  Let’s define a substring of some string s with indices from l to r as s[l…r]. 

  Your task is to construct such string s of minimum possible length that there are exactly k positions i such that s[i…i+n−1]=t. In other words, your task is to construct such string s of minimum possible length that there are exactly k substrings of s equal to t. 

  It is guaranteed that the answer is always unique. 

  Input 

  The first line of the input contains two integers n and k (1≤n,k≤50) — the length of the string t and the number of substrings. 

  The second line of the input contains the string t consisting of exactly n lowercase Latin letters. 

  Output 

  Print such string s of minimum possible length that there are exactly k substrings of s equal to t. 

  It is guaranteed that the answer is always unique. 

  Examples 

  Input 

  3 4 

  aba 

  Output 

  ababababa 

  Input 

  3 2 

  cat 

  Output 

  catcat
DIv3也只能做两题。。。 

这题是给一个长度为n的字符串 要拼成一个新串使得原来的串在新串中出现k次，用kmp预处理一下首尾的相同前后缀长度，然后就行了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
#include <vector>
#include <cstring>
#include <set>
using namespace std;
char s[55];
int Next[55];
void kmp_pre(char* pat){
    int l=strlen(pat);
    int i=0,j=-1;
    Next[0]=-1;
    while(i<l){
        if(j==-1 || pat[i]==pat[j]){
            i++;
            j++;
            Next[i]=j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    int n,k;
    scanf("%d%d",&n,&k);
    scanf("%s",s);
    kmp_pre(s);
    int cnt=Next[n];
    if(cnt==0){
        for(int i=0;i<k;i++){
            printf("%s",s);
        }
        printf("\n");
    }
    else{
        printf("%s",s);
        for(int i=0;i<k-1;i++){
            printf("%s",s+cnt);
        }
        printf("\n");
    }
    return 0;
}
```





