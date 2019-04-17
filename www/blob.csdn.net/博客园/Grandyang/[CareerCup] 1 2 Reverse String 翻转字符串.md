# [CareerCup] 1.2 Reverse String 翻转字符串 - Grandyang - 博客园







# [[CareerCup] 1.2 Reverse String 翻转字符串](https://www.cnblogs.com/grandyang/p/4649783.html)







1.2 Implement a function void reverse(char *str) in C or C++ which reverses a null-terminated string.



这道题让我们用C++或C语言来翻转一个字符串，不算一道难题，在之前那道[Reverse Words in a String 翻转字符串中的单词](http://www.cnblogs.com/grandyang/p/4606676.html)中用到了这个函数，跟那道题比起来，这题算简单的了。C语言的版本要比C++的稍微复杂一些，应为string类集成了很多有用的功能，比如得到字符串的长度，用下标直接访问啊等等，C语言实现的时候要注意首先要用一个while循环来找到最后一个字符，然后再往中间走。参见代码如下：



C++:

```
class Solution {
public:
    void reverse(string  &s) {
        int left = 0, right = s.size() - 1;
        while (left < right) {
            char tmp = s[left];
            s[left++] = s[right];
            s[right--] = tmp;
        } 
    }
};
```



C

```
class Solution {
public:
    void reverse(char *str) {
        char *right = str;
        if (str) {
            while (*right) ++right;
            --right;
            while (str < right) {
                char tmp = *str;
                *str++ = *right;
                *right-- = tmp;
            }
        }
    }
};
```














