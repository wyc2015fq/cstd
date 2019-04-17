# Registration system（NYOJ-991） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:05:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67
个人分类：[STL 的使用——容器与迭代器																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/8793969)








> 
# Problem Description

A new e-mail service "Berlandesk" is going to be opened in Berland in the near future. The site administration wants to launch their project as soon as possible, that's why they ask you to help. You're suggested to implement the prototype of site registration system. The system should work on the following principle.

Each time a new user wants to register, he sends to the system a request with his name. If such a name does not exist in the system database, it is inserted into the database, and the user gets the response OK, confirming the successful registration. If the name already exists in the system database, the system makes up a new user name, sends it to the user as a prompt and also inserts the prompt into the database. The new name is formed by the following rule. Numbers, starting with 1, are appended one after another to name (name1, name2, ...), among these numbers the least i is found so that namei does not yet exist in the database.

# **Input**

The first line contains number n (1 ≤ n ≤ 105). The following n lines contain the requests to the system. Each request is a non-empty line, and consists of not more than 32 characters, which are all lowercase Latin letters.

# Output

Print n lines, which are system responses to the requests: OK in case of successful registration, or a prompt with a new name, if the requested name is already taken.

# Sample Input

**4abacabaacabaabacabaacab**

# Sample Output

**OKOKabacaba1OK**


题意：n 个字符串，代表要依次输入系统中，若系统中之前没有这个字符串就输出 OK，若有就在已有的字符串后加数字，出现一次为 1，两次为 2，以此类推

思路：STL 中 map 的简单应用

# Source Program

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;

int main() {
    int n;
    cin>>n;

    map<string,int> mp;
    while(n--){

        string str;
        cin>>str;

        if(mp[str]==0)
            cout<<"OK"<<endl;
        else
            cout<<str<<mp[str]<<endl;
        mp[str]++;
    }
    return 0;
}
```






