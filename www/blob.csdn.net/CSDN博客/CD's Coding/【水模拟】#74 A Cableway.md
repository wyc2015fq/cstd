# 【水模拟】#74 A. Cableway - CD's Coding - CSDN博客





2014年10月29日 17:35:11[糖果天王](https://me.csdn.net/okcd00)阅读数：1299
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A group of university students wants to get to the top of a mountain to have a picnic there. For that they decided to use a cableway.


A cableway is represented by some cablecars, hanged onto some cable stations by a cable. A cable is scrolled cyclically between the first and the last cable stations (the first of them is located at the bottom of the mountain and the last one is located at
 the top). As the cable moves, the cablecar attached to it move as well.


The number of cablecars is divisible by three and they are painted three colors: red, green and blue, in such manner that after each red cablecar goes a green one, after each green cablecar goes a blue one and after each blue cablecar goes a red one. Each cablecar
 can transport no more than two people, the cablecars arrive with the periodicity of one minute (i. e. every minute) and it takes exactly 30minutes
 for a cablecar to get to the top.


All students are divided into three groups: *r* of them like to ascend only in the red cablecars, *g* of
 them prefer only the green ones and *b*of them prefer only the blue ones. A student never gets on a cablecar painted a color that he doesn't
 like,


The first cablecar to arrive (at the moment of time 0) is painted red. Determine the least time it will take all students to ascend to the mountain
 top.




Input


The first line contains three integers *r*, *g* and *b* (0 ≤ *r*, *g*, *b* ≤ 100).
 It is guaranteed that *r* + *g* + *b* > 0, it means that the group consists of at least one student.




Output


Print a single number — the minimal time the students need for the whole group to ascend to the top of the mountain.




Sample test(s)




input
1 3 2




output
34



input
3 2 1




output
33





Note


Let's analyze the first sample.


At the moment of time 0 a red cablecar comes and one student from the *r* group
 get on it and ascends to the top at the moment of time30.


At the moment of time 1 a green cablecar arrives and two students from the *g* group
 get on it; they get to the top at the moment of time31.


At the moment of time 2 comes the blue cablecar and two students from the *b* group
 get on it. They ascend to the top at the moment of time 32.


At the moment of time 3 a red cablecar arrives but the only student who is left doesn't like red and the cablecar leaves empty.


At the moment of time 4 a green cablecar arrives and one student from the *g* group
 gets on it. He ascends to top at the moment of time34.


Thus, all the students are on the top, overall the ascension took exactly 34 minutes.
















发现现在这样的A题好多呀，就是一个数组，每次来一个操作，直到某个条件满足时为止。

因为数据范围小所以可以直接模拟，这题也是一样，有红绿蓝三种颜色的索道，按照红绿蓝的顺序每分钟来一个，每个索道的缆车每次最多搭载2个人，问啥时候所有人都送上山总共要花多少时间（上山的时间是30Min别忘了加上）

那么我们就模拟一下，直到没人了为止，每次按顺序红绿蓝地每次减二（别忘了如果没人了不能减应该归零）。

Code：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/90
// Cableway

int main()
{
	int r,g,b,now=29;	cin>>r>>g>>b;
	int n[3]={r,g,b};
	while(n[0]+n[1]+n[2])
	{
		now++;
		if(n[now%3]>=2) n[now%3]-=2;
		else n[now%3]=0;
	} 
	cout<<now;
	return 0;
}
```








