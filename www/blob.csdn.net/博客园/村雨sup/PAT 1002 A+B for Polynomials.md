# PAT 1002 A+B for Polynomials - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1002 A+B for Polynomials （25 分)








This time, you are supposed to find 

### Input Specification:

Each input file contains one test case. Each case occupies 2 lines, and each line contains the information of a polynomial:



where 

### Output Specification:

For each test case you should output the sum of 

### Sample Input:

```
2 1 2.4 0 3.2
2 2 1.5 1 0.5
```

### Sample Output:

```
3 2 1.5 1 2.9 0 3.2
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


int main(){
    map<int,double> mp;
    map<int,double>::iterator it;

    int n;
    cin >> n;
    while(n--){
        int x;double y;
        cin >> x >> y;
        mp[x]+=y;
    }
    cin >> n;
    while(n--){
        int x;double y;
        cin >> x >> y;
        mp[x]+=y;
    }
    vector<pair<int,double>> vec;
    for(it=mp.begin();it!=mp.end();it++){
//        cout << it->first << " " << it->second << " ";
        if(it->second != 0) {
            vec.push_back(make_pair(it->first, it->second));
        }
    }
    cout << vec.size();
    for(int i=vec.size()-1;i >= 0;i--){
//        cout << vec[i].first << " " << fixed << setprecision(1)<< vec[i].second;
        printf(" %d %.1f",vec[i].first,vec[i].second);

    }

    return 0;
}
```

——map  make_pair vector 之间的骚操作。。
` `













