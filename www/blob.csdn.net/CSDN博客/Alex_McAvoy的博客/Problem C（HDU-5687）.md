# Problem C（HDU-5687） - Alex_McAvoy的博客 - CSDN博客





2019年02月20日 16:01:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：22
个人分类：[HDU																[字符串处理——字典树(Tire树)](https://blog.csdn.net/u011815404/article/category/8681920)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

度熊手上有一本神奇的字典，你可以在它里面做如下三个操作： 

  1、insert : 往神奇字典中插入一个单词 

  2、delete: 在神奇字典中删除所有前缀等于给定字符串的单词 

  3、search: 查询是否在神奇字典中有一个字符串的前缀等于给定的字符串 

# Input

这里仅有一组测试数据。第一行输入一个正整数N(1≤N≤100000)，代表度熊对于字典的操作次数，接下来N行，每行包含两个字符串，中间中用空格隔开。第一个字符串代表了相关的操作（包括： insert, delete 或者 search）。第二个字符串代表了相关操作后指定的那个字符串，第二个字符串的长度不会超过30。第二个字符串仅由小写字母组成。

# Output

对于每一个search 操作，如果在度熊的字典中存在给定的字符串为前缀的单词，则输出Yes 否则输出 No。

# **Sample Input**

**5insert helloinsert hehesearch hdelete hesearch hello**

# Sample Output

**YesNo**


思路：带有删除操作的字典树

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
#define N 500001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int tot = 1;
struct Node{
    int sum;//前缀
    int next[26];//子节点
    void init(){
        sum=0;
        memset(next,-1,sizeof next);
    }
}tire[N];
void insert(char *str){
    int len=strlen(str);
    int root=0;
    for(int i=0;i<len;i++){
        int x=str[i]-'a';
        if(tire[root].next[x]==-1)
            tire[root].next[x]=tot++;
        root=tire[root].next[x];
        tire[root].sum++;
    }
}
int search(char *str){
    int len=strlen(str);
    int root=0;
    for(int i=0;i<len;i++){
        int x=str[i]-'a';
        if(tire[root].next[x]==-1)
            return 0;
        root=tire[root].next[x];
    }
    return tire[root].sum;
}
void del(char *str,int word){
    int len=strlen(str);
    int root=0;
    if(word<0)
        return;
    for(int i=0;i<len;i++){
        int x=str[i]-'a';
        if(tire[root].next[x]==-1)
            return;
        tire[root].sum-=word;
        root=tire[root].next[x];
    }
    tire[root].sum=0;

    for(int i=0;i<26;i++)
        tire[root].next[i]=-1;
}

int main(){
    for(int i=0;i<N;i++)
        tire[i].init();

    int t;
    scanf("%d",&t);
    while(t--){
        char str[10],word[35];
        scanf("%s%s",str,word);
        if(str[0]=='i')//插入
            insert(word);
        else if(str[0]=='d')//删除
            del(word,search(word));
        else{//查询
            if(search(word)>0)
                printf("Yes\n");
            else
                printf("No\n");
        }
    }
    return 0;
}
```






