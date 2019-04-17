# 【pair_简单贪心】#3 A. Shortest path of the king - CD's Coding - CSDN博客





2014年03月31日 21:15:07[糖果天王](https://me.csdn.net/okcd00)阅读数：592标签：[python																[c++																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)













The king is left alone on the chessboard. In spite of this loneliness, he doesn't lose heart, because he has business of national importance. For example, he has to pay an official visit to square *t*.
 As the king is not in habit of wasting his time, he wants to get from his current position *s* to square *t* in
 the least number of moves. Help him to do this.
![](http://espresso.codeforces.com/d81704de61029c805216267515307935311c49e0.png)

In one move the king can get to the square that has a common side or a common vertex with the square the king is currently in (generally there are 8 different squares he can move to).




Input


The first line contains the chessboard coordinates of square *s*, the second line — of square *t*.


Chessboard coordinates consist of two characters, the first one is a lowercase Latin letter (from a to h),
 the second one is a digit from 1 to 8.




Output


In the first line print *n* — minimum number of the king's moves. Then in *n* lines
 print the moves themselves. Each move is described with one of the 8: L, R, U, D, LU, LD, RU or RD.

L, R, U, D stand
 respectively for moves left, right, up and down (according to the picture), and 2-letter combinations stand for diagonal moves. If the answer is not unique, print any of them.




Sample test(s)




input
a8
h1




output
7
RD
RD
RD
RD
RD
RD
RD















继续挂python2.7 ，做模版存着，我其实也不太会……



```python
# Class for Point  
class Point:  
     def __init__(self , x , y):  
         self.x = x   
         self.y = y  
  
# input start and end Point  
str = raw_input()  
start = Point(8-int(str[1]) , ord(str[0])-97)  
  
str = raw_input()  
end = Point(8-int(str[1]) , ord(str[0])-97)  
  
# solve this problem  
dir = [[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1],[0,-1],[-1,-1]]  
move = ["U","RU","R","RD","D","LD","L","LU"]  
ans = max(abs(start.x-end.x) , abs(start.y-end.y))  
  
# output  
print ans  
m = ans  
  
x = start.x  
y = start.y  
  
# print "%d %d" % (start.x , start.y)  
# print "%d %d" % (end.x , end.y)  
  
while m > 0:  
    for i in range(8):  
        tmpx = x+dir[i][0]  
        tmpy = y+dir[i][1]  
        if (tmpx >= 0 and tmpx < 8 and tmpy >= 0 and tmpy < 8):  
           dis = max(abs(tmpx-end.x) , abs(tmpy-end.y))    
           if dis < m:  
              print move[i]  
              x = tmpx  
              y = tmpy  
              break  
    m -= 1
```


然后是C++正餐：

坑在了没有取绝对值abs，导致0比负数大从而无法正确判定步数。

第一次尝试make_pair/pair，感觉良好。

还是感觉——  (XXX)?XX:XX 这个式子太喜欢了！！！






```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <utility>
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

using namespace std;

typedef pair<char,int> pl;

int main()
{
 pl src,dsn;
 cin>>src.first>>src.second;
 cin>>dsn.first>>dsn.second;
 //cout<<(src.first-dsn.first)<<" And "<<(src.second-dsn.second)<<endl;
 /*
 char src_first, dsn_first;
 int src_second, dsn_second;
 cin>>src_first>>src_second;
 cin>>dsn_first>>dsn_second;
 */
 int rx= src.first-dsn.first;
 int dy= src.second-dsn.second;
 int maxv= rx>dy?rx:dy;
 int minv= rx<dy?rx:dy;

 if(src.first==dsn.first && dsn.first==dsn.second){
                                                   cout<<"0";
                                                   return 0;
                                                  }
int mxv=max(abs(maxv),abs(minv));//之前一直是mxv=maxv,坑惨了 
 cout<<mxv;
 char x,y;
 for(int i=0;i<mxv;i++)
 {
         if(rx!=0||dy!=0)cout<<endl;
         
         if(rx!=0){
                   cout<<((rx>0)?"L":"R");
                   (rx>0)?rx--:rx++;
                  }
         if(dy!=0)
                  {
                   cout<<((dy>0)?"D":"U");
                   (dy>0)?dy--:dy++;
                  }
                  
         
 }
 //cin>>maxv;
 return 0;
}
```](https://so.csdn.net/so/search/s.do?q=python&t=blog)




