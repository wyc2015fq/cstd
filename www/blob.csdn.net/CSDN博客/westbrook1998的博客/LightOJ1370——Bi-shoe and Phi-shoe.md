# LightOJ1370——Bi-shoe and Phi-shoe - westbrook1998的博客 - CSDN博客





2018年08月20日 16:19:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：21标签：[欧拉函数](https://so.csdn.net/so/search/s.do?q=欧拉函数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Bamboo Pole-vault is a massively popular sport in Xzhiland. And Master Phi-shoe is a very popular coach for his success. He needs some bamboos for his students, so he asked his assistant Bi-Shoe to go to the market and buy them. Plenty of Bamboos of all possible integer lengths (yes!) are available in the market. According to Xzhila tradition,  

  Score of a bamboo = Φ (bamboo’s length)  

  (Xzhilans are really fond of number theory). For your information, Φ (n) = numbers less than n which are relatively prime (having no common divisor other than 1) to n. So, score of a bamboo of length 9 is 6 as 1, 2, 4, 5, 7, 8 are relatively prime to 9.  

  The assistant Bi-shoe has to buy one bamboo for each student. As a twist, each pole-vault student of Phi-shoe has a lucky number. Bi-shoe wants to buy bamboos such that each of them gets a bamboo with a score greater than or equal to his/her lucky number. Bi-shoe wants to minimize the total amount of money spent for buying the bamboos. One unit of bamboo costs 1 Xukha. Help him.  

  Input  

  Input starts with an integer T (≤ 100), denoting the number of test cases.  

  Each case starts with a line containing an integer n (1 ≤ n ≤ 10000) denoting the number of students of Phi-shoe. The next line contains n space separated integers denoting the lucky numbers for the students. Each lucky number will lie in the range [1, 106].  

  Output  

  For each case, print the case number and the minimum possible money spent for buying the bamboos. See the samples for details.  

  Sample Input  

  3  

  5  

  1 2 3 4 5  

  6  

  10 11 12 13 14 15  

  2  

  1 1  

  Sample Output  

  Case 1: 22 Xukha  

  Case 2: 88 Xukha  

  Case 3: 4 Xukha
开始学数论，这题是欧拉函数的题目，一开始错在比如找不到欧拉函数值为7的时候，我是去找8的了，找到了15，欧拉函数为8，但是实际上，欧拉函数值为10的对应长度更小，所以这里出错了

```matlab
for(int i=1;i<N;i++){
        for(int j=euler[i]; j>=0 && o[j]==0;j--){
            o[j]=i;
        }
}
```

这里o[j]保存的是欧拉函数为j的最小的数  

这里其实就是枚举长度，比对应的欧拉函数值小的都可以用这个i长度，这样就能保持一个贪心策略

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
const int N=1000010;
int euler[N];
int o[N];
void init(){
    for(int i=1;i<N;i++){
        euler[i]=i;
    }
    for(int i=2;i<N;i++){
        if(euler[i]==i){
            for(int j=i;j<N;j+=i){
                euler[j]=euler[j]/i*(i-1);
            }
        }
    }
    euler[1]=0;
    for(int i=1;i<N;i++){
        for(int j=euler[i]; j>=0 && o[j]==0;j--){
            o[j]=i;
        }
    }
}
int main(void){
    //freopen("out2.txt","w",stdout);
    init();
    int t;
    int n;
    int a;
    int c=1;
    scanf("%d",&t);
    while(t--){
        long long ans=0;
        scanf("%d",&n);
        while(n--){
            scanf("%d",&a);
            ans+=o[a];
        }
        printf("Case %d: %lld Xukha\n",c++,ans);
    }
    // for(int i=1;i<1001;i++){
    //     printf("%d\n",o[i]);
    // }
    return 0;
}
```





