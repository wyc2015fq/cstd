# hihoCoder #1015 : KMP算法 - xcw0754 - 博客园
# [hihoCoder #1015 : KMP算法](https://www.cnblogs.com/xcw0754/p/4450133.html)
|模式串：|b a b a b b|
|----|----|
|NEXT：|-1 0 0 1 2 3|
　　如果在模式串的s[i]处失配的情况下，将跳到s[next[i]]处继续匹配。按下标为0开始，例子中的s[2]=b，而next[2]=0，如果原串str[x]匹配到s[2]失配了，那么模式串应该变成这个字符s[next[2]]=s[0]='b'，继续和str[x]匹配。看下例子：
|原串(p=5)：|babab | **a**bcbababababb|
|----|----|
|模式串(q=5)：|babab | **b**|
在s[5]处失配，指针q应该置为q=next[5]=3，即s[3]处开始继续匹配str[p]=str[5]='a'。即往前跳2个字符。
|原串(p=5)：|babab | abcbababababb|
|----|----|
|模式串(q=3)：|bab | abb|
这么说吧：失配处原串p位置不变，q=next[q]，就继续开心地匹配下去了。
原理：假设一个模式串(用x表示任意字符)为xxxxxxxxxxxxxxxx，为了减少多余匹配工作量，如果有满足条件s[0~3]和s[t~y]是匹配的（必定等长），形如红色部分 xxxxxxxxxxxxxxxx那么在s[y+1]处失配时，s[t~y]这部分和s[0~k]相同，而s[t~y]又和str[失配处]前面一段一样，那就刚好节省匹配次数好了。即从蓝色对齐到绿色处，继续匹配。
思路：里面讲得已经够清楚了，但是有点繁琐。别人的代码真是发挥到了极致！
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <string>
 4 #include <cstring>
 5 #include <algorithm>
 6 using namespace std;
 7 void get_next(string &m, int *next)     //next数组要保证比m要大
 8 {
 9     next[0]=-1;
10     int i=0, j=-1;
11     while(i<m.size())
12     {
13         if(j==-1 || m[i]==m[j])      //j在原串上， i在模式串上
14             next[++i] = ++j;
15         else
16             j = next[j];
17     }
18 }
19 
20 int kmp(string &y, string &m,int *next )        //找到则返回索引
21 {
22     if(m.size()>y.size())   return 0;           //原串比模式串还小
23     int i=0, j=0, cnt=0;
24     while(i<y.size())
25     {
26         if(j==-1||y[i]==m[j])
27         {
28             i++;
29             j++;
30         }
31         else
32             j=next[j];
33         if(j==m.size()) cnt++;
34     }
35     return cnt;
36 }
37 
38 int main()
39 {
40     //freopen("input.txt", "r", stdin);
41     string m, y;
42     int next[10005];
43     int t;
44     cin>>t;
45     while(t--)
46     {
47         cin>>m>>y;
48         get_next(m, next);
49         cout<<kmp(y, m, next)<<endl;
50     }
51     return 0;
52 }
KMP
```

