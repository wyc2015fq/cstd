# PAT 1019 General Palindromic Number - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1019 General Palindromic Number （20 分)








A number that will be the same when it is written forwards or backwards is known as a Palindromic Number. For example, 1234321 is a palindromic number. All single digit numbers are palindromic numbers.

Although palindromic numbers are most often considered in the decimal system, the concept of palindromicity can be applied to the natural numbers in any numeral system. Consider a number 

Given any positive decimal integer 

### Input Specification:

Each input file contains one test case. Each case consists of two positive numbers 

### Output Specification:

For each test case, first print in one line `Yes` if 

### Sample Input 1:

```
27 2
```

### Sample Output 1:

```
Yes
1 1 0 1 1
```

### Sample Input 2:

```
121 5
```

### Sample Output 2:

```
No
4 4 1
```

鸣谢网友“CCPC拿不到牌不改名”修正数据！

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005






int main(){
    int a,b;
    scanf("%d%d",&a,&b);
    vector<int> vec;
    while(a){
        vec.push_back(a%b);
        a /= b;
    }
//    for(auto num:vec)cout << num << " ";

    int flag = 1;
    for(int i=0;i < vec.size()/2+1;i++){
        if(vec[i] != vec[vec.size()-i-1])flag = 0;
    }
    if(flag)cout << "Yes" << endl;
    else cout << "No" << endl;

    for(int i=vec.size()-1;i >= 0;i--){
        cout << vec[i];
        if(i!=0)cout << " ";
    }
    return 0;
}
```
















