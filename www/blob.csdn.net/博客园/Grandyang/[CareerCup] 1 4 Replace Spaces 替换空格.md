# [CareerCup] 1.4 Replace Spaces 替换空格 - Grandyang - 博客园







# [[CareerCup] 1.4 Replace Spaces 替换空格](https://www.cnblogs.com/grandyang/p/4650780.html)







1.4 Write a method to replace all spaces in a string with '%20'. You may assume that the string has sufficient space at the end of the string to hold the additional characters, and that you are given the "true" length of the string. (Note: if implementing in Java, please use a character array so that you can perform this operation in place.)



这道题让我们将一个字符串里所有的空格都替换为'%20'，而且说明最好用in place的方法，就是说不要建新的字符串。如果建个新的字符，那这题解法就显得略low，因为那样我们只需要遍历原字符串s，遇到字符直接加进来，遇到空格，直接加个'%20'就行了。这道题要使用高大上的in place的方法，那就是我们先便利一遍给定字符串s，统计空格的个数，然后再resize一下字符串，增大字符串s的容量，以便我们来替换。然后我们再从原来s的末尾位置向开头遍历，遇到空格，在末尾三个空添加'%20'，然后标示新位置，遇到非空格字符，则添加该字符，然后再标示新位置即可。代码如下：



```
class Solution {
public:
    void replaceSpaces(string &s) {
        int count = 0, len = s.size(), newLen = 0;
        for (int i = 0; i < len; ++i) {
            if (s[i] == ' ') ++count;
        }
        newLen = len + 2 * count;
        s.resize(newLen);
        for (int i = len - 1; i >= 0; --i) {
            if (s[i] == ' ') {
                s[newLen - 1] = '0';
                s[newLen - 2] = '2';
                s[newLen - 3] = '%';
                newLen -= 3;
            } else {
                s[newLen - 1] = s[i];
                newLen -= 1;
            }   
        }
    }
};
```














