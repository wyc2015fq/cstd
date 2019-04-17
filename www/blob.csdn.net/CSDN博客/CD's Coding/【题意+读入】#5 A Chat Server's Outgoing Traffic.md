# 【题意+读入】#5 A. Chat Server's Outgoing Traffic - CD's Coding - CSDN博客





2014年03月31日 21:40:31[糖果天王](https://me.csdn.net/okcd00)阅读数：656标签：[c++																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)

















Polycarp is working on a new project called "Polychat". Following modern tendencies in IT, he decided, that this project should contain chat as well. To achieve this goal, Polycarp has spent several hours in front of his laptop and implemented a chat server
 that can process three types of commands:
- 
Include a person to the chat ('Add' command).
- 
Remove a person from the chat ('Remove' command).
- 
Send a message from a person to all people, who are currently in the chat, including the one, who sends the message ('Send' command).


Now Polycarp wants to find out the amount of outgoing traffic that the server will produce while processing a particular set of commands.


Polycarp knows that chat server sends no traffic for 'Add' and 'Remove' commands. When 'Send' command is processed, server sends *l* bytes to each participant
 of the chat, where *l* is the length of the message.


As Polycarp has no time, he is asking for your help in solving this problem.




Input


Input file will contain not more than 100 commands, each in its own line. No line will exceed 100 characters. Formats of the commands will be the following:
- +<name> for 'Add' command.
- -<name> for 'Remove' command.
- <sender_name>:<message_text> for 'Send' command.

<name> and <sender_name> is a non-empty sequence of Latin letters
 and digits. <message_text> can contain letters, digits and spaces, but can't start or end with a space. <message_text> can
 be an empty line.


It is guaranteed, that input data are correct, i.e. there will be no 'Add' command if person with such a name is already in the chat, there will be no 'Remove' command
 if there is no person with such a name in the chat etc.


All names are case-sensitive.




Output


Print a single number — answer to the problem.




Sample test(s)




input
+Mike
Mike:hello
+Kate
+Dmitry
-Dmitry
Kate:hi
-Kate




output
9




input
+Mike
-Mike
+Mike
Mike:Hi   I am here
-Mike
+Kate
-Kate




output
14















C++：

//这道题麻烦在理解题意上，其实他的意思是，总共发送了多少字节，每个人说话的时候这句话要发给在场所有的人（包括他自己） 

//难点大概除了理解题意，还有一个难点就似乎在读入上 




```cpp
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
//这道题麻烦在理解题意上，其实他的意思是，总共发送了多少字节，每个人说话的时候这句话要发给在场所有的人（包括他自己） 
//难点大概除了理解题意，还有一个难点就似乎在读入上 
int main()
{
char str[110];

    int num=0;//记录在线人数
    int sum=0;//记录最终字节数
    int colon;//记录冒号的位置
    while(gets(str))
    {
        if(str[0]=='+')num++;
        else if(str[0]=='-')num--;
        else
        {
            for(int i=0; i<strlen(str); i++)
            {
                if(str[i]==':')
                {
                    colon=i;
                    break;
                }
            }
            sum+=(strlen(str)-1-colon)*num;
        }
    }
    printf("%d\n",sum);
    return 0;
}
```






