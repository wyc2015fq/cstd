# 【map+审题】#19 A. World Football Cup - CD's Coding - CSDN博客





2014年05月28日 20:12:37[糖果天王](https://me.csdn.net/okcd00)阅读数：1076标签：[map																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=map&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Everyone knows that 2010 FIFA World Cup is being held in South Africa now. By the decision of BFA (Berland's Football Association) next World Cup will be held in Berland. BFA took the decision to change some World Cup regulations:
- 
the final tournament features *n* teams (*n* is always
 even)
- 
the first *n* / 2 teams (according to the standings) come through to the knockout stage
- 
the standings are made on the following principle: for a victory a team gets 3 points, for a draw — 1 point, for a defeat — 0 points. In the first place, teams are ordered in the standings in decreasing order of their points; in the second place — in decreasing
 order of the difference between scored and missed goals; in the third place — in the decreasing order of scored goals
- 
it's written in Berland's Constitution that the previous regulation helps to order the teams without ambiguity.


You are asked to write a program that, by the given list of the competing teams and the results of all the matches, will find the list of teams that managed to get through to the knockout stage.




Input


The first input line contains the only integer *n* (1 ≤ *n* ≤ 50)
 — amount of the teams, taking part in the final tournament of World Cup. The following *n* lines contain the names of these teams, a name is a string of lower-case
 and upper-case Latin letters, its length doesn't exceed 30 characters. The following *n*·(*n* - 1) / 2 lines describe the held matches in the format name1-name2
 num1:num2, where*name*1, *name*2 — names of the
 teams; *num*1, *num*2 (0 ≤ *num*1, *num*2 ≤ 100)
 — amount of the goals, scored by the corresponding teams. Accuracy of the descriptions is guaranteed: there are no two team names coinciding accurate to the letters' case; there is no match, where a team plays with itself; each match is met in the descriptions
 only once.




Output


Output *n* / 2 lines — names of the teams, which managed to get through to the knockout stage in lexicographical order. Output each name in a separate line.
 No odd characters (including spaces) are allowed. It's guaranteed that the described regulations help to order the teams without ambiguity.




Sample test(s)




input
4
A
B
C
D
A-B 1:1
A-C 2:2
A-D 1:0
B-C 1:0
B-D 0:3
C-D 0:3




output
A
D




input
2
a
A
a-A 2:1




output
a













———————————————————————————————————————————————

《Updated 2015/07/25》

愚蠢的我哟，这是来自一年后的我对一年前的我伸出援手：

map<typeA,typeB>中也是可以使用结构体的~

为了既可以使用map的插入优势，又想要使用数组排序，那就先用map建造一个 map<string,team>（其中string为名字，team为包括名字的结构体，包括名字是为了最终提取出来的时候依然包含名字）；在map插入完毕之后，使用迭代器一个一个提取出来放入数组，对数组进行cmp排序即可。

这题的难(keng)点依然在于要好好审题，具体点就是：

先按照能力排序（看得分，相同时看净胜球，相同时看进球数）获得前一半的队伍

将这些队伍按照字典升序输出，AC代码如下：

C++ Source Code Below:



```cpp
#include <map>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

struct team
{
	int w,l;
	int score;
	string name;
	team(){w=0,l=0,score=0;}
} t[64];

map<string,team> mst;

bool cmp(const team &a, const team &b)
{
	if(a.score==b.score)
		if( (a.w-a.l)==(b.w-b.l) )
			return a.w>b.w;			
		else return (a.w-a.l)>(b.w-b.l);	
	else return a.score>b.score;
}

bool scmp(const team &a, const team &b)
{
	return a.name<b.name;
}

int main()
{
	int n=0;
	scanf("%d",&n);
	mst.clear();
	for(int i=0;i<n;i++)
	{
		cin>>t[i].name;
		mst[t[i].name].name=t[i].name;	
	} 
	int a=0,b=0;
	string battle,t1,t2;
	for(int i=0;i<(n-1)*n/2;i++)
	{	
		cin>>battle; 
		scanf("%d:%d",&a,&b);
		int div = battle.find('-');	//pos of '-'
		t1=battle.substr(0,div);
		t2=battle.substr(div+1,battle.length()-div-1);
		mst[t1].w+=a; mst[t2].w+=b;
		mst[t1].l+=b; mst[t2].l+=a;
		if(a==b)
		{
			mst[t1].score++;
			mst[t2].score++;
		}
		else if(a>b) mst[t1].score+=3;
		else mst[t2].score+=3;
	}
	int tpos=0;
	for(map<string,team>::iterator mit=mst.begin();mit!=mst.end();++mit)
	{
		t[tpos++] = mit->second;
		//cout<<t[tpos-1].name<<endl;
	}
	sort(t,t+n,cmp);
	sort(t,t+n/2,scmp);
	for(int i=0;i<n/2;i++) cout<<t[i].name<<endl;
	return 0;
}
```



———————————————————————————————————————————————




这次试了试map，map呢是一种Key&Value的形式，其特点是会随时按照Key排序（顺序），这一点对维护来说非常棒，但是Key是唯一固定的，不可以修改，只能删除。

这道题如果不需要最终将晋级的队伍按字典序输出的话，如下代码就是正确的：（作为初级map模板）



```cpp
#include<map> //Unique Key,Default Sort by key
#include<cstdio>
#include<string>
#include<cstdlib> 
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

//defination
typedef pair<string,int> si;
typedef map<string,int> t_map;
typedef map<int,string> a_map;


int cmp(const si& x,const si& y){return x.second>y.second;}

int main()
{
	int n=0;
	scanf("%d",&n);
	t_map team;
	a_map team_ans;
	//memset(team,0,sizeof(team));
	team.clear();
	team_ans.clear();
	for(int i=0;i<n;i++) //Read all team names by map
	{
		string name;
		cin>>name;
		//cout<<name;
		team.insert(pair<string,int>(name,0));
		//OR  insert(make_pair<string,int>(name,0));
		//OR  team[name]= 0;
		//Hint: Key can not be changed when Value can
	}
	for(int j=1;j<=(n-1)*n/2;j++) //get their goals and points
	{
		t_map::iterator it1,it2;
		char na1[31],na2[31],huiche;
		int num1,num2,flag=0,k=0;
		int cnt=0;
		//scanf("%s %d:%d",names,&num1,&num2);
		scanf("%c",&huiche);
		while(1)	//1st team
		{
			cnt=0;
			char tmp;
			scanf("%c",&tmp);
			if(tmp=='-')break;
			na1[cnt++]=tmp;
		}
		na1[cnt+1]='\0';
		while(1)	//2nd team
		{
			cnt=0;
			char tmp;
			scanf("%c",&tmp);
			if(tmp==' ')break;
			na2[cnt++]=tmp;
		}
		na2[cnt+1]='\0';
		string name1(na1);
		string name2(na2);
		//cout<<"1:"<<name1<<endl<<"2:"<<name2<<endl; 
		int plus1=0,plus2=0;
		scanf("%d:%d",&num1,&num2);
		if(num1>num2)plus1=3,plus2=0;
		else if(num1<num2)plus1=0,plus2=3;
		else plus1=plus2=1;
		team[name1] -= plus1;//According to default sort  ,but we need reverse,so
		team[name2] -= plus2;//According to default sort  ,but we need reverse,so
		//cout<<"1:"<<team[name1]<<endl<<"2:"<<team[name2]<<endl; 
	}
	for(t_map::iterator iter=team.begin();iter!=team.end();++iter)
	{
		team_ans.insert(make_pair<int,string>(iter->second,iter->first));
	}
	int cnt=0;
	for(a_map::iterator iter2=team_ans.begin();iter2!=team_ans.end();++iter2)
	{
		if(cnt==0)cout<<iter2->second;
		else cout<<endl<<iter2->second;
		cnt++;
		if(cnt==n/2)break;
	}
	return 0;
}
```


但这个是需要字典序的真讨厌……于是我作死的又转了一遍回来：





```cpp
#include<map> //Unique Key,Default Sort by key
#include<cstdio>
#include<string>
#include<cstdlib> 
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

//defination
typedef pair<string,int> si;
typedef map<string,int> t_map;
typedef map<int,string> a_map;

char ans[26][32]; 
int cmp(char* &a,char* &b){return strcmp(a,b);}
int cmp(const si& x,const si& y){return x.second>y.second;}

int main()
{
	int n=0;
	scanf("%d",&n);
	t_map team,team_fin;
	a_map team_ans;
	team.clear();
	team_ans.clear();
	team_fin.clear();
	for(int i=0;i<n;i++) //Read all team names by map
	{
		string name;
		cin>>name;
		//cout<<name;
		team.insert(pair<string,int>(name,0));
		//OR  insert(make_pair<string,int>(name,0));
		//OR  team[name]= 0;
		//Hint: Key can not be changed when Value can
	}
	for(int j=1;j<=(n-1)*n/2;j++) //get their goals and points
	{
		t_map::iterator it1,it2;
		char na1[31],na2[31],huiche;
		int num1,num2,flag=0,k=0;
		int cnt=0;
		//scanf("%s %d:%d",names,&num1,&num2);
		scanf("%c",&huiche);
		cnt=0;
		while(1)	//1st team
		{
			char tmp;
			scanf("%c",&tmp);
			if(tmp=='-')break;
			na1[cnt++]=tmp;
		}
		na1[cnt]='\0';
		cnt=0;// WA- This must be here ,not in the while(1)
		while(1)	//2nd team
		{
			char tmp;
			scanf("%c",&tmp);
			if(tmp==' ')break;
			na2[cnt++]=tmp;
		}
		na2[cnt]='\0';
		string name1(na1);
		string name2(na2);
		//cout<<"1:"<<name1<<endl<<"2:"<<name2<<endl; 
		int plus1=0,plus2=0;
		scanf("%d:%d",&num1,&num2);
		if(num1>num2)plus1=3,plus2=0;
		else if(num1<num2)plus1=0,plus2=3;
		else plus1=plus2=1;
		team[name1] -= plus1;//According to default sort  ,but we need reverse,so
		team[name2] -= plus2;//According to default sort  ,but we need reverse,so
		//cout<<"1:"<<team[name1]<<endl<<"2:"<<team[name2]<<endl; 
	}
	for(t_map::iterator iter=team.begin();iter!=team.end();++iter)//Change Key and Value in another map
	{
		team_ans.insert(make_pair<int,string>(iter->second,iter->first));
	}
	int cnt=0;
	for(a_map::iterator iter2=team_ans.begin();iter2!=team_ans.end();++iter2)//Change back to get a half size answer map
	{
		//if(cnt==0)cout<<iter2->second;
		//else cout<<endl<<iter2->second;
		team_fin.insert(make_pair<string,int>(iter2->second,iter2->first));
		cnt++;
		if(cnt==n/2)break;
	}
	for(t_map::iterator iter3=team_fin.begin();iter3!=team_fin.end();++iter3)
	{
		cout<<iter3->first<<endl;
	}
	
	return 0;
}
```


居然……WA at 5了……



这道题网上只有一个解题报告，而且跟我的好不一样，虽然觉得烦但是也没有退路了……加油debug吧Q^Q

虽说……怎么看怎么是对的哇…… 到底哪里出问题了哇……



看了看题意：我！了！个！去！

如果得分相同就看 (进球数 - 失球数) 的值, 如果还是相同就看进球数 (都是大的排在前面)


得分》净胜球》进球数

输出时：字典序。

长题目一定要好好审题Q^Q

解题报告 From:[yunyouxi0](http://my.csdn.net/Yunyouxi)@CSDN



```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

char str[100][100];
char s[100];
char s1[100], s2[100];
struct P {
  string name;
  int s;
  int w, l;
}p[110];

bool cmp(const P & a, const P & b ) {
  if(a.s == b.s) {
    if(a.w - a.l == b.w - b.l) {
      return a.w > b.w;
    }
    else return a.w - a.l > b.w - b.l;
  }
  return a.s > b.s;
}

bool cmp2(const P & a, const P & b) {
  return a.name < b.name;
}

int main() {
  int n, a, b;
  while(scanf("%d", &n) != EOF) {
    map<string,int>mm;
    map<string,int>w;
    map<string,int>l;
    set<string>name;
    set<string>::iterator ps;
    for( int i=0; i<n; i++ ) {
      scanf("%s", str[i]);
    }
    for( int i=0; i<n*(n-1)/2; i++ ) {
      scanf("%s", s);
      int len = strlen(s), k1 = 0, k2 = 0;
      bool f = 0;
      for( int i=0; i<len; i++ ) {
        if(s[i] == '-') {
          f = 1;
          continue;
        }
        if(f) s2[k2++] = s[i];
        else s1[k1++] = s[i];
      }
      s1[k1] = '\0', s2[k2] = '\0';
      scanf("%d:%d", &a, &b);
      string ts1 = s1;
      string ts2 = s2;
      if(name.find(ts1) == name.end()) name.insert(ts1);
      if(name.find(ts2) == name.end()) name.insert(ts2);
      w[ts1] += a; l[ts1] += b;
      w[ts2] += b; l[ts2] += a;
      if(a > b) mm[ts1] += 3;
      else if(a == b) mm[ts1]++, mm[ts2]++;
      else mm[ts2] += 3;
    }
    int k = 0;
    for( ps=name.begin(); ps != name.end(); ps++ ) {
      p[k].name = *ps;
      p[k].s = mm[*ps];
      p[k].w = w[*ps];
      p[k].l = l[*ps];
      k++;
    }
    sort(p, p + k, cmp);
    sort(p, p + n/2, cmp2);
    for( int i=0; i<n/2; i++ ) cout<<p[i].name<<endl;
  }
  return 0;
}
```








话说刚看到个解题报告： From：UsingTC @sina

解法:直接模拟

吾辈发现……这是个CE的代码呀……而且……感觉……有点……乱，稍微改了改（测试这里的bug都交了5发WA）：

算了不贴了，弃掉。




Python强人真多：粘贴备看



```python
import os
import sys

# solve
def solve():
    n = int(raw_input())
    dic = {}
    for i in range(n):
        str = raw_input()
        dic[str] = [0,0,0]
    
    i = n*(n-1)/2
    while i:
        list = raw_input().split()
        tmp1 = list[0].split("-")
        tmp2 = list[1].split(":")
        i -= 1

        if(int(tmp2[0]) > int(tmp2[1])):
            dic[tmp1[0]][0] += 3
            dic[tmp1[0]][1] += int(tmp2[1])
            dic[tmp1[0]][2] += int(tmp2[0])
            
            dic[tmp1[1]][1] += int(tmp2[0])
            dic[tmp1[1]][2] += int(tmp2[1])
        elif(int(tmp2[0]) == int(tmp2[1])):
            dic[tmp1[0]][0] += 1
            dic[tmp1[0]][1] += int(tmp2[1])
            dic[tmp1[0]][2] += int(tmp2[0])

            dic[tmp1[1]][0] += 1
            dic[tmp1[1]][1] += int(tmp2[0])
            dic[tmp1[1]][2] += int(tmp2[1])
        else:
            dic[tmp1[0]][1] += int(tmp2[1])
            dic[tmp1[0]][2] += int(tmp2[0])

            dic[tmp1[1]][0] += 3
            dic[tmp1[1]][1] += int(tmp2[0])
            dic[tmp1[1]][2] += int(tmp2[1])

    # out
    ans = sorted(dic.items() , key = lambda value:(value[1][0],value[1][2]-value[1][1],value[1][2]) , reverse = True)
    out = []
    for i in range(n/2):
        out.append(ans[i][0])
    out.sort()
    for s in out:
        print s

# main
if __name__ == "__main__":
    solve()
```











