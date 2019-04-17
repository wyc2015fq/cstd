# Wall Painting（HDU-4810） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 20:55:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50
个人分类：[HDU																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Ms.Fang loves painting very much. She paints GFW(Great Funny Wall) every day. Every day before painting, she produces a wonderful color of pigments by mixing water and some bags of pigments. On the K-th day, she will select K specific bags of pigments and mix them to get a color of pigments which she will use that day. When she mixes a bag of pigments with color A and a bag of pigments with color B, she will get pigments with color A xor B. 

When she mixes two bags of pigments with the same color, she will get color zero for some strange reasons. Now, her husband Mr.Fang has no idea about which K bags of pigments Ms.Fang will select on the K-th day. He wonders the sum of the colors Ms.Fang will get with  ![](https://img-blog.csdnimg.cn/20190117202251120.jpg) different plans. 

For example, assume n = 3, K = 2 and three bags of pigments with color 2, 1, 2. She can get color 3, 3, 0 with 3 different plans. In this instance, the answer Mr.Fang wants to get on the second day is 3 + 3 + 0 = 6. 

Mr.Fang is so busy that he doesn’t want to spend too much time on it. Can you help him? 

You should tell Mr.Fang the answer from the first day to the n-th day.

# Input

There are several test cases, please process till EOF. 

For each test case, the first line contains a single integer N(1 <= N <= 10 3).The second line contains N integers. The i-th integer represents the color of the pigments in the i-th bag.

# Output

For each test case, output N integers in a line representing the answers(mod 10 6 +3) from the first day to the n-th day.

# Sample Input

**41 2 10 1**

# **Sample Output**

**14 36 30 8**


题意：给出 n 个数，在 1-n 天中，每天按天数选择相应的数，每种选法选出来的数异或，最后求所有取法的和

思路：将 n 个数转为 30 位二进制，由于所有数字按位异或的和等于所有数字异或的和，按位异或的选择情况可以用组合数学来选择，然后对于每一位算一个组合数累加即可，奇数个异或得 1，偶数个异或得 0，最后再乘以自己的二进制位值，组合数可以用杨辉三角的规律提前打表

开始时统计每一列 1 的个数，第 i 列选 k 个异或时，选出奇数 m 个 1，剩下的 k-m 都选 0，即：f(num[i],m)*f(n-num[i],k-m)*1<<(i-1)，最后按列累加即可

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
#define MOD 1000003
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
LL n;
LL bit[31];
LL f[N][N];
LL res[N];
int main(){
    for(int i=0;i<N;i++){
        f[0][i]=0;
        f[i][0]=1;
    }
    for(int i=1;i<N;i++)
        for(int j=1;j<N;j++)
            f[i][j]=(f[i-1][j]+f[i-1][j-1])%MOD;

    while(scanf("%d",&n)!=EOF&&n){
        memset(bit,0,sizeof(bit));
        memset(res,0,sizeof(res));

        LL maxx=-INF;
        for(int i=1;i<=n;i++){
			LL x;
			scanf("%lld",&x);

			LL cnt=1;
		    while(x)
			{
				if(x&1)
                    bit[cnt]++;
				x=x>>1;
				cnt++;
			}
			maxx=max(maxx,cnt);
		}
        for(int i=1;i<=n;i++){
			for(int j=1;j<maxx;j++){
				LL val=(1<<(j-1));
				for(int k=1;k<=bit[j]&&i>=k;k+=2){
					res[i]=(res[i]+f[bit[j]][k]*f[n-bit[j]][i-k]%MOD*val%MOD)%MOD;
				}
			}
		}

        for (int i=1;i<n;i++)
                printf("%d ",res[i]);
        printf("%d\n",res[n]);
    }
    return 0;
}
```






