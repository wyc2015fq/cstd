# leetcode-76. Minimum Window Substring 最小覆盖子串 - 别说话写代码的博客 - CSDN博客





2019年03月21日 22:26:10[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given a string S and a string T, find the minimum window in S which will contain all the characters in T in complexity O(n).**Example:****Input: S** = "ADOBECODEBANC", **T** = "ABC"**Output:** "BANC"**Note:**- If there is no such window in S that covers all characters in T, return the empty string `""`.- If there is such window, you are guaranteed that there will always be only one unique minimum window in S.|给定一个字符串 S 和一个字符串 T，请在 S 中找出包含 T 所有字母的最小子串。**示例：****输入: S** = "ADOBECODEBANC", **T** = "ABC"**输出:** "BANC"**说明：**- 如果 S 中不存这样的子串，则返回空字符串 `""`。- 如果 S 中存在这样的子串，我们保证它是唯一的答案。|
|----|----|

思路：用个哈希表先统计t串中每个字符出现的次数。然后在s串中比，若当前 字符在t中出现，cnt++，如果当cnt==t.size()也就是t的长度的时候， 说明当前 滑动窗口包含了所有t中的元素。使用minlen记录 最短的滑动窗口大小，res保存最短滑动窗口的 字符串。用left记录滑动窗口最左边的位置，若最左边的字符不属于t或者包含了多余的t中的元素，就需要从左边缩小滑动窗口的大小。（因为题目保证子串唯一，所以可以用滑动窗口做）举个栗子：

S = "ADBANC"，T = "ABC"。首先第一个是A，嗯很好，T中有，第二个是D，T中没有，不理它，第三个是B，嗯很好，T中有，第四个又是A，多了一个，礼多人不怪嘛，收下啦，第五个是N，一边凉快去，第六个终于是C了，那么貌似好像需要整个S串，其实不然，我们注意之前有多一个A，但么我们就算去掉第一个A，也没事，因为第四个A可以代替之，第二个D也可以去掉，因为不在T串中，第三个B就不能再去掉了，不然就没有B了。所以最终的答案就"BANC"了。通过上面的描述，你有没有发现一个有趣的现象，我们是先扩展，再收缩，就好像一个窗口一样，先扩大右边界，然后再收缩左边界，上面的例子中我们是右边界无法扩大了后才开始收缩左边界，实际上对于复杂的例子，有可能是扩大右边界，然后缩小一下左边界，然后再扩大右边界等等。这就很像一个不停滑动的窗口了，这就是大名鼎鼎的滑动窗口Sliding Window了，简直是神器啊，能解很多子串，子数组，子序列等等的问题，是必须要熟练掌握的啊！



```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        string res="";
        int cnt=0,left=0,minlen=INT_MAX;
        vector<int> vec(256,0);
        for(auto c:t) vec[c]++;
        for(int i=0;i<s.size();++i)
        {
            if(--vec[s[i]]>=0) ++cnt; //cnt统计s串中输入t串的长度
            while(cnt==t.size())   //长度一样，当前滑动窗包含了t所有元素
            {
                if(minlen>i-left+1)   //判断当前窗口是不是最短的
                {
                    minlen=i-left+1;
                    res=s.substr(left,minlen);
                }
                if(++vec[s[left]]>0) --cnt;   //如果前面开始有不属于t或者多余的与t串重复的字符
                left++;
            }
        }
        return res;
    }
};
```

代码及思路参考自：[http://www.cnblogs.com/grandyang/p/4340948.html](http://www.cnblogs.com/grandyang/p/4340948.html)



