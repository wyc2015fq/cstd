# NOIP复习篇 - 在思索中前行！ - CSDN博客





2016年10月09日 11:23:37[_Tham](https://me.csdn.net/txl16211)阅读数：2059










# NOIP复习篇———枚举

----------------------------------------------------------------------------------------------------------------

高手的切磋不在于难题，而在于SB算法....NOIP来了，决不能犯SB错误

---------------------------------------------------------------------------------------------------------------------------------------------------------

1.1 枚举算法

#define#

  对于无从下手的问题，我们往往刻画其解空间，使得这个解空间有了上限和下限，这样，我们可以枚举这个区间值，使得问题得到解答

#优点#

  写起来很容易，构思简单，答案准确

#缺点#

  时间的长够你等一辈子

#模板#

  function findans(int l,int r)  上限l，下限r

  {

      int ans;

      for(ans=l -) r)

         if (ans可行)cnt++;  找到一个答案

 }




1.1.2 例题




                                                        《打地鼠》




题目描述 Description



打地鼠是这样的一个游戏：地面上有一些地鼠洞，地鼠们会不时从洞里探出头来很短时间后又缩回洞中。玩家的目标是在地鼠伸出头时，用锤子砸其头部，砸到的地鼠越多分数也就越高。


游戏中的锤子每次只能打一只地鼠，如果多只地鼠同时探出头，玩家只能通过多次挥舞锤子的方式打掉所有的地鼠。你认为这锤子太没用了，所以你改装了锤子，增加了锤子与地面的接触面积，使其每次可以击打一片区域。如果我们把地面看做m*n的方阵，其每个元素都代表一个地鼠洞，那么锤子可以覆盖R*C区域内的所有地鼠洞。但是改装后的锤子有一个缺点：每次挥舞锤子时，对于这R*C的区域中的所有地洞，锤子会打掉恰好一只地鼠。也就是说锤子覆盖的区域中，每个地洞必须至少有1只地鼠，且如果某个地洞中地鼠的个数大于1，那么这个地洞只会有1只地鼠被打掉，因此每次挥舞锤子时，恰好有R*C只地鼠被打掉。由于锤子的内部结构过于精密，因此在游戏过程中你不能旋转锤子（即不能互换R和C）。


你可以任意更改锤子的规格（即你可以任意规定R和C的大小），但是改装锤子的工作只能在打地鼠前进行（即你不可以打掉一部分地鼠后，再改变锤子的规格）。你的任务是求出要想打掉所有的地鼠，至少需要挥舞锤子的次数。


Hint：由于你可以把锤子的大小设置为1*1，因此本题总是有解的。





输入描述 Input Description



第一行包含两个正整数m和n；


下面m行每行n个正整数描述地图，每个数字表示相应位置的地洞中地鼠的数量。





输出描述 Output Description



输出一个整数，表示最少的挥舞次数。





样例输入 Sample Input



3 3

1 2 1

2 4 2

1 2 1 


样例输出 Sample Output



4





数据范围及提示 Data Size & Hint



使用2*2的锤子，分别在左上、左下、右上、右下挥舞一次。



对于30%的数据，m,n≤5；


对于60%的数据，m,n≤30；


对于100%的数据，m,n≤100，其他数据不小于0，不大于105。

#分析#

  题目的意思是要我们找出最适合的锤子的大小，使得每次打击都不会打空或者打出界且打完所有地鼠所需要的次数最小。

  我们发现，如果求打击次数，很明显就是sum/(r*c) 这里sum表示所有地鼠个数和，r和c分别表示改装后的锤子的长和宽，那么我们只需要枚举r和c即可，但是，在枚举的时候，要注意不能打空

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40756433#)[copy](http://blog.csdn.net/u012746396/article/details/40756433#)

