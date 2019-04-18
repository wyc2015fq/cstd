# 1080 MOOC期终成绩 ——C++实现 - 心纯净，行致远 - CSDN博客





2018年11月19日 11:18:49[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：42








# 题目

> 
1080 MOOC期终成绩 （25 分）

对于在中国大学MOOC（[http://www.icourse163.org/](http://www.icourse163.org/) ）学习“数据结构”课程的学生，想要获得一张合格证书，必须首先获得不少于200分的在线编程作业分，然后总评获得不少于60分（满分100）。总评成绩的计算公式为 G=(G​mid−term​​×40%+G​final​​×60%)，如果 G​mid−term​​>G​final​​；否则总评 G 就是 G​final​​。这里 G​mid−term​​ 和 G​final​​ 分别为学生的期中和期末成绩。

现在的问题是，每次考试都产生一张独立的成绩单。本题就请你编写程序，把不同的成绩单合为一张。

### 输入格式：

输入在第一行给出3个整数，分别是 P（做了在线编程作业的学生数）、M（参加了期中考试的学生数）、N（参加了期末考试的学生数）。每个数都不超过10000。

接下来有三块输入。第一块包含 P 个在线编程成绩 G​p​​；第二块包含 M 个期中考试成绩 G​mid−term​​；第三块包含 N 个期末考试成绩 G​final​​。每个成绩占一行，格式为：`学生学号 分数`。其中`学生学号`为不超过20个字符的英文字母和数字；`分数`是非负整数（编程总分最高为900分，期中和期末的最高分为100分）。

### 输出格式：

打印出获得合格证书的学生名单。每个学生占一行，格式为：

`学生学号` G​p​​ G​mid−term​​ G​final​​ G

如果有的成绩不存在（例如某人没参加期中考试），则在相应的位置输出“−1”。输出顺序为按照总评分数（四舍五入精确到整数）递减。若有并列，则按学号递增。题目保证学号没有重复，且至少存在1个合格的学生。

### 输入样例：

```
6 6 7
01234 880
a1903 199
ydjh2 200
wehu8 300
dx86w 220
missing 400
ydhfu77 99
wehu8 55
ydjh2 98
dx86w 88
a1903 86
01234 39
ydhfu77 88
a1903 66
01234 58
wehu8 84
ydjh2 82
missing 99
dx86w 81
```

### 输出样例：

```
missing 400 -1 99 99
ydjh2 200 98 82 88
dx86w 220 88 81 84
wehu8 300 55 84 84
```


# 算法 

这道题我的思路是比较好的，首先先根据Gp筛选出符合的学生，显然要用map<string,int>处理，再把输入的Gm存到结构体中，然后根据输入的Gf算出最后的成绩，成绩合格就进行输出。而不是将所有的信息都存起来。

1、因为所有人必须要G编程>=200分，所以用v数组保存所有G编程>=200的人，（一开始gm和gf都为-1），用map映射保存名字所对应v中的下标（为了避免与“不存在”混淆，保存下标+1，当为0时表示该学生的姓名在v中不存在）

**这道题是典型的根据输出形式确定存储的数据结构。**由于输出中有-1，因此可以定义和输出形式相同的数据结构，用结构体再合适不过啦。

2、G期中中出现的名字，如果对应的map并不存在（==0），说明该学生编程成绩不满足条件，则无须保存该学生信息。将存在的人的期中考试成绩更新

3、G期末中出现的名字，也必须保证在map中存在。先更新G期末和G总为新的成绩，当G期末<G期中时再将G总更新为(G期中x 40% + G期末x 60%)

4、将v数组中所有G总满足条件的放入ans数组中，对ans排序（总分递减，总分相同则姓名递增），最后输出ans中的学生信息

心平静下来，好好分析，这道题的对于训练STL容器还是很不错的。即考察了字符串，又考察了map、vector，还有基础的结构体，很综合很全面。题目非常好！

补充：这道题中节点的初始化是在将gp成绩读入的时候，见代码1。很自然地想法是可以在定义结构体的时候就进行初始化，这样也是成立的，见代码2.

# 代码

代码1：读入gp成绩的时候初始化node

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

struct node{	//根据输出形式确定输出的数据结构 
	string id;
	int gp,gm,gf,g;
};

bool cmp(node a,node b){
	return a.g!=b.g?a.g>b.g:a.id<b.id;
}

int main(){
	int P,M,N,tmp,cnt=1;	cin>>P>>M>>N;
	vector<node> v,ans;	//vector是动态数组，无需声明大小 v存储合理的，ans存储排序的 
	map<string,int> idx;	//根据gp存储合理的学号 
	string str;
	for(int i=0;i<P;i++){	//gp
		cin>>str>>tmp;
		if(tmp>=200){
			v.push_back(node{str,tmp,-1,-1,0});
			idx[str]=cnt++;
		}		
	}
	for(int i=0;i<M;i++){	//gm
		cin>>str>>tmp;
		if(idx[str])	v[idx[str]-1].gm=tmp;
	}
	for(int i=0;i<N;i++){	//gf
		cin>>str>>tmp;
		if(idx[str]){
			int tem=idx[str]-1;
			v[tem].gf=v[tem].g=tmp;
			if(v[tem].gm>v[tem].gf)	v[tem].g=int(0.4*v[tem].gm+0.6*v[tem].gf+0.5);
		}
	}
	for(int i=0;i<v.size();i++)
		if(v[i].g>=60)	ans.push_back(v[i]);
	sort(ans.begin(),ans.end(),cmp);
	for(int i=0;i<ans.size();i++)	printf("%s %d %d %d %d\n",ans[i].id.c_str(),ans[i].gp,ans[i].gm,ans[i].gf,ans[i].g);
	return 0;
}
```

代码2：建立node就将需要初始化的成绩初始化

```cpp
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

struct node{
	string id;
	int gp=-1,gm=-1,gf=-1,g=0;
};
bool cmp(node a,node b){
	return a.g!=b.g?a.g>b.g:a.id<b.id;
}
int main(){
	int P,M,N,tmp,cnt=1;	cin>>P>>M>>N;
	string s;
	vector<node> v(P),ans;
	map<string,int> idx;
	for(int i=0;i<P;i++){	//gp
		cin>>s>>tmp;
		if(tmp>=200){
			idx[s]=cnt++;
			v[idx[s]-1].id=s;
			v[idx[s]-1].gp=tmp;
		}
	} 
	for(int i=0;i<M;i++){
		cin>>s>>tmp;
		if(idx[s])	v[idx[s]-1].gm=tmp;
	}
	for(int i=0;i<N;i++){
		cin>>s>>tmp;
		if(idx[s]){
			v[idx[s]-1].gf=v[idx[s]-1].g=tmp;
			if(v[idx[s]-1].gm>v[idx[s]-1].gf)	v[idx[s]-1].g=int(0.4*v[idx[s]-1].gm+0.6*v[idx[s]-1].gf+0.5)	;
		}
	}
	for(int i=0;i<v.size();i++)
		if(v[i].g>=60)	ans.push_back(v[i]);
	sort(ans.begin(),ans.end(),cmp);
	for(int i=0;i<ans.size();i++)	printf("%s %d %d %d %d\n",ans[i].id.c_str(),ans[i].gp,ans[i].gm,ans[i].gf,ans[i].g);
	return 0;
}
```





