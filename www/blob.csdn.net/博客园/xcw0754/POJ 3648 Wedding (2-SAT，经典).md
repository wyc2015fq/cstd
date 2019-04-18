# POJ 3648 Wedding (2-SAT，经典) - xcw0754 - 博客园
# [POJ 3648 Wedding (2-SAT，经典)](https://www.cnblogs.com/xcw0754/p/4641206.html)
题意：新郎和新娘结婚，来了n-1对夫妻，这些夫妻包括新郎之间有通奸关系（包括男女，男男，女女），我们的目地是为了满足新娘，新娘对面不能坐着一对夫妻，也不能坐着有任何通奸关系的人，另外新郎一定要坐新娘对面。但是输出时输出坐在新娘这一边的人（不需要输出新娘）。
分析：
　　问题只是要求不能 “通奸对” 不能同时出现在新娘对面（即新郎那边），这个必须考虑到2*n个人的座位问题。一开始以为只考虑女的怎么坐就行了，男肯定坐对面，但是后来想这样子只能保证两边都没有通奸对出现，不符合题意，有些数据过不了。
　　考虑数据大小，2*n个人，那么数组要开4*n大小，每个人都有两个选择，尽管一对夫妻必须对着坐。
　　不如假设新娘一定坐在左边，即 i*2+1这边，那么新郎必定在j*2了，可以通过(j*2+1)->(j*2)控制新郎位置，新娘同理。 而我们要选择的是没有冲突的一个解，则要按照这个去构造与新郎同边的人的情况不冲突。当接到一条通奸边u-v时，应该是有边 u*2 -> v*2+1和 v*2 -> u*2+1 。注意这只是控制新郎这边的。除了这些边之外，每对夫妻之间也有个硬性要求是，必须对着坐，所以要给这n对夫妻一些固定的边，将他们绑定在一起。
　　挑选出一组解之后，其中i*2+1的就是解了，但是这也有n*2个人，我们只要n个，其中有一半和新娘同颜色的才是答案，剩下n人。
```
1 #include <iostream>
  2 #include <stdio.h>
  3 #include <string.h>
  4 #include <vector>
  5 #include <stack>
  6 #include <algorithm>
  7 #include <map>
  8 //#include <bits/stdc++.h>
  9 #define LL long long
 10 #define pii pair<int,int>
 11 #define INF 0x7f7f7f7f
 12 using namespace std;
 13 const int N=140;
 14 vector<int> vect[N];
 15 
 16 int col[N], s[N], c;
 17 bool color(int x)
 18 {
 19     if(col[x^1])    return false;
 20     if(col[x])      return true;
 21     col[x]=2;
 22     s[c++]=x;
 23     for(int i=0; i<vect[x].size(); i++)
 24         if(!color(vect[x][i]))  return false;
 25     return true;
 26 }
 27 
 28 
 29 int cal(int n)
 30 {
 31     memset(col,0,sizeof(col));
 32     memset(s,0,sizeof(s));
 33     for(int i=0; i<n; i+=2)
 34     {
 35         if(!col[i] && !col[i+1])
 36         {
 37             c=0;
 38             if(!color(i))
 39             {
 40                 while(c)    col[s[--c]]=0;      //清除这次错误的路径
 41                 if(!color(i+1)) return false;   //再试试
 42             }
 43         }
 44     }
 45     return true;
 46 }
 47 
 48 
 49 void print(int n)
 50 {
 51     int t=col[0];
 52     for(int i=5; i<n; i+=2)         //新娘一定坐在i*2+1这边
 53     {
 54         if(col[i])                  //再排除掉一半人。
 55         {
 56             if(i%4==1)   printf("%dw ",i/4);
 57             else         printf("%dh ",i/4);
 58         }
 59     }
 60     printf("\n");
 61 }
 62 
 63 
 64 int main()
 65 {
 66     freopen("input.txt", "r", stdin);
 67     int n, m, a, c;
 68     char b, d;
 69     while(scanf("%d%d",&n,&m), n+m)
 70     {
 71         for(int i=n*4; i>=0; i--)   vect[i].clear();        //2*30个人，要120个大小
 72 
 73         for(int i=0; i<n*2; i++ )   //先解决老婆与老公之间的边。老婆是偶数。
 74         {
 75             vect[i*2].push_back((i^1)*2+1);
 76             vect[i*2+1].push_back((i^1)*2);
 77         }
 78         // 先固定好新郎新娘的位置
 79         vect[1*2+1].push_back(1*2);  //保证新郎一定坐在i*2这边
 80         vect[0*2].push_back(0*2+1);  //保证新娘一定坐在i*2+1这边
 81 
 82         for(int i=0; i<m; i++)                  //不能只考虑n个女人怎么坐
 83         {
 84             scanf("%d%c %d%c", &a, &b, &c, &d);
 85             if(a==c)    continue;               //夫妇肯定对着坐，不用管
 86 
 87             a<<=1;  //恢复它们的真正号数
 88             c<<=1;
 89             if(b=='h')  a++;
 90             if(d=='h')  c++;
 91 
 92             vect[a*2].push_back(c*2+1);         //如果你坐新郎那边，我必须坐对面了。但是你坐新娘那边，我也可以坐，不冲突。
 93             vect[c*2].push_back(a*2+1);         //如果我坐新郎那边，你必须坐对面了
 94         }
 95 
 96         if(!cal(n*4))   puts("bad luck");
 97         else    print(n*4);
 98     }
 99     return 0;
100 }
AC代码
```
　　下面是摘自别人的分析：
　　很明显的2-sat模型，虽然要输出新娘这一边的人，但是我们构建的是对面的，为什么？因为我们要保证对面的人没有矛盾（但是新娘这一侧是允许有矛盾的，因为新娘看不到她这一侧的人）。另外我们要保证新郎一定坐在对面，在2-sat中固定一个元素的a的方法是~a->a，这个也好理解，a和~a必须选一个，如果选了a那么就选了，如果选了~a,又因为~a->a，说明a也必须选，也就是说无论如何a都要被选到。
　　按照2-sat正常的流程坐下来，会得到一个可行解，也就是和新郎颜色相同的点，它们都是可行解，它们是没有矛盾的，可以坐在新娘对面，所以剩下的点（其实就是和新娘同色的点）就是和新娘坐在一侧的。

