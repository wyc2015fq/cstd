# python 判断变量是否是 None 的三种写法 - lincappu - 博客园







# [python 判断变量是否是 None 的三种写法](https://www.cnblogs.com/lincappu/p/8305763.html)





代码中经常会有变量是否为None的判断，有三种主要的写法：
第一种是`if x is None`；
第二种是 `if not x：`；
第三种是`if not x is None`（这句这样理解更清晰`if not (x is None)`） 。
如果你觉得这样写没啥区别，那么你可就要小心了，这里面有一个坑。先来看一下代码：




[?](http://www.jb51.net/article/93165.htm#)

```
`>>> x ``=``1`

`>>> ``not``x`

`False`

`>>> x ``=``[``1``]`

`>>> ``not``x`

`False`

`>>> x ``=``0`

`>>> ``not``x`

`True`

`>>> x ``=``[``0``]     ``# You don't want to fall in this one.`

`>>> ``not``x`

`False`
```




在python中 None, False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()都相当于False ，即：



复制代码代码如下:


not None == not False == not '' == not 0 == not [] == not {} == not ()



因此在使用列表的时候，如果你想区分x==[]和x==None两种情况的话, 此时`if not x:`将会出现问题：




[?](http://www.jb51.net/article/93165.htm#)

```
`>>> x ``=``[]`

`>>> y ``=``None`

`>>> `

`>>> x ``is``None`

`False`

`>>> y ``is``None`

`True`

`>>> `

`>>> `

`>>> ``not``x`

`True`

`>>> ``not``y`

`True`

`>>> `

`>>> `

`>>> ``not``x ``is``None`

`>>> ``True`

`>>> ``not``y ``is``None`

`False`

`>>>`
```




也许你是想判断x是否为None，但是却把`x==[]`的情况也判断进来了，此种情况下将无法区分。
对于习惯于使用if not x这种写法的pythoner，必须清楚x等于None, False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()时对你的判断没有影响才行。 
而对于`if x is not None`和`if not x is None`写法，很明显前者更清晰，而后者有可能使读者误解为`if (not x) is None`，因此推荐前者，同时这也是谷歌推荐的风格

**结论：**
`if x is not None`是最好的写法，清晰，不会出现错误，以后坚持使用这种写法。
使用if not x这种写法的前提是：必须清楚x等于None, False, 空字符串"", 0, 空列表[], 空字典{}, 空元组()时对你的判断没有影响才行。












