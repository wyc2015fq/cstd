# Wannafly挑战赛23A——字符串 - westbrook1998的博客 - CSDN博客





2018年08月31日 22:52:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[字符串																[最小覆盖子串](https://so.csdn.net/so/search/s.do?q=最小覆盖子串&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
链接：[https://www.nowcoder.com/acm/contest/161/A](https://www.nowcoder.com/acm/contest/161/A)

  来源：牛客网 

  题目描述  

  小N现在有一个字符串S。他把这这个字符串的所有子串都挑了出来。一个S的子串T是合法的，当且仅当T中包含了所有的小写字母。小N希望知道所有的合法的S的子串中，长度最短是多少。 

  输入描述: 

  一行一个字符串S。只包含小写字母。S的长度不超过106. 

  输出描述: 

  一行一个数字，代表最短长度。数据保证存在一个合法的S的子串。 

  示例1 

  输入 

  复制 

  ykjygvedtysvyymzfizzwkjamefxjnrnphqwnfhrnbhwjhqcgqnplodeestu 

  输出 

  复制 

  49
作弊签到了一题….. 

赛中问大佬的，学了最小覆盖子串这个问题 

代码：
```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=105;
//最小满足条件的子串长度
char s[N];
char t[N];
//目标串和原串每个字母出现的次数,这里考虑26个小写字母
int sHash[26];
int tHash[26];
int main(void){
    int tlen=strlen(t);
    int slen=strlen(s);
    //预处理目标字符串的hash数组
    for(int i=0;i<tlen;i++){
        tHash[t[i]-'a']++;
    }
    //临时的字符串起点
    int ss=0;
    int found=0;
    //子串的起点和终点
    int start=-1;
    int end=slen;
    int minLen=slen;
    for(int i=0;i<slen;i++){
        sHash[s[i]-'a']++;
        //加后出现次数不大于目标串该字符的出现次数，即找到一个匹配字符
        //比如t是aab  当s是aaa时找到前两个a时found++，第三个a则不会
        if(sHash[s[i]-'a']<=tHash[s[i]-'a']){
            found++;
        }
        //找到符合要求的子串
        if(found==tlen){
            //将子串开头没用的跳过（没用指该字符出现次数任大于目标串中该字符出现次数
            //因此删去也不影响）
            while(ss<i && sHash[s[ss]-'a']>tHash[s[ss]-'a']){
                sHash[s[ss]-'a']--;
                //起点加1
                ss++;
            }
            //更新答案
            if(i-ss<minLen){
                minLen=i-ss+1;
                start=ss;
                end=i;
            }
            //删去开头，匹配字符数记得减1（因为经过前面的操作这里的开头肯定是有用的）
            sHash[s[ss]-'a']--;
            found--;
            ss++;
        }
    }
    return 0;
}
```

然后赛后看了代码发现更简单，不过原理是一样的 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1e6+10;
char s[N];
int a[N];
int main(void){
    scanf("%s",s);
    int l=strlen(s);
    //匹配的字符数
    int num=0;
    int ans=0x3f3f3f3f;
    for(int i=0,j=0;i<l;i++){
        if(!a[s[i]-'a']){
            num++;
        }
        a[s[i]-'a']++;
        while(num==26){
            ans=min(ans,i-j+1);
            a[s[j]-'a']--;
            //子串开头字符删掉有影响
            if(!a[s[j++]-'a']){
                num--;
                break;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}
```







