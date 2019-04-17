# PAT Rational Sum - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### Rational Sum (20)


时间限制 1000 ms 内存限制 65536 KB 代码长度限制 100 KB 判断程序 Standard (来自 [小小](https://www.nowcoder.com/profile/59))



### **题目描述**
Given N rational numbers in the form "numerator/denominator", you are supposed to calculate their sum.

### **输入描述:**
Each input file contains one test case. Each case starts with a positive integer N (<=100), followed in the next line N rational numbers "a1/b1 a2/b2 ..." where all the numerators and denominators are in the range of "long int".  If there is a negative number, then the sign must appear in front of the numerator.


### **输出描述:**
For each test case, output the sum in the simplest form "integer numerator/denominator" where "integer" is the integer part of the sum, "numerator" < "denominator", and the numerator and the denominator have no common factor.  You must output only the fractional part if the integer part is 0.

### **输入例子:**
5
2/5 4/15 1/30 -2/60 8/3

### **输出例子:**
3 1/3


```
#include<iostream>
#include<cstring>
#include<string>
using namespace std;

long long gcd(long long x,long long y){
    return y?gcd(y,x%y):x;
}

long long lcm(long long x,long long y){
    return  x/gcd(x,y)*y;
}

char s[111];

int main(){
    long long a=0,b=1; //a / b
    int n;
    cin >> n;
    while(n--){
        cin >> s;
        char *t = strstr(s,"/");
        if(t) *t = ' ';
         
         // a/b + c/d
        long long c,d;
        sscanf(s,"%lld %lld",&c,&d);
        long long aa = a*d + b*c;
        long long bb = b*d;
        long long g = gcd((aa<0)?(-aa):aa,bb);
        a = aa/g;
        b = bb/g;
    }
    long long x = a/b,y = a%b;
    if(y == 0) printf("%lld\n",x);
    else{
        if(x) printf("%lld ",x);
        printf("%lld/%lld\n",y,b);
    }
    return 0;
}
```















