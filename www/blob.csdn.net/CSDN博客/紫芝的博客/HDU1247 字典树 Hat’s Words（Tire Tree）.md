# HDU1247 字典树 Hat’s Words（Tire Tree） - 紫芝的博客 - CSDN博客





2018年07月30日 10:54:36[紫芝](https://me.csdn.net/qq_40507857)阅读数：34
个人分类：[字典树](https://blog.csdn.net/qq_40507857/article/category/7617893)









# Hat’s Words

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 18399    Accepted Submission(s): 6532****

**Problem Description**

A hat’s word is a word in the dictionary that is the concatenation of exactly two other words in the dictionary.

You are to find all the hat’s words in a dictionary.

**Input**

Standard input consists of a number of lowercase words, one per line, in alphabetical order. There will be no more than 50,000 words.Only one case.

**Output**

Your output should contain all the hat’s words, one per line, in alphabetical order.

**Sample Input**

a 

ahat 

hat 

hatword 

hziee 

word

**Sample Output**

ahat 

hatword

**Author**

戴帽子的

**Recommend**

Ignatius.L   |   We have carefully selected several similar problems for you:  [1251](http://acm.hdu.edu.cn/showproblem.php?pid=1251)[1075](http://acm.hdu.edu.cn/showproblem.php?pid=1075)[1671](http://acm.hdu.edu.cn/showproblem.php?pid=1671)[1298](http://acm.hdu.edu.cn/showproblem.php?pid=1298)[1800](http://acm.hdu.edu.cn/showproblem.php?pid=1800)

### 题意：

给定一些单词，要你从中找到某些单词，而这个单词是由另外两个单词组成的。

其实我们就是利用字符的ascii码来给他对应的索引，就是把每个单词拆成两部分，看看是不是每一部分在字典树中都是一个单词

比如说建树的时候存储apple这个单词，对应如下图

![](https://img-blog.csdn.net/20130827093716781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcnVubmluZ2h1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**在算法导论中，Trie并不叫字典树，而叫基数树，实际上并不是只与字典树有关，是一个N叉树。字典树的功能是对于很多串进行压缩，压缩的方法是跟据这个字符串的前缀，每个节点表示一个字符，从根节点到叶子节点表示一个字符串。**

```cpp
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;
const int MAX = 26;
struct Trie
{
     Trie *next[MAX];
     bool isword;
};
Trie *root = new Trie;
char word[50000][30];
void createTrie(char str[])
{
    int len = strlen(str);
    Trie *p = root,*q = NULL;
    for(int i=0;i<len;i++)
    {
         int id = str[i]-'a';
         if(p->next[id]==NULL)
         {
              q = new Trie;
              for(int j=0;j<MAX;j++)
               q->next[j] = NULL;
               q->isword = false;
              p->next[id] = q;
         }
          if(i==len-1)
          p->next[id]->isword = true;
          p = p->next[id];
    }

}
bool findTrie(char str[])
{
     int len = strlen(str);
     Trie *p = root;
   for(int i=0;i<len;i++)
   {
        int id = str[i]-'a';
        if(p->next[id]==NULL)
        {
              return false;
        }
         p = p->next[id];
   }
   if(p->isword)
   return true;
   else
     return false;
}
void del(Trie *root)
{
   for(int i=0;i<MAX;i++)
   {
        if(root->next[i]!=NULL)
        {
             del(root->next[i]);
        }
   }
   delete root;
}
int main()
{
    int num=0;
    char str1[30],str2[30];
    for(int i=0;i<MAX;i++)
    {
         root->next[i] = NULL;
    }
    root->isword = false;
    while(gets(word[num]))
    {
         createTrie(word[num]);
         num++;
    }
    for(int i=0;i<num;i++)
    {
         int len = strlen(word[i]);
         if(len==1)
          continue;
        for(int j=0;j<len;j++)//从每个单词的各部分拆开
     {
          int k;
          if(j==len-1) continue;
         for(k=0;k<=j;k++)
          {
               str1[k] = word[i][k];
          }
          str1[k]='\0';
          int k2=0;
          for(int l=k;l<len;l++)
          {
               str2[k2++]=word[i][l];
          }
          str2[k2]='\0';
          if(findTrie(str1)&&findTrie(str2))
          {
               cout<<word[i]<<endl;
               break;//居然错在这里了(可能会重复输出)
          }
     }
    }
     del(root);
    return 0;
}
```





