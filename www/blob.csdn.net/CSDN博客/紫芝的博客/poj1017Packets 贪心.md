# poj1017Packets 贪心 - 紫芝的博客 - CSDN博客





2018年07月22日 17:01:40[紫芝](https://me.csdn.net/qq_40507857)阅读数：34
个人分类：[贪心](https://blog.csdn.net/qq_40507857/article/category/7568514)









# Packets
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 48748||**Accepted:** 16512|

Description

A factory produces products packed in square packets of the same height h and of the sizes 1*1, 2*2, 3*3, 4*4, 5*5, 6*6. These products are always delivered to customers in the square parcels of the same height h as the products have and of the size 6*6. Because of the expenses it is the interest of the factory as well as of the customer to minimize the number of parcels necessary to deliver the ordered products from the factory to the customer. A good program solving the problem of finding the minimal number of parcels necessary to deliver the given products according to an order would save a lot of money. You are asked to make such a program.

Input

The input file consists of several lines specifying orders. Each line specifies one order. Orders are described by six integers separated by one space representing successively the number of packets of individual size from the smallest size 1*1 to the biggest size 6*6. The end of the input file is indicated by the line containing six zeros.

Output

The output file contains one line for each line in the input file. This line contains the minimal number of parcels into which the order from the corresponding line of the input file can be packed. There is no line in the output file corresponding to the last ``null'' line of the input file.

Sample Input

```
0 0 4 0 0 1 
7 5 1 0 0 0 
0 0 0 0 0 0
```

Sample Output

```
2 
1
```

题意：

工厂出售边长分别为1,2,3,4,5,6的正方形板子，但工厂只有6*6的板子，其他的板子都是从这种板子上裁剪而来的。

现在给出分别这些板子的需求量，问最少需要多少块6*6的板子。

题解：

从面积大的开始取，一块6*6的板子可以去一块5*5的板子剩下11个 1*1的板子。

取一块4*4的板子剩下5个2*2的板子。 6*6的板子可以取四块3*3的板子，

在一块6*6板子上取3*3的板子数目为  1  2  3  4  的时候剩下 2*2的板子分别为  5  3  1  0，

剩余部分还可以去1*1的板子。 若2*2的板子有剩余，还可以分割成1*1的板子。

```
#include<bits/stdc++.h>
using namespace std;
int a[10];
int dir[4]={0,5,3,1};
//a[2]%4=x,dir[x]表示剩余的2*2板子的数量
int main()
{
    while(1){
        int sum=0;
        for(int i=1;i<7;i++)
        {
            scanf("%d",&a[i]);
            sum+=a[i];
        }
    if(!sum)    break;
    int ans=a[6]+a[5]+a[4]+(a[3]+3)/4;
    int cnt2=a[4]*5+dir[a[3]%4];//2*2板子的剩余最大值
    if(a[2]>cnt2)//当上面板子不够时
        ans+=(a[2]-cnt2+8);
    int cnt1=ans*36-a[6]*36-a[5]*25-a[4]*16-a[3]*9-a[2]*4;
    if(a[1]>cnt1)
        ans+=(a[1]-cnt1+35)/36;
    printf("%d\n",ans);
    }
    return 0;
}
```





