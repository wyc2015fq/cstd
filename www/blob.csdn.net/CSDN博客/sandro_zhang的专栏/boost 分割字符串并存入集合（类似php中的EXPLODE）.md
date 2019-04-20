# boost 分割字符串并存入集合（类似php中的EXPLODE） - sandro_zhang的专栏 - CSDN博客
2011年09月29日 16:00:58[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：482标签：[php																[token																[iterator																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=token&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[boost](https://blog.csdn.net/sandro_zhang/article/category/892177)
```cpp
string teststr = "1,2,3,4,5";
boost::char_separator<char> separator(",");
boost::tokenizer<boost::char_separator<char> > tokens(teststr, separator);
boost::tokenizer<boost::char_separator<char> >::iterator token_iter;
map<string>numbermap;
for (token_iter = tokens.begin(); token_iter != tokens.end(); token_iter++)
    numbermap.push_back(*token_iter);
```
