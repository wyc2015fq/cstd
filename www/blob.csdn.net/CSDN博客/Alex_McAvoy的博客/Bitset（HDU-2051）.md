# Bitset（HDU-2051） - Alex_McAvoy的博客 - CSDN博客





2018年03月29日 19:59:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








# Problem Description

    Give you a number on base ten,you should output it on base two.(0<=n<=1000)

# Input

    For each case there is a postive number n on base ten, end of file.

# Output

    For each case output a number on base two.

# Sample Input

**123 **

# Sample Output

**11011**

# Source Program

```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    int a[100];
    int i,j;

    while(cin>>n)
    {
        i=0;//位数计数清零  
        while(n)//进制转换,每一位放在a[i]中
        {
            a[i]=n%2;
            n=n/2;
            i++;
        }
        for(j=i-1;j>=0;j--)//从低位取出，逆序输出
            cout<<a[j];
        cout<<endl;
    }
    return 0;
}
```



