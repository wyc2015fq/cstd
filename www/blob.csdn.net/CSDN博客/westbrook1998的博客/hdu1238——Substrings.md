# hdu1238——Substrings - westbrook1998的博客 - CSDN博客





2018年08月21日 21:22:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33标签：[字符串																[暴力](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
You are given a number of case-sensitive strings of alphabetic characters, find the largest string X, such that either X, or its inverse can be found as a substring of any of the given strings.  

  Input 

  The first line of the input file contains a single integer t (1 <= t <= 10), the number of test cases, followed by the input data for each test case. The first line of each test case contains a single integer n (1 <= n <= 100), the number of given strings, followed by n lines, each representing one string of minimum length 1 and maximum length 100. There is no extra white space before and after a string.  

  Output 

  There should be one line per test case containing the length of the largest string found.  

  Sample Input 

  2 

  3 

  ABCD 

  BCDFF 

  BRCD 

  2 

  rose 

  orchid 

  Sample Output 

  2 

  2
这题是求出几个字符串的共同的最大子串，这题数据量很小 暴力即可 

用string类的substr和find

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
string s[105];
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        int _min=0x3f3f3f;;
        int idx=0;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            cin>>s[i];
            if(s[i].size()<_min){
                _min=s[i].size();
                idx=i;
            }
        }
        int ans=0;
        //从大到小枚举最短串的子串长度
        for(int i=_min;i>0;i--){
            //枚举子串开头
            for(int j=0;j<_min-i+1;j++){
                string s1=s[idx].substr(j,i);
                string s2=s1;
                reverse(s2.begin(),s2.end());
                //枚举其他串
                int k;
                for(k=0;k<n;k++){
                    if(k==idx){
                        continue;
                    }
                    if(s[k].find(s1,0)==-1 && s[k].find(s2,0)==-1){
                        break;
                    }
                }
                if(k==n){
                    ans=max(ans,i);
                }
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```






