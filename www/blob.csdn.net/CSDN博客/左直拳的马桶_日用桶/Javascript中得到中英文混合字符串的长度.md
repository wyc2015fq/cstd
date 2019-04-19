# Javascript中得到中英文混合字符串的长度 - 左直拳的马桶_日用桶 - CSDN博客
2006年04月28日 15:43:00[左直拳](https://me.csdn.net/leftfist)阅读数：8157
Javascript中得到中英文混合字符串的长度
左直拳
有同事在公司的OA上发了个贴子，介绍在javascript中如何得到中英文混合字符串的长度。
用的是正则表达式。
var str = ”坦克是tank的音译”; 
var len = str.match(/[^ -~]/g) == null ? str.length : str.length + str.match(/[^ -~]/g).length ;
我查了一下书，有点明白了：
西文常用字符集由空格“”(0x20)到“~”(0x7e)构成，汉字会落在这个字符集外，而正则表达式[^ -~]表示除空格到“~”外的字符集。
string.match(regex) 会以数组形式返回字符串string中匹配正则表达式regex的子串，所以，
str.match(/[^ -~]/g)会以数组的形式返回一个个汉字。例如
var str = "dd大哥";
//显示"大，哥"，数组中返回两个汉字，数组长度为2
alert(str.match(/[^ -~]/g));
这样，var len = str.match(/[^ -~]/g) == null ? str.length : str.length + str.match(/[^ -~]/g).length ;就可以得到str的正确长度了。
在javascript中，一个汉字的长度也被计算为1，常常在提交到数据库中造成长度超标的错误，现在用这个方法，可以在提交前先检测一番了。
