# Codeforces502B——Misha and Changing Handles - westbrook1998的博客 - CSDN博客





2018年08月26日 19:44:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：28标签：[map](https://so.csdn.net/so/search/s.do?q=map&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Misha hacked the Codeforces site. Then he decided to let all the users change their handles. A user can now change his handle any number of times. But each new handle must not be equal to any handle that is already used or that was used at some point. 

  Misha has a list of handle change requests. After completing the requests he wants to understand the relation between the original and the new handles of the users. Help him to do that. 

  Input 

  The first line contains integer q (1 ≤ q ≤ 1000), the number of handle change requests. 

  Next q lines contain the descriptions of the requests, one per line. 

  Each query consists of two non-empty strings old and new, separated by a space. The strings consist of lowercase and uppercase Latin letters and digits. Strings old and new are distinct. The lengths of the strings do not exceed 20. 

  The requests are given chronologically. In other words, by the moment of a query there is a single person with handle old, and handle new is not used and has not been used by anyone. 

  Output 

  In the first line output the integer n — the number of users that changed their handles at least once. 

  In the next n lines print the mapping between the old and the new handles of the users. Each of them must contain two strings, old and new, separated by a space, meaning that before the user had handle old, and after all the requests are completed, his handle is new. You may output lines in any order. 

  Each user who changes the handle must occur exactly once in this description. 

  Examples 

  Input 

  5 

  Misha ILoveCodeforces 

  Vasya Petrov 

  Petrov VasyaPetrov123 

  ILoveCodeforces MikeMirzayanov 

  Petya Ivanov 

  Output 

  3 

  Petya Ivanov 

  Misha MikeMirzayanov 

  Vasya VasyaPetrov123
给出一些换昵称的记录，然后最后要求有多少个用户已经他们最原本的昵称和现在的昵称 

用map 但是如果正向用的话比较麻烦 所以我就是反向用 键存的是新的昵称 值存的是老的昵称 这样更改昵称时 就通过当前的新昵称找到原本的老昵称，再次以这个老昵称做键，最新的昵称做值这样即可

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
#include <string>
#include <iostream>
using namespace std;
//new old
map<string,string> mp;
int main(void){
    int n;
    string a,b;
    scanf("%d",&n);
    getchar();
    while(n--){
        cin >> a >> b;
        if(mp.find(a)!=mp.end()){
            //cout << mp.find(a)->second << endl;
            mp[b]=mp[a];
            mp.erase(a);
        }
        else{
            mp[b]=a;
        }
        //cout << mp[b] << endl;
    }
    map<string,string>::iterator it=mp.begin();
    printf("%d\n",mp.size());
    for(;it!=mp.end();it++){
        cout << it->second <<" "<< it->first <<endl;
    }
    return 0;
}
```





