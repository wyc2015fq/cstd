# hdu2673——shǎ崽 OrOrOrOrz - westbrook1998的博客 - CSDN博客





2018年08月04日 18:20:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：54标签：[优先队列](https://so.csdn.net/so/search/s.do?q=优先队列&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Acmer in HDU-ACM team are ambitious, especially shǎ崽, he can spend time in Internet bar doing problems overnight. So many girls want to meet and Orz him. But Orz him is not that easy.You must solve this problem first.  

  The problem is :  

  Give you a sequence of distinct integers, choose numbers as following : first choose the biggest, then smallest, then second biggest, second smallest etc. Until all the numbers was chosen .  

  For example, give you 1 2 3 4 5, you should output 5 1 4 2 3  

  Input 

  There are multiple test cases, each case begins with one integer N(1 <= N <= 10000), following N distinct integers. 

  Output 

  Output a sequence of distinct integers described above. 

  Sample Input 

  5 

  1 2 3 4 5 

  Sample Output 

  5 1 4 2 3
优先队列瞎搞 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;
//默认从大到小
priority_queue<int> b;
//从小到大
priority_queue<int,vector<int>,greater<int>> s;
vector<int> res;
int main(void){
    int n;
    while(~scanf("%d",&n)){
        res.clear();
        while(!b.empty()){
            b.pop();
        }
        while(!s.empty()){
            s.pop();
        }
        int t;
        for(int i=0;i<n;i++){
            scanf("%d",&t);
            b.push(t);
            s.push(t);
        }
        for(int i=0;i<(n+1)/2;i++){
            if(b.top()!=s.top()){
                res.push_back(b.top());
                res.push_back(s.top());
                b.pop();
                s.pop();
            }
            else{
                res.push_back(b.top());
                b.pop();
                s.pop();
            }
        }
        for(int i=0;i<res.size()-1;i++){
            printf("%d ",res[i]);
        }
        printf("%d\n",res[res.size()-1]);
    }
    return 0;
}
```





