# Polycarp and Div3（CF-1005D） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 07:55:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58








# Problem Description

Polycarp likes numbers that are divisible by 3.

He has a huge number ss. Polycarp wants to cut from it the maximum number of numbers that are divisible by 3. To do this, he makes an arbitrary number of vertical cuts between pairs of adjacent digits. As a result, after mm such cuts, there will be m+1 parts in total. Polycarp analyzes each of the obtained numbers and finds the number of those that are divisible by 3.

For example, if the original number is s=3121, then Polycarp can cut it into three parts with two cuts: 3|1|21. As a result, he will get two numbers that are divisible by 3.

Polycarp can make an arbitrary number of vertical cuts, where each cut is made between a pair of adjacent digits. The resulting numbers cannot contain extra leading zeroes (that is, the number can begin with 0 if and only if this number is exactly one character '0'). For example, 007, 01 and 00099 are not valid numbers, but 90, 0 and 10001 are valid.

What is the maximum number of numbers divisible by 33 that Polycarp can obtain?

# Input

The first line of the input contains a positive integer s. The number of digits of the number ss is between 11 and 2⋅10^5, inclusive. The first (leftmost) digit is not equal to 0.

# Output

Print the maximum number of numbers divisible by 3 that Polycarp can get by making vertical cuts in the given number s.

# Examples

**Input**

3121

**Output**

2

**Input**

6

**Output**

1

**Input**

1000000000000000000000000000000000

**Output**

33

**Input**

201920181

**Output**

4

# Note

In the first example, an example set of optimal cuts on the number is 3|1|21.

In the second example, you do not need to make any cuts. The specified number 6 forms one number that is divisible by 3.

In the third example, cuts must be made between each pair of digits. As a result, Polycarp gets one digit 1 and 33 digits 0. Each of the 33 digits 0 forms a number that is divisible by 3.

In the fourth example, an example set of optimal cuts is 2|0|1|9|201|81. The numbers 0, 9, 201 and 81 are divisible by 3.

————————————————————————————————————————————

题意：输入一串数字，不断划分，求最多有多少能被3整除的数字

思路：贪心，先判断每一位是否能被3整除，如果能就 cnt++；再记录前缀和，判断加起来的数字是不是3的倍数，如果是3的倍数，cnt++；如果连续三个数都不是3的倍数，那么对前面两个数来说，对3求余只可能是同时余1或同时余2，然后对于第三个数对3取余可能是1或者2，如果是是[111]或[222]，那就全部取，如果是[221]，那就取后面的两个，如果是[112]，那就取后面两个

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10000005
#define MOD 123
#define E 1e-6
using namespace std;
int main()
{
    string str;
    cin>>str;

    long long cnt=0,ans=0,pre;
    for(int i=0;i<str.size(); i++)
    {
        ans++;
        pre+=str[i]-'0';

        if(ans==3||(str[i]-'0')%3==0||pre%3==0)
        {
            ans=0;
            pre=0;
            cnt++;
        }
    }
    cout<<cnt<<endl;
    return 0;
}
```





