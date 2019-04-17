# 【水推理】#72 A. Toy Army - CD's Coding - CSDN博客





2014年10月27日 15:24:58[糖果天王](https://me.csdn.net/okcd00)阅读数：620
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The hero of our story, Valera, and his best friend Arcady are still in school, and therefore they spend all the free time playing turn-based strategy "GAGA: Go And Go Again". The gameplay is as follows.


There are two armies on the playing field each of which consists of *n* men (*n* is
 always even). The current player specifies for each of her soldiers an enemy's soldier he will shoot (a target) and then all the player's soldiers shot simultaneously. This is a game world, and so each soldier shoots perfectly, that is he absolutely always
 hits the specified target. If an enemy soldier is hit, he will surely die. It may happen that several soldiers had been indicated the same target. Killed soldiers do not participate in the game anymore.


The game "GAGA" consists of three steps: first Valera makes a move, then Arcady, then Valera again and the game ends.


You are asked to calculate the maximum total number of soldiers that may be killed during the game.




Input


The input data consist of a single integer *n* (2 ≤ *n* ≤ 108, *n* is
 even). Please note that before the game starts there are 2*n*soldiers on the fields.




Output


Print a single number — a maximum total number of soldiers that could be killed in the course of the game in three turns.




Sample test(s)




input
2




output
3




input
4




output
6






Note


The first sample test:


1) Valera's soldiers 1 and 2 shoot at Arcady's soldier 1.


2) Arcady's soldier 2 shoots at Valera's soldier 1.


3) Valera's soldier 1 shoots at Arcady's soldier 2.


There are 3 soldiers killed in total: Valera's soldier 1 and Arcady's soldiers 1 and 2.










这题的代码长度已经突破天际了……



意思是 有两组部队，V军和A军，V军先攻击然后轮到A军然后轮到V军。每个军人每次只能打死一个敌人但是必中必死，问就这三轮攻击最多死多少人。

死的最多即活着的最少。最后一轮攻击后假设或者k个人，那么他们在最后一轮攻击的时候杀了敌人的k个人且是敌军的最后k个人，而这k个人活着的时候杀了这半边的k个人，再向前追溯一次就是第一次V军攻击的结果是：这边有2k个人，杀了对面一些人之后对面剩下k个，因为两边最初都是n个人，为了让杀的人最多，那么2k=n，所以死了3k的人，即n的1.5倍。

读入然后输出1.5倍即可。






```cpp
#include <iostream>
using namespace std;
// http://codeforces.com/contest/84
// Toy Army
int main()
{
	int n;	
	cin>>n;
	cout<<n/2*3;
	return 0;
}
```





