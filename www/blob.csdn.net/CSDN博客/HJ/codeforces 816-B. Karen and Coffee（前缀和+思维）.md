# codeforces  816-B. Karen and Coffee（前缀和+思维） - HJ - CSDN博客
2017年08月01日 16:47:50[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：190
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
To stay woke and attentive during classes, Karen needs some coffee!
Karen, a coffee aficionado, wants to know the optimal temperature for brewing the perfect cup of coffee. Indeed, she has spent some time reading several recipe books, including the universally acclaimed “The Art of the Covfefe”.
She knows n coffee recipes. The i-th recipe suggests that coffee should be brewed between li and ri degrees, inclusive, to achieve the optimal taste.
Karen thinks that a temperature is admissible if at least k recipes recommend it.
Karen has a rather fickle mind, and so she asks q questions. In each question, given that she only wants to prepare coffee with a temperature between a and b, inclusive, can you tell her how many admissible integer temperatures fall within the range? 
Input
The first line of input contains three integers, n, k (1 ≤ k ≤ n ≤ 200000), and q (1 ≤ q ≤ 200000), the number of recipes, the minimum number of recipes a certain temperature must be recommended by to be admissible, and the number of questions Karen has, respectively.
The next n lines describe the recipes. Specifically, the i-th line among these contains two integers li and ri (1 ≤ li ≤ ri ≤ 200000), describing that the i-th recipe suggests that the coffee be brewed between li and ri degrees, inclusive.
The next q lines describe the questions. Each of these lines contains a and b, (1 ≤ a ≤ b ≤ 200000), describing that she wants to know the number of admissible integer temperatures between a and b degrees, inclusive. 
Output
For each question, output a single integer on a line by itself, the number of admissible integer temperatures between a and b degrees, inclusive. 
Examples 
Input
3 2 4 
91 94 
92 97 
97 99 
92 94 
93 97 
95 96 
90 100
Output
3 
3 
0 
4
Input
2 1 1 
1 1 
200000 200000 
90 100
Output
0
Note
In the first test case, Karen knows 3 recipes.
```
The first one recommends brewing the coffee between 91 and 94 degrees, inclusive.
The second one recommends brewing the coffee between 92 and 97 degrees, inclusive.
The third one recommends brewing the coffee between 97 and 99 degrees, inclusive.
```
A temperature is admissible if at least 2 recipes recommend it.
She asks 4 questions.
In her first question, she wants to know the number of admissible integer temperatures between 92 and 94 degrees, inclusive. There are 3: 92, 93 and 94 degrees are all admissible.
In her second question, she wants to know the number of admissible integer temperatures between 93 and 97 degrees, inclusive. There are 3: 93, 94 and 97 degrees are all admissible.
In her third question, she wants to know the number of admissible integer temperatures between 95 and 96 degrees, inclusive. There are none.
In her final question, she wants to know the number of admissible integer temperatures between 90 and 100 degrees, inclusive. There are 4: 92, 93, 94 and 97 degrees are all admissible.
In the second test case, Karen knows 2 recipes.
```
The first one, "wikiHow to make Cold Brew Coffee", recommends brewing the coffee at exactly 1 degree.
The second one, "What good is coffee that isn't brewed at at least 36.3306 times the temperature of the surface of the sun?", recommends brewing the coffee at exactly 200000 degrees.
```
A temperature is admissible if at least 1 recipe recommends it.
In her first and only question, she wants to know the number of admissible integer temperatures that are actually reasonable. There are none.
题目大意： 
  给出n个区间，然后进行q次区间查询，问每次查询区间中的数至少在k个区间中出现的个数。
解题思路： 
  用两个数组l[],r[]分别记录，然后用一个变量循环。用sum[i]保存前i个数出现次数不少于k次的数字个数。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 200005
long long l[maxn],r[maxn],sum[maxn];
int main()
{
    long long n,k,q;
    while(cin>>n>>k>>q)
    {
        long long a,b,cnt=0;
        memset(l,0,sizeof(l));
        memset(r,0,sizeof(r));
        memset(sum,0,sizeof(sum));
        for(int i=0;i<n;i++)
        {
            cin>>a>>b;
            l[a]++;
            r[b]++;
        }
        for(int i=1;i<200001;i++)
        {
            if(l[i])
              cnt+=l[i];
            sum[i]=cnt>=k?sum[i-1]+1:sum[i-1];
            if(r[i])
              cnt-=r[i];
        }
        for(int i=0;i<q;i++)
        {
            cin>>a>>b;
            cout<<sum[b]-sum[a-1]<<endl;
        }
    }
    return 0;
}
```
