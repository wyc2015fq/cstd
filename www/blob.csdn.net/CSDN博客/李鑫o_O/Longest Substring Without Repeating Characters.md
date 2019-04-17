# Longest Substring Without Repeating Characters - 李鑫o_O - CSDN博客





2016年03月14日 22:10:56[hustlx](https://me.csdn.net/HUSTLX)阅读数：205








```python
<span style="font-size:14px;">#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
int lengthOfLongestSubstring(string s) {
    vector<int> temp(256, -1);
    int max_l = 0;
    int begin = 0;
    for (int i = 0; i < s.size(); i++) {
        begin = max(temp[s[i]]+1, begin);
        temp[s[i]] = i;
        max_l = max(max_l, i - begin + 1);
    }
    return max_l;
}
int main() {
    string b = "aabc";
    string a = "0";
    int res=lengthOfLongestSubstring(b);
}</span>
```




