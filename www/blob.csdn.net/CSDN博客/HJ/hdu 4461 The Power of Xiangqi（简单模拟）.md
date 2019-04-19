# hdu  4461 The Power of Xiangqi（简单模拟） - HJ - CSDN博客
2017年04月08日 18:12:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：290
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description
![这里写图片描述](http://acm.hdu.edu.cn/data/images/4461-1.jpg)
Xiangqi is one of the most popular two-player board games in China. The game represents a battle between two armies with the goal of capturing the enemy’s “general” piece. 
Now we introduce some basic rules of Xiangqi. Xiangqi is played on a 10×9 board and the pieces are placed on the intersections (points). There are two groups of pieces marked by black or red Chinese characters, belonging to the two players separately. During the game, each player in turn moves one piece from the point it occupies to another point. No two pieces can occupy the same point at the same time. A piece can be moved onto a point occupied by an enemy piece, in which case the enemy piece is “captured” and removed from the board. When the general is in danger of being captured by the enemy player on the enemy player’s next move, the enemy player is said to have “delivered a check”. If the general’s player can make no move to prevent the general’s capture by next enemy move, the situation is called “checkmate”. 
Each player has 7 kinds of pieces in this game. Their names, offense power and symbol letters are shown in the table below:
![这里写图片描述](http://acm.hdu.edu.cn/data/images/4461-2.jpg)
Now show you the pieces of red player and black player, you are going to find out which player has the larger total offense power. Since Ma and Pao working together can have good effect, if a player has no Ma or no Pao, or has neither, his total offense power will be decreased by one. But the total offense power can’t be decreased to zero, it is at least one.
Input 
The first line is an integer T ( T <= 20) meaning there are T test cases. 
For each test case: The first line shows which pieces the red player has. It begins with an integer n ( 0 < n <= 16) meaning the number of pieces. 
Then n letters follows, all separated by one or more blanks. Each letter is a symbol letter standing for a piece, as shown in the table above. 
The second line describes the situation of the black player, in the same format as the first line.
Output 
For each test case, if the red player has more offense power, then print “red”. If the black player has more offense power, then print “black”. If there is a tie, print “tie”.
Sample Input
3 
2 A B 
2 A B 
7 A A B C D D F  
7 A A B B C C F 
5 A A B B F 
3 A B F
Sample Output
tie 
black 
red
简单模拟题，注意一个条件：当棋子中‘B’和‘C’不是都有的话，在你价值大于1之后会自动减去一个1。
```cpp
#include<iostream>
using namespace std;
char s[10]={16,7,8,1,1,2,3};
int main()
{
    int t,x;
    char c;
    cin>>t;
    while(t--)
    {
        cin>>x;
        int a=0,b=0,a1=0,a2=0,b1=0,b2=0;
        for(int i=0;i<x;i++)
        {
            cin>>c;
            int k=c-'A';
            if(k==1)  a1++;
            if(k==2)  a2++;
            a+=s[k];
        }
        cin>>x;
        for(int i=0;i<x;i++)
        {
            cin>>c;
            int k=c-'A';
            if(k==1)  b1++;
            if(k==2)  b2++; 
            b+=s[k];
        }
        if((a1==0||a2==0)&&a>1) a--;
        if((b1==0||b2==0)&&b>1) b--;
        if(a>b)
          cout<<"red"<<endl;
        else if(a==b)
          cout<<"tie"<<endl;
        else
          cout<<"black"<<endl;
    }
    return 0;
}
```
