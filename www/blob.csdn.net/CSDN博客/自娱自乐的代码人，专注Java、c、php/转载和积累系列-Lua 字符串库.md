# 转载和积累系列 - Lua 字符串库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月13日 11:01:42[initphp](https://me.csdn.net/initphp)阅读数：1161








### string.len(s)

返回字符串长度




### string.rep(s, n)

返回重复n次字符串s的串




### string.lower(s)  

将字符串转化为小写方式




### string.upper(s)  

将字符串转化为大写




### string.sub(s,i,j)

函数截取字符串s的从第i个字符到第j个字符之间的串。




### string.char

string.char 将字符对应的数字转换成相应的字符。


`print(string.char(97)) --> a`



### string.byte(s, i)

将字符串s的第i个字符的转换成整数；第二个参数是可选的，缺省情况下i=1


`print(string.byte("abc", 2)) --> 98`



### string.format()

格式化字符串。



```
%c - 接受一个数字, 并将其转化为ASCII码表中对应的字符
%d, %i - 接受一个数字并将其转化为有符号的整数格式
%o - 接受一个数字并将其转化为八进制数格式
%u - 接受一个数字并将其转化为无符号整数格式
%x - 接受一个数字并将其转化为十六进制数格式, 使用小写字母
%X - 接受一个数字并将其转化为十六进制数格式, 使用大写字母
%e - 接受一个数字并将其转化为科学记数法格式, 使用小写字母e
%E - 接受一个数字并将其转化为科学记数法格式, 使用大写字母E
%f - 接受一个数字并将其转化为浮点数格式
%g(%G) - 接受一个数字并将其转化为%e(%E, 对应%G)及%f中较短的一种格式
%q - 接受一个字符串并将其转化为可安全被Lua编译器读入的格式
%s - 接受一个字符串并按照给定的参数格式化该字符串

--例子：
string.format("%%c: %c", 83)            输出S
string.format("%+d", 17.0)              输出+17
string.format("%05d", 17)               输出00017
string.format("%o", 17)                 输出21
string.format("%u", 3.14)               输出3
string.format("%x", 13)                 输出d
string.format("%X", 13)                 输出D
string.format("%e", 1000)               输出1.000000e+03
string.format("%E", 1000)               输出1.000000E+03
string.format("%6.3f", 13)              输出13.000
string.format("%q", "One\nTwo")         输出"One\
                                        　　Two"
string.format("%s", "monkey")           输出monkey
string.format("%10s", "monkey")         输出    monkey
string.format("%5.3s", "monkey")        输出  mon
```




### string.reverse

将字符串字符顺序进行逆转。


`print(string.reverse("abcd")) --> dcba`





### string.find

搜索字符串


`print(string.find("haha", 'ah') )  ----- 输出 2 3  `


### string.gmatch

返回一个迭代器函数,每次调用,返回下一个捕获从模式字符串。。




```
s = "hello world from Lua"
for w in string.gmatch(s, "%a+") do
	print(w)
end
```


```
t = {}
     s = "from=world, to=Lua"
     for k, v in string.gmatch(s, "(%w+)=(%w+)") do
       t[k] = v
     end
```







### string.match

查找第一个匹配模式的字符串。


`print(string.match("abcd", "a"))`











