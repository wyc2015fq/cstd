# Binary Tree（HDU-5573） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 19:12:11[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[HDU																[基础算法——递推](https://blog.csdn.net/u011815404/article/category/7615003)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

The Old Frog King lives on the root of an infinite tree. According to the law, each node should connect to exactly two nodes on the next level, forming a full binary tree. 

Since the king is professional in math, he sets a number to each node. Specifically, the root of the tree, where the King lives, is 11. Say froot=1. 

And for each node uu, labels as fufu, the left child is fu×2 and right child is fu×2+1. The king looks at his tree kingdom, and feels satisfied. 

Time flies, and the frog king gets sick. According to the old dark magic, there is a way for the king to live for another N years, only if he could collect exactly N soul gems. 

Initially the king has zero soul gems, and he is now at the root. He will walk down, choosing left or right child to continue. Each time at node x, the number at the node is fx (remember froot=1), he can choose to increase his number of soul gem by fx, or decrease it by fx. 

He will walk from the root, visit exactly K nodes (including the root), and do the increasement or decreasement as told. If at last the number is N, then he will succeed. 

Noting as the soul gem is some kind of magic, the number of soul gems the king has could be negative. 

Given N, K, help the King find a way to collect exactly N soul gems by visiting exactly K nodes.

# Input

First line contains an integer T, which indicates the number of test cases. 

Every test case contains two integers N and K, which indicates soul gems the frog king want to collect and number of nodes he can visit. 

⋅ 1≤T≤100. 

⋅ 1≤N≤109. 

⋅ N≤2K≤260.

# Output

For every test case, you should output " Case #x:" first, where xx indicates the case number and counts from 11. 

Then KK lines follows, each line is formated as 'a b', where aa is node label of the node the frog visited, and bb is either '+' or '-' which means he increases / decreases his number by a. 

It's guaranteed that there are at least one solution and if there are more than one solutions, you can output any of them. 

# Sample Input

**25 310 4**

# **Sample Output**

**Case #1:1 +3 -7 +Case #2:1 +3 +6 -12 +**

————————————————————————————————————————————

题意：一个二叉树，根节点编号为 1，子节点编号为 2x 和 2x+1，若到了某一个点，那么对应可以减少这个点的标号的数值，也可以增加这个点的标号的数值，给你一个数值和走的步数，输出一个合理的方案

思路：树的 2x 这一方向的值是 2、4、8、16、...，对于这个方向上的数值进行 + 或 - 的操作，无论什么数据，如果合法，一定可以从这个方向完成，如果最后结果是偶数，最后 +1 即可。

于是问题就转换成了从 1、2、4、6、8、16....走下去，对应每个数取正或取负，由于全是 2 的幂，因此可将问题转换为二进制，并且用 0 表示加法，用 1 表示减法。

经过 1<<k 后，再减去 n 得到 1<<k-n，然后将这个数的二进制从右向左第二位开始判断，如果是 1，表示这位的数字为负，如果是 0，表示这位数字为正

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
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 500001
#define LL long long
using namespace std;
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        LL n,k;
        scanf("%lld%lld",&n,&k);
        LL temp=(LL)1<<k;
        temp-=n;

        printf("Case #%d:\n",Case++);
        for(int i=1;i<k;i++){
            if(temp&(1<<i))
                printf("%lld -\n",((LL)1<<(i-1)));
            else
                printf("%lld +\n",((LL)1<<(i-1)));
        }

        temp=(LL)1<<(k-1);
        if(!(n%2))
            temp++;
        printf("%lld +\n",temp);
    }
    return 0;
}
```






