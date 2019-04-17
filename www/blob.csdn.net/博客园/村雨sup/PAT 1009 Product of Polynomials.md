# PAT 1009 Product of Polynomials - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1009 Product of Polynomials （25 分)








This time, you are supposed to find 

### Input Specification:

Each input file contains one test case. Each case occupies 2 lines, and each line contains the information of a polynomial:



where 

### Output Specification:

For each test case you should output the product of 

### Sample Input:

```
2 1 2.4 0 3.2
2 2 1.5 1 0.5
```

### Sample Output:

```
3 3 3.6 2 6.0 1 1.6
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

double a[1005] = {0};
double b[1005] = {0};
double c[maxnum] = {0};


int main(){
    int n;cin >> n;
    int t;
    for(int i=0;i < n;i++){
        int x;double y;
        cin >> x >> y;
        a[x] = y;
        if(!i)t = x;
    }
    int m;cin >> m;
    int start;
    for(int i=0;i < m;i++){
        int x;double y;
        cin >> x >> y;
        b[x] = y;
        if(!i) start = x;
    }

    for(int i=0;i < t+5;i++){
        for(int j=0;j < start+5;j++){
            c[i+j] += a[i]*b[j];
        }
    }

    int cnt = 0;
    for(int i=0;i < maxnum;i++)if(c[i])cnt++;
    cout << cnt;

    for(int i=maxnum-1;i>=0;i--){
        if(c[i]) printf(" %d %.1lf",i,c[i]);
    }


    return 0;
}
```

空间开的有点小炸，做了一下优化就好了，再一次理解错了提议，K<10，不一定就项系数<10，还好这题没出毒瘤卡边界数据，在超过1e5的地方直接放弃了。。
` `













