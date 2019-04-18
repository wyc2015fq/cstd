# LeetCode Valid Parentheses 有效括号 - xcw0754 - 博客园
# [LeetCode Valid Parentheses 有效括号](https://www.cnblogs.com/xcw0754/p/4106953.html)
```
1 class Solution {
 2 public:
 3 void push(char c){        //插入结点
 4     struct node *n=new struct node;
 5     n->nex=0;
 6     n->ch=c;
 7     n->pre=last;
 8     last->nex=n;
 9     last=last->nex;
10 }
11 bool jud(char c){        //判断
12     struct node *m;
13     if(c==']')
14         c='[';
15     else if(c=='}')
16         c='{';
17     else if(c==')')
18         c='(';
19     if(last->ch==c&&first.nex!=0){
20         m=last;
21         last=last->pre;
22         delete m;
23         last->nex=0;
24         return 1;
25     }
26     else
27         return 0;
28 }
29 bool isValid(string s) {    //主判断函数
30     char *p=&s[0];
31     while(*p!='\0'){
32         if(*p=='[' || *p=='{' || *p=='(')
33             push(*p);
34         else if(*p==']' || *p=='}' || *p==')'){
35             if(jud(*p)==false)    //判断错误
36                 return 0;
37         }
38         p++;
39     }
40     if(last==&first)
41         return 1;
42     else
43         return 0; 
44 }
45 private:   
46 struct node{        //结构体
47     struct node *pre;
48     struct node *nex;
49     char ch;
50 }first={0,0,0};
51 struct node *last=&first;  
52     
53 };
```
题意：判断括号是否成对的出现，并且是合法的，类似判断一个算术式子中的括号有没有错一样。过滤掉非3种括号的其他字符。
思路：因为不知道这个string到底多大，可能很大，可能很小，所以我选择用盏（链表）来解决。一旦有不成对出现的，立刻就可以返回了。我这里用多两个函数将功能模块化，一个用于进盏，一个用于转化括号为另一半并判断。正常情况下结束时盏中应该空（我用last是否为链表头first来判断）。
注意：此题用STL和string类来解决应该更简单。代码量也会减少很多。

