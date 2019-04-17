# 156 Ananagrams（vector记录单词，map记录次数 排序标准化单词） - PeterBishop - CSDN博客





2018年07月11日 11:14:10[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[UVA](https://blog.csdn.net/qq_40061421/article/category/7793583)









```cpp
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
using namespace std;
 
map<string, int> cnt;//统计可转变字符的出现次数
vector<string> words;//保存原先字符的大小写
 //将每个单词化为字符层进行排序，这个思想可以省去排列的考虑，解题关键
string repr(const string& s)//把每个单词“标准化”
{
    string ans = s;
    for(int i = 0; i < ans.length(); ++i)
        ans[i] = tolower(ans[i]);
    sort(ans.begin(), ans.end());
    return ans;
}
 
int main()
{
    string s;
    while(cin >> s)//对输入的单词进行处理
    {
        if(s == "#") break;
        words.push_back(s);
        string r = repr(s);
        if(!cnt[r]) cnt[r] = 0;
        cnt[r]++;
    }
    vector<string> ans;
    for(int i = 0; i < words.size(); ++i)
        if(cnt[repr(words[i])] == 1) ans.push_back(words[i]);//从所有的存储单词中依次进行转化查找
    sort(ans.begin(), ans.end());按照字典序排序
    for(int i = 0; i < ans.size(); ++i)
        cout << ans[i] << endl;
    return 0;
}
```




