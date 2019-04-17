# nyoj——113 getline - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 字符串替换

时间限制：3000 ms  |  内存限制：65535 KB

难度：2



- 描述
编写一个程序实现将字符串中的所有"you"替换成"we"

- 输入
输入包含多行数据 

每行数据是一个字符串，长度不超过1000 
数据以EOF结束- 输出
对于输入的每一行，输出替换后的字符串- 样例输入
you are what you do- 样例输出
we are what we do- 来源
[水题比赛](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E6%B0%B4%E9%A2%98%E6%AF%94%E8%B5%9B)- 上传者
[hzyqazasdf](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=hzyqazasdf)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000000;


int main()
{
    string sym;
    while(getline(cin,sym)){
        string pattern = "you";
        string add = "we";
        while(1){
            if(sym.find(pattern) > 1005)
                break;
            int pos = sym.find(pattern);
            string s1 = sym.substr(0,pos);
            string s2 = sym.substr(pos+3,sym.length()-pos-3);
            sym = s1 + add + s2;
        }
        cout << sym << endl;
    }
    return 0;
}
```

很简单的一道题，遇到了stl中的一个坑，ios默认读到空格为止，所以要用getline











