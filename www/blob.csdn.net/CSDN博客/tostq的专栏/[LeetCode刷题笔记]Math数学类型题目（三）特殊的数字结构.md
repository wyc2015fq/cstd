# [LeetCode刷题笔记]Math数学类型题目（三）特殊的数字结构 - tostq的专栏 - CSDN博客





2016年07月22日 22:11:45[tostq](https://me.csdn.net/tostq)阅读数：704标签：[leetcode																[c语言																[题库																[算法																[数学](https://so.csdn.net/so/search/s.do?q=数学&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/tostq/article/category/6309640)






**原创文章**


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)


特殊的数字结构这类题一般都是折腾数字本身，特别要注意到观察数字的结构和特征。

常见的出题类型主要分为了回文数字、数字翻转、数字分割等几类。




**一、回文数（对称数）**

**9. Palindrome Number**

判断一个数是否是回文数（即数是对称的，如7654567）

这题的难点在于要求不能额外分配空间，也就是说我们不能先通过保存倒置数位，再来进行头尾数位的相互比较。换句话说，题目要求我们要同时比较头尾。

**（1）一种可行的方法是同时保存首尾数字**


```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0)return false;
        // wei
        int w;
        int xtemp=x;
        for(w=0;xtemp!=0;w++)xtemp=xtemp/10;
        //printf("%d",w);
        
        int h=x,t=x;
        int wh=w-1,wt=1;
        while(h==t&&wh>=0){
            t=x%10;
            h=x/pow(10,wh);
            x=(x-h*pow(10,wh))/10;
            wh=wh-2;
            //printf("|t=%d,h=%d,x=%d,wh=%d|",t,h,x,wh);
        }
        return (h==t)?true:false;
    }
};
```


**（2）另一种更为巧妙的方法，是先计算数字的倒转数，再比较两个数是否相等**


```cpp
bool isPalindrome(int x) {
    if(x<0)return false;
    int w;
    int xtemp=x;
    for(w=0;xtemp!=0;w++)xtemp=xtemp/10;
    
    xtemp=x;
    int cmp=0;
    while(w--){
        cmp=cmp*10+xtemp%10;
        xtemp=xtemp/10;
    }
    return cmp==x;
}
```



**二、数字翻转**

**7. Reverse Integer**

输入：x = 123, 输出：321

输入：x = -123, 输出：-321

这道并不难，前面的回文介绍了数字倒转的方法，然而这道题的正确提交率并不高。

主要原因在于数据溢出，比如-2147483645，其数字倒转后肯定是溢出了，这个时候我们需要返回0，而实际上是不进行处理的话，返回肯定是非0的。

这里我们介绍一种非常简单判断是否溢出的方法，比如判断x*y=z是否溢出，只需要判断z/y==x就可以了。下面是具体的解法：


```cpp
class Solution {
public:
    int reverse(int x) {
        int sign=(x>0)?1:-1;
        x=(x>0)?x:-x;
        int res=0;
        while(x>0){
            int newres=res*10+x%10;
            if((newres-x%10)%10!=0||(newres-x%10)/10!=res)return 0; // good
            x=x/10;
            res=newres;
        }
        return sign*res;
    }
};
```



**三、高兴数**

**202. Happy Number**

高兴数定义了这样一个过程：将数字的各位数平方相加得到一个新数，然后将这个新数的各位数平方继续相加，如果这个过程最终结果为1，则为高兴数，如果一直循环的话，则不是高兴数。

比如：19 就是一个高兴数

1^2 + 9^2 = 82

8^2 + 2^2 = 68

6^2 + 8^2 = 100

1^2 + 0^2 + 0^2 = 1

**（1）通过Hash表记录**。

一种比较直观的方法，就是记录这个过程，来判断是否出现过循环，

这里我们可以引入前面介绍的Hash表unordered_map


