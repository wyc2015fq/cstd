# FLY（CF-1011C） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 19:15:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

Natasha is going to fly on a rocket to Mars and return to Earth. Also, on the way to Mars, she will land on n−2 intermediate planets. Formally: we number all the planets from 1 to n. 1 is Earth, nn is Mars. Natasha will make exactly nn flights: 1→2→…n→1.

Flight from x to y consists of two phases: take-off from planet x and landing to planet y. This way, the overall itinerary of the trip will be: the 1-st planet → take-off from the 1-st planet → landing to the 2-nd planet → 2-nd planet → take-off from the 2-nd planet → …… → landing to the n-th planet → the n-th planet → take-off from the n-th planet → landing to the 1-st planet → the 1-st planet.

The mass of the rocket together with all the useful cargo (but without fuel) is m tons. However, Natasha does not know how much fuel to load into the rocket. Unfortunately, fuel can only be loaded on Earth, so if the rocket runs out of fuel on some other planet, Natasha will not be able to return home. Fuel is needed to take-off from each planet and to land to each planet. It is known that 1 ton of fuel can lift off aiai tons of rocket from the i-th planet or to land bibi tons of rocket onto the ii-th planet.

For example, if the weight of rocket is 9 tons, weight of fuel is 33 tons and take-off coefficient is 8 (ai=8), then 1.5 tons of fuel will be burnt (since 1.5⋅8=9+3). The new weight of fuel after take-off will be 1.51.5tons.

Please note, that it is allowed to burn non-integral amount of fuel during take-off or landing, and the amount of initial fuel can be non-integral as well.

Help Natasha to calculate the minimum mass of fuel to load into the rocket. Note, that the rocket must spend fuel to carry both useful cargo and the fuel itself. However, it doesn't need to carry the fuel which has already been burnt. Assume, that the rocket takes off and lands instantly.

# Input

The first line contains a single integer n (2≤n≤1000) — number of planets.

The second line contains the only integer m (1≤m≤1000) — weight of the payload.

The third line contains n integers a1,a2,…,an (1≤ai≤1000), where aiai is the number of tons, which can be lifted off by one ton of fuel.

The fourth line contains n integers b1,b2,…,bn (1≤bi≤1000), where bibi is the number of tons, which can be landed by one ton of fuel.

It is guaranteed, that if Natasha can make a flight, then it takes no more than 10^9 tons of fuel.

# Output

If Natasha can fly to Mars through (n−2) planets and return to Earth, print the minimum mass of fuel (in tons) that Natasha should take. Otherwise, print a single number −1.

It is guaranteed, that if Natasha can make a flight, then it takes no more than 10^9 tons of fuel.

The answer will be considered correct if its absolute or relative error doesn't exceed 10^−6. Formally, let your answer be p, and the jury's answer be q. Your answer is considered correct if |p−q|/max(1,|q|) ≤10^−6.

# Examples

**Input**

2

12

11 8

7 5

**Output**

10.0000000000

**Input**

3

1

1 4 1

2 5 3

**Output**

-1

**Input**

6

2

4 6 3 3 5 6

2 6 3 6 5 3

**Output**

85.4800000000

————————————————————————————————————————————

题意：给出飞机的初始质量 n，以及每次起飞降落消耗的汽油m，每个机场给的汽油量是1吨，求最开始携带的汽油重量

思路：模拟，倒着推回去即可

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
#define N 200001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int b[N];
int main()
{
    int n;
    double m;
    cin>>n>>m;

    bool flag=true;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        if(a[i]<=1)
            flag=false;
    }
    for(int i=1;i<=n;i++)
    {
        cin>>b[i];
        if(b[i]<=1)
            flag=false;
    }

    if(!flag)
    {
        cout<<-1<<endl;
        return 0;
    }

    double sum=0;
    double temp;
    for(int i=1;i<=n;i++)
    {
        temp=m/(a[i]-1);
        m+=temp;
        sum+=temp;

        temp=m/(b[i]-1);
        m+=temp;
        sum+=temp;
    }

    printf("%.10f\n",sum);

    return 0;
}
```






