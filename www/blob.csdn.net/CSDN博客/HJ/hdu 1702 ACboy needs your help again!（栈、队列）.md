# hdu 1702  ACboy needs your help again!（栈、队列） - HJ - CSDN博客
2017年01月14日 21:06:15[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：249
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[STL](https://blog.csdn.net/feizaoSYUACM/article/category/6308005)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
ACboy needs your help again!
Problem Description 
ACboy was kidnapped!! 
he miss his mother very much and is very scare now.You can’t image how dark the room he was put into is, so poor :(. 
As a smart ACMer, you want to get ACboy out of the monster’s labyrinth.But when you arrive at the gate of the maze, the monste say :” I have heard that you are very clever, but if can’t solve my problems, you will die with ACboy.” 
The problems of the monster is shown on the wall: 
Each problem’s first line is a integer N(the number of commands), and a word “FIFO” or “FILO”.(you are very happy because you know “FIFO” stands for “First In First Out”, and “FILO” means “First In Last Out”). 
and the following N lines, each line is “IN M” or “OUT”, (M represent a integer). 
and the answer of a problem is a passowrd of a door, so if you want to rescue ACboy, answer the problem carefully!
Input 
The input contains multiple test cases. 
The first line has one integer,represent the number oftest cases. 
And the input of each subproblem are described above.
Output 
For each command “OUT”, you should output a integer depend on the word is “FIFO” or “FILO”, or a word “None” if you don’t have any integer.
Sample Input
4 
4 FIFO 
IN 1 
IN 2 
OUT 
OUT 
4 FILO 
IN 1 
IN 2 
OUT 
OUT 
5 FIFO 
IN 1 
IN 2 
OUT 
OUT 
OUT 
5 FILO 
IN 1 
IN 2 
OUT 
IN 3 
OUT
Sample Output
1 
2 
2 
1 
1 
2 
None 
2 
3
题意：告诉你FIFO或FILO（进步前辈先出、进步前辈后出），然后对每次操纵进行模仿，输出成果。
```cpp
#include<iostream>
#include<queue>
#include<stack>
using namespace std;
int main()
{
    int t,n,a;
    char s[10],s1[10];
    cin>>t;
    while(t--)
    {
        cin>>n>>s;
        stack<int> p;
        queue<int> q;
        while(n--)
        {
            cin>>s1;
            if(s1[0]=='I')
            {
                cin>>a;
                if(s[2]=='F')
                  q.push(a);
                else
                  p.push(a);
            }
            else
            {
                if(s[2]=='F')
                {
                    if(!q.empty()) 
                    {
                        cout<<q.front()<<endl;
                        q.pop();
                    }
                    else
                      cout<<"None"<<endl;
                }
                else
                {
                    if(!p.empty())
                    {
                        cout<<p.top()<<endl;
                        p.pop();
                    }
                    else
                      cout<<"None"<<endl;
                }
            }
        }
    }
    return 0;
}
```
