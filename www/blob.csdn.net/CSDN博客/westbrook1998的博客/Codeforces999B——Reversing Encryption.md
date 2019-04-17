# Codeforces999B——Reversing Encryption - westbrook1998的博客 - CSDN博客





2018年08月10日 17:44:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30标签：[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
A string s of length n can be encrypted by the following algorithm: 

  iterate over all divisors of n in decreasing order (i.e. from n to 1), 

  for each divisor d, reverse the substring s[1…d] (i.e. the substring which starts at position 1 and ends at position d). 

  For example, the above algorithm applied to the string s=”codeforces” leads to the following changes: “codeforces” → “secrofedoc” → “orcesfedoc” → “rocesfedoc” → “rocesfedoc” (obviously, the last reverse operation doesn’t change the string because d=1). 

  You are given the encrypted string t. Your task is to decrypt this string, i.e., to find a string s such that the above algorithm results in string t. It can be proven that this string s always exists and is unique. 

  Input 

  The first line of input consists of a single integer n (1≤n≤100) — the length of the string t. The second line of input consists of the string t. The length of t is n, and it consists only of lowercase Latin letters. 

  Output 

  Print a string s such that the above algorithm results in t. 

  Examples 

  Input 

  10 

  rocesfedoc 

  Output 

  codeforces 

  Input 

  16 

  plmaetwoxesisiht 

  Output 

  thisisexampletwo 

  Input 

  1 

  z 

  Output 

  z 

  Note 

  The first example is described in the problem statement.
题目好像说的不是很好 不过看样例也能看出来 

我的思路是首先暴力将所有因数存起来 

然后就暴力反转即可
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
string s;
int main(void){
    int n;
    scanf("%d",&n);
    cin >> s;
    priority_queue<int,vector<int>,greater<int>> divi;
    for(int i=1;i*i<=n;i++){
        if(n%i==0){
            divi.push(i);
            divi.push(n/i);
        }
    }
    int pre=0;
    while(divi.size()!=0){
        int d=divi.top();
        divi.pop();
        if(pre==d){
            continue;
        }
        pre=d;
        int i=0;
        int j=d-1;
        while(i<j){
            char t=s[i];
            s[i]=s[j];
            s[j]=t;
            i++;
            j--;
        }
    }
    cout << s << endl;
    return 0;
}
```






