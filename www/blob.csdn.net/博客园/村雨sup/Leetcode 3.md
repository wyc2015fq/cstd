# Leetcode 3 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





O(N^3)的复杂度竟然过了(╯‵□′)╯︵┻━┻

```
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        
        if(s == "")
            return 0;
        int res = 1;
        
        for(int i=0;i < s.length();i++){
            int count = 1;
            for(int j=i+1;j < s.length();j++){
                int flag = 0;
                for(int k=i;k < j;k++){
                    if(s[k] == s[j]){flag = 1;break;}
                }
                if(flag == 0) count++;
                else break;
            }
            if(count > res) res = count;
        }
        return res;
    }
};
```

_

复杂度O(N)解法

这里我们可以建立一个256位大小的整型数组来代替HashMap，这样做的原因是ASCII表共能表示256个字符，所以可以记录所有字符，然后我们需要定义两个变量res和left，其中res用来记录最长无重复子串的长度，left指向该无重复子串左边的起始位置，然后我们遍历整个字符串，对于每一个遍历到的字符，如果哈希表中该字符串对应的值为0，说明没有遇到过该字符，则此时计算最长无重复子串，i - left +１，其中ｉ是最长无重复子串最右边的位置，left是最左边的位置，还有一种情况也需要计算最长无重复子串，就是当哈希表中的值小于left，这是由于此时出现过重复的字符，left的位置更新了，如果又遇到了新的字符，就要重新计算最长无重复子串。最后每次都要在哈希表中将当前字符对应的值赋值为i+1。代码如下：

```
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int Map[256] = {0};
        int left = 0;
        int res = 0;
        for(int i=0;i < s.length();i++){
            if(Map[s[i]] == 0 || Map[s[i]] < left){
                res = max(res,i-left+1);
            }
            else{
                left = Map[s[i]];
            }
            Map[s[i]] = i+1;
        }
        return res;
    }
};
```

_













