# PAT 1011 World Cup Betting - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1011 World Cup Betting （20 分)








With the 2010 FIFA World Cup running, football fans the world over were becoming increasingly excited as the best players from the best teams doing battles for the World Cup trophy in South Africa. Similarly, football betting fans were putting their money where their mouths were, by laying all manner of World Cup bets.

Chinese Football Lottery provided a "Triple Winning" game. The rule of winning was simple: first select any three of the games. Then for each selected game, bet on one of the three possible results -- namely `W` for win, `T` for tie, and `L` for lose. There was an odd assigned to each result. The winner's odd would be the product of the three odds times 65%.

For example, 3 games' odds are given as the following:

```
W    T    L
1.1  2.5  1.7
1.2  3.1  1.6
4.1  1.2  1.1
```

To obtain the maximum profit, one must buy `W` for the 3rd game, `T` for the 2nd game, and `T` for the 1st game. If each bet takes 2 yuans, then the maximum profit would be ( yuans (accurate up to 2 decimal places).

### Input Specification:

Each input file contains one test case. Each case contains the betting information of 3 games. Each game occupies a line with three distinct odds corresponding to `W`, `T` and `L`.

### Output Specification:

For each test case, print in one line the best bet of each game, and the maximum profit accurate up to 2 decimal places. The characters and the number must be separated by one space.

### Sample Input:

```
1.1 2.5 1.7
1.2 3.1 1.6
4.1 1.2 1.1
```

### Sample Output:

```
T T W 39.31
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

double shuzu[3][3];


int main(){

    for(int i=0;i < 3;i++){
        for(int j=0;j < 3;j++){
            cin >> shuzu[i][j];
        }
    }

    double sum = 1.0;
    for(int i=0;i < 3;i++){
        map<double,char> mp;
        mp[shuzu[i][0]] = 'W';
        mp[shuzu[i][1]] = 'T';
        mp[shuzu[i][2]] = 'L';
        double t = max(shuzu[i][0],max(shuzu[i][1],shuzu[i][2]));
        sum*=t;
        cout << mp[t] << " ";
    }
    sum = (sum*0.65-1)*2;
    printf("%.2lf",sum);
    
    return 0;
}
```

傻逼题写了半个小时，还是一开始没想明白就上手写了，结果写的时候一直再改。

还有要熟悉结构化编程，每次实现一个功能可以先试一下正确不正确。
` `













