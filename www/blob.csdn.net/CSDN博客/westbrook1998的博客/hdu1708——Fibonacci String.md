# hdu1708——Fibonacci String - westbrook1998的博客 - CSDN博客





2018年05月17日 12:29:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：60








> 
After little Jim learned Fibonacci Number in the class , he was very interest in it.  

  Now he is thinking about a new thing – Fibonacci String .  

  He defines : str[n] = str[n-1] + str[n-2] ( n > 1 )  

  He is so crazying that if someone gives him two strings str[0] and str[1], he will calculate the str[2],str[3],str[4] , str[5]….  

  For example :  

  If str[0] = “ab”; str[1] = “bc”;  

  he will get the result , str[2]=”abbc”, str[3]=”bcabbc” , str[4]=”abbcbcabbc” …………;  

  As the string is too long ,Jim can’t write down all the strings in paper. So he just want to know how many times each letter appears in Kth Fibonacci String . Can you help him ?  

  Input 

  The first line contains a integer N which indicates the number of test cases.  

  Then N cases follow.  

  In each case,there are two strings str[0], str[1] and a integer K (0 <= K < 50) which are separated by a blank.  

  The string in the input will only contains less than 30 low-case letters.  

  Output 

  For each case,you should count how many times each letter appears in the Kth Fibonacci String and print out them in the format “X:N”.  

  If you still have some questions, look the sample output carefully.  

  Please output a blank line after each test case.  

  To make the problem easier, you can assume the result will in the range of int.  

  Sample Input 

  1 

  ab bc 3 

  Sample Output 

  a:1 

  b:3 

  c:2 

  d:0 

  e:0 

  f:0 

  g:0 

  h:0 

  i:0 

  j:0 

  k:0 

  l:0 

  m:0 

  n:0 

  o:0 

  p:0 

  q:0 

  r:0 

  s:0 

  t:0 

  u:0 

  v:0 

  w:0 

  x:0 

  y:0 

  z:0
斐波那其字符串，求第k个字符串中每个字母出现的个数，用一个二维数组解决

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int MAXN=50;
int dp[MAXN][MAXN];
int main(void){
    int t;
    char a[55];
    char b[55];
    scanf("%d",&t);
    while(t--){
        int k;
        scanf("%s %s %d",a,b,&k);
        memset(dp,0,sizeof(dp));
        for(int i=0;a[i]!='\0';i++){
            dp[0][a[i]-'a']++;
        }
        for(int i=0;b[i]!='\0';i++){
            dp[1][b[i]-'a']++;
        }
        for(int i=2;i<=k;i++){
            for(int j=0;j<26;j++){
                dp[i][j]=dp[i-1][j]+dp[i-2][j];
            }
        }
        for(int i=0;i<26;i++){
            printf("%c:%d\n",i+'a',dp[k][i]);
        }
        printf("\n");
    }
    return 0;
}
```





