# codeforces-758-B Blown Garland（简单数学） - HJ - CSDN博客
2017年01月25日 17:52:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：392
个人分类：[====ACM====																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Nothing is eternal in the world, Kostya understood it on the 7-th of January when he saw partially dead four-color garland.
Now he has a goal to replace dead light bulbs, however he doesn’t know how many light bulbs for each color are required. It is guaranteed that for each of four colors at least one light is working.
It is known that the garland contains light bulbs of four colors: red, blue, yellow and green. The garland is made as follows: if you take any four consecutive light bulbs then there will not be light bulbs with the same color among them. For example, the garland can look like “RYBGRYBGRY”, “YBGRYBGRYBG”, “BGRYB”, but can not look like “BGRYG”, “YBGRYBYGR” or “BGYBGY”. Letters denote colors: ‘R’ — red, ‘B’ — blue, ‘Y’ — yellow, ‘G’ — green.
Using the information that for each color at least one light bulb still works count the number of dead light bulbs of each four colors. 
Input
The first and the only line contains the string s (4 ≤ |s| ≤ 100), which describes the garland, the i-th symbol of which describes the color of the i-th light bulb in the order from the beginning of garland:
```
'R' — the light bulb is red,
'B' — the light bulb is blue,
'Y' — the light bulb is yellow,
'G' — the light bulb is green,
'!' — the light bulb is dead.
```
The string s can not contain other symbols except those five which were described.
It is guaranteed that in the given string at least once there is each of four letters ‘R’, ‘B’, ‘Y’ and ‘G’.
It is guaranteed that the string s is correct garland with some blown light bulbs, it means that for example the line “GRBY!!!B” can not be in the input data. 
Output
In the only line print four integers kr, kb, ky, kg — the number of dead light bulbs of red, blue, yellow and green colors accordingly. 
Examples 
Input
RYBGRYBGR
Output
0 0 0 0
Input
!RGYB
Output
0 1 0 0
Input
!!!!YGRB
Output
1 1 1 1
Input
!GB!RG!Y!
Output
2 1 1 0
Note
In the first example there are no dead light bulbs.
In the second example it is obvious that one blue bulb is blown, because it could not be light bulbs of other colors on its place according to the statements.
题目大意： 
  给出’G’、’B’、’R’、’G’、’!’五种字符，前面四种是确定而最后一种’!’是不确定的，对于任何一个字符串，总是由着五种字符组成，要求计算出字符串中’!’应该填的字符，最后输出四种字符各自应该加上去的数目。（保证能根据已知条件把字符填满）
解题思路： 
  发现循环节，处理四个一循环，每个位置存在一个变量中，最后查找一边累加就好了。
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
char s[10005];
int main()
{
    while(cin>>s)
    {
        int len=strlen(s),aa=0,bb=0,cc=0,dd=0;
        char a,b,c,d;
        for(int i=0;i<len;i++)   //把四个一循环的内容存到a,b,c,d中
        {
            if(s[i]!='!')
            {
                if((i+1)%4==0)
                  a=s[i];
                else if((i+1)%4==1)
                  b=s[i];
                else if((i+1)%4==2)
                  c=s[i];
                else
                  d=s[i];
            }           
        }
        for(int i=0;i<len;i++)
        {
            if(s[i]=='!')
            {
                if((i+1)%4==0)    //为了区分，把新加上去的用小写表示
                  s[i]=a+32;
                else if((i+1)%4==1)
                  s[i]=b+32;
                else if((i+1)%4==2)
                  s[i]=c+32;
                else
                  s[i]=d+32;
            }
        }
        for(int i=0;i<len;i++)    //最后统计输出即可
        {
            if(s[i]=='r')  aa++;
            else if(s[i]=='b')  bb++;
            else if(s[i]=='y')  cc++;
            else if(s[i]=='g')  dd++;
        }
        printf("%d %d %d %d\n",aa,bb,cc,dd);
    }
    return 0;
}
```
