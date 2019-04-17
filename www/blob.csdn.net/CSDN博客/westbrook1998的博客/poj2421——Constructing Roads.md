# poj2421——Constructing Roads - westbrook1998的博客 - CSDN博客





2018年06月25日 20:54:16[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[图论																[最小生成树																[Kruskal](https://so.csdn.net/so/search/s.do?q=Kruskal&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
There are N villages, which are numbered from 1 to N, and you should build some roads such that every two villages can connect to each other. We say two village A and B are connected, if and only if there is a road between A and B, or there exists a village C such that there is a road between A and C, and C and B are connected.  

  We know that there are already some roads between some villages and your job is the build some roads such that all the villages are connect and the length of all the roads built is minimum. 

  Input 

  The first line is an integer N (3 <= N <= 100), which is the number of villages. Then come N lines, the i-th of which contains N integers, and the j-th of these N integers is the distance (the distance should be an integer within [1, 1000]) between village i and village j.  

  Then there is an integer Q (0 <= Q <= N * (N + 1) / 2). Then come Q lines, each line contains two integers a and b (1 <= a < b <= N), which means the road between village a and village b has been built. 

  Output 

  You should output a line contains an integer, which is the length of all the roads to be built such that all the villages are connected, and this value is minimum. 

  Sample Input 

  3 

  0 990 692 

  990 0 179 

  692 179 0 

  1 

  1 2 

  Sample Output 

  179
继续最小生成树的练习 这题稍微的那么一点点点变形 把已有的路权值要设为0就行 然后注意要连通起来！ 这里第一个写错的地方 然后第二个就是把cmp函数写成大于了 

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
```](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)




