# Keywords Search（HDU-2222） - Alex_McAvoy的博客 - CSDN博客





2019年03月06日 17:32:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：17
个人分类：[HDU																[字符串处理——AC自动机](https://blog.csdn.net/u011815404/article/category/8727853)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In the modern time, Search engine came into the life of everybody like Google, Baidu, etc. 

Wiskey also wants to bring this feature to his image retrieval system. 

Every image have a long description, when users type some keywords to find the image, the system will match the keywords with description of image and show the image which the most keywords be matched. 

To simplify the problem, giving you a description of image, and some keywords, you should tell me how many keywords will be match. 

# **Input**

First line will contain one integer means how many cases will follow by. 

Each case will contain two integers N means the number of keywords and N keywords follow. (N <= 10000) 

Each keyword will only contains characters 'a'-'z', and the length will be not longer than 50. 

The last line is the description, and the length will be not longer than 1000000. 

# Output

Print how many keywords are contained in the description.

# Sample Input

**15shehesayshrheryasherhs**

# Sample Output

**3**


题意：给出 t 组数据，每组 n 个单词，再给出一个字符串，求这 n 个单词在字符串中出现的次数

思路：字符串多模版匹配，AC 自动机模版题

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
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int tot;//编号
int trie[N][26];//字典树
int val[N];//字符串结尾标记
int fail[N];//失配指针
void insert(char *s){//插入模式串
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;s[i];i++){
        int id=s[i]-'a';//子节点编号
        if(trie[root][id]==0)//若之前没有从root到id的前缀
            trie[root][id]=++tot;//插入
        root=trie[root][id];//顺着字典树往下走
    }
    val[root]++;
}
void build(){//构建fail指针域建立字典图
    queue<int>q;
    for(int i=0;i<26;i++)//将根节点的子节点入队
        if(trie[0][i])
            q.push(trie[0][i]);

    while(!q.empty()){
        int k=q.front();//对于队首节点k，其fail指针已求得，现在要求的是他子节点的fail指针
        q.pop();
        for(int i=0;i<26;i++){//遍历字符集
            if(trie[k][i]){//若字符i对应的子节点存在
                fail[trie[k][i]]=trie[fail[k]][i];//将这个子节点fail指针赋给fail[k]的字符i对应的节点
                q.push(trie[k][i]);
            }
            else
                trie[k][i]=trie[fail[k]][i];//将fail[k]的子节点直接赋成k的子节点
        }
    }
}
int query(char *t){//对文本串进行匹配
    int res=0;//存储结果
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;t[i];i++){//对文本串进行遍历
        int id=t[i]-'a';//子节点编号
        root=trie[root][id];//在字典图中不断穿梭跳动
        int j=root;
        while(j&&val[j]!=-1){//利用fail指针找出所有匹配的模式串
            res+=val[j];//累加到答案中
            val[j]=-1;
            j=fail[j];//fail指针跳转
        }
    }
    return res;
}
char P[N];
char T[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(trie,0,sizeof(trie));
        memset(val,0,sizeof(val));
        memset(fail,0,sizeof(fail));
        tot=0;

        int n;//模式串个数
        scanf("%d",&n);
        while(n--){
            scanf("%s",P);//输入模式串
            insert(P);//插入字典树中
        }
        build();//构建失配指针与字典图

        scanf("%s",T);//输入文本串
        int res=query(T);
        printf("%d\n",res);
    }
    return 0;
}
```






