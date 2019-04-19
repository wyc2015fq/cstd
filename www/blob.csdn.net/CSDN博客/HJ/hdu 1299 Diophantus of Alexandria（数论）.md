# hdu 1299  Diophantus of Alexandria（数论） - HJ - CSDN博客
2016年04月25日 15:24:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：569
Problem Description 
Diophantus of Alexandria was an egypt mathematician living in Alexandria. He was one of the first mathematicians to study equations where variables were restricted to integral values. In honor of him, these equations are commonly called diophantine equations. One of the most famous diophantine equation is x^n + y^n = z^n. Fermat suggested that for n > 2, there are no solutions with positive integral values for x, y and z. A proof of this theorem (called Fermat’s last theorem) was found only recently by Andrew Wiles.
Consider the following diophantine equation: 
1 / x + 1 / y = 1 / n where x, y, n ∈ N+ (1)
Diophantus is interested in the following question: for a given n, how many distinct solutions (i. e., solutions satisfying x ≤ y) does equation (1) have? For example, for n = 4, there are exactly three distinct solutions: 
1 / 5 + 1 / 20 = 1 / 4 
1 / 6 + 1 / 12 = 1 / 4 
1 / 8 + 1 / 8 = 1 / 4
Clearly, enumerating these solutions can become tedious for bigger values of n. Can you help Diophantus compute the number of distinct solutions for big values of n quickly?
Input 
The first line contains the number of scenarios. Each scenario consists of one line containing a single number n (1 ≤ n ≤ 10^9). 
Output 
The output for every scenario begins with a line containing “Scenario #i:”, where i is the number of the scenario starting at 1. Next, print a single line with the number of distinct solutions of equation (1) for the given value of n. Terminate each scenario with a blank line. 
Sample Input 
2 
4 
1260
Sample Output 
Scenario #1: 
3
Scenario #2: 
113
分析题意，就是给我们一个整数，要求算出这个整数的倒数可以分成两个整数倒数和的个数有多少。
从题目给的例子来说吧 
譬如给你整数4，则可以得到以下结果，即可知输出应该为3
1 / 5 + 1 / 20 = 1 / 4 
1 / 6 + 1 / 12 = 1 / 4 
1 / 8 + 1 / 8 = 1 / 4
通常给出的思路是：我可以暴力求解啊，比如输入4， 我就从5开始，一直寻找，直到所有结果都找到为止。于是就要考虑循环的跳出条件，因为开始你并不知道这个数的结果有多少种。运用数学知识，1 / x + 1 / y = 1 / n，假设x,y一大一小，即x>=y。则1/x<=1/y，所以有1/n<=1/y+1/y=2/y，变形得到较小的那个数y的范围是y<2n（注：x,y都必须大于n)。
因此用循环解答可以得到如下 
for(int y=n+1;y<=2n;y++)    //时间复杂度为n，即线性运算 
…
范围条件都选好了，那么怎么判断该数是否满足条件呢？ 
将式子1 / x + 1 / y = 1 / n变形，得到1/x=1/n-1/y，再通分，将等式两边同时分子分母交换位置。 
得到x=yk/(y-k) ，由x规定为整数可知，只要判断yk是否整除y-k即可得到结果是否满足条件。 
另外，如果要考虑y是不是数据过大，可以进一步变形，令y=k+i，而x=k^2/i+k， 
因此只要判断k的平方是否整除i即可。 
得到的循环为for(int i=1;i<=k;i++)。
不过要注意，题目所给的n的范围最大是10的九次方，按照常理来说，应该是超时了，不信的话诸位可以试试，输出的结果不是WA，而是Time Limit Exceeded。因为题目所给的数据太大了。
在这里要另外转换思路求解了
既然线性运算超时，就必然要减少一些不必要数字的循环，因此思考方向就要转换到在n到2n之间，有那些数字是满足条件的，要想出一个筛选的条件。
继续看公式1 / x + 1 / y = 1 / n，前面一种思路已经打开了我们的思维，得到了公式x=k^2/i+k，用k的平方是否整除i来判断是否满足条件。我们知道，一个数k，如果整除i（i大于0，小于k），那么i一定就是k的因子了，不过在这里我们要的是k的平方的因子个数。
很容想到唯一分解定理：对于任一自然数n皆可唯一的表示为素数之积 
N = p1^a1 * p2^a2 * p3^a3 * … * pn^an
具体实现看代码：
```
#include<iostream>
using namespace std;
int main()
{
    long long s[100000],t;
    long long i,j,k=0;
    memset(s,0,sizeof(s));
    for(i=2;i<100000;i++)
    {
        if(s[i]==0)
        {
            for(j=j*2;j<100000;j=j+i)
              s[j]=1;
        }
    }
    for(i=2;i<100000;i++)
    {
        if(s[i]==0)
          s[k++]=i;
    }
    s[k]='\0';        //以上即为素数打表模板，规模为10的五次方
    cin>>t;
    for(int count=1;count<=t;count++)
    {
        long long a,h,sum=1;
        cin>>a;
        for(i=0;s[i]!='\0';i++)
        {
            h=0;
            while(a%s[i]==0)  //对同一个素数一直整除，直到除不尽为止，减少循环次数
            {
                a=a/s[i];
                h++;    //记录下素数的个数
            }
            sum=sum*(2*h+1);  //由k的素因子个数转换得到k的平方的素因子的个数
        }
        if(a>1) sum=sum*3;   //如果得到的a是一个素数，另外处理
        sum=sum/2+1;    //常规处理，求出最后满足条件的个数
        cout<<"Scenario #"<<count<<":"<<endl;
        cout<<sum<<endl<<endl;
    }
    return 0;
}
```
