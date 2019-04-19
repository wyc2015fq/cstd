# CoffeeScript里的字符串插值 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月21日 09:22:49[左直拳](https://me.csdn.net/leftfist)阅读数：2766
拼接字符串是我们常干的事情。与其用很多的 “” + “”，不如用一下字符串插值，可读性好些。
方法是在字符串中加入#{ 变量、表达式、函数等}
```
getOtherName = ->
  "everyone"
name = "leftfist"
hi = "Hello #{name} !"
alert hi
hi = "Hello #{getOtherName()}"
alert hi
```
#{ }两个花括号方寸之间，就像是化外之地，或者以前的租界，完全不受字符串的影响，似乎写什么都可以。
但要注意，这仅限于双引号（"）括起来的字符串，如果一个字符串是单引号定义的，那这种机制将不会存在。
