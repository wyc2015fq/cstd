# HDU 1059 Dividing 分配（多重背包，母函数） - xcw0754 - 博客园
# [HDU 1059 Dividing 分配（多重背包，母函数）](https://www.cnblogs.com/xcw0754/p/4245027.html)
题意：
　　两个人共同收藏了一些石头，现在要分道扬镳，得分资产了，石头具有不同的收藏价值，分别为1、2、3、4、5、6共6个价钱。问：是否能公平分配？
输入：
　　每行为一个测试例子，每行包括6个数字，分别对应6种价钱的石头数目，比如101200代表价值为1的石头有1个，价值为2的石头有0个....价值为4的石头有2个。他们具有的石头数量的上限为2万个。
思路：
　　想用多重背包的方式解决，也想转01背包比较简单。直接转01背包会超时，得想办法。可以用多重背包的方法，用二进制来减少复杂度，应该可行。我用的是偷懒的办法，将数据都减小，比如某个价值的石头有1千个，是偶数，那就可以直接分掉，每人500啦，但是可能会有奇数个的情况，所以不能单独将某价值的石头一次性分完。那么留多少合适？我觉得留4合适，但是实际上留了8个以下才能AC。
出现下面情况说明了不能将某个价值的石头总数mod2：
价值：1　2　3　4　5　6
数量：1　0　8　0　1　0
解释：假如%2的话，价值为3的石头就变为0个，剩下两个石头，价值分别为1和5，分不了！
所以余数必须大于2，测试了一下，6不行，8以上的偶数就行了。
现在，数据的大小降下来了，转成01背包就简单多了。将剩下的所有石头的的价值的一半假设为背包的容量，将石头的体积设为等价于其价值大小。什么意思呢？
举个例子：
价值：1　2　3　4　5　6
数量：1　0　2　0　1　0
体积：1　2　3　4　5　6
假设这是数值大小降低后的结果，一眼可以看出1+5=3*2，可以公平分配，所有石头的总价值为12，假设其中一个人是我，那么我必须分配到价值为6的石头才是公平的，不用管到底分得1块还是2块石头。而其价值与体积已经假设是相等的，那么设我的背包大小为6，我想从这些石头（4块）中挑出一些石头，使我的背包装满，只要能刚好装满，证明能公平分配，即dp[x]=x。 这个可列举一些小的数据后计算就能验证。
```
1 #include <iostream>
 2 using namespace std;
 3 int num[6];
 4 int dp[200];
 5 bool divide(int sum)
 6 {
 7     int k,i,j;
 8     for(i=0;i<6;i++)
 9         for(k=0;k<num[i];k++)
10             for(j=sum;j>i;j--)
11                 if(dp[j-(i+1)]+(i+1)>dp[j])
12                 dp[j]=dp[j-(i+1)]+(i+1);
13     if(dp[sum]==sum)
14         return true;
15     else
16         return false;
17 }
18 int main()
19 {
20     int i,n=0,sum;
21     while(1)
22     {
23         for(i=0,sum=0;i<6;i++)
24         {
25             scanf("%d",&num[i]);
26             num[i]%=8;
27             sum+=num[i]*(i+1);    //所有石头的价值总和    
28         }
29         if(!num[0]&&!num[1]&&!num[2]&&!num[3]&&!num[4]&&!num[5])    return 0;    //全0就退出        
30         if(sum%2!=0)    //价值的总和不能被被2整除的话就肯定分不了
31         {
32             printf("Collection #%d:\nCan't be divided.\n\n",++n);
33             continue;
34         }
35         else sum>>=1;    //可以分的话，总和得减半
36         memset(dp,0,sizeof(int)*(sum+1));    //清内存
37         if(divide(sum)==true)
38             printf("Collection #%d:\nCan be divided.\n\n",++n);
39         else
40             printf("Collection #%d:\nCan't be divided.\n\n",++n);
41     }
42     return 0;
43 }
1059
```
**另一解法：**使用普通型母函数。
思路：同样需要将数据降小，好像不一定对于这样的题都可以进行数据降小处理，但是这里可以就方便多了。母函数主要算的是组成某一种值的方法数，既然有组成的方法，那肯定是能公平分配了，可能还不止一种公平的分配方法呢。利用这点，我们不需要求出分配一半价值的所有方法，只需在检测出有一种方法就可以返回了，无需继续算。
代码思路：用笔算的话我们能算出来，其实方法就是模拟我们的笔算方法，有六种价值，那么就只有6个括号要相乘。而每个括号中的幂次数最多为价值的一半即可，我们要求的就是组成一半价值的方法。
看代码还是容易点吧，代码这么多是因为大部分是固定的模版啊！
```
1 #include <iostream>
 2 #include <cstring>
 3 #define N 100
 4 using namespace std;
 5 int num[6];
 6 int ans[N];    //
 7 int sup[N];
 8 bool divide(int sum)    //取部分石头，能组成价值为sum，证明可公平分配。
 9 {
10     int k,i,j;
11     for(i=2;i<=6;i++)    //作6个括号相乘
12     {
13         if(num[i-1]==0)    continue;
14         for(j=0;j<=sum&&j<=num[i-1]*i;j+=i)    //j是跳的，间距是i
15             for(k=0;k<=sum;k++)
16                 sup[j+k]+=ans[k];    //重点在这，好好理解
17         memcpy(ans,sup,sizeof(int)*(sum+1));    //这样复制更快
18         memset(sup,0,sizeof(sup));    //这样清零更快
19         if(ans[sum]>0)    return true;    //有1种方法以上都可以返回了
20     }
21     return false;
22 }
23 int main()
24 {
25     int i,n=0,sum;
26     while(1)
27     {
28         for(i=0,sum=0;i<6;i++)
29         {
30             scanf("%d",&num[i]);
31             num[i]%=8;        //数值降小
32             sum+=num[i]*(i+1);    //所有石头的价值总和（已降）    
33         }
34         if(!num[0]&&!num[1]&&!num[2]&&!num[3]&&!num[4]&&!num[5])    return 0;    //全0就退出        
35         if(sum%2!=0)    //价值的总和不能被被2整除的话就肯定分不了
36         {
37             printf("Collection #%d:\nCan't be divided.\n\n",++n);
38             continue;
39         }
40         else sum>>=1;    //可以分的话，总和得减半
41         //**********以上部分是固定的****************************************
42         memset(ans,0,sizeof(ans));    //清内存
43         memset(sup,0,sizeof(sup));    //清内存
44         for(i=0;i<=num[0];i++)    //初始化ans
45             ans[i]=1;
46         if(divide(sum)==true)
47             printf("Collection #%d:\nCan be divided.\n\n",++n);
48         else
49             printf("Collection #%d:\nCan't be divided.\n\n",++n);
50     }
51 }
1059
```

