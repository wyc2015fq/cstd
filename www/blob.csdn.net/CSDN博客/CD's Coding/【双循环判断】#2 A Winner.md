# 【双循环判断】#2 A. Winner - CD's Coding - CSDN博客





2014年03月31日 21:10:27[糖果天王](https://me.csdn.net/okcd00)阅读数：704
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The winner of the card game popular in Berland "Berlogging" is determined according to the following rules. If at the end of the game there is only one player with the maximum number of points, he is the winner. The situation becomes more difficult if the number
 of such players is more than one. During each round a player gains or loses a particular number of points. In the course of the game the number of points is registered in the line "name
 score", where name is a player's name, and score is
 the number of points gained in this round, which is an integer number. If score is negative, this means that the player has lost in the round. So, if two or more players have the maximum number of points (say, it equals to *m*)
 at the end of the game, than wins the one of them who scored at least *m* points first. Initially
 each player has 0 points. It's guaranteed that at the end of the game at least one player has a positive number of points.




Input


The first line contains an integer number *n* (1  ≤  *n*  ≤  1000), *n* is
 the number of rounds played. Then follow *n*lines, containing the information about the rounds in "name
 score" format in chronological order, where name is a string of lower-case Latin letters with the length from 1 to 32, and score is
 an integer number between -1000 and 1000, inclusive.




Output


Print the name of the winner.




Sample test(s)




input
3
mike 3
andrew 5
mike 2




output
andrew




input
3
andrew 3
andrew 2
mike 5




output
andrew












依然是得瑟的来一个Python2.7代码^_^





```python
# define some variable  
n = int(raw_input())  
maxScore = {}  
input = []  
  
# n times input  
while n > 0:  
    list = raw_input().split()  
    input.append(list)  
    name = list[0]  
    score = int(list[1])  
    if maxScore.has_key(name):  
       maxScore[name] += score  
    else:  
       maxScore[name] = score  
    n -= 1  
  
# find maxScore = ans  
ans = 0  
for key in maxScore:  
    ans = max(ans , maxScore[key])  
  
# def to find the time >= ans  
def getTime(str):  
    sum = 0  
    cnt = 0  
    for list in input:  
        name = list[0]  
        score = int(list[1])  
        if name == str:  
           sum += score  
           if sum >= ans:  
              return cnt  
        cnt += 1   
  
# one by one if score == ans  
time = 2147483647  
for key in maxScore:  
    if maxScore[key] == ans:  
       t = getTime(key)  
       if time > t:  
          time = t  
          ansName = key  
  
# output  
print ansName
```


然后是C++：

这道题其实几个循环就好：循环读入咱们就不算了，循环1st判断最高值是哪个，循环2nd找到第一次超过这个最高值的是谁，然后，把他给我扔出去！！！



```cpp
#include <iostream>
#include <map>
#include <string.h>
#include <algorithm>
using namespace std;
int score[1010];
map<string, int> p1,p2;
string name[1010];
int main()
{
    int n,i,j;
    while(cin>>n)
    {
        int m = 0;
        for(i = 0;i < n;i ++)
        {
            cin>>name[i]>>score[i];
            p1[name[i]] += score[i];
        }
        for(i = 0;i < n;i ++)
        if(p1[name[i]] > m) m = p1[name[i]];
        for(i = 0;i < n;i ++)
        {
            p2[name[i]] += score[i];
            if(p2[name[i]] >= m&&p1[name[i]] == m)
            {
                cout<<name[i]<<endl;
                break;
            }
        }
    }
}
```




















