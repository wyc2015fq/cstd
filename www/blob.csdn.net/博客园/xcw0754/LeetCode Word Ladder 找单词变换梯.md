# LeetCode Word Ladder 找单词变换梯 - xcw0754 - 博客园
# [LeetCode Word Ladder 找单词变换梯](https://www.cnblogs.com/xcw0754/p/4378869.html)
题意：给出两个单词，以及一个set集合，当中是很多的单词。unordered_set是无序的集合，也就是说找的序列也是无序的了，是C++11的标准，可能得升级你的编译器版本了。要求找出一个从start到end这两个单词的变换序列。从start开始，每次可以变一个字母，且所变之后的单词必须在set中，最后要求变成end，问经过了多少个中间变换？注意要加多2次（start和end也要算），这是规定。
思路：广度搜索，以start为树根，一层一层扩展，直到找到end，返回数的深度即可。步骤是这样的，先画出树根start，遍历set，所有能被start够经过1个字母的变换得到的，取出来（要删掉）做为第二层，也就是作为树根的孩子。接着以第二层的每个元素为起点，继续遍历set中的元素，直到搜到end，计算深度返回。
注：千辛万苦用g++的4.8.1版才能编译测试，网传可以用对当前单词的每个字母用a～z每个字母代替一次，再在set中查找出来，这个方法感觉看不出优势，n个单词，单词长为k，最差大概n*k*26次。下面这个是n*k。很累没有详细验证，大概就这样吧。搞了3天，才知道被那个for括号中第二个式子给玩坏了，它每次循环都会检查，也就是更新界限。
```
1 class Solution {
 2 public:
 3     bool mat(string &lef,const string &rig)    /*返回两个字符串是否匹配（允许一个字母不匹配）*/
 4     {
 5         int count=0;
 6         for(int i=0; i<lef.size(); i++)
 7         {
 8             if(lef[i]!=rig[i])
 9             {
10                 count++;
11                 if(count>=2) return false;
12             }
13         }
14         return true;    //不可能出现相等的，即count=0的情况
15     }
16     
17     int ladderLength(string start, string end, unordered_set<string> &dict) {
18         if( start.empty() || end.empty() || start==end || start.length() != end.length() )  return 0;
19         if( mat(start,end) )    return 2;    //只有一个不匹配
20         if( dict.find(end) == dict.end() )    dict.insert(end);//end必须在set中
21         if( dict.find(start)!=dict.end() )    dict.erase(start);    //start必须不在setzhong
22         unordered_set<string>::iterator dist = dict.find(end);    //终点指针
23         unordered_set<string>::iterator it   = dict.begin();
24         queue<string> que;
25         que.push(start);        //起点先进队
26         int count=1;
27         while(!que.empty())    
28         {
29             count++;
30             int q=que.size();        //注意这里，不能将que.size()放在下一行的括号中代替q，它每次循环都检查一遍
31             for(int i=0; i<q; i++)    //此for扫描同一层的元素
32             {
33                 it = dict.begin();
34                 while( it!=dict.end() )     //搜dict中每个元素
35                 {
36                     if( mat( que.front(), *it) )    
37                     {
38                         if( it == dist )    return count;    //找到终点end
39                         que.push(*it);    
40                         it = dict.erase(it);    //在集合中删去
41                     }
42                     else    it++;
43                 }
44                 que.pop();
45             }
46         }
47         return 0;
48     }
49 };
word ladder
```

