# Boost正则表达式 - wishchinYang的专栏 - CSDN博客
2013年09月25日 16:34:13[wishchin](https://me.csdn.net/wishchin)阅读数：616
在C++编程中，有一点让人挺遗憾的就是C++尚不支持正则表达式，这让很多用户为了编写支持正则表达式程序而不得不放弃C++。然而，Boost.Regex库填补了C++在这方面的空白，它使C++很好的支持各种引擎的正则表达式。
详细文章链接：[http://club.topsage.com/thread-2276543-1-1.html](http://club.topsage.com/thread-2276543-1-1.html)
boost regex笔记：[http://blog.csdn.net/shiwei0124/article/details/4553665](http://blog.csdn.net/shiwei0124/article/details/4553665)
        在C++编程中，有一点让人挺遗憾的就是C++尚不支持正则表达式，这让很多用户为了编写支持正则表达式程序而不得不放弃C++。然而，Boost.Regex库填补了C++在这方面的空白，它使C++很好的支持各种引擎的正则表达式。
结合我的学习，逐步分析Boost.Regex库。
……
Boost.Regex默认使用Perl正则表达式，关于Perl正则表达式的使用，这里就不多说明了，可以参考相关资料。
Boost的正则表达式封装在boost::basic_regex对象中，与std::basic_string一样，boost::basic_regex表示的是一族类，也与std::basic_string一样typedef了几个特例：
typedef basic_regex<char> regex;
typedef basic_regex<wchar>wregex;
Boost.Regex的几个中重要的成员函数如下：
1.explicit basic_regex（const CharT* p,flag_type regex_constants::normal）;
该构造函数接受一个包含正则表达式的字符序列，以及一个表示正则表达式所选的参数信息——例如是否大小写或使用什么引擎的正则表达式。这里注意，如果传递的字符序列是无效的正则表达式，则会抛出异常regex_error。
2.bool emply（）const；
该成员函数检测basic_regex示例是否包含有效的正则表达式。
3.unsigned mark_count() const;
该成员函数返回该basic_regex示例中的正则表达式包含的有标记的子表达式的个数。
4.flag_type flags()const;
该函数返回一个位掩码，其中包含basic_regex所设置的选项标志。具体标志选项有：icase—忽略字符大小写和JavaScript—regex使用JavaScript语法
另外，Boost.Regex定义了几个自由函数，实现正则表达式的匹配查询及修改：
1.判断正则表达式时候匹配整个字符串
 1![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)template <typename CharT,typename Allocator,typename traits>
 2![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 3![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)bool regex_match（
 4![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 5![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const CharT*, 
 6![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 7![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)match_result<const chart*，Allocator>&m,
 8![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 9![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const basic_regex<CharT,traits>& reg,
10![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
11![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)match_flag_type flags=match_default);
12![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
2.查找字符串中与正则表达式匹配的子序列
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)template <typename CharT,typename Allocator,typename traits>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)bool regex_rearch（
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const CharT*, 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)match_result<const chart*，Allocator>&m,
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const basic_regex<CharT,traits>& reg,
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)match_flag_type flags=match_default);
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
3.查找字符串中所有正则表达式的匹配，并根据参数fmt格式化所匹配的串
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)template <typename CharT,typename Allocator,typename traits>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)basic_string<CharT > regex_replace（
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const basic_string<CharT >& s,   
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const basic_regex<CharT,traits>& reg,
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)const basic_string<CharT >& fmt,       
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)match_flag_type flags=match_default);
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
使用说明
1.创建regex对象：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)Include<boost/regex.hpp>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)regex reg(“(.*)”);
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
2.regex_match
该函数用来对一个字符串的完全匹配，在很多校验信息中可以广泛使用，具体使用示例见附后的测试代码
3.regex_rearch
说到这个函数，必须要说明下boost.match_result。 regex_rearch在执行查找时，通过一个match_result类型的对象来报告匹配的自表达式。
match_result主要封装了一个std::vector<sub_match<<…>> >类型的对象,而sub_match类继承自std::pair,主要记录匹配的结果信息。关于match_result和sub_match的详细了解可以阅读boost设计源码：
使用示例查看附后的测试源码。
4.regex_replace
该函数根据指定的fmt格式化通过正则表达式匹配的子串。需要注意的是，该函数不会修改原字符串，只是将格式化后的结果返回。具体使用示例见附后测试源码。
5.regex_iterator
通过多次调用regex_rearch我们可以处理所有满足匹配的字串。但是，Regex库还给我们提供了一个更优雅的方法——即通过regex_iterator。通过字符串和正则表达式构造regex_iterator的时候会构建一个match_result的对象用于保存匹配结果信息，再通过重载++运算符达到遍历所有匹配信息的目的。关于regex_iterator的详细了解可以参考Regex的设计源码：
使用示例查看附后的测试源码。
6.regex_token_iterator
与regex_iterator相似，Regex还提供了一个列举与正则表达式不匹配的子表达式，就是regex_token_iterator。与stl的设计类似，是通过迭代器适配器实现的。这个特性让我们很容易的分割字符串。关于regex_token_iterator的详细了解请查看Regex的设计源码：
7．测试源码
 1![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<iostream>
 2![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<string>
 3![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<cassert>
 4![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<boost/regex.hpp>
 5![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<vector>
 6![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include<iterator>
 7![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
 8![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif)bool validate_identify_card(const std::string&s){
 9![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    boost::regex reg("//d{17}[A-Z0-9]");
10![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return boost::regex_match(s,reg);
11![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
12![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif)class regex_callback{
13![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)public:
14![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    template<typename T>
15![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)voidoperator()(const T& what){
16![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cout<<what[1].str()<<std::endl;
17![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
18![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)};
19![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
20![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif)int main(){
21![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
22![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("//d{3}([a-zA-Z]+).(//d{2}|N/A)//s//1");
23![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string correct="123Hello N/A Hello";
24![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string incorrect="123Hello 12 hello";
25![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        assert(boost::regex_match(correct,reg)==true);
26![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        assert(boost::regex_match(incorrect,reg)==false);
27![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
28![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
29![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="421124598608976345";
30![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        assert(validate_identify_card(s)==true);
31![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
32![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
33![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("(new)|(delete)");
34![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::smatch m;
35![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int new_counter=0,delete_counter=0;
36![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="Calls to new must be followed by delete.Calling simply new results in a Leak!";
37![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string::const_iterator it=s.begin();
38![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string::const_iterator end=s.end();
39![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)while(boost::regex_search(it,end,m,reg)){//这里参数必须是const属性
40![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)if(m[1].matched){
41![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                std::cout<<"The expression(new) matched"<<std::endl;
42![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                new_counter++;
43![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
44![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)if(m[2].matched){
45![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                std::cout<<"The expression(delete) matched"<<std::endl;
46![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                delete_counter++;
47![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
48![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            it=m[0].second;
49![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
50![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cout<<"new_counter="<<new_counter<<std::endl;
51![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cout<<"delete_counter="<<delete_counter<<std::endl;
52![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
53![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){//使用boost::regex_replace
54![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("(colo)(u)(r)",boost::regex::icase|boost::regex::perl);
55![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="Colour,colour,color,colOurize";
56![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        s=boost::regex_replace(s,reg,"$1$3");//regex_replace不直接修改s的值，而是返回新值
57![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cout<<s<<std::endl;
58![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
59![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){//使用boost::regex_iterator迭代器输出所有匹配项
60![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("(//d+),?");
61![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="1,2,3,4,5,6,7,85,ad2348(,hj";
62![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::sregex_iterator it(s.begin(),s.end(),reg);
63![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::sregex_iterator end;
64![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        for_each(it,end,regex_callback());
65![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
66![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
67![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("/");
68![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::vector<std::string> vec;
69![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="Split/Vulue/Teather/Neusoft/Write/By/Lanwei";
70![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::sregex_token_iterator it(s.begin(),s.end(),reg,-1);
71![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::sregex_token_iterator end;
72![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)while(it!=end)
73![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            vec.push_back(*it++);
74![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        copy(vec.begin(),vec.end(),std::ostream_iterator<std::string>(std::cout,"/n"));
75![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
76![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
77![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::smatch m;
78![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        boost::regex reg("(new)|(delete)");
79![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string s="Calls to new must be followed by delete.Calling simply new results in a Leak!";
80![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string::const_iterator it=s.begin();
81![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::string::const_iterator end=s.end();
82![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)while(boost::regex_search(it,end,m,reg)){
83![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"size="<<m.size()<<std::endl;
84![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"m[-2]="<<m[-2]<<std::endl;
85![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"m[-1]="<<m[-1]<<std::endl;
86![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"m[0]="<<m[0]<<std::endl;
87![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"m[1]="<<m[1]<<std::endl;
88![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)//std::cout<<"m[2].type="<<typeid(m[2].first).name()<<std::endl;
89![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            std::cout<<"m[2]="<<m[2]<<std::endl;
90![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// std::cout<<"m[2].first="<<m[2].first<<std::endl;
91![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)//std::cout<<"m[2].second="<<m[2].second<<std::endl;
92![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            it=m[0].second;
93![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
94![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
95![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return0;
96![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
97![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
学习小结：
关于Boost.Regex库的初步学习暂时告以段落。这是个非常有用的库，打破了用户只有通过POSIX C的API实现正则表达式的局限。然而，这是一个伟大的库博大精深的库，以上的了解只不过的凤毛麟角，其内部还有很多的隐藏秘密需要花大量的时间去挖掘，探索。
          所以，如果不是纯粹的要长期使用C++boost的话，就不要在Boost里面探索了！！！
