# Lua语法小贴士（九）table库 - 凯奥斯的注释 - CSDN博客





2016年11月02日 22:46:38[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：708
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# table库方法包括：concat，insert，maxn，remove和sort。

table.concat(tbl[,sep[,i[,j]]])

连接表格为字符串，tbl为需要连接的字符串表，sep为分隔符，i为起始下标，j为结束下标。

示例：



```
local alphabet = {"a","b","c"}
print(table.concat(alphabet))				--abc
print(table.concat(alphabet,","))			--a,b,c
print(table.concat(alphabet,",", 1,2))		--a,b
print(table.concat(alphabet,",", 2,3))		--b,c
```

table.insert(tbl,[pos,]val)
在表中插入数值，tbl为需要插入的表，pos为插入的位置，val为被插入的值。

示例：



```
local tbl = {}
table.insert(tbl,"a")
table.insert(tbl,"b")
table.insert(tbl, 3, "c")
print(table.concat(tbl))				--abc
```

table.maxn(tbl)
返回表中的最大数值。

示例：



```
local t = {1,2,3}
print(table.maxn(t))		--3
t[5] = 5
print(table.maxn(t))		--5
t[1024] = 1024
print(table.maxn(t))		--1024
```

table.remove(tbl, pos)
移除表中的元素，tbl为需要移除的表，pos为被移除的位置。

示例：



```
local tt = {"a","b","c","d"}
print(table.remove(tt, 3))		--c
print(table.remove(tt, 4))		--
print(table.concat(tt))			--abd
```
table.sort(tbl[,comp])
对表进行排序，tbl是需要排序的表，comp为排序方法。

示例：



```
local tc = {1,3,2,6,5,4}
table.sort(tc)
print(table.concat(tc))		--123456
local compFunc = function(a, b)
	return a > b
end
table.sort(tc,compFunc)
print(table.concat(tc))		--654321
```





