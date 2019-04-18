# acdream 1683 村民的怪癖(KMP，经典变形) - xcw0754 - 博客园
# [acdream 1683 村民的怪癖(KMP，经典变形)](https://www.cnblogs.com/xcw0754/p/4572778.html)
#### Problem Description
娜娜费劲九牛二虎之力终于把糖果吃完了(说好的吃不完呢？骗人，口亨~)，于是，缘溪行，忘路之远近。忽逢桃花林，夹岸数百步，中无杂树，芳草鲜美，落英缤纷，娜娜甚异之。复前行，欲穷其林。林尽水源，便得一山，山有小口，仿佛若有光。便舍船，从口入。初极狭，才通人。复行数十步，豁然开朗。土地平旷，屋舍俨然，有良田美池桑竹之属。阡陌交通，鸡犬相闻。其中往来种作，男女衣着，悉如外人。黄发垂髫，并怡然自乐。（摘自《桃花源记》）
娜娜与村民交流了好久才发现这里的人们给孩子的命名方式很奇怪，首先村民们的名字都是用专门的符号来记录，正好是26个符号，于是娜娜就把它们替换成‘a’~‘z’，然后首先把爸爸的名字作为孩子的姓，妈妈的名字作为孩子的名。这时候肯定有人会问，不是独生子女怎么办？很简单~取拼接好的名字的前缀与后缀相同的部分从短到长依次作为孩子的姓名，啥，不够？那就不许你再生孩子！
不过由于桃花村民与世隔绝太久了，以致于他们总是无法正确判断一对夫妻最多能生多少个孩子，于是就把这个任务交给你了。
P.S. 若用S[1,n]表示一个长度为n的字符串，那么S[1,i](1<=i<=n)表示S的一个前缀，S[j,n](1<=j<=n)表示S的一个后缀。具体看样例解释
#### Input
多组数据，首先是一个正整数t(t<=20)，表示数据的组数
对于每一组数据，包含两个只由'a'~'z'组成的不含空格的字符串S1，S2，分别代表丈夫的姓名以及妻子的姓名。(1<=|S1|,|S2|<=100000)
#### Output
对于每组数据，输出一个整数，表示这对夫妻最多可以生育多少个孩子。
#### Sample Input
2
ababc ababa
aaaaa aaa
#### Sample Output
3
8
#### Hint
对于样例1，把丈夫和妻子的姓名拼接在一起是ababcababa，可以作为孩子的姓名的是a、aba、ababcababa，故最多生育3个孩子
对于样例2，把丈夫和妻子的姓名拼接在一起是aaaaaaaa，可以作为孩子的姓名的是a、aa、aaa、aaaa、aaaaa、aaaaaa、aaaaaaa、aaaaaaaa，故最多生育8个孩子
题意：
　　给两个串a和b，合并他们成为1个串s，即s=a+b。如果串s的前缀与后缀匹配了，只要长度不同，都算可以算作一个独立的匹配，问有多少这样的匹配（注意前缀与后缀是可以重叠的）？
　　假设s="abcdefg"，等长前缀和后缀有：len=7的abcdefg和abcdefg，len=6的abcdef和bcdefg，len=5的abcde和cdefg.....直到1个的a和g。
思路：
　　KMP的经典变形，这里仅有一个模式串而已，没原串。只需要对模式串求next数组就可以得到结果了，时间是O(n)。具体看下例：
假如有两串：ababc ababa
合并后变为：ababcababa
求next数组之后变为：
第1  2  3  4  5  6  7  8  9  10个
   0  0  1  2  0  1  2  3  4  3
   a  b  a   b  c  a  b  a   b  a
找最长的合法串：第10个字符为a，而next[10]表示s[10-3+1,10]等同于s[1,3]这两个小串是相同的，也就是一个合法的名字，而且该名字是最长的（也就是长度9，因为10个的话就是自身了，毋庸置疑）。如下两个红色串：
ababcababa
接下来找次长的合法串：第next[10]个（即第3个，是a）的next应该是next[3]，即1。也就是说s[1]=s[3]。而这个串在上一步才提到，是等于尾串的！看上面尾部红色的字符，aba=前部的aba，而前部aba中的后部a又等于前部的a。这说明了又是一个合法的名字。
ababcababa
接下来到next[1]=0了，也就没有再多可以匹配的了， 仅剩1个字符无法跟别人匹配。
这是模式串next数组本身的特点。细心点就可以发现。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=1005;
 5 
 6 void get_next(string &m, vector<int> &next) //求next数组
 7 {
 8     next.push_back(-1); //一开始是-1
 9     int i=0, j=-1;
10     while(i<m.size())
11     {
12         if(j==-1 || m[i]==m[j])      //j在原串上， i在模式串上
13         {
14             next.push_back(++j);
15             ++i;
16         }
17         else    j = next[j];
18     }
19 }
20 
21 int cal(string &m)
22 {
23     vector<int> next;
24     get_next(m, next);
25     int len=m.size();
26     if(next[len]==len-1)  return len;  //全都一样的
27     int i=next[len], cnt=0;
28     while(i>0)      //只要next[i]>0就是一个匹配
29     {
30         cnt++;
31         i=next[i];//注意串s是以0开头的，而next是以1开头的。
32     }
33     return cnt+1;   //本身就是一个符合条件的串
34 }
35 
36 int main()
37 {
38     freopen("e://input.txt", "r", stdin);
39     int t;
40     cin>>t;
41     string s1,s2;
42     while(t--)
43     {
44         cin>>s1>>s2;
45         s1+=s2;
46         printf("%d\n",cal(s1));
47     }
48     return 0;
49 }
AC代码
```
