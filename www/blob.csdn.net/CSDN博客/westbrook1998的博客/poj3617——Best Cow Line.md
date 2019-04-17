# poj3617——Best Cow Line - westbrook1998的博客 - CSDN博客





2018年08月06日 21:53:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
FJ is about to take his N (1 ≤ N ≤ 2,000) cows to the annual”Farmer of the Year” competition. In this contest every farmer arranges his cows in a line and herds them past the judges. 

  The contest organizers adopted a new registration scheme this year: simply register the initial letter of every cow in the order they will appear (i.e., If FJ takes Bessie, Sylvia, and Dora in that order he just registers BSD). After the registration phase ends, every group is judged in increasing lexicographic order according to the string of the initials of the cows’ names. 

  FJ is very busy this year and has to hurry back to his farm, so he wants to be judged as early as possible. He decides to rearrange his cows, who have already lined up, before registering them. 

  FJ marks a location for a new line of the competing cows. He then proceeds to marshal the cows from the old line to the new one by repeatedly sending either the first or last cow in the (remainder of the) original line to the end of the new line. When he’s finished, FJ takes his cows for registration in this new order. 

  Given the initial order of his cows, determine the least lexicographic string of initials he can make this way. 

  Input 

  * Line 1: A single integer: N 

  * Lines 2..N+1: Line i+1 contains a single initial (‘A’..’Z’) of the cow in the ith position in the original line 

  Output 

  The least lexicographic string he can make. Every line (except perhaps the last one) contains the initials of 80 cows (‘A’..’Z’) in the new line. 

  Sample Input 

  6 

  A 

  C 

  D 

  B 

  C 

  B 

  Sample Output 

  ABCBCD
贪心  

犯了一下几个错误 

第一次是直接while循环输出 就没考虑当前比较的两个字符相同的情况 这种情况要继续判断下一对 而不能直接随便输出一个 

然后第二次是没注意80个要换行 

第三次是80个到了之后没清零…
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=2010;
char s[MAXN];
int main(void){
    int n;
    scanf("%d",&n);
    getchar();
    for(int i=0;i<n;i++){
        s[i]=getchar();
        getchar();
    }
    int cnt=0;
    int l=0;
    int r=n-1;
    while(l<=r){
        //标记取的地方
        bool left=false;
        for(int i=0;l+i<=r;i++){
            if(s[l+i]<s[r-i]){
                left=true;
                break;
            }
            else if(s[l+i]>s[r-i]){
                left=false;
                break;
            }
        }
        if(left){
            putchar(s[l++]);

        }
        else{
            putchar(s[r--]);
        }
        cnt++;
        if(cnt==80){
            putchar('\n');
            cnt=0;
        }
    }
    putchar('\n');
    return 0;
}
```






