# 2018 ACM-ICPC亚洲区域赛 北京赛区 - 紫芝的博客 - CSDN博客





2018年11月19日 19:09:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：80








# 2018 ACM-ICPC亚洲区域赛 北京赛区

## A - Jin Yong’s Wukong Ranking List

HihoCoder - 1870

## 题意

给N对关系，每对关系表示为A 比B 厉害，如果A比B厉害，B比C厉害，那么A比C厉害，即关系具有传递性。如果这N对关系存在矛盾，则输出矛盾的哪一个关系，否则输出0

## 分析

如果A比B厉害，B比A厉害就和之前的关系矛盾，构成一个环，使用DFS判断有向图是否有环即可

```cpp
#include <iostream>
#include <vector>
#include <string.h>
#include<map>
using namespace std;
/*
color代表每个结点的状态，-1代表还没被访问，0代表正在被访问，1代表访问结束
如果一个状态为“0”的结点，与他相连的结点状态也为0的话就代表有环,这个可以用dfs实现
*/
vector<int> vec[120];
int color[120];
bool flag;//标记是否有环

void dfs(int x){
    if(flag){//如果有环就返回，否者继续收索
        return;
    }
    color[x] = 0;//x正在被反问，状态为0
    for(int i = 0 ; i < vec[x].size() ; i++){
        if(color[vec[x][i]] == -1){
        //与x相连的结点状态也为-1，代表还未被访问，则继续搜索
            dfs(vec[x][i]);
        } else if(color[vec[x][i]] == 0){
        //与x相连的结点状态也为0，代表有环，返回
            flag = true;
            return;
        }
    }
    color[x] = 1;//对x的访问结束
}

int main(){

    int n ;
    while(cin>>n){
        int k=1;
    for(int i=0;i<120;i++)
        vec[i].clear();

    map<string,int> mp;
    string ans1,ans2;//记录答案
    bool ans=false;//标记是否找到环

    for(int i = 0 ; i < n ; i++){//建立邻接表
        string u,v;
        cin >> u >> v;
        if(!mp[u])  mp[u]=k++;
        if(!mp[v])  mp[v]=k++;
        //cout<<mp[u]<<" "<<mp[v]<<endl;
        vec[mp[u]].push_back(mp[v]);
        flag = false;
        if(!ans){//还没找到环
        memset(color, -1, sizeof(color));
            dfs(mp[u]);
        if(flag)//存在环
            ans1=u,ans2=v,ans=true;
        }
    }
    if(ans)
        cout<<ans1<<" "<<ans2<<endl;
    else
        cout<<0<<endl;
    }
    return 0;
}
```



