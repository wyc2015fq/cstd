
# leetcode小白解题记录——第十题 - lin_tuer的博客 - CSDN博客


2016年09月30日 17:13:07[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：946


Regular Expression Matching
Implement regular expression matching with support for '.' and '*'.
'.' Matches any single character.
'*' Matches zero or more of the preceding element.
The matching should cover the entire input string (not partial).
The function prototype should be:
bool isMatch(const char *s, const char *p)
Some examples:
isMatch("aa","a") return false
isMatch("aa","aa") return true
isMatch("aaa","aa") return false
isMatch("aa", "a*") return true
isMatch("aa", ".*") return true
isMatch("ab", ".*") return true
isMatch("aab", "c*a*b") return true
这里面，动态规划，以及递归的思想很重要分析：
First of all, this is one of the most difficulty problems. It is hard to think through all different cases. The problem should be simplified to handle 2 basic cases:
the second char of pattern is "*"
the second char of pattern is not "*"
For the 1st case, if the first char of pattern is not ".", the first char of pattern and string should be the same. Then continue to match the remaining part.
For the 2nd case, if the first char of pattern is "." or first char of pattern == the first i char of string, continue to match the remaining part.
```java
public boolean isMatch(String s, String p) {
	// base case
	if (p.length() == 0) {
		return s.length() == 0;
	}
 
	// special case
	if (p.length() == 1) {
 
		// if the length of s is 0, return false
		if (s.length() < 1) {
			return false;
		}
 
		//if the first does not match, return false
		else if ((p.charAt(0) != s.charAt(0)) && (p.charAt(0) != '.')) {
			return false;
		}
 
		// otherwise, compare the rest of the string of s and p.
		else {
			return isMatch(s.substring(1), p.substring(1));
		}
	}
 
	// case 1: when the second char of p is not '*'
	if (p.charAt(1) != '*') {
		if (s.length() < 1) {
			return false;
		}
		if ((p.charAt(0) != s.charAt(0)) && (p.charAt(0) != '.')) {
			return false;
		} else {
			return isMatch(s.substring(1), p.substring(1));
		}
	}
 
	// case 2: when the second char of p is '*', complex case.
	else {
		//case 2.1: a char & '*' can stand for 0 element
		if (isMatch(s, p.substring(2))) {
			return true;
		}
 
		//case 2.2: a char & '*' can stand for 1 or more preceding element, 
		//so try every sub string
		int i = 0;
		while (i<s.length() && (s.charAt(i)==p.charAt(0) || p.charAt(0)=='.')){
			if (isMatch(s.substring(i + 1), p.substring(2))) {
				return true;
			}
			i++;
		}
		return false;
	}
}
```
大神解答网址：http://www.programcreek.com/2012/12/leetcode-regular-expression-matching-in-java/


