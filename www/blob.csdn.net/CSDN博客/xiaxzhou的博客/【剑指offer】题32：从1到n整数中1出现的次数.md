# 【剑指offer】题32：从1到n整数中1出现的次数 - xiaxzhou的博客 - CSDN博客





2017年07月03日 17:46:45[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：158标签：[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









剑指offer解法：

> 
重点理解：


1346~11345中后4位中1出现的次数 

与 

10000~20000中后4位中1出现的次数一样。
这是因为1346~10000中后4位1出现的次数和11346~20000一样

```cpp
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace::std;

int func(char* strN)
{
    if (!strN || *strN<'0'||*strN>'9'||*strN == '\0')
    {
        return 0;
    }
    int first = *strN - '0';
    int leng = strlen(strN);
    if (leng == 1&&first == 0)
        return 0;
    if (leng== 1&&first>0)
        return 1;

    int numFirstDigit = 0;

    if (first>1)//最高位出现的1的次数
    {
        numFirstDigit = pow(10, leng - 1);
    }
    else
    {
        numFirstDigit = atoi(strN + 1) + 1;
    }

    int numOterDigit = first * (leng - 1)*pow(10, leng - 2);//其他位出现1次数

    int numRecursive = func(strN + 1);//递归求下一个数据段

    return numFirstDigit + numOterDigit + numRecursive;
}

int main()
{
    int n(0);
    cin >> n;
    char str[50];
    sprintf(str, "%d", n);
    cout<<func(str);

    return 0;

}
```

动态规划解法：

> 
使用$all[i]$表示所有小于等于i位的数字中1出现的次数


如： 

all[3]表示 0-999 中1出现的次数 

all[4]表示 0-9999 中1出现的次数
> 
使用$part[i]$表示所有小于等于i位的数字且小于等于整数n的第1-i位组成的数字中1出现的次数


如：n = 5354， 则 : 
$part[2]$表示0-54中1出现的次数 
$part[3]$表示0-354中1出现的次数 
$part[4]$表示0-5354中1出现的次数
则状态转换方程： 


$all[i]=all[i-1]*10 + 10^{i-1};$

> 
解释： 
**all[i-1]*10表示：先不考虑第i位的1**： 

  all[i-1]*10表示第i位的有0-9共10种可能，则每种可能会增加all[i-1]个1； 
**10^{i-1}表示：单独考虑第i位的1**:  

  第i位固定为1时，剩下i-1位有10^{i-1}种可能


$part[i] =\begin{cases}all[i-1]*k+part[i-1]+0&k==0\\all[i-1]*k+part[i-1]+num[i-1]+1&k==1\\all[i-1]*k+part[i-1]+10^{i-1} &k>1\\\end{cases}$

其中 

k表示第i位的数字。 

num[i-1]表示前i-1位组成的数字：如n = 5354，num[2] = 54,num[3] = 354,
> 
**解释**： 

  all[i-1]*k表示第i位从0增加到k-1，且不考虑第i位的1时，增加的1的个数 

  part[i-1]表示第i位为k时，不考虑第i位的1时，增加的1的个数 

  0表示第i位为0时第i位的1的个数 

  num[i-1]+1表示第i位为1时第i位的1的个数 

  10^{i-1} 表示第i位大于1时第i位的1的个数
```
int NumberOf1Between1AndN_Solution(int n)
{
    int pre_allnum(0);
    int pre_part(0);
    int cur_allnum(0);
    int cur_part(0);

    int index(0);
    int partn(0);
    while (n)
    {
        index++;
        int k = n % 10;
        n = n / 10;
        cur_allnum = pre_allnum * 10 + pow(10,(index-1));

        cur_part = pre_allnum*(k);
        cur_part += pre_part;
        if (k==1)
        {
            cur_part += partn+1;

        }else if (k > 1)
        {
            cur_part += pow(10, (index - 1));
        }

        pre_allnum = cur_allnum;
        pre_part = cur_part;
        partn += k * pow(10, (index - 1));
    }

    return cur_part;

}
```











