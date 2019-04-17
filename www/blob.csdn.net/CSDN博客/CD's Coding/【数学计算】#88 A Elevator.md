# 【数学计算】#88 A. Elevator - CD's Coding - CSDN博客





2015年01月30日 01:27:37[糖果天王](https://me.csdn.net/okcd00)阅读数：625
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)













And now the numerous qualifying tournaments for one of the most prestigious Russian contests Russian Codec Cup are over. All *n*participants
 who have made it to the finals found themselves in a huge *m*-floored 108-star
 hotel. Of course the first thought to come in a place like this is "How about checking out the elevator?".


The hotel's elevator moves between floors according to one never changing scheme. Initially (at the moment of time 0) the elevator is located on the 1-st
 floor, then it moves to the 2-nd floor, then — to the 3-rd floor and
 so on until it reaches the *m*-th floor. After that the elevator moves to floor *m* - 1,
 then to floor *m* - 2, and so on until it reaches the first floor. This process is repeated infinitely. We know that the elevator has infinite capacity; we
 also know that on every floor people get on the elevator immediately. Moving between the floors takes a unit of time.


For each of the *n* participant you are given *s**i*,
 which represents the floor where the *i*-th participant starts, *f**i*,
 which represents the floor the *i*-th participant wants to reach, and *t**i*,
 which represents the time when the *i*-th participant starts on the floor *s**i*.


For each participant print the minimum time of his/her arrival to the floor *f**i*.


If the elevator stops on the floor *s**i* at
 the time *t**i*,
 then the *i*-th participant can enter the elevator immediately. If the participant starts on the floor *s**i* and
 that's the floor he wanted to reach initially (*s**i* = *f**i*),
 then the time of arrival to the floor *f**i* for
 this participant is considered equal to *t**i*.




Input


The first line contains two space-separated integers *n* and *m* (1 ≤ *n* ≤ 105, 2 ≤ *m* ≤ 108).


Next *n* lines contain information about the participants in the form of three space-separated integers *s**i**f**i**t**i* (1 ≤ *s**i*, *f**i* ≤ *m*, 0 ≤ *t**i* ≤ 108),
 described in the problem statement.




Output


Print *n* lines each containing one integer — the time of the arrival for each participant to the required floor.




Sample test(s)




input
7 4
2 4 3
1 2 0
2 2 0
1 2 1
4 3 5
1 2 2
4 2 0




output
9
1
0
7
10
7
5




input
5 5
1 5 4
1 3 1
1 3 4
3 1 5
4 2 5




output
12
10
10
8
7






Note


Let's consider the first sample. The first participant starts at floor *s* = 2 by the time equal to *t* = 3.
 To get to the floor *f* = 4, he has to wait until the time equals 7,
 that's the time when the elevator will go upwards for the second time. Then the first participant should get on the elevator and go two floors up. In this case the first participant gets to the floor *f* at
 time equal to 9. The second participant starts at the time *t* = 0 on
 the floor *s* = 1, enters the elevator immediately, and arrives to the floor *f* = 2.
 The third participant doesn't wait for the elevator, because he needs to arrive to the same floor where he starts.








一句话：哇塞题目好长！话说我这个专题可以作为……A题缩句者~

有一栋m层高的大楼，有一个电梯是从一楼开始，一层一层向上，到顶之后一层一层下降，这样循环往复的过程。

有n个人他们要搭电梯，每个人有3个参数： s是这个人的出发层，f是这个人的目标层，t是指这个人从t时刻开始在电梯口等待，问：输出每一个人到达目的地时的到达时间。

需要注意的是，电梯从底到顶是m-1次移动，所以一个往复是2*m-2，当一个人错过了当前这轮电梯的时候他将只有等待下一班的到来，结合生活实际就会容易理解一些。




闲着没事干代码也缩了个句……

Code：



```cpp
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/117
// Elevator

int main()
{
    int n,m,s,f,t;
    scanf("%d%d",&n,&m);
    for(m=m*2-2; n--; printf("%d\n",s^f?(t+m-(s>f?m+2-s:s))/m*m+(s>f?m+2-f:f)-1:t))
    scanf("%d%d%d",&s,&f,&t);
    return 0;
}
```


那个for是不是过分了点…… 异或的意思就是不等啦，据说计算起来比较快……

帮大家扩个句哦：



```cpp
while(n--)
{
	if(s!=f)
	{
		if(s>f)
			cout<<(t+m-(m+2-s))/m*m + (m+2-f) -1 <<endl;
		else 
			cout<<(t+m-s)/m*m +f-1<<endl;
	}
	else cout<<t<<endl;
}
```


当然替换前需要把m=m*2-2写在前头哦。




为了便于理解，再放一个别人的写的比较分开的代码，可能会更加清晰明朗一些吧：

Code By MK3@Codeforces



```cpp
#include <iostream>
using namespace std;

int main()
{ 
  int n,m;
  int p,s,f,t,x; 
  cin >> n >> m; p=(m-1)*2; 
  while(n--)
  {
    cin >> s >> f >> t; 
    if(s==f) cout << t << endl; 
    else
    {
      x = t / p*p + (s<f ? s-1 : 2*m-1-s); 
      cout << (x<t ? x+p : x) + (f>s ? f-s : s-f) << endl; 
    }
  }
  return 0;
}
```





