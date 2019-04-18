# Lua语法小贴士（十六）pairs和next - 凯奥斯的注释 - CSDN博客





2017年09月14日 12:10:29[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2538
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# 今天看到一个帖子，是关于lua优化的，上面说清空一个表的时候，pairs的效率是next的500倍。我记得for pairs循环实际上就是调用的就是next，还特意查了源码，应证了我的记忆。源码就不贴了，直接给结论。






```
for k, v in pairs(tbl) do
end
```

基本等同于




```
for k, v in next, tbl, nil do
end
```




pairs(tbl)返回三个值，称为迭代方法（源码注释中为generator）、状态和初始值。

而实际上这个三个值便是next，tbl和nil。

第一次循环，将tbl和nil传入next方法，返回k和v（如果是空表，返回nil，中断循环）。

第二次循环，将tbl和k传入next方法，返回新的k和v（如果是空表，返回nil，中断循环）。

……




ipairs与之类似，只不过ipairs返回的迭代方法是一个没有暴露出来的方法，而初始值是0。

第一次循环，将tbl和i（初始值0）传入迭代方法，i++，然后用rawgeti获取第一个值v，返回i和v（如果是空表，直接返回，中断循环）。

第二次循环，将tbl和i（当前值1）传入迭代方法，i++，然后用rawgeti获取第二个值v，返回i和v（如果是空表，直接返回，中断循环）。

……




因为帖子没有给出代码，我也不敢武断的就说对方是错的。但是我认为，没有上下文的情况下，对方直接给出pairs比next快很多的结论也是有很大问题的。



