# PAT 1023 Have Fun with Numbers - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1023 Have Fun with Numbers （20 分)








Notice that the number 123456789 is a 9-digit number consisting exactly the numbers from 1 to 9, with no duplication. Double it we will obtain 246913578, which happens to be another 9-digit number consisting exactly the numbers from 1 to 9, only in a different permutation. Check to see the result if we double it again!

Now you are suppose to check if there are more numbers with this property. That is, double a given number with 

### Input Specification:

Each input contains one test case. Each case contains one positive integer with no more than 20 digits.

### Output Specification:

For each test case, first print in a line "Yes" if doubling the input number gives a number that consists of only a permutation of the digits in the original number, or "No" if not. Then in the next line, print the doubled number.

### Sample Input:

```
1234567899
```

### Sample Output:

```
Yes
2469135798
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

int a[30] = {0};
int count1[10] = {0}; //分别对倍乘前后的位计数
int count2[10] = {0};

int main(){
    string s;
    cin >> s;
    int len = s.size();
    for(int i=0;i < s.size();i++){
        a[i] = s[i]-'0';
    }
    for(int i=0;i < len;i++){
        count1[a[i]]++;
    }
    for(int i=0;i < len/2;i++){ //翻转
        swap(a[i],a[len-i-1]);
    }

    int jinwei = 0;   //倍乘
    for(int i=0;i <= len;i++){
        int num = a[i]*2 + jinwei;
        a[i] = num%10;
        jinwei = (num)/10;
    }

//    for(auto num:a) cout << num << " ";

    int pos = 0;
    for(int i=30;i >= 0;i--){
        if(a[i]){pos = i;break;}
    }
    for(int i=0;i <= pos;i++){
        count2[a[i]]++;
    }

    int flag = 1;
    for(int i=0;i < 10;i++){
        if(count1[i] != count2[i])flag = 0;
    }
    if(flag) cout << "Yes" << endl;
    else cout << "No" << endl;

    for(int i=pos;i >= 0;i--){
        cout << a[i];
    }


    return 0;
}
```

_
` `













