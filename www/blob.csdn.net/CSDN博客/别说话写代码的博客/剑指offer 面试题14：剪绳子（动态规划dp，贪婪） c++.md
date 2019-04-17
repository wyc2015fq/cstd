# 剑指offer 面试题14：剪绳子（动态规划dp，贪婪） c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 20:46:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：110








题目：给定一根长度为n的绳子，请把绳子剪成m段（m、n都是整数，n>1并且m>1），每段绳子的长度记为k[0],k[1],…,k[m]。请问k[0]* k[1] * … *k[m]可能的最大乘积是多少？例如，当绳子 的长度为8时，我们把它剪成长度分别为2,3,3的三段，得到的长度最大乘积是 18.

思路：

（1）动态规划dp，f(n)=max(f(i)*f(n-i))，当绳子长度为3时，最好的方法只能是剪为1和2，绳子长度为2，只能剪为1,1

f(4)=f(2)*f(2)=4     f(4)=f(1)*f(3)=3

f(5)=f(1)*f(4)=4     f(5)=f(2)*f(3)=6

f(6)=f(1)*f(5)=6     f(6)=f(2)*f(4)=8     f(6)=f(3)*f(3)=9

```cpp
class Solution
{
public:
    int maxProdeuctCutting(int length)
    {
        if(length < 2) return 0; //要切的是整数不符合条件
        if(length == 2) return 1;
        if(length == 3) return 2;
        int *products = new int[length+1]; // 生成数组保存计算结果
        products[0]=0;
        products[1]=1;
        products[2]=2;
        products[3]=3;  //这几个比较特殊，分割结果是预先定义好
        int max = 0;
        for(int i=4;i<length;++i)
        {
            max=0;
            for(int j=1;j<=i/2;++j)
            {
                int product = products[j] * products[i-j];
                if(product > max) max =product;
            }products[i] = max;
            
        }
        return products[length];
    }
};
```

(2)贪婪法  可以按照这个策略：当n>=5，尽可能多的剪 长度为 3的绳子，当剩下的长度为4时，把绳子剪成两段长度为2的绳子

```cpp
class Solution{
public:
    int maxproductcutting(int length)
    {
        if(length<2) return 0;
        if(length==2) return 1;
        if(length==3) return 2;
        int timesof3 = length/3;  //尽可能多剪长度为3的绳子
        if(length-timesof3*3 == 1) timesof3 -= 1;  //当绳子剩下的长度为4，应该剪为两个2
        int timesof2 = (length-timesof3*3)/2;  //2有几个
        return (int)(pow(3,timesof3))*(int)(pow(2,timesof2));
    }
};
```





