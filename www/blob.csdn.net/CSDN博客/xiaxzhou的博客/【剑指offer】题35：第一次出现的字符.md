# 【剑指offer】题35：第一次出现的字符 - xiaxzhou的博客 - CSDN博客





2017年07月04日 16:04:32[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：97
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









在一个字符串(1<=字符串长度<=10000，全部由字母组成)中找到第一个只出现一次的字符,并返回它的位置

```
int FirstNotRepeatingChar(string str)
{
    if (str.size()<1)
    {
        return -1;
    }
    hash_map<char,pair<int,int>> my_map;
    //vector<pair<int, int>> my_map(256);
    for (auto i = 0; i < str.size();++i)
    {
        my_map[str[i]].first++;
        my_map[str[i]].second = i;
    }
    for (auto i = 0; i < str.size();++i)
    {
        if (my_map[str[i]].first == 1 )
        {
            return my_map[str[i]].second;
        }
    }
    return -1;
}
```



