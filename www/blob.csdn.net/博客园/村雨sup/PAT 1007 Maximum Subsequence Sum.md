# PAT 1007 Maximum Subsequence Sum - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1007 Maximum Subsequence Sum （25 分)








Given a sequence of 

Now you are supposed to find the largest sum, together with the first and the last numbers of the maximum subsequence.

### Input Specification:

Each input file contains one test case. Each case occupies two lines. The first line contains a positive integer 

### Output Specification:

For each test case, output in one line the largest sum, together with the first and the last numbers of the maximum subsequence. The numbers must be separated by one space, but there must be no extra space at the end of a line. In case that the maximum subsequence is not unique, output the one with the smallest indices 

### Sample Input:

```
10
-10 1 2 3 4 -5 -23 3 7 -21
```

### Sample Output:
`10 1 4`


```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;



int main(){
    int n;
    cin >> n;
    int num[n];
    int flag = 1;
    for(int i=0;i < n;i++) {
        cin >> num[i];
        if(num[i] >= 0)flag = 0;
    }
    if(flag){
        cout << 0<<" " << num[0] <<" "<< num[n-1];
        return 0;
    }


    int sum = 0;
    int maxsum = 0;
    int j=0,k=0;
    int movej=0;

    for(int i=0;i < n;i++){
        sum += num[i];
        if(sum < 0) {sum = 0;movej=i+1;}
        if(maxsum < sum){
            j = movej;k = i;
            maxsum = sum;
        }
    }
    if(num[j] < 0)num[j] = 0;
    if(num[k] < 0)num[k] = 0;

    cout << maxsum << " " << num[j] << " " << num[k];


    return 0;
}
```

看错输出，应该是输出数字的，我输出下标了，还过了2个数据点？？

然后边界条件有个

3
-1 -1 0

应该是输出0 0 0














