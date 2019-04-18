# UVA 1663 Purifying Machine （二分图匹配，最大流） - xcw0754 - 博客园
# [UVA 1663 Purifying Machine （二分图匹配，最大流）](https://www.cnblogs.com/xcw0754/p/4667582.html)
题意：
　　给m个长度为n的模板串，模板串由0和1和*三种组成，且每串至多1个*，代表可0可1。模板串至多匹配2个串，即*号改成0和1，如果没有*号则只能匹配自己。问：模板串可以缩减为几个，同样可以匹配原来m个串同样能匹配的所有串。
思路：
　　差点想不出是二分图匹配了。
　　将原来m个串所能匹配的串给取出来放到集合中（记得去重），编上号。并为他们黑白着色，源点到白色点有容量1的边，黑色点到汇点有容量为1的边，对于该白色点所能匹配的所有黑色点，都有一条容量为1的边。跑一次最大流，得知匹配对数，这些匹配的都只用1个模板串，不匹配的独用一个模板串。
```
//#include <bits/stdc++.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <deque>
#include <set>
#include <algorithm>
#define LL long long
#define pii pair<int,int>
#define INF 0x7f7f7f7f
using namespace std;
const int N=1010;
int n, m, up;
char s[N][13];
int col[N*2], path[N*2], flow[N*2], edge_cnt;
vector<int> vect[N*2];
vector<int> vec[N*2];
struct node
{
    int from, to, cap, flow;
    node(){};
    node(int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){};
}edge[2100000];
void add_node(int from,int to,int cap,int flow)
{
    edge[edge_cnt]=node(from,to,cap,flow);
    vect[from].push_back(edge_cnt++);
}
bool ismatch(string &a,string &b)   //只有一个位不同即为匹配
{
    int cnt=0;
    for(int i=0; i<n; i++) 
        if( a[i]!=b[i] )    cnt++;
    if(cnt==1)  return true;
    else    return false;
}
void color(int s,int c)
{
    col[s]=c;
    for(int i=0; i<vec[s].size(); i++)
    {
        int t=vec[s][i];
        if(!col[t]) color(t,3-col[s]);
    }
}
set<string> sett;
string str[N*2];
int build_graph()
{
    //把包含*号的拆成两个数字,再转int,可能有重复。
    sett.clear();
    for(int i=0; i<m; i++)
    {
        int j=0;
        for(; j<n; j++)
        {
            if(s[i][j]=='*')
            {
                s[i][j]='0';
                sett.insert( string(s[i]) );    //重复的自动去掉
                s[i][j]='1';
                sett.insert( string(s[i]) );
                break;
            }
        }
        if(j==n)  sett.insert( string(s[i]) );
    }
    up=1;
    for(set<string>::iterator it=sett.begin(); it!=sett.end(); it++)
        str[up++]=*it;
    for(int i=0; i<=up+5; i++) vect[i].clear(),vec[i].clear();//第二个vec是为了着色用的
    for(int i=1; i<up; i++)        //匹配建无向图,着色用
    {
        for(int j=i+1; j<up; j++)
            if(ismatch(str[i],str[j]))
            {
                vec[i].push_back(j);
                vec[j].push_back(i);
            }
    }
    //黑白着色
    memset(col,0,sizeof(col));
    for(int i=1; i<up; i++)    if(!col[i]) color(i,1);
    //添加源点and汇点,重新建图
    memset(edge,0,sizeof(edge));
    edge_cnt=0;
    for(int i=1; i<up; i++)
    {
        if(col[i]==1)   //0是源点
        {
            add_node(0,i,1,0);
            add_node(i,0,0,0);
            for(int j=0; j<vec[i].size(); j++)  //相邻的点颜色不同
            {
                int q=vec[i][j];
                add_node(i,q,1,0);
                add_node(q,i,0,0);
            }
        }
        if(col[i]==2)   //up是汇点
        {
            add_node(i,up,1,0);
            add_node(up,i,0,0);
        }
    }
}
int BFS(int s,int e)
{
    deque<int> que(1,s);
    flow[s]=INF;
    while(!que.empty())
    {
        int x=que.front(); que.pop_front();
        for(int i=0; i<vect[x].size(); i++)
        {
            node e=edge[vect[x][i]];
            if(e.cap>e.flow && !flow[e.to])
            {
                path[e.to]=vect[x][i];
                flow[e.to]=min(flow[e.from], e.cap-e.flow);
                que.push_back(e.to);
            }
        }
        if(flow[e]) break;
    }
    return flow[e];
}
int max_flow(int s,int e)
{
    int ans_flow=0;
    while(true)
    {
        memset(flow,0,sizeof(flow));
        memset(path,0,sizeof(path));
        int tmp=BFS(s,e);
        if(!tmp)    return ans_flow;
        ans_flow+=tmp;
        int ed=e;
        while(ed!=s)
        {
            int t=path[ed];
            edge[t].flow+=tmp;
            edge[t^1].flow-=tmp;
            ed=edge[t].from;
        }
    }
}
int main()
{
    //freopen("input.txt", "r", stdin);
    char c;
    while(scanf("%d%d",&n,&m), n+m)
    {
        for(int i=0; i<m; i++)//输入
        {
            for(int j=0; j<n; )
            {
                c=getchar();
                if(c=='*' || c=='0' || c=='1')    s[i][j++]=c;
            }
            s[i][n]='\0';
        }
        build_graph();//建图
        printf("%d\n",up-1-max_flow(0,up));
    }
    return 0;
}
AC代码
```

