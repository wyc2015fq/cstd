# NYNU_省赛选拔题(10) - Vmetrio - 博客园







# [NYNU_省赛选拔题(10)](https://www.cnblogs.com/wangmengmeng/p/5271893.html)





## 题目描述

Severe acute respiratory syndrome (SARS), an atypical pneumonia of unknown aetiology, was recognized as a global threat in mid-March 2003. To minimize transmission to others, the best strategy is to separate the suspects from others. 
In the Not-Spreading-Your-Sickness University (NSYSU), there are many student groups. Students in the same group intercommunicate with each other frequently, and a student may join several groups. To prevent the possible transmissions of SARS, the NSYSU collects the member lists of all student groups, and makes the following rule in their standard operation procedure (SOP). 
Once a member in a group is a suspect, all members in the group are suspects. 
However, they find that it is not easy to identify all the suspects when a student is recognized as a suspect. Your job is to write a program which finds all the suspects.

## 输入


The input file contains several cases. Each test case begins with two integers n and m in a line, where n is the number of students, and m is the number of groups. You may assume that 0 < n <= 30000 and 0 <= m <= 500. Every student is numbered by a unique integer between 0 and n−1, and initially student 0 is recognized as a suspect in all the cases. This line is followed by m member lists of the groups, one line per group. Each line begins with an integer k by itself representing the number of members in the group. Following the number of members, there are k integers representing the students in this group. All the integers in a line are separated by at least one space. 

A case with n = 0 and m = 0 indicates the end of the input, and need not be processed.








## 输出


For each case, output the number of suspects in one line.




## 样例输入
100 4
2 1 2
5 10 13 11 12 14
2 0 1
2 99 2
200 2
1 5
5 1 2 3 4 5
1 0
0 0

## 样例输出
4
1
1




```
1 #include <iostream>
 2 #include <algorithm>
 3 #include <cstring>
 4 #include <string>
 5 using namespace std;
 6 int a[30010];
 7 void cmp()
 8  {
 9     int i;
10     for(i=0;i<30005;i++)
11        {
12         a[i]=i;
13        } 
14  }
15  int root(int x)
16  {
17     while(a[x]!=x)
18         x=a[x];
19     return x;
20  }
21  void build(int x,int y)
22  {
23     int i,j;
24     i=root(x);
25     j=root(y);
26     if(i!=j)
27     {
28         if(i==0)
29            a[j]=i;
30         else a[i]=j;
31     }
32  }
33  int main()
34  {
35     int n,m,k,p,q,i,j,first,next;
36     while(cin>>n>>m)
37     {  if(n==0&&m==0) break;
38         cmp();
39         int ii=m; 
40         while(m--)
41         {
42             cin>>k>>first;
43             for(i=1;i<k;i++)
44                 {
45                     cin>>next;
46                     build(first,next);
47                 }   
48         }
49         q=0;
50         if(ii==0) cout<<"1"<<endl;
51         else
52         {
53            for(i=0;i<n;i++)
54            {
55               if(!root(a[i]))q++;
56            }
57            cout<<q<<endl;   
58         }
59     }
60     return 0;
61  }
```














