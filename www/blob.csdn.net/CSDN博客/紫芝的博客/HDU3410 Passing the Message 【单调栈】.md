# HDU3410 Passing the Message 【单调栈】 - 紫芝的博客 - CSDN博客





2018年11月12日 19:45:35[紫芝](https://me.csdn.net/qq_40507857)阅读数：250








###  题意

**一个小孩可以看到左边比他矮，但是最高的孩子，同样可以看到右边比他矮，但是最高的小孩**

### 分析

**求左边比他矮，但是最高的孩子：维护一个单调栈，从栈顶到栈底单调递增；**
- 从左边第一个孩子开始，如果栈为空或者当前元素小于栈顶元素，说明他没法看到比他矮的孩子，直接入栈，左值为0；
- 当前元素大于栈顶元素，从栈顶元素出栈，（因为后边的小孩被当前孩子挡住了，不可能看到前面比当前孩子矮的孩子），同时更新最大值，即当前元素能看到的最远的左边界，直到栈为空或者当前元素小于栈顶元素，最后将当前元素入栈

**求右边比他矮，但是最高的孩子：维护一个单调栈，从栈顶到栈底单调递增；从右边第一个小孩开始遍历即可**

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
#define INF 0x3f3f3f3f
const int maxn=1e5+10;
int n;
LL a[maxn];
LL l[maxn],r[maxn];

//维护一个单调栈，从栈顶到栈底单调递增
stack<LL>s;
//左边
void left(){
    while(!s.empty())   s.pop();
    for(int i=1;i<=n;i++){
        LL M=-1;//保存最大值
        int k=0;//保存最大值的下标
        while(!s.empty()&&a[i]>a[s.top()]){//当前元素大于栈顶元素
            if(M<a[s.top()]){//更新
                M=a[s.top()];//后面的元素只会看最高的
                k=s.top();
            }
            s.pop();//全部出栈，没有存在的必要
        }
        if(M==-1)   l[i]=0;//没有找到小于当前元素
        else    l[i]=k;//记录小于当前元素的下标
        s.push(i);
    }
}
void right(){
    while(!s.empty())   s.pop();
    for(int i=n;i>=1;i--){
        LL M=-1;
        int k=0;
        while(!s.empty()&&a[i]>a[s.top()]){
            if(M<a[s.top()]){
                M=a[s.top()];
                k=s.top();
            }
            s.pop();
        }
        if(M==-1)   r[i]=0;
        else    r[i]=k;
        s.push(i);
    }
}
int main(){
    int T;
    int ca=1;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            scanf("%lld",&a[i]);
        }
        left();
        right();
       printf("Case %d:\n",ca++);
       for(int i=1;i<=n;i++){
        printf("%lld %lld\n",l[i],r[i]);
       }
    }
}
```





