# acdream 1210 Chinese Girls' Amusement (打表找规律) - xcw0754 - 博客园
# [acdream 1210 Chinese Girls' Amusement (打表找规律)](https://www.cnblogs.com/xcw0754/p/4607564.html)
题意：有n个女孩围成一个圈从第1号女孩开始有一个球，可以往编号大的抛去（像传绣球一样绕着环来传），每次必须抛给左边第k个人，比如1号会抛给1+k号女孩。给出女孩的人数，如果他们都每个人都想要碰到球一次，那么这个k应该是多少（满足 1 ≤ K ≤ N/2 且 k必须尽量大）？   例如：n=7，那么1号开始拿球，抛球的顺序是 1, 4, 7, 3, 6, 2, 5, 1.  当球重新回到1女孩手中时，每个人刚好只玩了一次。注：这个数字相当大(3 ≤ N ≤ 102000) 
思路：
方法（1）：
暴力本地打表，发现n为奇数时，k=(n-1)/2即可。n为偶数时，规律如下图（左列是n，右列是k）：
![](https://images0.cnblogs.com/blog2015/641737/201506/291519493844014.jpg)
如果n为偶数，且n/2为偶数时，k=(n/2)-1。如果n为偶数，且n/2为奇数时，k=(n/2)-2。需要用到大数的除法和减法。
方法（2）：如果能够经过若干次传球且未开始循环，将球抛到第n个女孩那里去，那么肯定可以全部人玩一次，比如1 2 3 4 5 6 7，应该k=3，第1次传给4号，第2次就到达7号，且还没开始循环。那么7号肯定可以传给3号，因为1号传给4号，7号自然就传给3号，接着球又到了6号那里。接续循环下去，球肯定会重回1号手中，且大家都只玩一次。
那么从n/2开始，逐个递减试，看能不能n模k为1。
```
1 // 求模 N%(N/2) != 0 即是结果！
 2 // 奇数：(n-1)/2
 3 // 偶数： n/2 - 1
 4  
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <string>
 8  
 9 using namespace std;
10  
11 // 减一操作  strNum 正序存储 如 100000
12 void subOne(string &strNum)
13 {
14     if (strNum[strNum.length()-1] > '0')
15     {
16         strNum[strNum.length()-1]--;
17         return;
18     }
19  
20     // 可能需要逐位减一
21     for(int i=strNum.length()-1; i>=0; --i)
22     {
23         if (strNum[i] == '0')
24         {
25             strNum[i] = '9';
26         }
27         else
28         {
29             strNum[i]--;
30             break;
31         }
32     }
33  
34     if (strNum[0] == '0')
35         strNum.erase(0, 1); // 移除头位的0
36 }
37  
38 // strNum / 2
39 void divHalf(string &strNum)
40 {
41     int s = 0;
42     string num;
43     for(int i=0; i<strNum.length(); ++i)
44     {
45         s = s*10 + (strNum[i]-'0');
46         num += (s/2 + '0');
47         s %= 2;
48     }
49  
50     // 去掉前导0
51     int i = 0;
52     for(i=0; i<num.length(); ++i)
53     {
54         if (num[i] != '0')
55             break;
56     }
57     strNum = num.substr(i);
58 }
59  
60 int main(void)
61 {
62     //freopen("in.txt", "r", stdin);
63  
64     string strNum;
65     while(cin>>strNum)
66     {
67         // 末位数字判断奇偶性
68         int k = strNum[strNum.length()-1]-'0';
69         if (k%2==0)
70         {
71             // 偶数： n/2 - 1
72             divHalf(strNum);
73             subOne(strNum);
74  
75             // 这里需要特殊处理
76             while(1)
77             {
78                 k = strNum[strNum.length()-1]-'0';
79                 if (k%2 != 0)
80                     break;
81  
82                 subOne(strNum);
83             }
84         }
85         else
86         {
87             // 奇数：(n-1)/2
88             subOne(strNum);
89             divHalf(strNum);
90         }
91         cout<<strNum<<endl;
92     }
93  
94     return 0;
95 }
别人的AC代码
```
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 using namespace std;
  4 const int N=2005;
  5 int a, b;
  6 int has[N];
  7 char s[N];
  8 char ans[N];
  9  
 10 void div(char * src,int n,char *dest)
 11 {
 12     int len = strlen(src),i,k,t=0,s=0;
 13     bool flag = true;    //商是否有了第一个有效位，防止商首部一直出现0
 14     for(i=0,k=0; i<len; i++)
 15     {
 16         t = s*10+(src[i]-48);    //新余数
 17         if(t/n>0 || t==0)        //余数为0要修改商
 18         {
 19             dest[k++] = t/n+48,s = t%n,flag = false;
 20         }
 21        else                    //不够除，修改余数
 22         {
 23             s = t;
 24             if(!flag)            //商已经有有效位了，补零
 25                 dest[k++] = '0';
 26         }
 27     }
 28     dest[k]='\0';
 29 }
 30  
 31  
 32 bool comp(string num1,string num2)
 33 {
 34     int leng=num1.length(),i;
 35     for(i=0;i<leng;i++){if(num1[i]!='0')break;}
 36     num1=num1.substr(i,leng);
 37     if(num1.length()==0)num1="0";
 38  
 39     leng=num2.length();
 40     for(i=0;i<leng;i++){if(num2[i]!='0')break;}
 41     num2=num2.substr(i,leng);
 42     if(num2.length()==0)num2="0";
 43  
 44     if(num1.length()>num2.length())return true;
 45     else if(num1.length()==num2.length())
 46     {
 47         if(num1>=num2)return true;
 48         else return false;
 49     }
 50     else return false;
 51 }
 52 void _minus(string num1,string num2,string &result)
 53 {
 54  
 55     if(comp(num2,num1)){string ss=num1;num1=num2;num2=ss;}
 56     reverse(num1.begin(),num1.end());
 57     reverse(num2.begin(),num2.end());
 58  
 59     result="";
 60  
 61     int i;
 62     for(i=0;i<int(num1.length())&&i<int(num2.length());i++)
 63     {
 64         char c=num1[i]-num2[i]+48;
 65         result=result+c;
 66     }
 67     if(i<int(num1.length()))
 68         for(;i<int(num1.length());i++)
 69             result=result+num1[i];
 70  
 71     int jiewei=0;
 72     for(i=0;i<int(result.length());i++)
 73     {
 74         int zhi=result[i]-48+jiewei;
 75         if(zhi<0)   {zhi=zhi+10;jiewei=-1;}
 76         else    jiewei=0;
 77         result[i]=(char)(zhi+48);
 78     }
 79  
 80     for(i=result.length()-1;i>=0;i--)
 81         if(result[i]!='0')
 82             break;
 83  
 84     result=result.substr(0,i+1);
 85     reverse(result.begin(),result.end());
 86     if(result.length()==0)result="0";
 87 }
 88 //****上面不用看了，大数模版
 89  
 90  
 91 int main()
 92 {
 93     //freopen("input.txt", "r", stdin);
 94  
 95     while(gets(s))
 96     {
 97         int len=strlen(s);
 98         if(((s[len-1]-'0')&1)==1) //奇数
 99         {
100             div(s,2,ans);
101             puts(ans);
102         }
103         else
104         {
105             div(s,2,ans);   //先减半
106             len=strlen(ans);
107             if(((ans[len-1]-'0')&1)==0 )    //减半后为偶数
108             {
109                 string tmp(ans);
110                 string res="";
111                 _minus(tmp,"1",res);//减1
112                 cout<<res<<endl;
113             }
114             else//减半后为奇数
115             {
116                 string tmp(ans);
117                 string res="";
118                 _minus(tmp,"2",res);//减2
119                 cout<<res<<endl;
120             }
121         }
122     } 
123     return 0;
124 }
AC代码
```
![](file:///C:\Users\xiao\Documents\Tencent Files\295327631\Image\Group\D{]E2H@U~KC1{$XLA@@G]87.jpg)

