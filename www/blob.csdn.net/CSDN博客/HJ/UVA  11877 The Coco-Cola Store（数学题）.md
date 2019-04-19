# UVA   11877  The Coco-Cola Store（数学题） - HJ - CSDN博客
2016年08月20日 09:45:47[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：364
个人分类：[====ACM====																[----------数学----------																[递推公式&结论题																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308108)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Once upon a time, there is a special coco-cola store. If you return three empty bottles to the shop, 
you’ll get a full bottle of coco-cola to drink. If you have n empty bottles right in your hand, how many full bottles of coco-cola can you drink?
Input 
There will be at most 10 test cases, each containing a single line with an integer  n(1大于等于n小于100). The input terminates with n= 0, which should not be processed.
Output 
For each test case, print the number of full bottles of coco-cola that you can drink.
Spoiler 
Let me tell you how to drink 5 full bottles with 10 empty bottles: get 3 full bottles with 9 empty 
bottles, drink them to get 3 empty bottles, and again get a full bottle from them. Now you have 2 empty bottles.Borrowanotheremptybottlefromtheshop, then get another full bottle. Drink it, and nally return this empty bottle to the shop!
SampleInput 
3 
10 
81 
0
SampleOutput 
1 
5 
40
解析：基础数学题，只要一些情况考虑清楚即可，代码如下：
```cpp
#include<iostream>
#include<algorithm>
using namespace std; 
int main()
{
    int n,s,k;
    while(cin>>n)
    {  
        s=0;
        if(n==0)
          break;
        if(n==1)    //如果只有一瓶，则只能喝一瓶
          s=1;
        while(n>2)   //如果大于两瓶，则根据题目要求，每喝三瓶可以兑换一瓶
        {
            s=s+n/3;
            k=n/3+n%3;  
            n=k;      //迭代
        }
        if(n==2)  //最后如果还剩两瓶，则可以先赊一瓶，然后喝完三瓶再换一瓶还给老板，因此还能多喝一瓶
          s=s+1;
        cout<<s<<endl;
    }
    return 0 ;
}
```
