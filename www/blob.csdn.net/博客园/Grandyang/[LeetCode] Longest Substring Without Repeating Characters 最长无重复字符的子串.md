# [LeetCode] Longest Substring Without Repeating Characters 最长无重复字符的子串 - Grandyang - 博客园







# [[LeetCode] Longest Substring Without Repeating Characters 最长无重复字符的子串](https://www.cnblogs.com/grandyang/p/4480780.html)








Given a string, find the length of the longest substring without repeating characters.


Example 1:
Input: "abcabcbb"
Output: 3 
Explanation: The answer is `"abc"`, with the length of 3. 


Example 2:
Input: "bbbbb"
Output: 1
Explanation: The answer is `"b"`, with the length of 1.


Example 3:
Input: "pwwkew"
Output: 3
Explanation: The answer is `"wke"`, with the length of 3. 
             Note that the answer must be a substring, `"pwke"` is a *subsequence* and not a substring.






这道求最长无重复子串的题和之前那道 [Isomorphic Strings](http://www.cnblogs.com/grandyang/p/4465779.html) 很类似，属于LeetCode的早期经典题目，博主认为是可以跟[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)媲美的一道题。给了我们一个字符串，让我们求最长的无重复字符的子串，注意这里是子串，不是子序列，所以必须是连续的。我们先不考虑代码怎么实现，如果给一个例子中的例子"abcabcbb"，让你手动找无重复字符的子串，该怎么找。博主会一个字符一个字符的遍历，比如a，b，c，然后又出现了一个a，那么此时就应该去掉第一次出现的a，然后继续往后，又出现了一个b，则应该去掉一次出现的b，以此类推，最终发现最长的长度为3。所以说，我们需要记录之前出现过的字符，记录的方式有很多，最常见的是统计字符出现的个数，但是这道题字符出现的位置很重要，所以我们可以使用HashMap来建立字符和其出现位置之间的映射。进一步考虑，由于字符会重复出现，到底是保存所有出现的位置呢，还是只记录一个位置？我们之前手动推导的方法实际上是维护了一个滑动窗口，窗口内的都是没有重复的字符，我们需要尽可能的扩大窗口的大小。由于窗口在不停向右滑动，所以我们只关心每个字符最后出现的位置，并建立映射。窗口的右边界就是当前遍历到的字符的位置，为了求出窗口的大小，我们需要一个变量left来指向滑动窗口的左边界，这样，如果当前遍历到的字符从未出现过，那么直接扩大右边界，如果之前出现过，那么就分两种情况，在或不在滑动窗口内，如果不在滑动窗口内，那么就没事，当前字符可以加进来，如果在的话，就需要先在滑动窗口内去掉这个已经出现过的字符了，去掉的方法并不需要将左边界left一位一位向右遍历查找，由于我们的HashMap已经保存了该重复字符最后出现的位置，所以直接移动left指针就可以了。我们维护一个结果res，每次用出现过的窗口大小来更新结果res，就可以得到最终结果啦。

这里我们可以建立一个HashMap，建立每个字符和其最后出现位置之间的映射，然后我们需要定义两个变量res和left，其中res用来记录最长无重复子串的长度，left指向该无重复子串左边的起始位置的前一个，由于是前一个，所以初始化就是-1，然后我们遍历整个字符串，对于每一个遍历到的字符，如果该字符已经在HashMap中存在了，并且如果其映射值大于left的话，那么更新left为当前映射值。然后映射值更新为当前坐标i，这样保证了left始终为当前边界的前一个位置，然后计算窗口长度的时候，直接用i-left即可，用来更新结果res。

这里解释下程序中那个if条件语句中的两个条件m.count(s[i]) && m[s[i]] > left，因为一旦当前字符s[i]在HashMap已经存在映射，说明当前的字符已经出现过了，而若m[s[i]] > left 成立，说明之前出现过的字符在我们的窗口内，那么如果要加上当前这个重复的字符，就要移除之前的那个，所以我们让left赋值为m[s[i]]，由于left是窗口左边界的前一个位置（这也是left初始化为-1的原因，因为窗口左边界是从0开始遍历的），所以相当于已经移除出滑动窗口了。举一个最简单的例子"aa"，当i=0时，我们建立了a->0的映射，并且此时结果res更新为1，那么当i=1的时候，我们发现a在HashMap中，并且映射值0大于left的-1，所以此时left更新为0，映射对更新为a->1，那么此时i-left还为1，不用更新结果res，那么最终结果res还为1，正确，代码如下：



C++ 解法一： 

```
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int res = 0, left = -1, n = s.size();
        unordered_map<int, int> m;
        for (int i = 0; i < n; ++i) {
            if (m.count(s[i]) && m[s[i]] > left) {
                left = m[s[i]];  
            }
            m[s[i]] = i;
            res = max(res, i - left);            
        }
        return res;
    }
};
```



下面这种写法是上面解法的精简模式，这里我们可以建立一个256位大小的整型数组来代替HashMap，这样做的原因是ASCII表共能表示256个字符，但是由于键盘只能表示128个字符，所以用128也行，然后我们全部初始化为-1，这样的好处是我们就不用像之前的HashMap一样要查找当前字符是否存在映射对了，对于每一个遍历到的字符，我们直接用其在数组中的值来更新left，因为默认是-1，而left初始化也是-1，所以并不会产生错误，这样就省了if判断的步骤，其余思路都一样：



C++ 解法二：

```
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> m(128, -1);
        int res = 0, left = -1;
        for (int i = 0; i < s.size(); ++i) {
            left = max(left, m[s[i]]);
            m[s[i]] = i;
            res = max(res, i - left);
        }
        return res;
    }
};
```



Java 解法二：

```
public class Solution {
    public int lengthOfLongestSubstring(String s) {
        int[] m = new int[256];
        Arrays.fill(m, -1);
        int res = 0, left = -1;
        for (int i = 0; i < s.length(); ++i) {
            left = Math.max(left, m[s.charAt(i)]);
            m[s.charAt(i)] = i;
            res = Math.max(res, i - left);
        }
        return res;
    }
}
```



下面这种解法使用了set，核心算法和上面的很类似，把出现过的字符都放入set中，遇到set中没有的字符就加入set中并更新结果res，如果遇到重复的，则从左边开始删字符，直到删到重复的字符停止：



C++ 解法三：

```
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int res = 0, left = 0, i = 0, n = s.size();
        unordered_set<char> t;
        while (i < n) {
            if (!t.count(s[i])) {
                t.insert(s[i++]);
                res = max(res, (int)t.size());
            }  else {
                t.erase(s[left++]);
            }
        }
        return res;
    }
};
```



Java 解法三：

```
public class Solution {
    public int lengthOfLongestSubstring(String s) {
        int res = 0, left = 0, right = 0;
        HashSet<Character> t = new HashSet<Character>();
        while (right < s.length()) {
            if (!t.contains(s.charAt(right))) {
                t.add(s.charAt(right++));
                res = Math.max(res, t.size());
            } else {
                t.remove(s.charAt(left++));
            }
        }
        return res;
    }
}
```



类似题目：

[Longest Substring with At Most Two Distinct Characters](http://www.cnblogs.com/grandyang/p/5185561.html)



参考资料：

[https://leetcode.com/problems/longest-substring-without-repeating-characters/solution/](https://leetcode.com/problems/longest-substring-without-repeating-characters/solution/)

[https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1737/C++-code-in-9-lines.](https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1737/C++-code-in-9-lines.)

[https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1812/Share-my-Java-solution-using-HashSet](https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1812/Share-my-Java-solution-using-HashSet)

[https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1729/11-line-simple-Java-solution-O(n)-with-explanation](https://leetcode.com/problems/longest-substring-without-repeating-characters/discuss/1729/11-line-simple-Java-solution-O(n)-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












