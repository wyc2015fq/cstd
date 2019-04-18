# HDU 1010 Tempter of the Bone 骨头诱惑（DFS+剪枝） - xcw0754 - 博客园
# [HDU 1010 Tempter of the Bone 骨头诱惑（DFS+剪枝）](https://www.cnblogs.com/xcw0754/p/4225087.html)
题意：
　　必须在第t秒走到格子D上，S为起点，D为终点，点就是可以走，X就是墙。
思路：
将迷宫外围四面都筑墙‘X’。深度搜索+奇偶剪枝，再加一个剪枝“无法在指定时间内到达”。
```
1 #include <iostream>
 2 #include <vector>
 3 #include <string>
 4 #include <math.h>
 5 using namespace std;
 6 vector<string> v;
 7 int n,m;
 8 int x_1,y_1,x_2,y_2;
 9 bool maze(int x,int y,int t)    //目前走到第n行m列的位置
10 {
11     if(t==1)    //若t小于0，则检查四个方向是否有门D
12     {
13         if(x_2==x && (y_2==y-1||y_2==y+1))
14             return true;
15         if(y_2==y && (x_2==x-1||x_2==x+1))
16             return true;
17     }
18     else    //若不为0，调用4次此函数，分别是四个方向，若该方向不能走，则不调用
19     {
20         t--;
21         v[x][y]='0';    //封掉该格子
22         if(v[x-1][y]=='.' && maze(x-1,y,t)==true )
23             return true;
24         if(v[x+1][y]=='.' && maze(x+1,y,t)==true )
25             return true;
26         if(v[x][y-1]=='.' && maze(x,y-1,t)==true )
27             return true;
28         if(v[x][y+1]=='.' && maze(x,y+1,t)==true )
29             return true;
30     }
31     v[x][y]='.';    //开放该格子
32     return false;
33 }
34 int main()
35 {
36     int t,i,j;    //x为行，y为列。
37     string temp="",tem="XXXXXXXX";
38     while(scanf("%d %d %d",&n,&m,&t)&&n!=0)
39     {
40         v.push_back(tem);
41         for(i=0;i<n;i++)
42         {
43             cin>>temp;
44             temp='X'+temp;
45             temp+='X';    //围起来
46             v.push_back(temp);
47             if(temp.find('S')!=-1)
48             {
49                 x_1=i+1;y_1=temp.find('S');
50             }
51             if(temp.find('D')!=-1)
52             {
53                 x_2=i+1;y_2=temp.find('D');
54             }
55         }
56         v.push_back(tem);
57         
58         if(    abs(y_2-y_1)+abs(x_2-x_1)>t)
59             printf("NO\n");
60         else if( (abs(y_2-y_1)+abs(x_2-x_1))%2!=t%2 )
61             printf("NO\n");
62         else if(maze(x_1,y_1,t)==true)
63             printf("YES\n");
64         else
65             printf("NO\n");
66         v.clear();
67     }
68     return 0;
69 }
1010
```
题目直译：
小狗在一个古老的迷宫里发现一根骨头，迷宫让它很惊讶。但是，当他拿起骨头，迷宫开始振动，而小狗能够感觉到地面在下沉。
它意识到骨头是个陷阱，他准备分身一试逃出迷宫。
迷宫是个矩形，其大小是N*M，有个门在迷宫里。在一开始，门是关的，它将会在第T秒开一小段时间（小于1秒）。
因此，小狗必须在刚好T秒时到达门的旁边。每一秒，小狗能够移动一步到当前块块的上、下、左、右的块。
一旦它进入一个块，这块的地面将开始下沉并消失在下一秒（即每个位置只能走一次）。它不能够呆在一个块上超过1秒，也不能进入一个已走过过的块。
这只小狗能活下去吗？请帮助他。
输入：第一行包括3个整数N、M、T，分别代表迷宫的大小、迷宫门会开的时刻。
接下来N行是迷宫的布局，每行包括M个字符，只能是以下四种之一：
	X：一面墙，不能进入
	S:狗的起点
	D：迷宫门
	.：一个空块。
 1<N
 M<7
 0 < T < 50

