# Codeforces977B——Two-gram - westbrook1998的博客 - CSDN博客





2018年08月15日 11:33:32[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40








> 
Two-gram is an ordered pair (i.e. string of length two) of capital Latin letters. For example, “AZ”, “AA”, “ZA” — three distinct two-grams. 

  You are given a string s consisting of n capital Latin letters. Your task is to find any two-gram contained in the given string as a substring (i.e. two consecutive characters of the string) maximal number of times. For example, for string s = “BBAABBBA” the answer is two-gram “BB”, which contained in s three times. In other words, find any most frequent two-gram. 

  Note that occurrences of the two-gram can overlap with each other. 

  Input 

  The first line of the input contains integer number n (2≤n≤100) — the length of string s. The second line of the input contains the string s consisting of n capital Latin letters. 

  Output 

  Print the only line containing exactly two capital Latin letters — any two-gram contained in the given string s as a substring (i.e. two consecutive characters of the string) maximal number of times. 

  Examples 

  Input 

  7 

  ABACABA 

  Output 

  AB 

  Input 

  5 

  ZZZAA 

  Output 

  ZZ 

  Note 

  In the first example “BA” is also valid answer. 

  In the second example the only two-gram “ZZ” can be printed because it contained in the string “ZZZAA” two times.
我是用一个26*26的数组来记录两个大写字母串出现的次数

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int lt[26][26];
char s[105];
int main(void){
    int n;
    scanf("%d",&n);
    scanf("%s",s);
    for(int i=0;i<n-1;i++){
        lt[s[i]-'A'][s[i+1]-'A']++;
    }
    int ans=0;
    int ai=0;
    int aj=0;
    for(int i=0;i<26;i++){
        for(int j=0;j<26;j++){
            if(lt[i][j]>ans){
                ans=lt[i][j];
                ai=i;
                aj=j;
            }
        }
    }
    printf("%c%c\n",(char)(ai+'A'),(char)(aj+'A'));
    return 0;
}
```





