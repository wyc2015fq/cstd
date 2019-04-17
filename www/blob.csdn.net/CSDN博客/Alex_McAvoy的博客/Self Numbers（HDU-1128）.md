# Self Numbers（HDU-1128） - Alex_McAvoy的博客 - CSDN博客





2019年02月17日 17:26:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In 1949 the Indian mathematician D.R. Kaprekar discovered a class of numbers called self-numbers. For any positive integer n, define d(n) to be n plus the sum of the digits of n. (The d stands for digitadition, a term coined by Kaprekar.) For example, d(75) = 75 + 7 + 5 = 87. Given any positive integer n as a starting point, you can construct the infinite increasing sequence of integers n, d(n), d(d(n)), d(d(d(n))), .... For example, if you start with 33, the next number is 33 + 3 + 3 = 39, the next is 39 + 3 + 9 = 51, the next is 51 + 5 + 1 = 57, and so you generate the sequence 

33, 39, 51, 57, 69, 84, 96, 111, 114, 120, 123, 129, 141, ... 

The number n is called a generator of d(n). In the sequence above, 33 is a generator of 39, 39 is a generator of 51, 51 is a generator of 57, and so on. Some numbers have more than one generator: for example, 101 has two generators, 91 and 100. A number with no generators is a self-number. There are thirteen self-numbers less than 100: 1, 3, 5, 7, 9, 20, 31, 42, 53, 64, 75, 86, and 97. 

# Input

No Input

# Output

Write a program to output all positive self-numbers less than or equal 1000000 in increasing order, one per line. 

# **Sample Input**

No Input

# Sample Output

**135792031425364|| <-- a lot more numbers|9903991499259927993899499960997199829993|||**


题意：找出 1 到 1000000 的所有自私数，自私数不存在一个数与他的各位数的和相加等于它本身

思路：用桶排模拟一下即可

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
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int bucket[N];
int judge(int i){
    int t=i;
    while(t){
        i+=t%10;
        t/=10;
    }
    return i;
}
int main(){
    for(int i=1;i<=1000000;i++){
        bucket[judge(i)]=1;
        if(!bucket[i])
            printf("%d\n",i);
    }
    return 0;
}
```






