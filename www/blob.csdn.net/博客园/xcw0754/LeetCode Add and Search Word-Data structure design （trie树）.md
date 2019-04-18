# LeetCode Add and Search Word - Data structure design （trie树） - xcw0754 - 博客园
# [LeetCode Add and Search Word - Data structure design （trie树）](https://www.cnblogs.com/xcw0754/p/4511682.html)
![](https://images0.cnblogs.com/blog2015/641737/201505/181316218229773.png)
题意：实现添加单词和查找单词的作用，即实现字典功能。
思路：'.' 可以代表一个任何小写字母，可能是".abc"或者"a.bc"或者"abc."，能应对这三种就没有问题了。在每个单词的尾字母上标上tag=1，代表从树根到此节点有一个单词。暂时想不到更快的办法。
```
1 class WordDictionary {
 2 public:
 3     WordDictionary(){
 4         tree=create();
 5     }
 6     void addWord(string word) {
 7         node *tmp=tree;
 8         node *p=0;    //负责创建结点
 9         for(int i=0; i<word.size(); i++)
10         {
11             if(!tmp->next[word[i]-'a'])    //没有这个分支，创建它
12             {
13                 p=create();
14                 tmp->next[word[i]-'a']=p;
15             }
16             tmp=tmp->next[word[i]-'a'];    //往下走
17         }
18         tmp->tag=1;
19     }
20 
21     bool search(string word) {
22         if(DFS(tree,word.c_str())==true)//搜
23             return true;
24         return false;
25     }
26 private:
27 
28     struct node
29     {
30         bool tag;
31         node *next[26];
32 
33     };
34     node *tree;//先建立树根
35     node *create()
36     {
37         node *tmp=new(node);
38         tmp->tag=0;
39         for(int i=0; i<26; i++)
40             tmp->next[i]=0;
41         return tmp;
42     }
43     bool DFS(node *t,const char *p)
44     {
45         if(*(p+1)=='\0')
46         {
47             if(*p=='.') //'.'刚好是最后一个
48             {
49                 for(int i=0; i<26; i++)
50                     if(t->next[i]&&t->next[i]->tag==1)
51                         return true;
52                 return false;   //无匹配
53             }
54             if(t->next[*p-'a'] && t->next[*p-'a']->tag==1)    return 1;
55             return false;
56         }
57 
58         if(*p=='.')//要搜索全部
59         {
60             for(int i=0; i<26; i++)
61                 if( t->next[i] && DFS(t->next[i],p+1) )
62                     return true;
63             return false;
64         }
65 
66         if( t->next[*p-'a'] && DFS(t->next[*p-'a'],p+1))
67             return true;
68         return false;
69     }
70 };
71 
72 
73 // Your WordDictionary object will be instantiated and called as such:
74 // WordDictionary wordDictionary;
75 // wordDictionary.addWord("word");
76 // wordDictionary.search("pattern");
AC代码
```

