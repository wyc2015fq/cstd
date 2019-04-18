# LeetCode: Regular Expression Matching - bigfacesafdasgfewgf - CSDN博客





2015年10月03日 11:11:30[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：691











Implement regular expression matching with support for `'.'` and `'*'`.
'.' Matches any single character.
'*' Matches zero or more of the preceding element.

The matching should cover the entire input string (not partial).

The function prototype should be:
bool isMatch(const char *s, const char *p)

Some examples:
isMatch("aa","a") → false
isMatch("aa","aa") → true
isMatch("aaa","aa") → false
isMatch("aa", "a*") → true
isMatch("aa", ".*") → true
isMatch("ab", ".*") → true
isMatch("aab", "c*a*b") → true

代码：



```cpp
bool isMatch(char* s, char* p) {
    if( *p == '\0' )
        return *s == '\0';
        
    if( *( p + 1 ) != '*' )
    {
        if( *p == *s || ( *p == '.' && *s != '\0' ) )
            return isMatch( s + 1, p + 1 );
        return false;
    }
    else
    {
        while( *p == *s || ( *p == '.' && *s != '\0' ) )  //实现*的扩展
        {
            if( isMatch( s, p + 2 ) )  //需要跳过*来比较后面的p+2字符串
                return true;
            s++;  //保证了s, s+1都是重复的，类似于*扩展作用
        }
        return isMatch( s, p + 2 );
    }
}
```

1）这道题目的思路可以动态规划，解题思路可以参考：
http://blog.csdn.net/tingmei/article/details/8049850     

http://blog.csdn.net/doc_sgl/article/details/12719761


http://my.oschina.net/jdflyfly/blog/283584


根据下一个字符是否是'*'分情况判断。

如果p的下一个字符不是'*'，只需判断当前字符是否相等，或者p[cur]='.'，递归处理s[i+1]和p[j+1]；

如果是p的下一个'*'，则当前s和p相等或者p='.'情况下，依次判断s[0...s.length]和p2]是否match；程序中这部分使用while循环来实现‘*’的扩展功能








2）char数组判断结尾可以使用\0，也可以使用0；


















