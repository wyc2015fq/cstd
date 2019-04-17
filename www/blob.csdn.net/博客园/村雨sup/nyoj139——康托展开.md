# nyoj139——康托展开 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 我排第几个

时间限制：1000 ms  |  内存限制：65535 KB

难度：3



- 描述

现在有"abcdefghijkl”12个字符，将其所有的排列中按字典序排列，给出任意一种排列，说出这个排列在所有的排列中是第几小的？


- 输入
第一行有一个整数n（0<n<=10000）;
随后有n行，每行是一个排列；- 输出
输出一个整数m，占一行，m表示排列是第几位；- 样例输入
3
abcdefghijkl
hgebkflacdji
gfkedhjblcia- 样例输出
1
302715242
260726926


```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int maxn = 1000000;
const int moder = 1000000;

//阶乘

int fac[12] = {39916800,3628800,362880,40320,5040,720,120,24,6,2,1,1};


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        string s1;
        cin >> s1;
        ll res = 0;
        int a[12];
        for(int i=0;i < 12;i++){
            a[i] = s1[i] - 'a';
        }
        for(int i=0;i < 12;i++){
            int cnt = 0;
            for(int j=i+1;j < 12;j++){
                if(a[j] < a[i]) cnt++;
            }
            res += fac[i]*cnt;
        }
        printf("%lld\n",res+1);
    }
    return 0;
}
```

——很痛苦，有点难，每次向后找比自己小的数的个数，然后乘以对应的阶乘











