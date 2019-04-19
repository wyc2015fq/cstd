# hdu  4165  Pills - HJ - CSDN博客
2016年07月18日 01:18:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：462
Problem Description 
Aunt Lizzie takes half a pill of a certain medicine every day. She starts with a bottle that contains N pills.
On the first day, she removes a random pill, breaks it in two halves, takes one half and puts the other half back into the bottle.
On subsequent days, she removes a random piece (which can be either a whole pill or half a pill) from the bottle. If it is half a pill, she takes it. If it is a whole pill, she takes one half and puts the other half back into the bottle.
In how many ways can she empty the bottle? We represent the sequence of pills removed from the bottle in the course of 2N days as a string, where the i-th character is W if a whole pill was chosen on the i-th day, and H if a half pill was chosen (0 <= i < 2N). How many different valid strings are there that empty the bottle?
Input 
The input will contain data for at most 1000 problem instances. For each problem instance there will be one line of input: a positive integer N <= 30, the number of pills initially in the bottle. End of input will be indicated by 0.
Output 
For each problem instance, the output will be a single number, displayed at the beginning of a new line. It will be the number of different ways the bottle can be emptied.
Sample Input 
6 
1 
4 
2 
3 
30 
0
Sample Output 
132 
1 
14 
2 
5 
3814986502092304
解析：卡特林（Catalan）数（可以百度）
```cpp
#include<iostream>
using namespace std;
long long catalan(int n)
{
    long long count=1;
    for(int i=1,j=2*n;i<=n;i++,j--)
      count=count*j/i;
    return count;
}
int main()
{
    int n;
    while(cin>>n)
    {
        if(n==0) break;
        cout<<catalan(n)/(n+1)<<endl;
    }
    return 0;
}
```
