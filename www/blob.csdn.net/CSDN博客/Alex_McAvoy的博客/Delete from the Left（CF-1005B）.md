# Delete from the Left（CF-1005B） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 20:21:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50
个人分类：[CodeForces																[字符串处理——模拟与暴力](https://blog.csdn.net/u011815404/article/category/8650811)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given two strings s and t. In a single move, you can choose any of two strings and delete the first (that is, the leftmost) character. After a move, the length of the string decreases by 1. You can't choose a string if it is empty.

For example:

by applying a move to the string "where", the result is the string "here",

by applying a move to the string "a", the result is an empty string "".

You are required to make two given strings equal using the fewest number of moves. It is possible that, in the end, both strings will be equal to the empty string, and so, are equal to each other. In this case, the answer is obviously the sum of the lengths of the initial strings.

Write a program that finds the minimum number of moves to make two given strings s and t equal.

# Input

The first line of the input contains s. In the second line of the input contains t. Both strings consist only of lowercase Latin letters. The number of letters in each string is between 1 and 2⋅10^5, inclusive.

# Output

Output the fewest number of moves required. It is possible that, in the end, both strings will be equal to the empty string, and so, are equal to each other. In this case, the answer is obviously the sum of the lengths of the given strings.

# Examples

**Input**

test

west

**Output**

2

**Input**

codeforces

yes

**Output**

9

**Input**

test

yes

**Output**

7

**Input**

b

ab

**Output**

1


题意：给出两个字符串，每次只能删除串1前面的一个字母或者删除串2前面的一个字母，求删除多少个字母后两个字符串相同

思路：由于要求两个字符串相同，从后向前找相同的字母即可，一旦遇到不同的，就停止，最后输出两字符串的长度减去两倍的相同的字母即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int main()
{
    string str1,str2;
    cin>>str1>>str2;

        int cnt=0;
        int len1=str1.length();//字符串长度
        int len2=str2.length();//字符串长度
        for(int i=len1-1,j=len2-1;i>=0&&j>=0;i--,j--)//从右向左，统计相同的个数，直至不同
        {
            if(str1[i]==str2[j])
                cnt++;
            else
                break;
        }
        cout<<len1+len2-2*cnt<<endl;//串1长度+串2长度-2*相同的个数

    return 0;
}
```






