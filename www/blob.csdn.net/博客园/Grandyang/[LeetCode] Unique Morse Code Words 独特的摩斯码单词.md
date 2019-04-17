# [LeetCode] Unique Morse Code Words 独特的摩斯码单词 - Grandyang - 博客园







# [[LeetCode] Unique Morse Code Words 独特的摩斯码单词](https://www.cnblogs.com/grandyang/p/9338988.html)







International Morse Code defines a standard encoding where each letter is mapped to a series of dots and dashes, as follows: `"a"` maps to `".-"`, `"b"` maps to `"-..."`, `"c"` maps to `"-.-."`, and so on.

For convenience, the full table for the 26 letters of the English alphabet is given below:
[".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."]
Now, given a list of words, each word can be written as a concatenation of the Morse code of each letter. For example, "cab" can be written as "-.-.-....-", (which is the concatenation "-.-." + "-..." + ".-"). We'll call such a concatenation, the transformation of a word.

Return the number of different transformations among all words we have.
Example:
Input: words = ["gin", "zen", "gig", "msg"]
Output: 2
Explanation: 
The transformation of each word is:
"gin" -> "--...-."
"zen" -> "--...-."
"gig" -> "--...--."
"msg" -> "--...--."

There are 2 different transformations, "--...-." and "--...--.".



Note:
- The length of `words` will be at most `100`.
- Each `words[i]` will have length in range `[1, 12]`.
- `words[i]` will only consist of lowercase letters.



这道题说的就是大名鼎鼎的摩斯码了，给了我们所有字母的摩斯码的写法，然后给了我们一个单词数组，问我们表示这些单词的摩斯码有多少种。因为某些单词的摩斯码表示是相同的，比如gin和zen就是相同的。最简单直接的方法就是我们求出每一个单词的摩斯码，然后将其放入一个HashSet中，利用其去重复的特性，从而实现题目的要求，最终HashSet中元素的个数即为所求，参见代码如下：



```
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        vector<string> morse{".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
        unordered_set<string> s;
        for (string word : words) {
            string t = "";
            for (char c : word) t += morse[c - 'a'];
            s.insert(t);
        }
        return s.size();
    }
};
```



讨论：这道题其实没有充分发挥其潜力，摩斯码的场景很好，只是作为一道Easy题未免有些可惜了。一个比较显而易见的follow up就是，给我们一个摩斯码，问其有几种可能的单词组，比如给我们一个"--...-."，那么我们知道至少有两种zen和gin，可能还有更多，这样是不是就更加有趣了呢？



类似题目：

[https://leetcode.com/problems/unique-morse-code-words/solution/](https://leetcode.com/problems/unique-morse-code-words/solution/)

[https://leetcode.com/problems/unique-morse-code-words/discuss/120675/C++JavaPython-Easy-and-Concise-Solution](https://leetcode.com/problems/unique-morse-code-words/discuss/120675/C++JavaPython-Easy-and-Concise-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












