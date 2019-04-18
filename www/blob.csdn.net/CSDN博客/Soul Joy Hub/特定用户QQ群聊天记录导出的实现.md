# 特定用户QQ群聊天记录导出的实现 - Soul Joy Hub - CSDN博客

2016年06月13日 10:01:00[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1860


                
一、把QQ群的聊天记录txt格式导出

消息管理器 -> 选择要导出的群 -> 右击、导出

这里要注意 ： 导出之后的 文本是 unicode 编码的，需要转换 ==|| 之前不知道，搞了大半天。

重新建一个txt ， 把原来的 txt 内容 复制 到 新的 txt ，保存就行了。

# 二、详细代码

## 1. head.h

```
1 #ifndef TxtSearch
 2 #define TxtSearch
 3 
 4 #include<string>
 5 #include<map>
 6 #include<set>
 7 #include<iostream>
 8 #include<vector>
 9 #include<fstream>
10 #include<sstream>
11 #include<ctype.h>
12 
13 typedef unsigned char BYTE;
14 typedef unsigned short WORD;
15 
16 bool IsChineseChar(WORD DoubleByte);
17 
18 void WordAndChar_print(std::string );
19 
20 
21 class TextQuery
22 {
23 public:
24 
25     typedef std::vector<std::string>::size_type line_no;
26 
27     void read_file(std::ifstream & );
28     std::set<line_no> run_query(const std::string&) const;
29     std::string text_line(line_no) const;
30 private:
31     void store_file( std::ifstream &  );
32     void build_map();
33 
34 
35     std::vector<std::string> lines_of_text;
36     std::vector<std::string> bak_lines_of_text;
37     std::map< std::string,std::set<line_no> > word_map;
38 
39 };
40 
41 void print_resluts(const std::set< TextQuery::line_no> & , 
42                    const std::string & , const TextQuery &);
43 
44 
45 #endif
```

## 2、function.cpp

```
1 #include"head.h"
 2 
 3 bool IsChineseChar(WORD DoubleByte)
 4 {                                                              
 5     return ( (DoubleByte < 0) || (DoubleByte > 127) );
 6 }  
 7 
 8 
 9 void TextQuery::read_file( std::ifstream &  is)
10 {
11     store_file(is);
12     build_map();
13 }
14 
15 std::set<TextQuery::line_no> TextQuery::run_query(const std::string& str) const
16 {
17     std::map<std::string, std::set<line_no> >::const_iterator 
18         loc = word_map.find(str);
19 
20     if(loc == word_map.end())
21     {
22         return std::set<line_no>();
23     }
24 
25     return loc->second;
26 }
27 std::string TextQuery::text_line(line_no line_num) const
28 {
29     if(line_num < lines_of_text.size())
30         return lines_of_text[line_num];
31 
32     throw std::out_of_range("line num is out of range");
33 }
34 
35 
36 void TextQuery::store_file(std::ifstream &  is)
37 {
38     std::string txtline;
39     while(std::getline(is,txtline))
40     {
41         lines_of_text.push_back(txtline);
42         int len = txtline.length();
43         for(int i = 0 ; i <len ; i ++)  //pretreatment
44         {
45             bool b1  = 0,b2 =0;
46             if(!IsChineseChar(txtline[i]) 
47                 &&!isalnum(txtline[i]) )
48                 txtline[i] = ' ';
49             b1 = IsChineseChar(txtline[i]);
50             if(i + 1 < len)
51                 b2 = IsChineseChar(txtline[i+1]);
52         }
53 
54         bak_lines_of_text.push_back(txtline);
55     }
56 }
57 
58 
59 void TextQuery::build_map()
60 {
61     for(line_no line_num = 0 ; line_num != bak_lines_of_text.size() ; ++line_num)
62     {
63         std::istringstream strline(bak_lines_of_text[line_num]);
64         std::string word;
65         while(strline >> word)
66         {
67             word_map[word].insert(line_num);
68         }
69     }
70 }
71 
72 void print_resluts(const std::set< TextQuery::line_no> & ss, 
73                    const std::string & str, const TextQuery &tq)
74 {
75     typedef std::set< TextQuery::line_no> lineset;
76 
77     lineset::size_type size = ss.size();
78     std::string path = str + ".txt";
79     std::ofstream outfine(path);
80     outfine<<str<<" occurs "<<size <<"times"<<std::endl;
81     lineset::const_iterator it = ss.begin();
82 
83     for(; it != ss.end() ; ++ it)
84     {
85         outfine<<"\t( line "<<(*it)+1<<" )"<<tq.text_line(*it)<<std::endl;
86         outfine<<"\t( line "<<(*it)+2<<" )"<<tq.text_line(*it+1)<<std::endl;
87     }
88 }
```

## 3、main.cpp

```
1 #include"head.h"
 2 
 3 int main()
 4 {
 5     std::ifstream infile("char.txt");
 6 
 7     if(!infile.is_open())
 8     {
 9         printf("No input file!\n");
10         return -1;
11     }
12     TextQuery tq;
13     tq.read_file(infile);
14     std::string query;
15     while (printf("enter user ID to look for:\n"),
16         std::cin>>query )
17     {
18         std::set<TextQuery::line_no> loc = tq.run_query(query);
19         print_resluts(loc,query,tq);
20     }
21     return 0;
22 }
```

# 三、需要改进的地方

## 1、

```
1 for(; it != ss.end() ; ++ it)
2     {
3         outfine<<"\t( line "<<(*it)+1<<" )"<<tq.text_line(*it)<<std::endl;
4         outfine<<"\t( line "<<(*it)+2<<" )"<<tq.text_line(*it+1)<<std::endl;
5     }
```

找到 用户名所在的行后， 我 直接把 用户名 所在的 下一行 作为 聊天内容，但其实 聊天内容里面 可以换行。

## 2、

如果 聊天 内容 里面 出现 用户名，会 误以为 是 用户名 所在的行，把这行输出 并把 下一行 输出。

