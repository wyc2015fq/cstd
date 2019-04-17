# 2018ICPC青岛重现赛C - westbrook1998的博客 - CSDN博客





2018年11月11日 13:41:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








给两个01序列，然后s串选择两个区间取反，使s==t，求方案数

也是分类讨论，可惜少想了一个情况…

首先预处理出有多少个区间不同cnt，如果cnt大于2，那肯定是不存在的，答案是0，而如果刚好是两个不同的区间，那就是6，这个试一下就知道了，然后如果cnt是0，也就是两个串本来就是相同的，那答案就是n*(n-1)/2+n，怎么算的呢，既然本来就相同，那就随便选一个区间改两次就相当于没改，所以就是C的n取2，然后一开始忘记加上n，因为区间可以是1个字符，然后当cnt为1的情况，一开始分了两种情况，一直wa，后来发现这两种情况可以合并的，当只有一个区间不同时，如果这个不同的区间就是整个串，那就是相当于要把这个串分成两部分然后分别取反，那就是2*(n-1)，而如果这个不同区间左右还有相同的区间，一开始想的是改变这个不同的区间连上左右两边的，第二次就把相同的那部分重新取反回来，所以答案是2*(n-len) ，其中len为相同区间总长度，结果就是这里想错了，还有方案是可以只取反不同区间的那部分，就相当于要加上2*(len-1)，这样这两种情况其实就可以合并了
代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e6+50;
typedef long long ll;
int T;
ll n;
char s[N],t[N];
//string s,t;
int main(void){
    scanf("%d",&T);
    while(T--){
        scanf("%lld",&n);
        // cin >> s;
        // cin >> t;
        scanf("%s",s);
        scanf("%s",t);
        int cnt=0;
        int len=0;
        for(int i=0;i<n;i++){
            if(s[i]!=t[i]){
                cnt++;
                while(s[i]!=t[i] && i<n){
                    i++;
                    len++;
                }  
            }
        }
        if(cnt>2){
            printf("0\n");
        }else if(cnt==2){
            printf("6\n");
        }else if(cnt==1){
            printf("%lld\n",2*(n-1));
            // if(n==len){
            //     printf("%lld\n",2*(n-1));
            // }else{
            //     printf("%lld\n",2*(n-len));
            // }
        }else if(cnt==0){
            printf("%lld\n",n*(n-1)/2+n);
        }
    }
    return 0;
}
```




