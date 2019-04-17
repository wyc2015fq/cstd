# 【Map的使用】为什么成都的时间和重庆不一样！ @华为Heat-1 - CD's Coding - CSDN博客





2014年04月29日 19:27:30[糖果天王](https://me.csdn.net/okcd00)阅读数：589标签：[c++																[华为编程大赛](https://so.csdn.net/so/search/s.do?q=华为编程大赛&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)









|记票统计| |
|----|----|
|描述:|模拟n个人参加选举的过程，并输出选举结果：假设候选人有四人，分别用“A”、”B”、”C”、”D”表示，选举时开始计票, 若输入的不是“A”、”B”、”C”、”D”则视为无效票。选举结束后获取各个候选人的得票数以及无效票的个数，输出结果以添加候选人的顺序进行顺序输出,最后一行为无效的数量。同时getVoteResult命令为结束命令。|
|运行时间限制:|无限制|
|内存限制:|无限制|
|输入:|输入为多行形式输入，每一行为一条命令。输入的命令只会是有效命令不会有非法命令，但可能存在非法的投票，上面已经描述了。添加候选人的命令如下：addCandidate为命令 xx1为候选人addCandidate xx1投票的命令如下：vote为命令 xx1为候选人的字符串vote xx1统计投票的命令如下：getVoteResult为命令getVoteResult|
|输出:|输出结果以添加候选人的顺序进行顺序输出,最后一行为无效的数量。|
|样例输入:|addCandidate xx1addCandidate xx2addCandidate xx3addCandidate xx4addCandidate xx5addCandidate xx6vote xx1vote xx3vote xx4vote xx1vote xx2vote xx7vote xx4vote xx5vote xx3vote xx2vote xx1vote xx7getVoteResult|
|样例输出:|xx1 3xx2 2xx3 2xx4 2xx5 1xx6 02|
|||


这场是ZoeCUR的比赛Heat1,吾辈协力下完成的代码：



```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <cstdlib>
#include <string>
#include <map>
using namespace std;

int main()
{
	int invalid=0;
	map<string,int> cad;
	string temp;
	string name;
	string add="addCandidate";
	string vot="vote";
	string get="getVoteResult";
	while(cin>>temp)
	{
		if(temp==add)
		{
			cin>>name;
			cad.insert(pair<string,int>(name,0));
		}
		if(temp==vot)
		{
			cin>>name;
			map<string,int>::iterator it;
			it=cad.find(name);
			if(cad.count(name)==0)invalid++;
			else it->second ++;
		}
		if(temp==get)
		{
			map<string,int>::iterator it;
			for(it=cad.begin();it!=cad.end();++it)
			cout<<it->first<<" "<<it->second<<endl;
			cout<<invalid;
			return 0;
		}
	}
}
```



再贴个独角兽那道题~

来自星星的都教授除了所有感官比地球人高出七倍，始终容颜不老以外，还拥有一项在地球人看来特别神奇的能力，他会瞬间(0.00000000001s以内)按照他的规则比较地球人熟悉的两个十进制数字的大小，他比较的规则如下：   

1、将要比较的两个数字分别转换成二进制数字；   

2、计算两个二进制数字中1的个数，个数多的数字为两者中的大者；   

3、负数按照其绝对值进行比较；   

请利用地球人发明的计算机程序逼近都教授的特异功能，实现时可以有以下约束：   

1、输入数据为范围在-32768到32768(地球人的十进制世界）之间的任意两个数字；   

2、如果经过比较后2个数相等，输出为0，如果不相等，输出最大值。如果输入非法，输出-1



```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;
double cmp (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}

int main()
{
	int num=0;
	scanf("%f",&num);
	double len[40];
	for(int i=0;i<num;i++)
	{
		scanf("%f",&len[i]);
	}
	qsort(len,num,sizeof(double),cmp);
	double flag=0;
	for(int j=0;j<num;j++)
	{
		if(flag==0) flag=len[j];
		else if(flag==len[j]) flag=0.0;
		else
		{
			printf("%lld",flag);
			return 0;	 
		}
	}
	printf("-1");
	return 0;
}
```






