# Lua语法小贴士（二）string库 - 凯奥斯的注释 - CSDN博客





2016年10月11日 11:25:31[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1751
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









## string库

byte方法，返回byte值：



```
print(string.byte("abc")) --97
print(string.byte("abc", 2, 3)) --98 99
```



char方法，连接char成字符串：



`print(string.char(100,101,102)) --def`



find方法，查找子字符串的起始和结束下标：



```
local s = "It's 10 o'clock in the morning."
local p = "%d+ o'"
print(string.find(s,p)) --6 10
print(string.find(s,p,7)) --7 10
print(string.find(s,p,1,true)) --nil
print(string.find(s,"o'",1,true)) --9 10
```

第三个参数代表从第几个字符开始查找，第四个参数表示是否是明文（即不使用变量模式）


match方法，查找子字符串：



```
local s = "Today is 10/10/2016"
local p = "%d+/%d+/%d+"
print(string.match(s,p)) --10/10/2016
print(string.match(s,p,11)) --0/10/2016
```




format方法，格式化字符串：


`print(string.format("%d,%f,%x,%s",10,7.25,92,"num")) -- 10,7.250000,5c,num`

gmach方法，返回匹配模式的遍历：



```
local s = "Hello world from lua"
for w in string.gmatch(s,"%a+") do
    print(w)
end
```



len方法，返回长度：




```
print("") --0
print("a\000bc\000") --5
```
lower方法，转换成小写字母：



`print(string.lower("LUA")) --lua`upper方法，转换成大写字母：

`print(string.upper("lua")) --LUA`


rep方法，重复字符串：



`print(string.rep("hia",3)) --hiahiahia`reverse方法，翻转字符串：



`print(string.reverse("lemon")) --monel`

sub方法，子串：


`print(string.sub("function"),5,8) --tion`



gsub方法，替换子串，返回替换后的字符串和被替换的次数：



```
local s = "All over the world"
lcoal p = "l"
local r = "x"
print(string.gsub(s,p,r))  --Axx over the worxd 3
print(string.gsub(s,p,r,1)) --Axl over the world 1
print(string.gsub(s,p,r,2)) --Axx over the world 2
print(string.gsub(s,p,r,3)) --Axx over the worxd 3
print(string.gsub(s,p,r,4)) --Axx over the worxd 3
```

dump方法，将传入的function转换成二进制的形式，这样就可以使用loadstring来获取function的副本。function必须是不包含upvalues的Lua function。
```
local function DumpTest()
    print("Test")
end
local a = string.dump(DumpTest)
print(a)
local b = loadstring(a)
print(b)
b()
```






参考：


[Lua语法小贴士（一）字符串基础操作](http://blog.csdn.net/ecidevilin/article/details/52775530)


[Lua语法小贴士（二）string库](http://blog.csdn.net/ecidevilin/article/details/52786404)


[Lua语法小贴士（三）魔法字符](http://blog.csdn.net/ecidevilin/article/details/52786488)



