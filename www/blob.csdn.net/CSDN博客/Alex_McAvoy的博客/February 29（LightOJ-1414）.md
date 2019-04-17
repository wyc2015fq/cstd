# February 29（LightOJ-1414） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:25:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[LightOJ																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

It is 2012, and it's a leap year. So there is a "February 29" in this year, which is called leap day. Interesting thing is the infant who will born in this February 29, will get his/her birthday again in 2016, which is another leap year. So February 29 only exists in leap years. Does leap year comes in every 4 years? Years that are divisible by 4 are leap years, but years that are divisible by 100 are not leap years, unless they are divisible by 400 in which case they are leap years.

In this problem, you will be given two different date. You have to find the number of leap days in between them.

# Input

Input starts with an integer T (≤ 550), denoting the number of test cases.

Each of the test cases will have two lines. First line represents the first date and second line represents the second date. Note that, the second date will not represent a date which arrives earlier than the first date. The dates will be in this format - "month day, year", See sample input for exact format. You are guaranteed that dates will be valid and the year will be in between 2 * 103 to 2 * 109. For your convenience, the month list and the number of days per months are given below. You can assume that all the given dates will be a valid date.

# Output

For each case, print the case number and the number of leap days in between two given dates (inclusive).

# Examples

**Input**

4

January 12, 2012

March 19, 2012

August 12, 2899

August 12, 2901

August 12, 2000

August 12, 2005

February 29, 2004

February 29, 2012

**Output**

Case 1: 1

Case 2: 0

Case 3: 1

Case 4: 3


题意：t 组数据，每组给出两个日期，求这两个日期间闰日的个数

思路：

首先可以将用字母表示的时间使用 map 存储，转换为数字，然后再考虑两个时间范围内的闰日个数。

由于数据范围的原因，对每个日期进行暴力枚举一定会超时，那么可以考虑计算起始时间中闰年的个数与终止时间中闰年的个数，两个结果之差就是两日期间闰年的个数，计算起始、终止时间的闰年个数可以使用容斥原理，即：闰年数 = 能被 4 整除 + 能被 400 整除 - 能被 100 整除

![](https://img-blog.csdnimg.cn/20190213133207888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 3001
#define LL long long
const int MOD=1e9+7;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
map<string,int> mp;
bool judge(int year){//判断是否是闰年
    if(year%4==0)
        if(year%400==0||year%100!=0)
            return true;
    return false;
}
int calculate(int year){//计算当前年有多少个闰年
    int run=year/4+year/400-year/100;
    return run;
}
int main(){
    mp["January"]=1;
    mp["February"]=2;
    mp["March"]=3;
    mp["April"]=4;
    mp["May"]=5;
    mp["June"]=6;
    mp["July"]=7;
    mp["August"]=8;
    mp["September"]=9;
    mp["October"]=10;
    mp["November"]=11;
    mp["December"]=12;

    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        char str1[20],str2[20];
        int m1,m2;//月
        int d1,d2;//日
        int y1,y2;//年
        scanf("%s%d,%d",str1,&d1,&y1);
        scanf("%s%d,%d",str2,&d2,&y2);

        if(mp.count(str1)!=0)
            m1=mp[str1];
        if(mp.count(str2)!=0)
            m2=mp[str2];

        int res=calculate(y2)-calculate(y1);//两个年份之间有多少个闰年，不含起始年含最终年
        if(judge(y1)&&m1<=2)
            res++;
        if(judge(y2) && (m2<2||m2==2&&d2<=28) )
            res--;

        printf("Case %d: %d\n",Case++,res);
    }
    return 0;
}
```






