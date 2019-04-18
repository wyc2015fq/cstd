# C++ string中的find()函数 - 心纯净，行致远 - CSDN博客





2018年11月24日 23:21:37[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：152








**1. string中find()返回值是字母在母串中的位置（下标记录），如果没有找到，那么会返回一个特别的标记npos。（返回值可以看成是一个int型的数）**

```cpp
#include<cstring>
#include<cstdio>
#include<iostream>
using namespace std;
int main()
{
    ////find函数返回类型 size_type
    string s("1a2b3c4d5e6f7jkg8h9i1a2b3c4d5e6f7g8ha9i");
    string flag;
    string::size_type position;
    //find 函数 返回jk 在s 中的下标位置
    position = s.find("jk");
    if (position != s.npos)  //如果没找到，返回一个特别的标志c++中用npos表示，我这里npos取值是4294967295，
    {
        printf("position is : %d\n" ,position);
    }
    else
    {
        printf("Not found the flag\n");
    }
}
/*
position is : 13

--------------------------------
Process exited after 0.8539 seconds with return value 0
请按任意键继续. . .
*/
```

**2.返回子串出现在母串中的首次出现的位置，和最后一次出现的位置。**


```cpp
flag = "c";
position = s.find_first_of(flag);
printf("s.find_first_of(flag) is :%d\n",position);
position = s.find_last_of(flag);
printf("s.find_last_of(flag) is :%d\n",position);
```

![](https://images2018.cnblogs.com/blog/1358881/201808/1358881-20180806112334612-1756679922.png)

**3.查找某一给定位置后的子串的位置**

```cpp
//从字符串s 下标5开始，查找字符串b ,返回b 在s 中的下标 
position=s.find("b",5);
cout<<"s.find(b,5) is : "<<position<<endl;
```

![](https://images2018.cnblogs.com/blog/1358881/201808/1358881-20180806112652146-1439323605.png)



4.查找所有子串在母串中出现的位置

```cpp
//查找s 中flag 出现的所有位置。 
flag="a"; 
position=0; 
int i=1; 
while((position=s.find(flag,position))!=string::npos) { 
cout<<"position "<<i<<" : "<<position<<endl; 
position++; 
i++;
 }
```

![](https://images2018.cnblogs.com/blog/1358881/201808/1358881-20180806113411182-2033985085.png)



5.反向查找子串在母串中出现的位置，通常我们可以这样来使用，当正向查找与反向查找得到的位置不相同说明子串不唯一。

```cpp
//反向查找，flag 在s 中最后出现的位置 
flag="3"; 
position=s.rfind (flag); 
printf("s.rfind (flag) :%d\n",position);
```

**例题：**

**1.**给出一个字符串，串中会出现有人名，找到一个只有一个人名的字符串。

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 vector<string> s;
 4 int main()
 5 {
 6     s.push_back("Danil");
 7     s.push_back("Olya");
 8     s.push_back("Slava");
 9     s.push_back("Ann");
10     s.push_back("Nikita");///建立动态数组
11     string a;
12     cin>>a;
13     int res = 0;
14     for(int i = 0; i < 5; i++)
15     {
16         if(a.find(s[i]) != a.npos)
17         {
18             res++;
19             if(a.rfind(s[i]) != a.find(s[i]))///一个字符中出现多个一样的名字
20             {
21                 res++;
22             }
23         }
24     }
25     if(res == 1) 
26     {
27         cout<<"YES"<<endl;
28     }
29     else 
30     {
31         cout<<"NO"<<endl;
32     }
33     return 0;
34 }
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)





2.你有n个字符串。 每个字符串由小写英文字母组成。 重新排序给定的字符串，使得对于每个字符串，在它之前的所有字符串都是它的子串。

**[https://www.cnblogs.com/wkfvawl/p/9229758.html](https://www.cnblogs.com/wkfvawl/p/9229758.html)**

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```
1 #include<string>
 2 #include<cstdio>
 3 #include<algorithm>
 4 #include<iostream>
 5 using namespace std;
 6 bool cmp(string a, string b)
 7 {
 8     if (a.length() == b.length())
 9         return a < b;
10     return a.length() < b.length();
11 }
12 int main()
13 {
14     int n;
15     string s[111];
16     scanf("%d", &n);
17     for (int i = 0; i < n; i++)
18     {
19         cin >> s[i];
20     }
21     sort(s, s + n, cmp);
22     int flag = 1;
23     for (int i = 1; i < n; i++)
24     {
25         if (s[i].find(s[i-1]) == string::npos)
26         {
27             flag = 0;
28             break;
29         }
30     }
31     if (flag)
32     {
33         cout << "YES" << endl;
34         for (int i = 0; i < n; i++)
35         {
36             cout << s[i] << endl;
37         }
38     }
39     else
40     {
41         cout << "NO" << endl;
42     }
43     return 0;
44 }
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)



 3.查询区间内子串在母串中的个数。

**[https://www.cnblogs.com/wkfvawl/p/9452869.html](https://www.cnblogs.com/wkfvawl/p/9452869.html)**



![复制代码](https://common.cnblogs.com/images/copycode.gif)

```
1 #include<cstdio>
 2 #include<cstring>
 3 #include<string>
 4 #include<iostream>
 5 #include<algorithm>
 6 using namespace std;
 7 int main()
 8 {
 9     int n,m,q,i,j,l,r,len;
10     int counts;
11     int vis[10010];
12     string s1,s2;
13     cin>>n>>m>>q;
14     cin>>s1>>s2;
15     len=s2.size();
16     memset(vis,0,sizeof(vis));
17     string::size_type pos=0;
18     while((pos=s1.find(s2,pos))!=string::npos)
19     {
20         vis[pos+1]=pos+1;
21         pos++;
22     }
23     for(i=1;i<=q;i++)
24     {
25         counts=0;
26         scanf("%d%d",&l,&r);
27         for(j=l;j<=r;j++)
28         {
29             if(vis[j]!=0&&vis[j]+len-1<=r)
30             {
31                counts++;
32             }
33         }
34         printf("%d\n",counts);
35     }
36     return 0;
37 }
```



