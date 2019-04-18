# HDU 1023 Train Problem II (卡特兰数，经典) - xcw0754 - 博客园
# [HDU 1023 Train Problem II (卡特兰数，经典)](https://www.cnblogs.com/xcw0754/p/4559690.html)
题意：
　　给出一个数字n，假设火车从1~n的顺序分别进站，求有多少种出站序列。
思路：
　　卡特兰数的经典例子。n<101，用递推式解决。需要使用到大数。n=100时大概有200位以下。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=101;
 4 vector<string>  vect;
 5 void _mult(string num1, string num2, string &result )
 6 {
 7     reverse(num1.begin(),num1.end());    //反转
 8     reverse(num2.begin(),num2.end());
 9     result="";
10     int i, j, re_int[200];    //********这里的150是位数，根据需要可以增大或减小*********
11     memset(re_int, 0, sizeof(re_int));
12     for(i=0; i<num1.length(); i++)    //两串作乘法，结果存放于re_int数组中， 最多可达150位！
13         for(j=0; j<num2.length(); j++)
14             re_int[i+j] += ((num1[i]-48) * (num2[j]-48));
15     int jinwei=0, zhi;
16     for(i=0; i<num1.length()+num2.length(); i++)    //单独处理进位问题，上一步中的数组每个元素都有可能超过10的，所以没处理进位
17     {
18         zhi = re_int[i]+jinwei;
19         re_int[i] = zhi%10;
20         jinwei = zhi/10;
21     }
22     for(i=num1.length()+num2.length()-1; i>=0; i--)    //将i打个标记，数组re_int的前面部分可能全0，要去掉
23         if(re_int[i]!=0)    break;
24     for(;i>=0;i--)    //将整型数组转成字符串
25         result = result+(char)(re_int[i]+48);
26     if(result=="")    //若结果还是空，乘法的结果是0？
27         result="0";
28 }
29 void div(char * src,int n,char *dest)
30 {
31     int len = strlen(src),i,k,t=0,s=0;
32     bool flag = true;    //商是否有了第一个有效位，防止商首部一直出现0
33     for(i=0,k=0; i<len; i++)
34     {
35         t = s*10+(src[i]-48);    //新余数
36         if(t/n>0 || t==0)        //余数为0要修改商
37         {
38             dest[k++] = t/n+48,s = t%n,flag = false;
39         }
40         else                    //不够除，修改余数
41         {
42             s = t;
43             if(!flag)            //商已经有有效位了，补零
44                 dest[k++] = '0';
45         }
46     }
47     dest[k]='\0';
48 }
49 
50 void precal()
51 {
52     string s="1";
53     vect.push_back(s);
54     vect.push_back(s);
55     char c[200], dest[200];
56     for(int i=2; i<101; i++)
57     {
58         string q1="",res;
59         int a=4*i-2; //第一个括号
60         while(a)
61         {
62             q1+=(a%10+'0');
63             a/=10;
64         }
65         reverse(q1.begin(),q1.end());
66         _mult(q1,vect[i-1],res);    //乘法
67         strcpy(c,res.c_str());
68         div(c,i+1,dest);            //除法
69         s=dest;
70         vect.push_back(dest);
71     }
72 }
73 int main()
74 {
75     //freopen("e://input.txt", "r", stdin);
76     int n;
77     precal();
78     while(~scanf("%d", &n))
79         cout<<vect[n]<<endl;
80     return 0;
81 }
AC代码
```

