# Codeforces1064C——Oh Those Palindromes - westbrook1998的博客 - CSDN博客





2018年10月14日 20:33:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：73








> 
给一个字符串，叫你重新排序，使得该字符串的权值最大，这个权值由该字符串所有是回文串的子串（包括本身，也包括单个字符）的长度之和


想了一个xjb贪心的算法，然后还没码完，队友发现直接排序即可… 就是把相同的字符放一起，也算是一种贪心吧，原理未知

过了

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int n;
const int N=1e5+50;
char s[N];
int main(void){
    scanf("%d",&n);
    scanf("%s",s);
    sort(s,s+n);
    printf("%s\n",s);
    return 0;
}
```



