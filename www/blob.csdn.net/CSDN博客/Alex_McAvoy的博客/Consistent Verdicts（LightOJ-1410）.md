# Consistent Verdicts（LightOJ-1410） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 21:32:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79
个人分类：[LightOJ																[STL 的使用——算法](https://blog.csdn.net/u011815404/article/category/8793970)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

In a 2D plane N persons are standing and each of them has a gun in his hand. The plane is so big that the persons can be considered as points and their locations are given as Cartesian coordinates. Each of the N persons fire the gun in his hand exactly once and no two of them fire at the same or similar time (the sound of two gun shots are never heard at the same time by anyone so no sound is missed due to concurrency). The hearing ability of all these persons is exactly same. That means if one person can hear a sound at distance R1, so can every other person and if one person cannot hear a sound at distance R2 the other N-1 persons cannot hear a sound at distance R2 as well.

![](https://img-blog.csdnimg.cn/20190118203656937.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

The N persons are numbered from 1 to N. After all the guns are fired, all of them are asked how many gun shots they have heard (not including their own shot) and they give their verdict. It is not possible for you to determine whether their verdicts are true but it is possible for you to judge if their verdicts are consistent. For example, look at the figure above. There are five persons and their coordinates are (1, 2), (3, 1), (5, 1), (6, 3) and (1, 5) and they are numbered as 1, 2, 3, 4 and 5 respectively. After all five of them have shot their guns, you ask them how many shots each of them have heard. Now if there response is 1, 1, 1, 2 and 1 respectively then you can represent it as (1, 1, 1, 2, 1). But this is an inconsistent verdict because if person 4 hears 2 shots then he must have heard the shot fired by person 2, then obviously person 2 must have heard the shot fired by person 1, 3 and 4 (person 1 and 3 are nearer to person 2 than person 4). But their opinions show that Person 2 says that he has heard only 1 shot. On the other hand (1, 2, 2, 1, 0) is a consistent verdict for this scenario so is (2, 2, 2, 1, 1). In this scenario (5, 5, 5, 4, 4) is not a consistent verdict because a person can hear at most 4 shots.

Given the locations of N persons, your job is to find the total number of different consistent verdicts for that scenario. Two verdicts are different if opinion of at least one person is different.

# Input

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing a positive integer N (1 ≤ N ≤ 700). Each of the next N lines contains two integers xi yi (0 ≤ xi, yi ≤ 30000) denoting a co-ordinate of a person. Assume that all the co-ordinates are distinct.

# Output

For each case, print the case number and the total number of different consistent verdicts for the given scenario.

# **Sample Input**

**231 12 24 421 15 5**

# Sample Output

**Case 1: 4Case 2: 2**


题意：t 组数据，每组给出 n 个坐标代表 n 个人的位置，每个人的听力都相同但不知道具体范围，现在每人依次开枪射击，然后每人都给出一个数字代表听到的枪响的次数，汇总结果，根据每个人的坐标输出有几种一致的结果，即所有人的答案都不矛盾的结果

思路：由于两人相同的距离时，均能听到的声音相同，因此每一个不同的方案就是每两个点中间的距离，故而本题实质就是根据人与人的距离来判断结果，直接根据坐标计算所有人之间的距离，存储好后去重即可，要注意的是，由于听力具体范围没有给出，可能无限大，即可能所有人都能听到其他人枪响，因此最后的结果需要 +1

用 STL 的 set 容器来做是最简单的方法，但由于其内部是基于红黑树实现的，内部排序比较慢，在本题中使用 set 容器光荣的 TLE 了，因此可以使用 STL 中的 unique() 函数来进行去重

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
#define N 1001
#define LL long long
using namespace std;
int n;
struct Node{
    int x;
    int y;
}a[N];
int res[N*N];
int calculate(int x1,int y1,int x2,int y2){
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        scanf("%d%",&n);
        for(int i=1;i<=n;i++)
            scanf("%d%d",&a[i].x,&a[i].y);

        int cnt=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                res[cnt++]=calculate(a[i].x,a[i].y,a[j].x,a[j].y);

        sort(res,res+cnt);//用unique前需要sort
        int ans=unique(res,res+cnt)-res;//unique去重
        ans++;
		printf ("Case %d: %d\n",Case++,ans);

    }
    return 0;
}
```