[print](http://blog.csdn.net/u012746396/article/details/40756433#)[?](http://blog.csdn.net/u012746396/article/details/40756433#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=101;  
- 
- int g[MaxN][MaxN],allsum=0;  
- int n,m,r,c,ans=0x7fffffff;  
- int tmp[MaxN][MaxN];  
- 
- bool ok(){  
- int i,j,i1,j1;  
- for(i=1;i<=n;i++)  
- for(j=1;j<=m;j++)  
-         tmp[i][j]=g[i][j];  
- for(i=1;i<=n;i++)//枚举中心点 
- for(j=1;j<=m;j++)  
- if(tmp[i][j]){  
- if(i+r<=n+1 && j+c<=m+1){  
- int delta=tmp[i][j];  
- for(i1=0;i1<r;i1++)  
- for(j1=0;j1<c;j1++)  
-                 tmp[i+i1][j+j1]-=delta;  
- if(tmp[i1+i][j1+j]<0)returnfalse;  
-           }  
- elsereturnfalse;  
-         }  
- returntrue;  
- }  
- 
- int main(){  
-   scanf("%d%d",&n,&m);  
- for(r=1;r<=n;r++)  
- for(c=1;c<=m;c++)  
-     {  
-         scanf("%d",&g[r][c]);  
-         allsum+=g[r][c];  
-     }  
- for(r=1;r<=n;r++)//枚举锤子的长宽 
- for(c=1;c<=m;c++)  
-     {  
- if(allsum%(r*c)==0 && allsum/(r*c)<ans && ok()==true)  
-           ans=allsum/(r*c);  
-     }  
-   cout<<ans;  
- return 0;  
- }  
- 





《Jam的计数法》




题目描述 Description


Jam是个喜欢标新立异的科学怪人。他不使用阿拉伯数字计数，而是使用小写英文字母计数，他觉得这样做，会使世界更加丰富多彩。在他的计数法中，每个数字的位数都是相同的（使用相同个数的字母），英文字母按原先的顺序，排在前面的字母小于排在它后面的字母。我们把这样的“数字”称为Jam数字。在Jam数字中，每个字母互不相同，而且从左到右是严格递增的。每次，Jam还指定使用字母的范围，例如，从2到10，表示只能使用{b,c,d,e,f,g,h,i,j}这些字母。如果再规定位数为5，那么，紧接在Jam数字“bdfij”之后的数字应该是“bdghi”。（如果我们用U、V依次表示Jam数字“bdfij”与“bdghi”，则U<V<
 span>，且不存在Jam数字P，使U<P<V<
 span>）。你的任务是：对于从文件读入的一个Jam数字，按顺序输出紧接在后面的5个Jam数字，如果后面没有那么多Jam数字，那么有几个就输出几个。





输入描述 Input Description


有2行，第1行为3个正整数，用一个空格隔开：

s t w

（其中s为所使用的最小的字母的序号，t为所使用的最大的字母的序号。w为数字的位数，这3个数满足：1≤s<T<
 span>≤26, 2≤w≤t-s ）

第2行为具有w个小写字母的字符串，为一个符合要求的Jam数字。

所给的数据都是正确的，不必验证。





输出描述 Output Description


最多为5行，为紧接在输入的Jam数字后面的5个Jam数字，如果后面没有那么多Jam数字，那么有几个就输出几个。每行只输出一个Jam数字，是由w个小写字母组成的字符串，不要有多余的空格





样例输入 Sample Input


2 10 5

bdfij





样例输出 Sample Output


bdghi

bdghj

bdgij

bdhij

befgh



#分析#

  题目的大意是给出一串字符，然后求出它的后续排列当且仅当不能有字符越界

  我们观察题目，发现题目就是要我们枚举一个需要改变的位置，这个位置自然就是t+96+j-w+1

  然后对于后几位处理：str[k]=str[k-1]+1

  不要忘记如果找不到相应位置时，则没有Jam数字了

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40756433#)[copy](http://blog.csdn.net/u012746396/article/details/40756433#)

[print](http://blog.csdn.net/u012746396/article/details/40756433#)[?](http://blog.csdn.net/u012746396/article/details/40756433#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=300;  
- 
- char str[MaxN];  
- int s,t,w,l;  
- 
- int main(){  
- int i;  
-   cin>>s>>t>>w;  
-   scanf("%s",str);  
-   l=strlen(str)-1;  
- for(i=1;i<=5;i++){  
- int j=l;  
- while(j>=0 && str[j]==t+97+j-w)j--;  
- if(j==-1)break;  
-     str[j]++;  
- for(int k=j+1;k<=w-1;k++)str[k]=str[k-1]+1;  
-     cout<<str<<endl;  
-   }  
- return 0;  
- }  


                                                    《合数和》




题目描述 Description



用户输入一个数，然后输出从1开始一直到这个数为止（包括这个数）中所有的合数的和。





输入描述 Input Description



一个整数N，0<N<=1000





输出描述 Output Description



一行，一个整数，即从1到N中所有合数的和





样例输入 Sample Input



样例一：100




样例二：9





样例输出 Sample Output



样例一：3989




样例二：27



#分析#

  题意：找出n以内的合数，并累加起来，输出【感觉比较easy...

  至于找合数，我们有2种方法：

  1）从1~n枚举，对于每一个数用根号n的时间复杂度判断是否为合数，时间复杂度O(nlogn)，有可能会TLE，我们还有更好的方法：

  2）先用O(n)的筛法筛除n以内的质数，剩余的就是合数，然后累加，时间复杂度O(2n)

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40756433#)[copy](http://blog.csdn.net/u012746396/article/details/40756433#)

[print](http://blog.csdn.net/u012746396/article/details/40756433#)[?](http://blog.csdn.net/u012746396/article/details/40756433#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=1001;  
- 
- int p[MaxN],sum,n;  
- 
- int getans(){  
- int i,j;  
-     memset(p,1,sizeof(p));  
- for(i=2;i<=n;i++)  
- if(p[i])  
- for(j=i*i;j<=n;j+=i)  
-           p[j]=0;  
- for(i=2;i<=n;i++)  
- if(!p[i])sum+=i;  
- return sum;  
- }  
- 
- int main(){  
-   scanf("%d",&n);  
-   cout<<getans();  
- return 0;  
- }  




1.3 枚举算法的反思
  只有不断反思才能得到进步，枚举算法着实好，可惜，时间复杂度过于高，但我们可以优化，不过说实在的，枚举的优化也确实多。

***优化1：二分答案***

对于某些题目，它的答案是具有单调性的，所以可以用类似二分查找的方式查找答案，再判断是否可行，时间复杂度从O(N)降至O(logN)，详情参考《奇怪的函数》

*** 优化2：离散化***

对于不必要枚举的状态或者解空间，我们可以不必算它，所以在一些题目中，可以离散掉不需要的内容，然后剩下的再进行枚举，详情参考《最佳室温》和《海报》

***优化3：搜索***

搜索也是对解空间的刻画，只不过将解空间用解空间树联系起来，然后得到解的有序性，去除了一些不必要的状态，但是，往往也是会TLE的，搜索同样也有优化，这在后面的搜索复习中会提到（形如：记忆化搜索，BFS,DFS，PFS）

***优化4：动态规划***

对于解空间的处理，我们得到小问题，再不断将小问题合并，得出原问题的解，这是动态规划的长处，***空间换时间***

以得到问题的求解

***优化5：数据结构***

对于查找类的题目，没有必要去一一枚举，转换，只需要用数据结构来优化浪费的查找过程即可，主要体现在堆上面，详情参考《dijkstra算法》《prim算法》《合并果子》

----------------------------------------------------------------------------------------------------------------------------------------------------








# NOIP复习篇———贪心


-------------------------------------------------------------------------------------------------------------------

高手的切磋不在于难题，而在于SB算法....NOIP来了，决不能犯SB错误

---------------------------------------------------------------------------------------------------------------------------------------------------------

1.1 贪心算法

#定义#

  对于某个问题，不从宏观去考虑问题，而是从微观考虑问题，尽管大多数时候是不奏效的，但也有时候很有效。

#讲解#

  贪心算法，是指人们为了解决问题而下意识的为自己的最大利益而设计的方案，往往题目中会出现“最小”“最大”等关键词，而贪心算法的最大难度莫过于证明贪心和想出贪心策略，对于NOIP普及组的难度，不会考察这点..举个简单的例子，人们在穿越草地时往往选择直线穿过（两点之间，线段最短），这便是贪心算法。

  而贪心算法拥有的一个重要特征就是——最优子结构

#模板#

  function greedy(){

     int a[]={问题的子问题};

     sort(a+1,a+n+1);

     for(i=1;i<=n;i++)

       if(a[i]可行)直接记录，并退出

 }

1.2 例题

1.《旅行家的预算》




题目描述 Description



一个旅行家想驾驶汽车以最少的费用从一个城市到另一个城市（假设出发时油箱是空的）。给定两个城市之间的距离D1、汽车油箱的容量C（以升为单位）、每升汽油能行驶的距离D2、出发点每升汽油价格P和沿途油站数N（N可以为零），油站i离出发点的距离Di、每升汽油价格Pi（i=1，2，……N）。计算结果四舍五入至小数点后两位。如果无法到达目的地，则输出“No Solution”。





输入描述 Input Description



第一行D1 C D2 P N


之后N行，每行2个数表示离出发点的距离Di和每升汽油的价格Pi





输出描述 Output Description



最消费用，保留2位小数





样例输入 Sample Input



275.6 11.9 27.4 2.8 2


102.0 2.9


220.0 2.2





样例输出 Sample Output



26.95





数据范围及提示 Data Size & Hint



N<=100




#分析#

  这道题目贪心的味道很浓，或者说太浓了，我们希望花费最小，而又给定了加油站的位置，所以选择权在我们手中，则排除搜索和动态规划的可能性。

  那么，怎么贪呢？我们假设自己是那个旅行家，不要忘记了，***贪心是从局部考虑，而不是整体！***所以，我们只要考虑当前所在的加油站的决策即可，我们发现：

      （1）如果当前加油站有可以到达的油站，我们自然选择距离最小的（当且仅当当前加油站的油价>可以到达的加油站的油价）

      （2）如果当前加油站有可以到达的油站但没有一个油价较低，我们不妨在这一站加满油，开往下一站

      （3）如果当前加油站没有可以到达的加油站，则无法到达，输出"No Soultion"

  时间复杂度O(N^2)

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40799083#)[copy](http://blog.csdn.net/u012746396/article/details/40799083#)

[print](http://blog.csdn.net/u012746396/article/details/40799083#)[?](http://blog.csdn.net/u012746396/article/details/40799083#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=101;  
- 
- int n;  
- double d1,c,go,price_start;  
- double now,ans,minway,d;  
- 
- struct oil{  
- double dis,price;  
- friendbool operator< (oil a,oil b){return a.price<b.price;}  
- }s[MaxN];  
- 
- int main(){  
- int i,k,p;  
-   cin>>d1>>c>>go>>price_start>>n;  
- for(i=1;i<=n;i++)cin>>s[i].dis>>s[i].price;  
-   sort(s+1,s+n+1);  
-   s[k=0].dis=0;  
-   s[k].price=price_start;  
-   s[n+1].dis=d1;  
-   s[n+1].price=0;  
- while(now<d1){  
-     minway=(double)(0x7fffffff);  
-     p=-1;  
- for(i=1;i<=n+1;i++)  
- if(i!=k && s[k].price>s[i].price && c*go+s[k].dis>=s[i].dis && minway>(s[i].dis-s[k].dis) && s[i].dis>s[k].dis){  
-            minway=s[i].dis-s[k].dis;  
-            p=i;  
-        }  
- if(p!=-1){  
-       ans+=((s[p].dis-s[k].dis)/go-d)*s[k].price;  
-       now=s[p].dis;  
-       k=p;  
-     }  
- else {  
- if(((s[k+1].dis-s[k].dis)/go)>c){cout<<"No Solution";return 0;}  
-         ans+=(c-d)*s[k].price;//在此站把油加满 
-         d=c-(s[k+1].dis-s[k].dis)/go;  
-         now=s[k+1].dis;  
-         k++;  
-     }  
-   }  
-   printf("%.2lf",ans);  
- return 0;  
- }  
- 


2.《线段覆盖三步曲第一步——线段覆盖1》




题目描述 Description



    给定x轴上的N（0<N<100）条线段，每个线段由它的二个端点a_I和b_I确定，I=1,2,……N.这些坐标都是区间（－999，999）的整数。有些线段之间会相互交叠或覆盖。请你编写一个程序，从给出的线段中去掉尽量少的线段，使得剩下的线段两两之间没有内部公共点。所谓的内部公共点是指一个点同时属于两条线段且至少在其中一条线段的内部（即除去端点的部分）。





输入描述 Input Description



    输入第一行是一个整数N。接下来有N行，每行有二个空格隔开的整数，表示一条线段的二个端点的坐标。





输出描述 Output Description



    输出第一行是一个整数表示最多剩下的线段数。





样例输入 Sample Input



3


6  3


1  3


2  5





样例输出 Sample Output



2





数据范围及提示 Data Size & Hint



0<N<100



#分析#

  同样是一道贪心味道很浓的一题（想一想为什么？tip:看定义），那么，怎么贪呢？

  我们挖掘一下题目的最优子结构，首先，从贪心思想（只看局部）我们可以知道，只需要考虑每条线段就行了

  那么，我们设MAX表示当前最长不重叠线段的右端点，很明显初始时MAX=-oo

  这里要注意一点，要让线段按右端点排序，再处理，不然就不满足最优子结构了

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40799083#)[copy](http://blog.csdn.net/u012746396/article/details/40799083#)

[print](http://blog.csdn.net/u012746396/article/details/40799083#)[?](http://blog.csdn.net/u012746396/article/details/40799083#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=1000001;  
- 
- struct line{  
- int L,R;  
- friendbool operator< (line a,line b){return a.R<b.R;}  
- }a[MaxN];  
- 
- int ans,n,i,MAX;  
- 
- int main(){  
-   freopen("line.in","r",stdin);  
-   freopen("line.out","w",stdout);  
-   scanf("%d",&n);  
- for(i=1;i<=n;i++){  
-     scanf("%d %d",&a[i].L,&a[i].R);  
- if(a[i].L>a[i].R)swap(a[i].L,a[i].R);  
-   }  
-   sort(a+1,a+n+1);  
-   MAX=-(1<<30);  
- for(i=1;i<=n;i++){  
- if(MAX<=a[i].L){  
-         ans++;  
-         MAX=a[i].R;  
-       }  
-   }  
-   printf("%d",ans);  
- return 0;  
- }  

                                 3.《线段覆盖三步曲第三步——线段覆盖3》




题目描述 Description



在一个数轴上有n条线段，现要选取其中k条线段使得这k条线段两两没有重合部分（端点可以重合），问最大的k为多少。





输入描述 Input Description



输入格式


输入文件的第1行为一个正整数n，下面n行每行2个数字ai，bi，描述每条线段。





输出描述 Output Description



输出格式


　　输出文件仅包括1个整数，为k的最大值





样例输入 Sample Input



3


0 2


2 4


1 3





样例输出 Sample Output



2





数据范围及提示 Data Size & Hint



数据范围


对于20%的数据，n≤10；


对于50%的数据，n≤1000；


对于70%的数据，n≤100000；


对于100%的数据，n≤1000000，0≤ai＜bi≤1000000。



 #分析#

  又是一道贪心题，同例2一样，只不过输出ans+1即可

 【读者可能好奇为什么直接《线段覆盖3》，因为《线段覆盖2》是接下来的动态规划复习专题将提到的

#代码#

  略




                                                     4.《地鼠游戏》





题目描述 Description



    王钢是一名学习成绩优异的学生，在平时的学习中，他总能利用一切时间认真高效地学习，他不但学习刻苦，而且善于经常总结、完善自己的学习方法，所以他总能在每次考试中得到优异的分数，这一切很大程度上是由于他是一个追求效率的人。


    但王钢也是一个喜欢玩的人，平时在学校学习他努力克制自己玩，可在星期天他却会抽一定的时间让自己玩一下，他的爸爸妈妈也比较信任他的学习能力和学习习惯，所以在星期天也不会象其他家长一样对他抓紧，而是允许他在星期天上午可以自由支配时间。


    地鼠游戏是一项需要反应速度和敏捷判断力的游戏。游戏开始时，会在地板上一下子冒出很多地鼠来，然后等你用榔头去敲击这些地鼠，每个地鼠被敲击后，将会增加相应的游戏分值。问题是这些地鼠不会傻傻地等你去敲击，它总会在冒出一会时间后又钻到地板下面去（而且再也不上来），每个地鼠冒出后停留的时间可能是不同的，而且每个地鼠被敲击后增加的游戏分值也可能是不同，为了胜出，游戏参与者就必须根据每个地鼠的特性，有选择地尽快敲击一些地鼠，使得总的得分最大。


这个极具挑战性的游戏王钢特别喜欢，最近他经常在星期天上午玩这个游戏，慢慢地他不但敲击速度越来越快（敲击每个地鼠所需要的耗时是1秒），而且他还发现了游戏的一些特征，那就是每次游戏重新开始后，某个地鼠冒出来后停留的时间都是固定的，而且他记录了每个地鼠被敲击后将会增加的分值。于是，他在每次游戏开始后总能有次序地选择敲击不同的地鼠，保证每次得到最大的总分值。





输入描述 Input Description



    输入包含3行，第一行包含一个整数n（1<=n<=100）表示有n个地鼠从地上冒出来，第二行n个用空格分隔的整数表示每个地鼠冒出后停留的时间，第三行n个用空格分隔的整数表示每个地鼠被敲击后会增加的分值（<=100）。每行中第i个数都表示第i个地鼠的信息。





输出描述 Output Description



    输出只有一行一个整数，表示王钢所能获得的最大游戏总分值。





样例输入 Sample Input



5


5  3  6  1  4


7  9  2  1  5





样例输出 Sample Output




24




#分析#

  贪心味道很浓【感觉我逗比哒。。。】 题目让我们求分数最大，由于时间都是需要1s的，所以我们对结构体S排序（按时间从小到大），然后枚举时间，同时把同一时间的数统统放入堆中，每一秒，我们累加堆顶，堆可以用优先队列实现。

#代码#




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40799083#)[copy](http://blog.csdn.net/u012746396/article/details/40799083#)

[print](http://blog.csdn.net/u012746396/article/details/40799083#)[?](http://blog.csdn.net/u012746396/article/details/40799083#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=10001;  
- 
- struct mice{  
- int time,score;  
- friendbool operator< (mice a,mice b){return a.time<b.time;}  
- }g[MaxN];  
- 
- priority_queue<int > q;  
- 
- int n,ans=0;  
- 
- int main(){  
-   freopen("mouse.in","r",stdin);  
-   freopen("mouse.out","w",stdout);  
- int i,j,MAX=0;  
-   cin>>n;  
- for(i=1;i<=n;i++){cin>>g[i].time;MAX=max(MAX,g[i].time);}  
- for(i=1;i<=n;i++)cin>>g[i].score;  
-   sort(g+1,g+n+1);  
-   j=n;  
- for(i=MAX;i>=1;i--)  
-   {  
- while(j>0 && g[j].time==i){q.push(g[j].score);j--;}  
- if(!q.empty()){ans+=q.top();q.pop();}  
-   }  
-   cout<<ans;  
- return 0;  
- }  




1.3 反思

可以说，贪心的覆盖范围很广，不是一个晚上就能复习完的，不过，要做到举一反三，从而触类旁通，弄懂贪心算法的整体，说到底，还是8个字***“考虑局部，无视整体”***










# NOIP复习篇———动态规划


-------------------------------------------------------------------------------------------------------------------

高手的切磋不在于难题，而在于SB算法....NOIP来了，决不能犯SB错误

---------------------------------------------------------------------------------------------------------------------------------------------------------

1.1 动态规划

#定义#

  动态规划属于运筹学分类的一种，是解决多阶段决策问题的一种手段，其内部结构和搜索很相似：

    动态规划：由已知推到未知

    搜索：由未知推到已知

  不同的方法在实现上可能会有时间上的大量差别，动态规划枚举子问题求最优值，搜索枚举决策

#技巧#

  解决动态规划问题是每个OIer必备的基础，作为只能应付PJ的我也只是略懂...

  动态规划问题求解步骤：

  1）对问题进行分析，通过反证法证明问题具有最优子结构和无后效性

  2）找到问题的阶段

  3）根据阶段，找到描述阶段的量，这个量就组成了状态

  4）思考每一个问题的由来，例如fibnacio数列的每个子问题的关系是：f[n]=f[n-1]+f[n-2]，这步就是动态转移方程

  5）实现代码，有几个状态枚举几层，逐个描述，然后主体就是动态转移方程

  6）对于无从下手的动态规划问题，可以先用搜索的理解来找出转移方程，实在无奈，果断选择记忆化搜索

  7）对于空间过大的动态规划，我们可以利用《滚动数组》《离散化》等手段来优化

  8）寻找问题的初始阶段，如果fibnacio就是f(0)=f(1)=1

#动态规划的分类#

  动态规划分为以下几类：

  【1】序列型动态规划

  【2】棋盘型动态规划

  【3】区间型动态规划

  【4】划分型动态规划

  【5】背包型动态规划

  【6】状态压缩型动态规划

  【7】环形动态规划

  【8】树形动态规划

 对于PJ的难度，只需要考察【1】【2】【3】【4】【5】【7】




1.2 例题

  1.2.1 序列型动态规划例题——《拦截导弹》（NOIP1999提高组）




题目描述 Description



    某国为了防御敌国的导弹袭击，发展出一种导弹拦截系统。但是这种导弹拦截系统有一个缺陷：虽然它的第一发炮弹能够到达任意的高度，但是以后每一发炮弹都不能高于前一发的高度。某天，雷达捕捉到敌国的导弹来袭。由于该系统还在试用阶段，所以只有一套系统，因此有可能不能拦截所有的导弹。





输入描述 Input Description



输入导弹依次飞来的高度（雷达给出的高度数据是不大于30000的正整数）





输出描述 Output Description



输出这套系统最多能拦截多少导弹，如果要拦截所有导弹最少要配备多少套这种导弹拦截系统。





样例输入 Sample Input



7


389 207 155 300 299 170 158 65 





样例输出 Sample Output



6


2





数据范围及提示 Data Size & Hint



导弹的高度<=30000，导弹个数<=20




 【分析】

    将题目数学建模：导弹抽象为点，能连续打抽象为边

    则得到下图：![](https://img-blog.csdn.net/20141105221841031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjc0NjM5Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    那么很明显，题目是要我们求最长连续的一段不上升子序列（第一问），第二问就是最长不下降子序列的长度

    那么，如何求最长不下降（上升）子序列呢？

    以不下降举例：

    （1）证明最优子结构：

                假设：f(n)与f'(n)无关

                          则f'(n)的任意一个值都不等于f(n)

                          而一个导弹的高度取决于前一个导弹的高度

                          所以矛盾

                          故假设不成立，原命题成立。

             显然一定具有无后效性

    （2）阶段：已经处理的导弹的数目

    （3）状态：f(i)表示以第i颗导弹为结尾的最长不下降子序列

    （4）显然，f(i)=max{f(j)}+1     (2<=i<=n,1<=j<i)

    （5）时间复杂度为O(N^2)，空间复杂度O(N)，貌似没办法优化，但实质上是有的！

             我们发现，对于每一个阶段的值，有些处理是不必要的，而且整个线段是具有单调性的

             所以利用单调队列进行优化

    至此，这道题目已经AC了！




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- //朴素O(N^2)代码
- #include<iostream>
- #include<cstdio>
- usingnamespace std;  
- int a[30000];  
- int n=1,i,j,max1;  
- int f[600000];  
- int main()  
- {  
- while(scanf("%d",&a[n++])!=EOF);  
-     n--;  
-     f[1]=1;     
- for(i=2;i<=n;i++){f[i]=1;  
- for(j=1;j<=i-1;j++)  
- if(f[j]+1>f[i] && a[j]>=a[i])f[i]=f[j]+1;  
-         }  
-     max1=0;  
- for(i=1;i<=n;i++)if(f[i]>max1)max1=f[i];  
-         cout<<max1-1<<endl;  
-     f[1]=1;  
- for(i=2;i<=n;i++){f[i]=1;  
- for(j=1;j<=i-1;j++)  
- if(f[j]+1>f[i] && a[j]<a[i])f[i]=f[j]+1;  
-         }  
-     max1=0;  
- for(i=1;i<=n;i++)if(f[i]>max1)max1=f[i];  
-         cout<<max1;  
- // cout<<f[n];
- return 0;  
- }  




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- //O(nlogn)的代码，详情见鄙人博客的文章《单调队列优化LIS》
- #include <iostream>
- usingnamespace std;  
- #include <cstdio>
- 
- constint MaxN=100001;  
- 
- int n,i,top=0,x,stack[MaxN];  
- 
- int main(){  
-     cin>>n;  
-     stack[top]=-1;  
- for(i=1;i<=n;i++){  
-         cin>>x;  
- if(x>stack[top]){stack[++top]=x;}  
- else
-         {  
- int low=0,high=top,mid;  
- while(low<high){  
-                 mid=(low+high)>>1;  
- if(x>stack[mid])  
-                     low=mid+1;  
- else
-                     high=mid-1;  
-             }  
-             stack[low]=x;  
-         }  
-     }  
-     cout<<top;  
- return 0;  
- }  


                                    1.2.1 序列型动态规划例题——《线段覆盖2》（线段覆盖三部曲第二部）




题目描述 Description



数轴上有n条线段，线段的两端都是整数坐标，坐标范围在0~1000000，每条线段有一个价值，请从n条线段中挑出若干条线段，使得这些线段两两不覆盖（端点可以重合）且线段价值之和最大。


n<=1000





输入描述 Input Description



第一行一个整数n，表示有多少条线段。


接下来n行每行三个整数, ai bi ci，分别代表第i条线段的左端点ai，右端点bi（保证左端点<右端点）和价值ci。





输出描述 Output Description



输出能够获得的最大价值





样例输入 Sample Input



3


1 2 1


2 3 2


1 3 4





样例输出 Sample Output



4





数据范围及提示 Data Size & Hint



数据范围


对于40%的数据，n≤10；


对于100%的数据，n≤1000；


0<=ai,bi<=1000000


0<=ci<=1000000



【分析】

   同样，可以讲线段映射到数轴上（按右端点排序）【算是一种离散化吧】然后进行DP

   （1）阶段：线段的条数

   （2）状态：f(i)表示前i条线段中任意选择线段且必须选择第i条线段的最大价值

   （3）决策：f(i)=max{f(j)+val(i)}

  到了这里，题目已经解决了。




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=1001;  
- 
- struct line{  
- int L,R,val;  
- friendbool operator< (line a,line b){return a.R<b.R;}//重载运算符来排序
- }a[MaxN];  
- 
- int f[MaxN],n,i,j;  
- //f(i)表示前i条线段任意选择且必选择第i条线段能获得的最大值 
- int main(){  
-   scanf("%d",&n);  
- for(i=1;i<=n;i++)  
-     scanf("%d%d%d",&a[i].L,&a[i].R,&a[i].val);  
-   sort(a+1,a+n+1);  
- for(i=1;i<=n;i++)f[i]=a[i].val;  
- for(i=2;i<=n;i++){  
- for(j=1;j<i;j++)  
- if(a[j].R<=a[i].L && f[j]+a[i].val>f[i])f[i]=a[i].val+f[j];  
- if(f[i]>f[0])f[0]=f[i];  
-   }  
-   cout<<f[0];  
- return 0;  
- }  




 1.2.2 区间型动态规划例题——《石子合并》





题目描述 Description



有n堆石子排成一列，每堆石子有一个重量w[i], 每次合并可以合并相邻的两堆石子，一次合并的代价为两堆石子的重量和w[i]+w[i+1]。问安排怎样的合并顺序，能够使得总合并代价达到最小。





输入描述 Input Description



第一行一个整数n（n<=100）


第二行n个整数w1,w2...wn  (wi <= 100)





输出描述 Output Description



一个整数表示最小合并代价





样例输入 Sample Input



4


4 1 1 4





样例输出 Sample Output



18



【分析】

   （1）证明最优子结构略

   （2）阶段：区间

   （3）状态：f(i,j)表示[i,j]这个区间的最大值

   （4）决策：f[i][j]=min{f[i][j],f[i][k]+f[k+1][j]+sum[i][j]  




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- usingnamespace std;  
- #include <cstdio>
- constint MaxN=101;  
- int n,s[MaxN],f[MaxN][MaxN];  
- int i,j,k,r;  
- int main(){  
-     cin>>n;  
- for(i=1;i<=n;i++){  
-         cin>>s[i];  
-         s[i]+=s[i-1];  
-     }  
- for(r=2;r<=n;r++)  
- for(i=1;i<=n-r+1;i++){  
-            j=i+r-1;  
-            f[i][j]=0x7fffffff;  
- for(k=i;k<j;k++)  
-              f[i][j]=min(f[i][j],f[i][k]+f[k+1][j]+s[j]-s[i-1]);  
-        }  
-     cout<<f[1][n];  
- return 0;  
- }  




1.2.2 区间型动态规划例题——《能量项链》（NOIP2006提高组）




题目描述 Description


在Mars星球上，每个Mars人都随身佩带着一串能量项链。在项链上有N颗能量珠。能量珠是一颗有头标记与尾标记的珠子，这些标记对应着某个正整数。并且，对于相邻的两颗珠子，前一颗珠子的尾标记一定等于后一颗珠子的头标记。因为只有这样，通过吸盘（吸盘是Mars人吸收能量的一种器官）的作用，这两颗珠子才能聚合成一颗珠子，同时释放出可以被吸盘吸收的能量。如果前一颗能量珠的头标记为m，尾标记为r，后一颗能量珠的头标记为r，尾标记为n，则聚合后释放的能量为m*r*n（Mars单位），新产生的珠子的头标记为m，尾标记为n。

需要时，Mars人就用吸盘夹住相邻的两颗珠子，通过聚合得到能量，直到项链上只剩下一颗珠子为止。显然，不同的聚合顺序得到的总能量是不同的，请你设计一个聚合顺序，使一串项链释放出的总能量最大。

例如：设N=4，4颗珠子的头标记与尾标记依次为(2，3)
 (3，5) (5，10)
 (10，2)。我们用记号⊕表示两颗珠子的聚合操作，(j⊕k)表示第j，k两颗珠子聚合后所释放的能量。则第4、1两颗珠子聚合后释放的能量为：

(4⊕1)=10*2*3=60。

这一串项链可以得到最优值的一个聚合顺序所释放的总能量为

((4⊕1)⊕2)⊕3）=10*2*3+10*3*5+10*5*10=710。





输入描述 Input Description


第一行是一个正整数N（4≤N≤100），表示项链上珠子的个数。第二行是N个用空格隔开的正整数，所有的数均不超过1000。第i个数为第i颗珠子的头标记（1≤i≤N），当i<N<
 span>时，第i颗珠子的尾标记应该等于第i+1颗珠子的头标记。第N颗珠子的尾标记应该等于第1颗珠子的头标记。

至于珠子的顺序，你可以这样确定：将项链放到桌面上，不要出现交叉，随意指定第一颗珠子，然后按顺时针方向确定其他珠子的顺序。





输出描述 Output Description


只有一行，是一个正整数E（E≤2.1*109），为一个最优聚合顺序所释放的总能量。





样例输入 Sample Input



4


2 3 5 10





样例输出 Sample Output



710



【分析】

   对于环形的题目，我们往往采用的方法是拉两倍，然后div 2 再找最小f[i][i+n-1]

  （1）阶段：区间以及划分

  （2）状态：f(i,j,k)表示[i,j]这个区间的最大值

  （3）对于环形问题，最终的答案在n的范围内寻找

            for(i=1;i<=n;i++)

                    MAX=max(MAX,f[i][i+n-1][m]);

    当然，这题同时涉足了两个领域：区间&环形




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=205;//n<=100,这里拉两倍
- 
- int f[MaxN][MaxN];//f(i,j)表示[i,j]这段区间的最优值(将最后一个点放入前面)
- int a[MaxN];  
- int n;  
- 
- int main(){  
- int i,j,k,r;  
-   cin>>n;  
- for(i=1;i<=n;i++)  
-   {  
-       cin>>a[i];  
-       a[i+n]=a[i];  
-   }  
- for(j=2;j<(n<<1);j++)  
- for(i=j-1;i>0 && j-i<n;i--)  
- for(k=i;k<j;k++)  
-         f[i][j]=max(f[i][j],f[i][k]+f[k+1][j]+a[i]*a[k+1]*a[j+1]);  
- int MAX=0;  
- for(i=1;i<=n;i++)  
-     MAX=max(MAX,f[i][i+n-1]);  
-   cout<<MAX;   
- return 0;  
- }  




                                      1.2.3 划分型动态规划例题——《乘积最大》（NOIP2000普及组）





题目描述 Description



今年是国际数学联盟确定的“2000——世界数学年”，又恰逢我国著名数学家华罗庚先生诞辰90周年。在华罗庚先生的家乡江苏金坛，组织了一场别开生面的数学智力竞赛的活动，你的一个好朋友XZ也有幸得以参加。活动中，主持人给所有参加活动的选手出了这样一道题目：


设有一个长度为N的数字串，要求选手使用K个乘号将它分成K+1个部分，找出一种分法，使得这K+1个部分的乘积能够为最大。


同时，为了帮助选手能够正确理解题意，主持人还举了如下的一个例子：


有一个数字串：312， 当N=3，K=1时会有以下两种分法：


1)  3*12=36


2)  31*2=62


   这时，符合题目要求的结果是：31*2=62


   现在，请你帮助你的好朋友XZ设计一个程序，求得正确的答案。





输入描述 Input Description



   程序的输入共有两行：


   第一行共有2个自然数N，K（6≤N≤40，1≤K≤6）


   第二行是一个长度为N的数字串。





输出描述 Output Description



   结果显示在屏幕上，相对于输入，应输出所求得的最大乘积（一个自然数）。





样例输入 Sample Input



4  2


1231





样例输出 Sample Output



62



【分析】

  对于划分型动态规划，往往考虑“定范围，取特殊”

  （1）阶段：划分

  （2）状态：f(i,j)表示前i段字符划分为j块所能得到的最大乘积

  （3）决策：f[i][j]=max(f[i][j],f[k][j-1]*merge(k+1,i));




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- typedeflonglong big;  
- 
- string s;  
- int n,k;  
- big f[41][7];//f(i,j)表示s中前i个数插入j个乘号所能得到的最大值 
- 
- big merge(int l,int r){  
-     big num=0;  
- for(;l<=r;l++)num=num*10+s[l]-'0';  
- return num;  
- }  
- 
- int main(){  
-   cin>>n>>k;  
-   cin>>s;  
- int i,j,ak;  
- for(i=0;i<n;i++)f[i][0]=merge(0,i);  
- for(i=1;i<n;i++)  
- for(j=1;j<=k;j++)  
- for(ak=0;ak<i;ak++)  
-         f[i][j]=max(f[i][j],f[ak][j-1]*merge(ak+1,i));  
-   cout<<f[n-1][k];  
- return 0;  
- }  


 1.2.3 划分型动态规划例题——《数的划分》（NOIP2001提高组）




题目描述 Description



将整数n分成k份，且每份不能为空，任意两种划分方案不能相同(不考虑顺序)。

例如：n=7，k=3，下面三种划分方案被认为是相同的。

1 1 5

1 5 1


5 1 1

问有多少种不同的分法。



输入描述 Input Description



输入：n，k (6<n<=200，2<=k<=6)





输出描述 Output Description




输出：一个整数，即不同的分法。




样例输入 Sample Input



 7 3





样例输出 Sample Output



4





数据范围及提示 Data Size & Hint



 {四种分法为：1，1，5;1，2，4;1，3，3;2，2，3;}



【分析】

  同样，划分型动态规划：

   （1）阶段：划分

   （2）状态：f(i,j)表示数字i被划分为j个数的和的方案数

   （3）决策：f[i][j]=f[i-j][j-1]+f[i-1][j-1]    (i>=j)




**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=210;  
- constint MaxM=7;  
- 
- int f[MaxN][MaxM];//f(i,j)表示数字i划分为j个数的和的方案数 
- int n,m;  
- 
- int main(){  
-   cin>>n>>m;  
- int i,j;  
-   f[0][0]=1;  
- for(i=1;i<=n;i++)  
- for(j=1;j<=m;j++)  
- if(i>=j)f[i][j]+=f[i-j][j]+f[i-1][j-1];  
-   cout<<f[n][m];  
- return 0;  
- }  



                                       1.2.3 划分型动态规划例题——《数字游戏》（NOIP2003普及组）




题目描述 Description



丁丁最近沉迷于一个数字游戏之中。这个游戏看似简单，但丁丁在研究了许多天之后却发觉原来在简单的规则下想要赢得这个游戏并不那么容易。游戏是这样的，在你面前有一圈整数（一共n个），你要按顺序将其分为m个部分，各部分内的数字相加，相加所得的m个结果对10取模后再相乘，最终得到一个数k。游戏的要求是使你所得的k最大或者最小。


例如，对于下面这圈数字（n=4，m=2）：


                                  2


                   4                           -1


                                 3


当要求最小值时，((2-1) mod 10)×((4+3) mod 10)=1×7=7，要求最大值时，为((2+4+3) mod 10)×(-1 mod 10)=9×9=81。特别值得注意的是，无论是负数还是正数，对10取模的结果均为非负值。


丁丁请你编写程序帮他赢得这个游戏。





输入描述 Input Description



输入文件第一行有两个整数，n（1≤n≤50）和m（1≤m≤9）。以下n行每行有个整数，其绝对值不大于104，按顺序给出圈中的数字，首尾相接。





输出描述 Output Description



输出文件有两行，各包含一个非负整数。第一行是你程序得到的最小值，第二行是最大值。





样例输入 Sample Input


4 2

4

3

-1

2





样例输出 Sample Output


7

81



【分析】

   又是一道环形DP套划分型DP套区间DP的题目....

  《拉两倍！》

  （1）阶段：划分

  （2）状态：f(i,j,k)表示将[i,j]这个区间划分为k块所能得到的最大值

  （3）由于要求最大和最小，所以跑两遍DP即可



**[cpp]**[view
 plain](http://blog.csdn.net/u012746396/article/details/40834627#)[copy](http://blog.csdn.net/u012746396/article/details/40834627#)

[print](http://blog.csdn.net/u012746396/article/details/40834627#)[?](http://blog.csdn.net/u012746396/article/details/40834627#)

- #include <iostream>
- #include <algorithm>
- #include <cstring>
- #include <cstdio>
- #include <cstdlib>
- #include <vector>
- #include <queue>
- #include <list>
- #include <deque>
- #include <string>
- usingnamespace std;  
- 
- constint MaxN=51;  
- constint MaxM=10;  
- constint oo=30001;  
- 
- int n,m,a[MaxN*2];//断环为链 
- int fmax[MaxN*2][MaxN*2][MaxM];//f(i,j,k)表示将环中i~j划分k段对10求模后的最大乘积 
- int fmin[MaxN*2][MaxN*2][MaxM];  
- int p,sum[MaxN*2][MaxN*2];  
- int MAX=-oo,MIN=oo;  
- 
- int main(){  
-   cin>>n>>m;  
- int i,j,k;  
- for(i=1;i<=n;i++){  
-     cin>>a[i];  
-     a[n+i]=a[i];  
-   }  
-   n<<=1;  
- for(i=1;i<=n;i++)  
- for(j=i;j<=n;j++){  
-         p=0;  
- for(k=i;k<=j;k++)  
-           p+=a[k];  
-         p=p%10;  
- if(p<0)p+=10;  
-         sum[i][j]=p;  
-     }  
- for(i=1;i<=n;i++)  
- for(j=1;j<=n;j++)  
- for(k=1;k<=m;k++)  
-       {  
-         fmax[i][j][k]=-oo;  
-         fmin[i][j][k]=oo;  
-       }  
- for(i=1;i<=n;i++)  
- for(j=i;j<=n;j++){  
-       fmax[i][j][1]=sum[i][j];  
-       fmin[i][j][1]=sum[i][j];  
-     }  
- for(i=1;i<=n;i++)  
- for(j=i;j<=n;j++)  
- for(k=2;k<=m;k++)  
- for(p=j-1;p>=i;p--)  
-           {  
-             fmin[i][j][k]=min(fmin[i][j][k],fmin[i][p][k-1]*sum[p+1][j]);  
-             fmax[i][j][k]=max(fmax[i][j][k],fmax[i][p][k-1]*sum[p+1][j]);  
-           }  
-   n>>=1;  
- for(i=1;i<=n;i++)  
-   {  
-     MAX=max(MAX,fmax[i][i+n-1][m]);  
-     MIN=min(MIN,fmin[i][i+n-1][m]);   
-   }  
-   cout<<MIN<<endl<<MAX;  
- return 0;  
- }  
- 





































