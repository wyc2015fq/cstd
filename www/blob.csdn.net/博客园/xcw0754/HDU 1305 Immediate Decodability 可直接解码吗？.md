# HDU 1305 Immediate Decodability 可直接解码吗？ - xcw0754 - 博客园
# [HDU 1305 Immediate Decodability 可直接解码吗？](https://www.cnblogs.com/xcw0754/p/4433620.html)
题意：一个码如果是另一个码的前缀，则 is not immediately decodable，不可直接解码，也就是给一串二进制数字给你，你不能对其解码，因解码出来可能有多种情况。
思路：将每个码按长度从短到长排序，逐个与其后面的码比较，若前面相同位数完全一样了，那就可以输出了。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string>
 4 #include <map>
 5 #include <cstring>
 6 #include <algorithm>
 7 using namespace std;
 8 vector< vector<int> > a;
 9 bool cmp(vector<int> a, vector<int> b){return a.size()<b.size()? true: false;}
10 bool isid()
11 {
12     bool issub=false;
13     int t, j, k;
14     sort(a.begin(), a.end(), cmp);  //按照元素的多少从少到多排序
15     /*test
16     for(t=0; t<a.size(); t++)
17     {
18         for(j=0; j<a[t].size(); j++)
19         {
20             cout<<a[t][j];
21         }
22         cout<<endl;
23     }*/
24     for( t=0; t<a.size(); t++ )     //从小到大，一个个跟后面的比较前a[t].size()个是不是重叠，
25     {
26         for(j=t+1; j<a.size(); j++)
27         {
28             issub=true;
29             for(k=0; k<a[t].size(); k++)
30             {
31                 if( a[t][k]!=a[j][k] )
32                 {
33                     issub = false;  //只要有一个不同，就不是真子串
34                     break;
35                 }
36             }
37             if( issub==true )   //有一个串是另一个串的前缀
38                 return false;
39         }
40     }
41     return true;
42 }
43 
44 int main()
45 {
46     //freopen("e:input.txt","r",stdin);
47     vector<int> tmp;
48     char c;
49     int i=0, t, j, k, cnt=0;
50     while( (c=getchar())!=EOF )
51     {
52         if( c=='9' )        //一个例子结束
53         {
54 
55             if( isid()==true )    //处理 a
56                 cout<<"Set "<< ++cnt<<" is immediately decodable"<<endl;
57             else
58                 cout<<"Set "<< ++cnt<<" is not immediately decodable"<<endl;
59             a.clear();      //例子结束，清理工作
60         }
61         else if( c=='\n' )  //只是换行
62         {
63             if(tmp.empty()==0)  //提交并清空
64             {
65                 a.push_back(tmp);
66                 tmp.clear();
67             }
68         }
69         else    //c是0和1的
70             tmp.push_back(c-'0') ;
71     }
72     return 0;
73 }
Immediate Decodability
```