```cpp
class Solution {
public:
    bool isHappy(int n) {
        unordered_map<int,bool> m;
        while(1){
            if(m.find(n)==true)return false;
            if(n==1)return true;
            m.insert(make_pair(n,true));
            int temp=0;
            while(n!=0){
                temp=temp+(n%10)*(n%10);
                n=n/10;
            }
            n=temp;
        }
    }
};
```



**（2）Floyd 循环定理**

另一种更好的方法，是利用循环定理

Floyd 循环定理表明这样的一个现象：速度不同的两人，在一个环形跑道上同时起跑，那么未来他们两个肯定会在某一时刻相遇。

因此我们有如下的方法。


```cpp
//Floyd Cycle detection algorithm
int powsum(int n){
    int sum=0;
    while(n!=0){
        sum=sum+(n%10)*(n%10);
        n=n/10;
    }
    return sum;
}
bool isHappy(int n) {
    int slow=n,fast=n;
    do{
        slow=powsum(slow);
        fast=powsum(fast);
        fast=powsum(fast);
    }while(slow!=fast);
    if(slow==1)return true;
    return false;
}
```



**四、组合序列**

**60. Permutation Sequence**

这道题是给定两个整数n（n=1~9），k，计算n!个组合数中的第k个的值，需要返回的是字符串形式。比如给n=3，k=6，有如下：

"123"

"132"

"213"

"231"

"312"

"321"

……

所以我们返回的是"321"

**解题的思路：**

1、因为我们已经知道n的大小，所以我们可以提前将一些存储起来。

2、注意序列是从1开始的，而不是从0开始的

3、这里是可以用递归的

**具体解法：**


```cpp
class Solution {
private:
    int fact[9] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320};
public:
    string getPermutation(int n, int k) {
        if(n==1)return "1";
        string s="";
        int div=(k-1)/fact[n-1]+1;
        int mod=(k-1)%fact[n-1]+1;
        string s1=getPermutation(n-1,mod);
        for(int i=0;i<s1.length();i++)
            if(s1[i]-'0'>=div)s1[i]++;
        s+=(char)(div+'0')+s1;
        return s;
    }
};
```



**五、数字分割**

**343. Integer Break**

这道题是给定一个正整数，将这个正整数分成多个正整数的和，使其分割的这些数相乘最大

比如n = 10, 返回 36 (10 = 3 + 3 + 4).

**（1）动态归划来解决**

这道题比较直观的方式是通过动态归划来解决，因为我们可以看到，n的结果同n-i的结果有明显的关系，假设n的结果为dp[n]。

则dp[n]=MAX(max(dp[n-i],(n-i))*max(i,dp[i]))

具体的解法：


```cpp
#define max(x,y) (x>y?x:y)
int integerBreak(int n) {
    if(n<4)return n-1;
    int* dp=(int*)malloc((n+1)*sizeof(int));
    dp[0]=0,dp[1]=0;
    dp[2]=1; //n=2
    dp[3]=2; //n=3
    
    for(int i=4;i<=n;i++){ // from 4
        int maxpro=0;
        for(int j=i-1;j>1;j--){
            maxpro=max(maxpro,max(j,dp[j])*max(i-j,dp[i-j]));
        }
        dp[i]=maxpro;
    }
    return dp[n];
}
```


**（2）另外，我们可以观察数的本身特征**

我们可以直观地认为如果把一个数分解成越来越多的份，其相乘应该是越来越大的

考虑到n=a+b，a+b<ab，仅当a或b等于1，或a=b=2时，不成立。

所以当我们把数分成多个3相加的形式，最后乘应该是最大的。

但是还需要考虑最后的余数。

如果余数为1，最后应该写成3*4，而不是3*3*1。

而余数为2，还是可以写成3*3*2，其是大于3*5的。

具体解法：


```cpp
int integerBreak(int n) {
    if(n<4)return n-1;
    int ret=n%3;
    int dbn=n/3;
    if(ret<2)return (3+ret)*(long long)pow(3,dbn-1);
    else return ret*(long long)pow(3,dbn);
}
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=题库&t=blog)](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




