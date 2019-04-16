# 转载和积累系列 - Lua table库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月14日 10:17:44[initphp](https://me.csdn.net/initphp)阅读数：1251








### table.concat(table, sep,  start, end)

table.concat()函数列出参数中指定table的数组部分从start位置到end位置的所有元素, 元素间以指定的分隔符(sep)隔开。除了table外, 其他的参数都不是必须的, 分隔符的默认值是空字符, start的默认值是1, end的默认值是数组部分的总长.



```
local tb = {"woshishen", "ooooooo"}
print(table.concat(tb, ":"))
```




### table.insert(table, pos, value)

table.insert()函数在table的数组部分指定位置(pos)插入值为value的一个元素. pos参数可选, 默认为数组部分末尾.



```
local tbl = {"alpha", "beta", "gamma"}
table.insert(tbl, "hello") --默认插入最后
table.insert(tbl, 2, "hehe") --第二个为止
print(table.concat(tbl, ":"))
```



### table.maxn(table)

table.maxn()函数返回指定table中所有正数key值中最大的key值. 如果不存在key值为正数的元素, 则返回0. 



```
local tbl = {"alpha", "beta", "gamma"}
print(table.maxn(tbl)) -- retrun 3
```




### table.remove(table, pos)

table.remove()函数删除并返回table数组部分位于pos位置的元素. 其后的元素会被前移. pos参数可选, 默认为table长度, 即从最后一个元素删起.



```
local tbl = {"alpha", "beta", "gamma"}
table.remove(tbl, 1) --删除第一个
print(table.concat(tbl, ";"))
```







### table.sort(table, comp)

table.sort()函数对给定的table进行升序排序.comp是一个可选的参数, 此参数是一个外部函数, 可以用来自定义sort函数的排序标准.



```
local tbl = {"alpha", "zeta", "gamma"}
table.sort(tbl) --升序排序
print(table.concat(tbl, ";"))
```




### table.getn(table)

返回table中元素的个数



```
local tbl = {"alpha", "zeta", "gamma"}
print(table.getn(tbl))
```





### table.setn(table, nSize)

设置table中的元素个数







### table.foreachi(table, function(i, v))

会期望一个从 1（数字 1）开始的连续整数范围，遍历table中的key和value逐对进行function(i, v)操作




### table.foreach(table, function(i, v))

与foreachi不同的是，foreach会对整个表进行迭代








