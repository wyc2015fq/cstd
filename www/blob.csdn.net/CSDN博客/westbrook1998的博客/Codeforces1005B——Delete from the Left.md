# Codeforces1005B——Delete from the Left - westbrook1998的博客 - CSDN博客





2018年08月08日 22:20:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：38








> 
You are given two strings s and t. In a single move, you can choose any of two strings and delete the first (that is, the leftmost) character. After a move, the length of the string decreases by 1. You can’t choose a string if it is empty. 

  For example: 

  by applying a move to the string “where”, the result is the string “here”, 

  by applying a move to the string “a”, the result is an empty string “”. 

  You are required to make two given strings equal using the fewest number of moves. It is possible that, in the end, both strings will be equal to the empty string, and so, are equal to each other. In this case, the answer is obviously the sum of the lengths of the initial strings. 

  Write a program that finds the minimum number of moves to make two given strings s and t equal. 

  Input 

  The first line of the input contains s. In the second line of the input contains t. Both strings consist only of lowercase Latin letters. The number of letters in each string is between 1 and 2⋅105, inclusive. 

  Output 

  Output the fewest number of moves required. It is possible that, in the end, both strings will be equal to the empty string, and so, are equal to each other. In this case, the answer is obviously the sum of the lengths of the given strings. 

  Examples 

  Input 

  test 

  west 

  Output 

  2 

  Input 

  codeforces 

  yes 

  Output 

  9 

  Input 

  test 

  yes 

  Output 

  7 

  Input 

  b 

  ab 

  Output 

  1 

  Note 

  In the first example, you should apply the move once to the first string and apply the move once to the second string. As a result, both strings will be equal to “est”. 

  In the second example, the move should be applied to the string “codeforces” 8 times. As a result, the string becomes “codeforces” → “es”. The move should be applied to the string “yes” once. The result is the same string “yes” → “es”. 

  In the third example, you can make the strings equal only by completely deleting them. That is, in the end, both strings will be empty. 

  In the fourth example, the first character of the second string should be deleted.
两个字符串 从左边删除 问一个删除多少个后两个字符串相同（或者空）  

思路就是从后面遍历 找出两个字符串相同的后缀大小 然后用两个字符串总长度去减

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=200050;
char s[MAXN];
char t[MAXN];
int main(void){
    scanf("%s",s);
    scanf("%s",t);
    int sl=strlen(s);
    int tl=strlen(t);
    int same=0;
    int min_len=min(sl,tl);
    for(int i=0;i<min_len;i++){
        if(s[sl-1-i]==t[tl-1-i]){
            same++;
        }
        else{
            break;
        }
    }
    printf("%d\n",sl+tl-same*2);
    return 0;
}
```





