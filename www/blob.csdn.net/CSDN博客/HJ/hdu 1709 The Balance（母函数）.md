# hdu  1709  The Balance（母函数） - HJ - CSDN博客
2017年02月02日 01:36:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：412
Problem Description 
Now you are asked to measure a dose of medicine with a balance and a number of weights. Certainly it is not always achievable. So you should find out the qualities which cannot be measured from the range [1,S]. S is the total quality of all the weights.
Input 
The input consists of multiple test cases, and each case begins with a single positive integer N (1<=N<=100) on a line by itself indicating the number of weights you have. Followed by N integers Ai (1<=i<=N), indicating the quality of each weight where 1<=Ai<=100.
Output 
For each input set, you should first print a line specifying the number of qualities which cannot be measured. Then print another line which consists all the irrealizable qualities if the number is not zero.
Sample Input
3 
1 2 4 
3 
9 2 1
Sample Output
0 
2 
4 5
Source 
HDU 2007-Spring Programming Contest
题目大意： 
给你n个砝码，总质量和为sum（n），现在给你一个天平，要你列出从1到sum之间不能用砝码称出来的重量。
解题思路： 
首先应该注意，天平两边都可以放砝码，用母函数解决
放在同一端：
```
a[j+k]+=a[j]
```
不同端：
```
a[abs(j-k)]+=a[j]
```
假设原来的砝码都放在右端，则可以把新加的砝码放在左端，得到新重量,此时
```
a[abs(j-k)]+=a[j]。
```
代码如下：
```cpp
#include <iostream>  
    #include <cstring>  
    #include <cmath>  
    using namespace std;  
    #define maxn 101   
    int a[maxn*maxn],b[maxn*maxn],c[maxn];  
    int main()  
    {  
        int n;  
        while(cin>>n)  
        {  
            int sum = 0;  
            for(int i = 0; i < n; i++)  
            {  
                cin>>c[i];  
                sum+=c[i];  
            }  
            memset(a,0,sizeof(a));  
            memset(b,0,sizeof(b));  
            for(int i = 0; i <= c[0]; i+=c[0])  
                a[i] = 1;  
            //int now = c[0];  
            for(int i = 1; i < n; i++)  //1。i代表第i个表达式   
            {  
                for(int j = 0; j <= sum; j++)  //2。j表示每个表达式的第i项   
                {  
                    for(int k = 0; k <= c[i]; k+=c[i])  //k代表每个表达式第i项的指数   
                    {  
                        b[j+k] += a[j];  
                        b[abs(j-k)] +=a[j];  
                    }  
                }  
                //now += c[i];  
                for(int j = 0; j <= sum; j++)  
                {  
                    a[j] = b[j];  
                    b[j] = 0;  
                }  
            }  
            int num = 0;  
            for(int i = 0; i <= sum; i++ )  
            {  
                if(!a[i])  
                    num++;  
            }  
            cout<<num<<endl;  
            int p = 1;  
            if(num){  
                for(int i = 0; i <= sum; i++)  
                {  
                    if(!a[i])  
                    {  
                        if(!p)  
                        cout<<" "<<i;  
                        else  
                        {  
                            cout<<i;  
                            p = 0;  
                        }      
                    }  
                }  
                cout<<endl;  
            }  
        }  
    }
```
