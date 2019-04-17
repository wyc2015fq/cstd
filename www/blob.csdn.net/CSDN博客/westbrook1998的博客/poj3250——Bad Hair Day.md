# poj3250——Bad Hair Day - westbrook1998的博客 - CSDN博客





2018年11月03日 12:46:17[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30标签：[单调栈](https://so.csdn.net/so/search/s.do?q=单调栈&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









给出几个牛的高度，牛只能向右看到比他挨的，求出所有牛一共可以看到的数量

学了单调栈，这题就是很模板的题目了，求出每个数作为最大值能向右延伸到最远的位置

代码：

```
#include <cstdio>
#include <stack>
#include <algorithm>
#include <cstring>
int n;
const int N=80050;
typedef long long ll;
ll a[N];
using namespace std;
int main(void){
    while(~scanf("%d",&n)){
        memset(a,0,sizeof(a));
        stack<int> s;
        for(int i=0;i<n;i++){
            scanf("%lld",&a[i]);
        }
        ll ans=0;
        for(int i=n-1;i>=0;i--){
            while(s.size()>0 && a[i]>a[s.top()]){
                s.pop();
            }
            if(s.size()==0){
                //printf("%d %d\n",i,n-1);
                ans+=n-1-i;
            }
            else{
                //printf("%d %d\n",i,s.top()-1);
                ans+=s.top()-i-1;
            }
            s.push(i);
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```



