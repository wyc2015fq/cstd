# HDU6188 Duizi and Shunzi 贪心 - 紫芝的博客 - CSDN博客





2018年05月13日 13:06:00[紫芝](https://me.csdn.net/qq_40507857)阅读数：67










# Duizi and Shunzi
****Time Limit: 6000/3000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 2015    Accepted Submission(s): 807****


Problem Description

Nike likes playing cards and makes a problem of it.

Now give you n integers, 1

We define two identical numbers (eg: 22) a Duizi,
and three consecutive positive integers (eg: 234) a Shunzi.

Now you want to use these integers to form Shunzi and Duizi as many as possible.

Let s be the total number of the Shunzi and the Duizi you formed.

Try to calculate .

Each number can be used only once.





Input

The input contains several test cases.

For each test case, the first line contains one integer n(1106). 
Then the next line contains n space-separated integers  (1)





Output

For each test case, output the answer in a line.





Sample Input


7

1 2 3 4 5 6 7

9

1 1 1 2 2 2 3 3 3

6

2 2 3 3 3 3 

6

1 2 3 3 4 5




Sample Output


2

4

3

2

*Hint*
Case 1（1，2，3）（4，5，6）Case 2（1，2，3）（1，1）（2，2）（3，3）Case 3（2，2）（3，3）（3，3）Case 4（1，2，3）（3，4，5）






Source

[2017ACM/ICPC广西邀请赛-重现赛（感谢广西大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2017ACM%2FICPC%B9%E3%CE%F7%D1%FB%C7%EB%C8%FC-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%B9%E3%CE%F7%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)




Recommend

liuyiding   |   We have carefully selected several similar problems for you:  [6275](http://acm.hdu.edu.cn/showproblem.php?pid=6275)[6274](http://acm.hdu.edu.cn/showproblem.php?pid=6274)[6273](http://acm.hdu.edu.cn/showproblem.php?pid=6273)[6272](http://acm.hdu.edu.cn/showproblem.php?pid=6272)[6271](http://acm.hdu.edu.cn/showproblem.php?pid=6271)






题意：

给出n个数字，两个相同的数字可以组成对子，


三个连续的数字可以组成顺子，问能组成对子和顺子的总数最大是多少




思路：


统计每个元素的个数，从头开始循环，对于前两个元素，
判断能不能组成对子，能组成就加上相应的数量，并减去用掉的元素。
从第三个元素起，先检查第三个元素能不能与前面两个元素组成顺子，




然后再判断能不能组成对子，减去和加上相应的数量


```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e6+10;//数字最大为10^6
int a[maxn];//a[i]表示i这个数字出现的次数
int main()
{
    int n;//表示有n个数字
    while(scanf("%d",&n)!=EOF){//多组样例测试模板
        memset(a,0,sizeof(a));//初始化每个数字的个数为0
        int duizi=0;//对子的个数
        int  shunzi=0;//顺子的个数
        int x;//每个数字
        for(int i=0;i<n;i++)//一共有n个数字
        {
            scanf("%d",&x);
            a[x]++;//数字x的个数+1
        }
        for(int i=1;i<maxn;i++)//从前到后遍历每个容器
        {
            if(i<=2)//对于前两个元素
            {
                if(a[i]>=2){//判断能不能组成对子
                    int num=a[i]/2;//对子的个数
                a[i]-=2*num;//并减去用掉的元素
                duizi+=num;//能组成就加上相应的数量
                }
            }
            else
            {   //从第三个元素起，先检查第三个元素能不能与前面两个元素组成顺子
                if(a[i]>0&&a[i-1]>0&&a[i-2]>0)
            {
                shunzi++;
                a[i]--;a[i-1]--;a[i-2]--;
            }
                if(a[i]>=2){//判断能不能组成对子
                    int num=a[i]/2;
                //减去和加上相应的数量
                a[i]-=2*num;
                duizi+=num;
                }
            }
        }
        printf("%d\n",duizi+shunzi);
    }
    return 0;
}
```







