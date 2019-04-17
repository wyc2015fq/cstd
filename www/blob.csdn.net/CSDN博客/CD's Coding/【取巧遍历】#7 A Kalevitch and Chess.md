# 【取巧遍历】#7 A. Kalevitch and Chess - CD's Coding - CSDN博客





2014年03月31日 22:10:00[糖果天王](https://me.csdn.net/okcd00)阅读数：826标签：[c++																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















A famous Berland's painter Kalevitch likes to shock the public. One of his last obsessions is chess. For more than a thousand years people have been playing this old game on uninteresting, monotonous boards. Kalevitch decided to put an end to this tradition
 and to introduce a new attitude to chessboards.


As before, the chessboard is a square-checkered board with the squares arranged in a 8 × 8 grid, each square is painted black or white. Kalevitch suggests that chessboards
 should be painted in the following manner: there should be chosen a horizontal or a vertical line of 8 squares (i.e. a row or a column), and painted black. Initially the whole chessboard is white, and it can be painted in the above described way one or more
 times. It is allowed to paint a square many times, but after the first time it does not change its colour any more and remains black. Kalevitch paints chessboards neatly, and it is impossible to judge by an individual square if it was painted with a vertical
 or a horizontal stroke.


Kalevitch hopes that such chessboards will gain popularity, and he will be commissioned to paint chessboards, which will help him ensure a comfortable old age. The clients will inform him what chessboard they want to have, and the painter will paint a white
 chessboard meeting the client's requirements.


It goes without saying that in such business one should economize on everything — for each commission he wants to know the minimum amount of strokes that he has to paint to fulfill the client's needs. You are asked to help Kalevitch with this task.




Input


The input file contains 8 lines, each of the lines contains 8 characters. The given matrix describes the client's requirements, W character stands for a white
 square, and B character — for a square painted black.


It is guaranteed that client's requirments can be fulfilled with a sequence of allowed strokes (vertical/column or horizontal/row).




Output


Output the only number — the minimum amount of rows and columns that Kalevitch has to paint on the white chessboard to meet the client's requirements.




Sample test(s)




input
WWWBWWBW
BBBBBBBB
WWWBWWBW
WWWBWWBW
WWWBWWBW
WWWBWWBW
WWWBWWBW
WWWBWWBW




output
3




input
WWWWWWWW
BBBBBBBB
WWWWWWWW
WWWWWWWW
WWWWWWWW
WWWWWWWW
WWWWWWWW
WWWWWWWW




output
1












C++：



题意是说，要把一个8X8的棋盘的黑色（B）的都刷白（W），一刷就一行或者一列，问最少刷多少次

这个的话我们可以先遍历一行全是黑的的有多少行，然后再看列需要刷多少行

官方标签：暴力，构造算法



```cpp
#include <iostream>
#include <cstring>
using namespace std;
int main()
{
    char s[8][8];
    int sum=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cin>>s[i][j];
        }
    }
    int sum2=0;
    int sum3=0;
    for(int i=0;i<8;i++)
    {
        int sum1=0;
        for(int j=0;j<8;j++)
        {
            if(s[i][j]=='B')
            sum1++;
        }
        if(sum1==8)
        sum2++;
        else
        sum3=sum1; //优化关键：横刷刷完了之后看看剩下点啥全是纵刷 虽然个人觉得为了不被Hack要不要加一个 (if sum1>sum3) sum3=sum1 
    }
    cout<<sum2+sum3<<endl;
    return 0;
}
```


Python：



```python
# input
matrix = []
for i in range(8):
    matrix.append(raw_input())

# getAns
ans = 0
# row
for i in range(8):
    if matrix[i][0] == 'B':
       for j in range(8):
           if matrix[i][j] == 'W':
              break
       if j == 7 and matrix[i][j] == 'B':
          ans += 1        

# col
for i in range(8):
    if matrix[0][i] == 'B':
       for j in range(8):
           if matrix[j][i] == 'W':
              break
       if j == 7 and matrix[j][i] == 'B':
           ans += 1

if ans == 16:
   ans = 8
print ans
```








