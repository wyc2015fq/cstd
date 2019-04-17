# Searching the String（ZOJ-3228） - Alex_McAvoy的博客 - CSDN博客





2019年03月09日 12:21:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：93
个人分类：[字符串处理——AC自动机																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/8727853)








> 
# Problem Description

Little jay really hates to deal with string. But moondy likes it very much, and she's so mischievous that she often gives jay some dull problems related to string. And one day, moondy gave jay another problem, poor jay finally broke out and cried, " Who can help me? I'll bg him! "

So what is the problem this time?

First, moondy gave jay a very long string A. Then she gave him a sequence of very short substrings, and asked him to find how many times each substring appeared in string A. What's more, she would denote whether or not founded appearances of this substring are allowed to overlap.

At first, jay just read string A from begin to end to search all appearances of each given substring. But he soon felt exhausted and couldn't go on any more, so he gave up and broke out this time.

I know you're a good guy and will help with jay even without bg, won't you?

# **Input**

Input consists of multiple cases( <= 20 ) and terminates with end of file.

For each case, the first line contains string A ( length <= 10^5 ). The second line contains an integer N ( N <= 10^5 ), which denotes the number of queries. The next N lines, each with an integer type and a string a ( length <= 6 ), type = 0 denotes substring a is allowed to overlap and type = 1 denotes not. Note that all input characters are lowercase.

There is a blank line between two consecutive cases.

# Output

For each case, output the case number first ( based on 1 , see Samples ).

Then for each query, output an integer in a single line denoting the maximum times you can find the substring under certain rules.

Output an empty line after each case.

# Sample Input

**ab20 ab1 ab**

**abababac20 aba1 aba**

**abcdefghijklmnopqrstuvwxyz30 abc1 def1 jmn**

# Sample Output

**Case 111**

**Case 232**

**Case 3110**


题意：多组数据，每组给出一个文本和 n 个模式，0 代表能重叠，1 代表不能重叠，求每个模版串在文本串中出现的次数

思路：AC 自动机

对于每个模版，进行一次 AC 自动机，然后由于模版串对文本串分为可重叠和不可重叠的，因此进行分类讨论：对于可重叠的，直接进行统计；对于不可重叠的，记录最后一次匹配的位置然后进行判断

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
const int N=500000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct AC_Automata{
    int tire[N][26];//字典树
    int val[N];//字符串结尾标记
    int fail[N];//失配指针
    int last[N];//last[i]=j表j节点表示的单词是i节点单词的后缀，且j节点是单词节点
    int tot;//编号
    int time[N];//上次的单词出现在文本串的位置
    int len[N];//单词节点的长度
    int cnt[N][2];//计数

    void init(){//初始化0号点
        tot=1;
        val[0]=0;
        last[0]=0;
        fail[0]=0;
        memset(tire[0],0,sizeof(tire[0]));
    }

    void insert(char *s){//构造trie
        int sLen=strlen(s);
        int root=0;
        for(int i=0;i<sLen;i++){
            int id=s[i]-'a';
            if(tire[root][id]==0){
                tire[root][id]=tot;
                memset(tire[tot],0,sizeof(tire[tot]));
                val[tot++]=0;
            }
            root=tire[root][id];
        }
        val[root]=1;
        time[root]=0;
        len[root]=sLen;
        cnt[root][0]=0;
        cnt[root][1]=0;
    }

    void build(){//构造fail与last
        queue<int> q;
        for(int i=0;i<26;i++){
            int root=tire[0][i];
            if(root!=0){
                fail[root]=0;
                last[root]=0;
                q.push(root);
            }
        }

        while(!q.empty()){//bfs求fail
            int k=q.front();
            q.pop();
            for(int i=0;i<26; i++){
                int u=tire[k][i];
                if(u==0)
                    continue;
                q.push(u);

                int v=fail[k];
                while(v && tire[v][i]==0)
                    v=fail[v];
                fail[u]=tire[v][i];
                last[u]=val[fail[u]]?fail[u]:last[fail[u]];
            }
        }
    }

    void query(char *s){//匹配
        int len=strlen(s);
        int j=0;
        for(int i=0;i<len;i++){
            int id=s[i]-'a';
            while(j && tire[j][id]==0)
                j=fail[j];
            j=tire[j][id];
            if(val[j])
                print(j,i+1);
            else if(last[j])
                print(last[j],i+1);
        }
    }

    void print(int i,int pos){
        if(val[i]){
            cnt[i][0]++;
            if(time[i]+len[i]<=pos){//判断是否有重叠
                time[i]=pos;
                cnt[i][1]++;
            }
            print(last[i],pos);
        }
    }

    int queryT(char *s,int op){//匹配单个文本
        int len=strlen(s);
        int root=0;
        for(int i=0;i<len;i++){
            int id=s[i]-'a';
            root=tire[root][id];
        }
        return cnt[root][op];
    }
}ac;
char P[N][10];
char T[N];
int op[N];
int main(){
    int Case=1;
    while(scanf("%s",T)!=EOF){
        ac.init();

        int n;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%d%s",&op[i],P[i]);
            ac.insert(P[i]);
        }
        ac.build();
        ac.query(T);
        printf("Case %d\n",Case++);
        for(int i=0;i<n;i++)
            printf("%d\n",ac.queryT(P[i],op[i]));
        printf("\n");
    }
    return 0;
}
```






