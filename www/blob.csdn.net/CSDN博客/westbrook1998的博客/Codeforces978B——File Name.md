# Codeforces978B——File Name - westbrook1998的博客 - CSDN博客





2018年08月11日 19:28:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You can not just take the file and send it. When Polycarp trying to send a file in the social network “Codehorses”, he encountered an unexpected problem. If the name of the file contains three or more “x” (lowercase Latin letters “x”) in a row, the system considers that the file content does not correspond to the social network topic. In this case, the file is not sent and an error message is displayed. 

  Determine the minimum number of characters to remove from the file name so after that the name does not contain “xxx” as a substring. Print 0 if the file name does not initially contain a forbidden substring “xxx”. 

  You can delete characters in arbitrary positions (not necessarily consecutive). If you delete a character, then the length of a string is reduced by 1. For example, if you delete the character in the position 2 from the string “exxxii”, then the resulting string is “exxii”. 

  Input 

  The first line contains integer n (3≤n≤100) — the length of the file name. 

  The second line contains a string of length n consisting of lowercase Latin letters only — the file name. 

  Output 

  Print the minimum number of characters to remove from the file name so after that the name does not contain “xxx” as a substring. If initially the file name dost not contain a forbidden substring “xxx”, print 0. 

  Examples 

  Input 

  6 

  xxxiii 

  Output 

  1 

  Input 

  5 

  xxoxx 

  Output 

  0 

  Input 

  10 

  xxxxxxxxxx 

  Output 

  8 

  Note 

  In the first example Polycarp tried to send a file with name contains number 33, written in Roman numerals. But he can not just send the file, because it name contains three letters “x” in a row. To send the file he needs to remove any one of this letters.
也是暴力遍历一遍，判断x连续的个数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
char s[105];
int n;
vector<int> res;
int main(void){
    scanf("%d",&n);
    scanf("%s",s);
    int flag=0;
    int tmp=0;
    for(int i=0;i<n;i++){
        if(s[i]=='x'){
            if(!flag){
                flag=1;
                tmp=1;
            }
            else{
                tmp++;
            }
        }
        else{
            flag=0;
            if(tmp>=3){
                res.push_back(tmp-2);
                tmp=0;
            }
        }
    }
    if(flag){
        if(tmp>=3){
            res.push_back(tmp-2);
        }
    }
    int len=res.size();
    int ans=0;
    for(int i=0;i<len;i++){
        ans+=res[i];
    }
    printf("%d\n",ans);
    return 0;
}
```





