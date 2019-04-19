# NBUT  1217    Dinner - HJ - CSDN博客
2016年08月19日 19:04:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：242
个人分类：[====ACM====](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
Little A is one member of ACM team. He had just won the gold in World Final. To celebrate, he decided to invite all to have one meal. As bowl, knife and other tableware is not enough in the kitchen, Little A goes to take backup tableware in warehouse. There are many boxes in warehouse, one box contains only one thing, and each box is marked by the name of things inside it. For example, if “basketball” is written on the box, which means the box contains only basketball. With these marks, Little A wants to find out the tableware easily. So, the problem for you is to help him, find out all the tableware from all boxes in the warehouse.
Input 
There are many test cases. Each case contains one line, and one integer N at the first, N indicates that there are N boxes in the warehouse. Then N strings follow, each string is one name written on the box.
Output 
For each test of the input, output all the name of tableware.
Sample Input 
3 basketball fork chopsticks 
2 bowl letter
Sample Output 
fork chopsticks 
bowl
Hint
The tableware only contains: bowl, knife, fork and chopsticks.
非常水的字符串比较题！ 
放到这里来纯粹是为了强调格式，强调格式，强调格式！！！
代码如下：
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
    char s[4][11]={"bowl","knife","fork","chopsticks"}; 
    int n;
    while(cin>>n)
    {
        int blag=0;
        for(int i=0;i<n;i++)
        {
            char a[30];
            cin>>a;
            for(int j=0;j<4;j++)
              if(strcmp(a,s[j])==0)
              {
                 if(!blag)   //控制格式，如果输出的字符串为第一个，则直接输出该字符串并做上标记
                 {
                    cout<<a;
                    blag=1;
                 }
                 else          //标记上之后从第二个开始，在输出每个字符串之前先输出一个空格
                   cout<<" "<<a;
              }
        }
        cout<<endl;   //最后输出换行
    } 
    return 0;
}
```
注：每个人的控制格式的风格都有不用，在这里只是提出一种建议，一个方法，并不强求。
