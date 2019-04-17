# Python内置函数(16)——ord - lincappu - 博客园







# [Python内置函数(16)——ord](https://www.cnblogs.com/lincappu/p/8144752.html)





**英文文档：**
- `ord`(*c*)
- 
- Given a string representing one Unicode character, return an integer representing the Unicode code point of that character. For example, `ord('a')` returns the integer `97` and `ord('€')` (Euro sign) returns `8364`. This is the inverse of `chr()`.
- 
- 
- 
- 
- 

　　返回 unicode 字符对应的整数 
- **说明：**
- 
- 
- 　　1. 函数功能传入一个Unicode 字符，返回其对应的整数数值。
- 

```
>>> ord('a')
97
>>> ord('@')
64
```

　　2. 其功能和chr函数刚好相反。

```
>>> chr(97)
'a'
>>> chr(64)
'@'
```












