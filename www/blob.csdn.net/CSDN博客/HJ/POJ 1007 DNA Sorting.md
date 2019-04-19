# POJ  1007  DNA Sorting - HJ - CSDN博客
2016年07月18日 02:02:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：255
POJ  1007  DNA Sorting
Description
One measure of `unsortedness'' in a sequence is the number of pairs of entries that are out of order with respect to each other. For instance, in the letter sequence`DAABEC”, this measure is 5, since D is greater than four letters to its right and E is greater than one letter to its right. This measure is called the number of inversions in the sequence. The sequence `AACEDGG'' has only one inversion (E and D)---it is nearly sorted---while the sequence`ZWQM” has 6 inversions (it is as unsorted as can be—exactly the reverse of sorted). 
You are responsible for cataloguing a sequence of DNA strings (sequences containing only the four letters A, C, G, and T). However, you want to catalog them, not in alphabetical order, but rather in order of `sortedness'', from`most sorted” to “least sorted”. All the strings are of the same length. 
Input 
The first line contains two integers: a positive integer n (0 < n <= 50) giving the length of the strings; and a positive integer m (0 < m <= 100) giving the number of strings. These are followed by m lines, each containing a string of length n.
Output 
Output the list of input strings, arranged from `most sorted'' to`least sorted”. Since two strings can be equally sorted, then output them according to the orginal order.
Sample Input 
10 6 
AACATGAAGG 
TTTTGGCCAA 
TTTGGCCAAA 
GATCAGATTT 
CCCGGGGGGA 
ATCGATGCAT
Sample Output 
CCCGGGGGGA 
AACATGAAGG 
GATCAGATTT 
ATCGATGCAT 
TTTTGGCCAA 
TTTGGCCAAA
解析：题目大致的意思是出现一个字符串，由A,C,T,G四个字母重复排列组成，要你计算逆序数，然后按逆序数大小来输出输入的字符串。由于字符串跟逆序数两个绑定在一起，所以建议用结构体，再用sort排序就OK了。
代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<string>
#include<cstdio>
#include<cstring>
using namespace std;
struct str             //要用结构体绑定
{
    char d[120];
    int sum;
}s[120];
int q(str a,str b)    //结构体绑定排序
{
    if(a.sum==b.sum) 
      return 1;
    else 
      return a.sum<b.sum;
}
int main()
{
    int m,n;
    while(cin>>m>>n)
    {
      for(int i=0;i<n;i++)
      {
          cin>>s[i].d;
          s[i].sum=0;
          for(int j=0;s[i].d[j]!='\0';j++)
          {
              for(int l=j+1;s[i].d[l]!='\0';l++)
                if(s[i].d[j]>s[i].d[l])
                  s[i].sum++;             //计算个数
          }
      }
      sort(s,s+n,q);     //sort排序
      for(int i=0;i<n;i++)
        cout<<s[i].d<<endl;
    }
    return 0;
}
```
