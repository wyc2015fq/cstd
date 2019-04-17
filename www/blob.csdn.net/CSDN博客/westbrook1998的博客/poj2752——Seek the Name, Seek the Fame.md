# poj2752——Seek the Name, Seek the Fame - westbrook1998的博客 - CSDN博客





2018年06月24日 19:19:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34标签：[KMP																[字符串匹配](https://so.csdn.net/so/search/s.do?q=字符串匹配&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
The little cat is so famous, that many couples tramp over hill and dale to Byteland, and asked the little cat to give names to their newly-born babies. They seek the name, and at the same time seek the fame. In order to escape from such boring job, the innovative little cat works out an easy but fantastic algorithm:  

  Step1. Connect the father’s name and the mother’s name, to a new string S.  

  Step2. Find a proper prefix-suffix string of S (which is not only the prefix, but also the suffix of S).  

  Example: Father=’ala’, Mother=’la’, we have S = ‘ala’+’la’ = ‘alala’. Potential prefix-suffix strings of S are {‘a’, ‘ala’, ‘alala’}. Given the string S, could you help the little cat to write a program to calculate the length of possible prefix-suffix strings of S? (He might thank you by giving your baby a name:)  

  Input 

  The input contains a number of test cases. Each test case occupies a single line that contains the string S described above.  

  Restrictions: Only lowercase letters may appear in the input. 1 <= Length of S <= 400000.  

  Output 

  For each test case, output a single line with integer numbers in increasing order, denoting the possible length of the new baby’s name. 

  Sample Input 

  ababcababababcabab 

  aaaaa 

  Sample Output 

  2 4 9 18 

  1 2 3 4 5
求字符串的相同前缀后缀长度的所有值 

这里先是用到KMP求Next数组 然后并不是简单地把Next数组全部输出 因为题意是要求整个字符串的前缀后缀的 所以看了别人的博客。。。看到了异常巧妙的做法 

这段代码的`len=Next[len]`也是思考到快崩溃 最后总算还能想出来 也没白想 希望对kmp有更深刻的理解了
```
while(len!=0 && Next[len]!=0){
            s.push(Next[len]);
            //跳到这个相同前缀的最后一个字符后
            //因为这个前缀字符串和后缀字符串是一样的
            //所以这时也相当于在后缀字符串的最后一个字符后
            //所以这时候再看next值也就是这个前缀字符串里的相同前缀后缀
            //很巧妙的就是此时这个前缀的前缀就和之前那个后缀的后缀相同！！
            //因为刚才已经说过第一次跳过来那个前缀和后缀是一样的
            //那么这个前缀的相同前缀后缀也就相当于那个后缀的前缀后缀！！
            //例如：
            //          a a b a a b a a b a a
            //next数组：-1 0 1 0 1 2 3 4 5 6 7 8
            //首先是整个字符串长度11 然后检查Next[11]，也就是前11个字符的最大前缀后缀长度
            //发现是8，然后这时候整个字符串就分为两部分(相同前缀后缀)各8个字符，有重复
            //然后检查Next[8]就是前面的这一部分8个字符的最长前缀后缀长度 发现是5 
            //也就是说这8个字符被分为两部分各(相同前缀后缀)5个字符，很巧妙的就是这时候第二部分8个字符
            //也被分为两部分各5个字符了,所以这时候整个完整的字符串就有5个字符的相同前缀后缀！！
            len=Next[len];
        }
```

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <stack>
using namespace std;
const int MAXN=400050;
char s[MAXN];
int Next[MAXN];
void kmp_pre(char* s){
    int l=strlen(s);
    int i=0;
    int j=-1;
    Next[0]=-1;
    while(i<l){
        if(j==-1 || s[i]==s[j]){
            i++;
            j++;
            Next[i]=j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    while(~scanf("%s",s)){
        memset(Next,0,sizeof(Next));
        kmp_pre(s);
        vector<int> res;
        int len=strlen(s);
        /*
        printf(" ");
        for(int i=0;i<len;i++){
            printf("%c ",s[i]);
        }
        printf("\n");
        for(int i=0;i<=len;i++){
            printf("%d ",Next[i]);
        }
        printf("\n");
        */
        stack<int> s;
        s.push(len);
        while(len!=0 && Next[len]!=0){
            s.push(Next[len]);
            //跳到这个相同前缀的最后一个字符后
            //因为这个前缀字符串和后缀字符串是一样的
            //所以这时也相当于在后缀字符串的最后一个字符后
            //所以这时候再看next值也就是这个前缀字符串里的相同前缀后缀
            //很巧妙的就是此时这个前缀的前缀就和之前那个后缀的后缀相同！！
            //因为刚才已经说过第一次跳过来那个前缀和后缀是一样的
            //那么这个前缀的相同前缀后缀也就相当于那个后缀的前缀后缀！！
            //例如：
            //          a a b a a b a a b a a
            //next数组：-1 0 1 0 1 2 3 4 5 6 7 8
            //首先是整个字符串长度11 然后检查Next[11]，也就是前11个字符的最大前缀后缀长度
            //发现是8，然后这时候整个字符串就分为两部分(相同前缀后缀)各8个字符，有重复
            //然后检查Next[8]就是前面的这一部分8个字符的最长前缀后缀长度 发现是5 
            //也就是说这8个字符被分为两部分各(相同前缀后缀)5个字符，很巧妙的就是这时候第二部分8个字符
            //也被分为两部分各5个字符了,所以这时候整个完整的字符串就有5个字符的相同前缀后缀！！
            len=Next[len];
        }
        while(s.size()){
            printf("%d ",s.top());
            s.pop();
        }
        printf("\n");
    }
    return 0;
}
```







