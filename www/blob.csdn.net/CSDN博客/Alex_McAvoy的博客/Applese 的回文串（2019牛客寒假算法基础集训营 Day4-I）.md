# Applese 的回文串（2019牛客寒假算法基础集训营 Day4-I） - Alex_McAvoy的博客 - CSDN博客





2019年02月03日 12:19:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：87








> 
# 【题目描述】

自从 Applese 学会了字符串之后，精通各种字符串算法，比如……判断一个字符串是不是回文串。

这样的题目未免让它觉得太无聊，于是它想到了一个新的问题。

如何判断一个字符串在任意位置(包括最前面和最后面)插入一个字符后能不能构成一个回文串？

# 【输入描述】

仅一行，为一个由字母和数字组成的字符串 s。

|s|≤10^5

# 【输出描述】

如果在插入一个字符之后可以构成回文串，则输出"Yes", 否则输出"No"。

# 【样例】

示例1

输入

applese

输出

No

示例2

输入

java

输出

Yes


思路：

首先，如果给出的串本身就是一个回文串，那么答案是 Yes

然后找到串中不匹配的位置，提取出中间不匹配的字符串，由于添加字符与删除字符本身是等价的，因此分别判断不匹配的字符串去掉头和去掉尾的两个字符串是否为回文，只要其中有一个为回文，那么就是 Yes

例如：对于 abcddecba，先取出 dde，然后判断 dd 和 de，只要其中有一个满足回文，即返回 Yes，否则返回 No

# 【源代码】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
bool judge(string str){//判断回文
    for(int i=0;i<str.size()/2;i++)
        if(str[i]!=str[str.size()-1-i])
            return false;
    return true;
}
int main(){
    string str;
    cin>>str;

    int pos;
    for(int i=0;i<str.size()/2;i++){
        if(str[i]!=str[str.size()-1-i]){
            pos=i;//记录不相等位置
            break;
        }
    }

    bool flag=false;
    if(pos==str.size()/2)
        flag=true;
    else
        flag=(judge(str.substr(pos+1,str.size()-2*pos-1))||judge(str.substr(pos,str.size()-2*pos-1)));
    

    if(flag)
        cout<<"Yes"<<endl;
    else
        cout<<"No"<<endl;

    return 0;

}
```





