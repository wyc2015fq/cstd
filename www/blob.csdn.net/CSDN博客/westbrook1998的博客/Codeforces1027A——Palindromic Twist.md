# Codeforces1027A——Palindromic Twist - westbrook1998的博客 - CSDN博客





2018年08月28日 12:59:21[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given a string s consisting of n lowercase Latin letters. n is even. 

  For each position i (1≤i≤n) in string s you are required to change the letter on this position either to the previous letter in alphabetic order or to the next one (letters ‘a’ and ‘z’ have only one of these options). Letter in every position must be changed exactly once. 

  For example, letter ‘p’ should be changed either to ‘o’ or to ‘q’, letter ‘a’ should be changed to ‘b’ and letter ‘z’ should be changed to ‘y’. 

  That way string “codeforces”, for example, can be changed to “dpedepqbft” (‘c’ → ‘d’, ‘o’ → ‘p’, ‘d’ → ‘e’, ‘e’ → ‘d’, ‘f’ → ‘e’, ‘o’ → ‘p’, ‘r’ → ‘q’, ‘c’ → ‘b’, ‘e’ → ‘f’, ‘s’ → ‘t’). 

  String s is called a palindrome if it reads the same from left to right and from right to left. For example, strings “abba” and “zz” are palindromes and strings “abca” and “zy” are not. 

  Your goal is to check if it’s possible to make string s a palindrome by applying the aforementioned changes to every position. Print “YES” if string s can be transformed to a palindrome and “NO” otherwise. 

  Each testcase contains several strings, for each of them you are required to solve the problem separately. 

  Input 

  The first line contains a single integer T (1≤T≤50) — the number of strings in a testcase. 

  Then 2T lines follow — lines (2i−1) and 2i of them describe the i-th string. The first line of the pair contains a single integer n (2≤n≤100, n is even) — the length of the corresponding string. The second line of the pair contains a string s, consisting of n lowercase Latin letters. 

  Output 

  Print T lines. The i-th line should contain the answer to the i-th string of the input. Print “YES” if it’s possible to make the i-th string a palindrome by applying the aforementioned changes to every position. Print “NO” otherwise. 

  Example 

  Input 

  5 

  6 

  abccba 

  2 

  cf 

  4 

  adfa 

  8 

  abaazaba 

  2 

  ml 

  Output 

  YES 

  NO 

  YES 

  NO 

  NO 

  Note 

  The first string of the example can be changed to “bcbbcb”, two leftmost letters and two rightmost letters got changed to the next letters, two middle letters got changed to the previous letters. 

  The second string can be changed to “be”, “bg”, “de”, “dg”, but none of these resulting strings are palindromes. 

  The third string can be changed to “beeb” which is a palindrome. 

  The fifth string can be changed to “lk”, “lm”, “nk”, “nm”, but none of these resulting strings are palindromes. Also note that no letter can remain the same, so you can’t obtain strings “ll” or “mm”.
其实题意细节没怎么读懂 就是一个字母可以-1也可以+1 然后看能不能转换为回文串 

但是ml这个是输出No 我就猜两个字符都要同时变换的  

所以就直接双指针判断两个字符是否相等和是否相差2
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
int n;
char s[105];
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        scanf("%s",s);
        int l=0,r=n-1;
        bool flag=false;
        while(l<=r){
            if(s[l]!=s[r] && abs(s[l]-s[r])!=2){
                flag=true;
                break;
            }
            l++;
            r--;
        }
        if(flag){
            printf("NO\n");
        }
        else{
            printf("YES\n");
        }
    }
    return 0;
}
```






