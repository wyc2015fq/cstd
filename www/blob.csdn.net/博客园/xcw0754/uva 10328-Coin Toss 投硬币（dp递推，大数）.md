# uva 10328 - Coin Toss 投硬币（dp递推，大数） - xcw0754 - 博客园
# [uva 10328 - Coin Toss 投硬币（dp递推，大数）](https://www.cnblogs.com/xcw0754/p/4469515.html)
题意:抛出n次硬币（有顺序），求至少k个以上的连续正面的情况的种数。
思路：转换成求抛n个硬币，至多k-1个连续的情况种数，用所有可能出现的情况种数减去至多k-1个的情况，就得到答案了。此题涉及大数加减。
分析：
（1）假设n=k，那么只有一种情况。
（2）假设n=k+1，那么有3种情况，包含k个的两种，k+1个的一种。
（3）假设k=1，那么只有无正面这一种的情况不能被考虑，其他都能算，那么就是（1<<n）-1种。（n个硬币有2^n种结果）
（4）其他情况考虑递推。先把问题的规模降低，最小就是1了，再逐渐增大，每次规模增加1，直到n为止。用dp[n+1]这么大的数组就够了，而**dp[n]表示从第1个硬币到第n个硬币中最多出现k-1个连续正面的情况种数**。到这步，可以直接将拿到的**k自减一，假设结果为t**。那么问题转成求抛第i个硬币时，它和它前面不能超过t个正面。
（5）基于上面第4步的分析。可以看出，在抛第t个之前（包括第t个），最多就出现t个正面，不可能超过抛的次数的。那么dp[0~k]可以一次性初始化为1<<i，也就是有这么多抛法其结果都不会超过t个正面啦。
（6）考虑在抛第k+1个的情况，顶多也就一种不合法，也就是全面都是正面，那么种数就是(1<<k+1)-1。
（7）考虑在抛大于第k+2的情况，不妨设为dp[k+2]=dp[k+1]*2，但是这里面包含了大于t个的情况，只可能出现这样的情况：第k+2个刚好抛到正面，如果其前面与其连续的刚好有t个正面，那么就会造成连续t+1个正面，非法！但是不可能出现多于t+2个连续的可能，如果多于t+2，那么假设当前为正面，其前面必须有t+1个连续，而dp[k+1]中不会记录有非法的情况，所以不考虑。那么出现与第k+2个刚好连起来是t+1个连续的情况会有多少种?(k+2)-t-1这个位置肯定是反面的，不然就会造成连续t+2个了，那么在k+2-t-2及其之前所有不超过t个连续的情况种数就是出现“与k相连t+1个正面”的情况种数，减去这个数量即可。
总之，对于i大于k+2的，dp[i]=dp[i-1]*2-dp[i-t-2]。推到dp[n]时，所有可能都出来了。用所有可能数减去此数：答案= (1<<n)-dp[n]。
```
1 #include <iostream>
  2 #include <vector>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <string>
  6 #include <algorithm>
  7 using namespace std;
  8 int n, k;
  9 long long a[70];
 10 vector<string> ans;
 11 vector<string> pre;
 12 
 13 bool comp(string num1,string num2)
 14 {
 15     int leng=num1.length(),i;
 16     for(i=0;i<leng;i++){if(num1[i]!='0')break;}
 17     num1=num1.substr(i,leng);
 18     if(num1.length()==0)num1="0";
 19 
 20     leng=num2.length();
 21     for(i=0;i<leng;i++){if(num2[i]!='0')break;}
 22     num2=num2.substr(i,leng);
 23     if(num2.length()==0)num2="0";
 24 
 25     if(num1.length()>num2.length())return true;
 26     else if(num1.length()==num2.length())
 27     {
 28         if(num1>=num2)return true;
 29         else return false;
 30     }
 31     else return false;
 32 }
 33 string _minus(string num1,string num2)
 34 {
 35     string result;
 36     if(comp(num2,num1)){string ss=num1;num1=num2;num2=ss;}
 37     reverse(num1.begin(),num1.end());
 38     reverse(num2.begin(),num2.end());
 39 
 40     result="";
 41 
 42     int i;
 43     for(i=0;i<int(num1.length())&&i<int(num2.length());i++)
 44     {
 45         char c=num1[i]-num2[i]+48;
 46         result=result+c;
 47     }
 48     if(i<int(num1.length()))
 49         for(;i<int(num1.length());i++)
 50             result=result+num1[i];
 51 
 52     int jiewei=0;
 53     for(i=0;i<int(result.length());i++)
 54     {
 55         int zhi=result[i]-48+jiewei;
 56         if(zhi<0)   {zhi=zhi+10;jiewei=-1;}
 57         else    jiewei=0;
 58         result[i]=(char)(zhi+48);
 59     }
 60 
 61     for(i=result.length()-1;i>=0;i--)
 62         if(result[i]!='0')
 63             break;
 64 
 65     result=result.substr(0,i+1);
 66     reverse(result.begin(),result.end());
 67     if(result.length()==0)result="0";
 68     return result;
 69 }
 70 string sum(string s1,string s2)
 71 {
 72     if(s1.length()<s2.length())
 73     {
 74         string temp=s1;
 75         s1=s2;
 76         s2=temp;
 77     }
 78     int i,j;
 79     for(i=s1.length()-1,j=s2.length()-1;i>=0;i--,j--)
 80     {
 81         s1[i]=char(s1[i]+(j>=0?s2[j]-'0':0));   //注意细节
 82         if(s1[i]-'0'>=10)
 83         {
 84             s1[i]=char((s1[i]-'0')%10+'0');
 85             if(i) s1[i-1]++;
 86             else s1='1'+s1;
 87         }
 88     }
 89     return s1;
 90 }
 91 
 92 void cal64()
 93 {
 94     ans[0]="1";
 95     for(int i=1; i<=k; i++)
 96         ans[i]=pre[i];
 97     string tmp= sum(ans[k], ans[k]);
 98     ans[k+1]=_minus(tmp,"1");
 99 
100     for(int i=k+2; i<=n; i++)
101     {
102         tmp=sum( ans[i-1], ans[i-1] );
103         ans[i]=_minus(tmp,ans[i-k-2]);
104     }
105     cout<<_minus(pre[n], ans[n])<<endl;
106 
107 }
108 
109 void cal63()
110 {
111     a[0]=1;
112     for(int i=1; i<=k; i++)      //小于k的情况，直接2^k
113         a[i]=a[i-1]+a[i-1];
114 
115     a[k+1]=a[k]+a[k]-1;
116     for(int i=k+2; i<=n; i++)
117         a[i] = a[i-1]-a[i-k-2]+a[i-1];
118 
119     cout<<((long long)1<<n)-a[n]<<endl;
120 }
121 void init()
122 {
123     string tmp;
124     ans.resize(103,tmp);
125     pre.resize(103,tmp);
126     pre[0]="1";
127     for(int i=1; i<103; i++)
128         pre[i]=sum(pre[i-1], pre[i-1]);
129 }
130 int main()
131 {
132     //freopen("input.txt","r",stdin);
133     init();
134     while(cin>>n>>k)
135     {
136 
137         if(n==k)
138         {
139             cout<<"1"<<endl;
140             continue;
141         }
142         if(n==k+1)
143         {
144             cout<<"3"<<endl;
145             continue;
146         }
147         if(k==1)
148         {
149             cout<<_minus(pre[n],"1")<<endl;
150             continue;
151         }
152         k--;
153         if(n<62)    //这里对抛62次以下的情况用longlong直接干掉。
154             cal63();
155         else    //大于62就用大数来算
156             cal64();
157 
158     }
159     return 0;
160 }
AC代码
```

