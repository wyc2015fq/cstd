# HDU 1003 Max Sum 分治或动态规划 - 紫芝的博客 - CSDN博客





2018年07月24日 12:18:17[紫芝](https://me.csdn.net/qq_40507857)阅读数：40








# Max Sum

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 290991    Accepted Submission(s): 69018****

**Problem Description**

Given a sequence a[1],a[2],a[3]......a[n], your job is to calculate the max sum of a sub-sequence. For example, given (6,-1,5,4,-7), the max sum in this sequence is 6 + (-1) + 5 + 4 = 14.

**Input**

The first line of the input contains an integer T(1<=T<=20) which means the number of test cases. Then T lines follow, each line starts with a number N(1<=N<=100000), then N integers followed(all the integers are between -1000 and 1000).

**Output**

For each test case, you should output two lines. The first line is "Case #:", # means the number of the test case. The second line contains three integers, the Max Sum in the sequence, the start position of the sub-sequence, the end position of the sub-sequence. If there are more than one result, output the first one. Output a blank line between two cases.

**Sample Input**

```
2
5
6  -1  5  4  -7
7
0  6  -1  1  -6  7  -5
```

**Sample Output**

```cpp
Case 1:
14  1  4
Case 2:
7  1  6
```

**Author**

Ignatius.L

**Recommend**

We have carefully selected several similar problems for you:  [1176](http://acm.hdu.edu.cn/showproblem.php?pid=1176)[1087](http://acm.hdu.edu.cn/showproblem.php?pid=1087)[1069](http://acm.hdu.edu.cn/showproblem.php?pid=1069)[2084](http://acm.hdu.edu.cn/showproblem.php?pid=2084)[1058](http://acm.hdu.edu.cn/showproblem.php?pid=1058)

**给定K个整数的序列{ N1，N2 ... NK }，其任意连续子序列可表示为{ Ni，Ni+1 ...Nj }，其中 1<= i <= j <= K。最大连续子序列是所有连续子序列中元素和最大的一个， 例如给定序列{ -2，11 ，-4 ，13 ，-5 ，-2 }，其最大连续子序列为{ 11， -4 ，13 }，最大和 为20。 **

### 解法一：分治

```cpp
#include <cstdio>
#include <iostream>
using namespace std;

int a[100010];

struct node {
    int sum, left, right;
};//存储结果

node recursionMax (int left, int right)
{
    // 只有一个数
    if (left == right) return {a[left], left, right};

    int mid = (left + right) / 2;

    node maxLeft = recursionMax (left, mid); // 左
    node maxRight = recursionMax(mid + 1, right); // 右

    int left_sum = -0x7fffffff, right_sum = -0x7fffffff, li = mid, ri = mid + 1;
    int tmp = 0;
    //求左区间的最大值
    for (int i = mid; i >= left; i--)
    {
        tmp += a[i];
        if (left_sum <= tmp) {
            left_sum = tmp;
            li = i;
        }
    }

    tmp = 0;
    //求右区间的最大值
    for (int  j = mid + 1; j <= right; j++)
    {
        tmp += a[j];
        if (right_sum < tmp) {
            right_sum = tmp;
            ri = j;
        }
    }
    node ret = maxLeft.sum >= maxRight.sum ? maxLeft : maxRight;
    if (ret.sum > left_sum + right_sum) {
        return ret;
    } else if (ret.sum == left_sum + right_sum && ret.left < li) {
        return ret;

    } else {
        return {left_sum + right_sum, li, ri};
    }
}

int main ()
{
    int  n,T,Case=1;
    cin>>T;
    while (T--) {
            cin >> n;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
    if(Case!=1) printf("\n");
        node ret = recursionMax(0, n - 1);
        printf("Case %d:\n%d %d %d\n",Case++, ret.sum, ret.left + 1, ret.right + 1);
    }
    return 0;
}
```

### 解法2：动态规划

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 100010;
int a[maxn];
int main()
{
    int n,T,Case=1;
    cin>>T;
    while(T--){
        scanf("%d",&n);
        for(int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    if(Case!=1) printf("\n");
    if(n == 1)
        printf("%d\n",a[0]);
    else {
        int pos=0,begin=0,end=0;
        int max_sum=a[0],sum=a[0];
        for(int i=1;i<n;i++)
        {
            if(sum+a[i]<a[i])//如果当前值小于a[i]
            {
                sum=a[i];
                pos=i;//记录下改的位置
            }
                else    sum=sum+a[i];
            if(sum>max_sum){
        //当前值比最大值大，则头尾都要改
        max_sum=sum;
        begin=pos;
        end=i;
            }
        }
        printf("Case %d:\n%d %d %d\n",Case++,max_sum,begin+1,end+1);
    }
}
    return 0;
}
```





