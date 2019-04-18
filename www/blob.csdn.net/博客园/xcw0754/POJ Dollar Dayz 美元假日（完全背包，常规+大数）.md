# POJ Dollar Dayz 美元假日（完全背包，常规+大数） - xcw0754 - 博客园
# [POJ Dollar Dayz 美元假日（完全背包，常规+大数）](https://www.cnblogs.com/xcw0754/p/4489556.html)
题意：给出整数n和k，n代表拥有的钱数量，k代表有k种工具，其价钱分别为1~k。求n元能有多少种购买的方案。
思路：k最大有100，数量过大，要用大数。其他的基本和完全背包一样。
```
1 #include <iostream>
  2 #include <cstdio>
  3 #include <cstring>
  4 #include <algorithm>
  5 using namespace std;
  6 const int B=1005, N=150;
  7 int coin[100], n, k;
  8 
  9 struct bign {       //请先设置常数N作为缓冲区和最大整数的长度。
 10     int len, sex;
 11     int s[N];
 12 
 13     bign() {    //创建一个新的大数
 14         this -> len = 1;
 15         this -> sex = 0;
 16         memset(s, 0, sizeof(s));
 17     }
 18     bign (int number) {*this = number;}     //创建一个大数，用整型初始化
 19     bign (const char* number) {*this = number;} //创建一个大数，用字符串初始化
 20 
 21     void operator = (const char *number) {  //重载等号，等于一个字符串
 22         int begin = 0;
 23         len = 0;
 24         sex = 1;
 25         if (number[begin] == '-') {
 26             sex = -1;
 27             begin++;
 28         }
 29         else if (number[begin] == '+')
 30             begin++;
 31 
 32         for (int j = begin; number[j]; j++)
 33             s[len++] = number[j] - '0';
 34     }
 35 
 36     bign operator = (int number) {          //重载等号，等于一个整型
 37         char str[N];        //注意这里的N，要设一个缓冲区在外面供使用。
 38         sprintf(str, "%d", number);
 39         *this = str;
 40         return *this;
 41     }
 42 
 43     bign change(bign cur) {
 44         bign now;
 45         now = cur;
 46         for (int i = 0; i < cur.len; i++)
 47             now.s[i] = cur.s[cur.len - i - 1];
 48         return now;
 49     }
 50 
 51     void delZore() {    // 删除前导0.
 52         bign now = change(*this);
 53         while (now.s[now.len - 1] == 0 && now.len > 1) {
 54             now.len--;
 55         }
 56         *this = change(now);
 57     }
 58 
 59     void put() {    // 输出数值。
 60         delZore();
 61         if (sex < 0 && (len != 1 || s[0] != 0))
 62             cout << "-";
 63         for (int i = 0; i < len; i++)
 64             cout << s[i];
 65     }
 66 
 67     bign operator + (const bign &cur){      //只能是大数operator大数
 68         bign sum, a, b;
 69         sum.len = 0;
 70         a = a.change(*this);
 71         b = b.change(cur);
 72 
 73         for (int i = 0, g = 0; g || i < a.len || i < b.len; i++){
 74             int x = g;
 75             if (i < a.len) x += a.s[i];
 76             if (i < b.len) x += b.s[i];
 77             sum.s[sum.len++] = x % 10;
 78             g = x / 10;
 79         }
 80         return sum.change(sum);
 81     }
 82 
 83     bign operator - (const bign &cur) {     //只能是大数operator大数
 84         bign sum, a, b;
 85         sum.len = len;
 86         a = a.change(*this);
 87         b = b.change(cur);
 88 
 89         for (int i = 0; i < b.len; i++) {
 90             sum.s[i] = a.s[i] - b.s[i] + sum.s[i];
 91             if (sum.s[i] < 0) {
 92                 sum.s[i] += 10;
 93                 sum.s[i + 1]--;
 94             }
 95         }
 96         for (int i = b.len; i < a.len; i++) {
 97             sum.s[i] += a.s[i];
 98             if (sum.s[i] < 0) {
 99                 sum.s[i] += 10;
100                 sum.s[i + 1]--;
101             }
102         }
103         return sum.change(sum);
104     }
105 };
106 
107 void cal()
108 {
109     dp[0]=1;
110     for(int i=1; i<=n; i++)  dp[i]=0; //初始化
111     int tmp=min(k,n);   //k>n的部分买不起
112     for(int i=0; i<tmp; i++)
113         for(int j=0; j+coin[i]<=n; j++)
114             dp[j+coin[i]] = dp[j+coin[i]] + dp[j];
115 
116     dp[n].put();
117 }
118 
119 int main() {
120     //freopen("input.txt", "r", stdin);
121     for(int i=0; i<100; i++)    coin[i]=i+1;    //初始化
122     while(~scanf("%d%d",&n,&k))
123         cal();
124     return 0;
125 }
AC代码
```

