# PAT 1015 Reversible Primes - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1015 Reversible Primes （20 分)








A reversible prime in any number system is a prime whose "reverse" in that number system is also a prime. For example in the decimal system 73 is a reversible prime because its reverse 37 is also a prime.

Now given any two positive integers 

### Input Specification:

The input file consists of several test cases. Each case occupies a line which contains two integers 

### Output Specification:

For each test case, print in one line `Yes` if 

### Sample Input:

```
73 10
23 2
23 10
-2
```

### Sample Output:

```
Yes
Yes
No
```



```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

bool check[maxnum];
int prime[maxnum] = {0};


void isprime(){
    int cnt = 0;
    memset(check,true,sizeof(check));
    for(int i=2;i < maxnum;i++){
        if(check[i])prime[++cnt] = i;
        for(int j=1;j <= cnt;j++){
            if(i*prime[j] > maxnum)break;
            check[i*prime[j]] = false;
            if(i%prime[j] == 0)break;
        }
    }
}






int main(){
    int a,b;
    isprime();
    check[0] = false;
    check[1] = false;

//    for(int i=0;i < 1000;i++){
//        cout << check[i] << " ";
//    }
    while(scanf("%d",&a)){
        if(a < 0) break;
        scanf("%d",&b);
        vector<int> vec;
        int t = a;
        while(a){
            vec.push_back(a%b);
            a = a/b;
        }
        int sum = 0;
        int cnt = 1;
        for(int i=vec.size()-1;i >= 0;i--){
            sum += cnt*vec[i];
            cnt *= b;
        }
        if(check[sum]&&check[t]) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
    return 0;
}
```

复习欧拉筛














