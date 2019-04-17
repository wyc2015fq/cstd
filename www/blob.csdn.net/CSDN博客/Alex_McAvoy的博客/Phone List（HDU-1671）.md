# Phone List（HDU-1671） - Alex_McAvoy的博客 - CSDN博客





2019年02月20日 18:40:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30
个人分类：[HDU																[字符串处理——字典树(Tire树)](https://blog.csdn.net/u011815404/article/category/8681920)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Given a list of phone numbers, determine if it is consistent in the sense that no number is the prefix of another. Let’s say the phone catalogue listed these numbers:

1. Emergency 911

2. Alice 97 625 999

3. Bob 91 12 54 26

In this case, it’s not possible to call Bob, because the central would direct your call to the emergency line as soon as you had dialled the first three digits of Bob’s phone number. So this list would not be consistent.

# Input

The first line of input gives a single integer, 1 <= t <= 40, the number of test cases. Each test case starts with n, the number of phone numbers, on a separate line, 1 <= n <= 10000. Then follows n lines with one unique phone number on each line. A phone number is a sequence of at most ten digits.

# Output

For each test case, output “YES” if the list is consistent, or “NO” otherwise.

# **Sample Input**

**2391197625999911254265113123401234401234598346**

# Sample Output

**NOYES**


题意：每组数据给出 n 个字符串，若某个字符串是令一个的前缀，输出 NO，若均不存在，输出 YES

思路：字典树

每次读入一个字符串后，在每个字符串的最后打上一个标记，每次读入一个新的字符串后，先判断数中是否有这个字符串的前缀即可

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
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 50001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    struct Node *next[10];
    int flag;
    Node(){
        for(int i=0;i<10;i++)
            next[i]=NULL;
        flag=0;
    }
};
Node *tire;//结构体指针
bool flag;
void insert(char *s){
    Node *p = tire;
    int len=strlen(s);
    for(int i=0;i<len;i++){
        int x=s[i]-'0';

        if(p->next[x]==NULL)//建立新的节点
            p->next[x]=new Node;

        p=p->next[x];
        if(p->flag!=0){//之前存的字符串是当前字符串的前缀
            flag=false;
            break;
        }
    }
    p->flag++;//在字符串的结尾标记
    for(int i=0;i<10;i++){
        if(p->next[i]!=NULL){
            flag=false;
            return ;
        }
    }
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);

        tire=new Node;//结构体指针执行新构造的地址
        flag=true;
        while(n--){
            char str[20];
            scanf("%s",str);
            insert(str);
        }
        if(flag)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
```






