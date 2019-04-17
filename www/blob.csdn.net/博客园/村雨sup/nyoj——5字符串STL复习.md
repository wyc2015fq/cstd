# nyoj——5字符串STL复习 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## Binary String Matching

时间限制：3000 ms  |  内存限制：65535 KB

难度：3



- 描述
Given two strings A and B, whose alphabet consist only ‘0’ and ‘1’. Your task is only to tell how many times does A appear as a substring of B? For example, the text string B is ‘1001110110’ while the pattern string A is ‘11’, you should output 3, because the pattern A appeared at the posit

- 输入
The first line consist only one integer N, indicates N cases follows. In each case, there are two lines, the first line gives the string A, length (A) <= 10, and the second line gives the string B, length (B) <= 1000. And it is guaranteed that B is always longer than A.- 输出
For each case, output a single line consist a single integer, tells how many times do B appears as a substring of A.- 样例输入
3
11
1001110110
101
110010010010001
1010
110100010101011 - 样例输出
3
0
3 
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000000;


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        string pattern;
        string sym;
        cin >> pattern >>sym;
        int cnt = 0;
        while(sym.length()){
            if(sym.find(pattern) > 1005)
                break;
            int pos = sym.find(pattern);
            sym = sym.substr(pos+1);
            cnt++;
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```

——











