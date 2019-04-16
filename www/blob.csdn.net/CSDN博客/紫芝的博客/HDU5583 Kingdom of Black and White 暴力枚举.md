# HDU5583 Kingdom of Black and White 暴力枚举 - 紫芝的博客 - CSDN博客





2018年09月19日 10:35:02[紫芝](https://me.csdn.net/qq_40507857)阅读数：33
个人分类：[枚举](https://blog.csdn.net/qq_40507857/article/category/7595712)









# Kingdom of Black and White

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 4909    Accepted Submission(s): 1470****

**Problem Description**

In the Kingdom of Black and White (KBW), there are two kinds of frogs: black frog and white frog.


Now *N* frogs are standing in a line, some of them are black, the others are white. The total strength of those frogs are calculated by dividing the line into minimum parts, each part should still be continuous, and can only contain one kind of frog. Then the strength is the sum of the squared length for each part.


However, an old, evil witch comes, and tells the frogs that she will change the color of **at most one** frog and thus the strength of those frogs might change.


The frogs wonder the **maximum** possible strength after the witch finishes her job.

**Input**

First line contains an integer *T*, which indicates the number of test cases.


Every test case only contains a string with length *N*, including only 0 (representing

a black frog) and 1 (representing a white frog).


⋅ 1≤*T*≤50.


⋅ for 60% data, 1≤*N*≤1000.


⋅ for 100% data, 1≤*N*≤105.


⋅ the string only contains 0 and 1.

**Output**

For every test case, you should output "**Case #x: y**",where *x* indicates the case number and counts from 1 and *y* is the answer.

**Sample Input**

2 

000011 

0101

**Sample Output**

Case #1: 26 

Case #2: 10

**Source**

[2015ACM/ICPC亚洲区上海站-重现赛（感谢华东理工）](http://acm.hdu.edu.cn/search.php?field=problem&key=2015ACM%2FICPC%D1%C7%D6%DE%C7%F8%C9%CF%BA%A3%D5%BE-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%BB%AA%B6%AB%C0%ED%B9%A4%A3%A9&source=1&searchmode=source)


首先把相邻相同的字符分块，然后对于第i个分块和第i+1个分块，

枚举：

1.第i个分块字符个数+1和第i+1个分块字符个数-1的情况

2.第i个分块字符个数-1和第i+1个分块字符个数+1的情况

注意：

1.当第i+1个分块只有一个时，第i个分块、第i+1个分块、第i+2个分块会合成一个分块

2.当第i个分块只有一个时，第i-1个分块、第i个分块、第i+1个分块会合成一个分块

```cpp
#include<map>
#include<stack>
#include<queue>
#include<vector>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;
typedef long long ll;
#define inf 1000000000
#define MOD 100000000
#define  lowbit(x) (x&-x)
#define  eps 1e-10
const int mod=1000000007;
const int maxn=1e5+7;
char str[maxn];
ll v[maxn];
int main()
{
    int T;
    int ca=1;
    scanf("%d",&T);
    while(T--){
        scanf("%s",str);
      ll len=1;
      int k=1;
      ll ans=0;
      for(int i=1;str[i];i++)
      {
          if(str[i-1]!=str[i])
          {
              ans+=len*len;
              v[k++]=len;
              len=1;
          }
          else  len++;
      }
      v[k++]=len;
      ans+=len*len;
      v[0]=0;v[k]=0;
      ll MAX=ans;

       for(int i=1;i<k-1;i++)
       {
           //printf("%d,",v[i]);
           if(v[i+1]==1)
           {
               ll before=v[i]*v[i]+v[i+1]*v[i+1]+v[i+2]*v[i+2];
               ll now=(v[i]+v[i+1]+v[i+2])*(v[i]+v[i+1]+v[i+2]);
               MAX=max(MAX,ans-before+now);
           }
        else{
            ll before=v[i]*v[i]+v[i+1]*v[i+1];
            ll now=(v[i]+1)*(v[i]+1)+(v[i+1]-1)*(v[i+1]-1);
            MAX=max(MAX,ans-before+now);
        }
        if(v[i]==1)
        {
            ll before=v[i-1]*v[i-1]+v[i]*v[i]+v[i+1]*v[i+1];
            ll now=(v[i-1]+v[i]+v[i+1])*(v[i-1]+v[i]+v[i+1]);
            MAX=max(MAX,ans-before+now);
        }
        else{
            ll before=v[i]*v[i]+v[i+1]*v[i+1];
            ll now=(v[i]-1)*(v[i]-1)+(v[i+1]+1)*(v[i+1]+1);
            MAX=max(MAX,ans-before+now);
        }
       }
      printf("Case #%d: %lld\n",ca++,MAX);
}
 return 0;
}
```





