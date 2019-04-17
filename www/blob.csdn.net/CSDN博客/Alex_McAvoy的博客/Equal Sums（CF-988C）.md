# Equal Sums（CF-988C） - Alex_McAvoy的博客 - CSDN博客





2018年08月07日 19:10:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[CodeForces																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given k sequences of integers. The length of the i-th sequence equals to nini.

You have to choose exactly two sequences i and j (i≠j) such that you can remove exactly one element in each of them in such a way that the sum of the changed sequence ii (its length will be equal to ni−1) equals to the sum of the changed sequence j (its length will be equal to nj−1).

Note that it's required to remove exactly one element in each of the two chosen sequences.

Assume that the sum of the empty (of the length equals 0) sequence is 0.

# Input

The first line contains an integer k (2≤k≤2⋅10^5) — the number of sequences.

Then k pairs of lines follow, each pair containing a sequence.

The first line in the i-th pair contains one integer ni (1≤ni<2⋅10^5) — the length of the ii-th sequence. The second line of the i-th pair contains a sequence of nini integers ai,1,ai,2,…,ai,ni.

The elements of sequences are integer numbers from −10^4 to 10^4.

The sum of lengths of all given sequences don't exceed 2⋅10^5, i.e. n1+n2+⋯+nk≤2⋅10^5.

# Output

If it is impossible to choose two sequences such that they satisfy given conditions, print "NO" (without quotes). Otherwise in the first line print "YES" (without quotes), in the second line — two integers i, x (1≤i≤k,1≤x≤ni), in the third line — two integers j, y (1≤j≤k,1≤y≤nj). It means that the sum of the elements of the ii-th sequence without the element with index x equals to the sum of the elements of the j-th sequence without the element with index y.

Two chosen sequences must be distinct, i.e. i≠j. You can print them in any order.

If there are multiple possible answers, print any of them.

# Examples

**Input**

2

5

2 3 1 3 2

6

1 1 2 2 2 1

**Output**

YES

2 6

1 2

**Input**

3

1

5

5

1 1 1 1 1

2

2 3

**Output**

NO

**Input**

4

6

2 2 2 2 2 2

5

2 2 2 2 2

3

2 2 2

5

2 2 2 2 2

**Output**

YES

2 2

4 1


题意：给 n 个数组，找出两个数组，这两个数组各删除一个数字后，使得两个数组的和相同

思路：k 个序列 n 个数，最大要存 4E10 个数，普通的数组是无法存储的，因此只能选择 vector，由于要求删除某一序列的某一个数，因此对每个序列进行枚举，记录总值减去该个数的值，由于数可能是负数，因此无法用数组桶排来记录，只能将得到的数记录到 map 中，若这个数已经在 map 中出现了，则之前统计过，直接输出答案即可，若这个值没有在 map 中出现过，则需将这个值打上标记，再用两个 map 分别记录这次删除的是第几个序列的第几个数

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
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
using namespace std;
vector<int> vec[N];
map<int,int>bag1;
map<int,int>bag2;
map<int,int>bag3;
int main()
{
    int k;
    cin>>k;

    for(int i=1;i<=k;i++)
    {
        int n;
        cin>>n;

        int sum=0;
        for(int j=0;j<n;j++)
        {
            int num;
            cin>>num;
            vec[i].push_back(num);
            sum+=vec[i][j];
        }

        for(int j=0;j<n;j++)
        {
            int temp=sum-vec[i][j];
            if(bag1[temp])
            {
                if(bag2[temp]==i)
                    continue;
                else
                {
                    cout<<"YES"<<endl;
                    cout<<i<<" "<<j+1<<endl;
                    cout<<bag2[temp]<<" "<<bag3[temp]<<endl;
                    return 0;
                }
            }
            else
            {
                bag1[temp]=1;
                bag2[temp]=i;
                bag3[temp]=j+1;
            }
        }

    }
    cout<<"NO"<<endl;
    return 0;
}
```






