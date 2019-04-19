# ive函数 -- split 字符串分割函数 - 数据之美的博客 - CSDN博客
2018年06月06日 09:38:48[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：57
个人分类：[hive/sql](https://blog.csdn.net/oppo62258801/article/category/6961227)
hive字符串分割函数
split(str, regex) - Splits str around occurances that match regex
Time taken: 0.769 seconds, Fetched: 1 row(s)
返回值为一个数组
a.基本用法：
例1：
split('a,b,c,d',',')
得到的结果：
["a","b","c","d"]
b.截取字符串中的某个值：
当然，我们也可以指定取结果数组中的某一项
例2：
split('a,b,c,d',',')[0]
得到的结果：
a
c.特殊字符的处理：
特殊分割符号
regex 为字符串匹配的参数，所以遇到特殊字符的时候需要做特殊的处理
例3:  "." 点
split('192.168.0.1','.')
得到的结果：
[]
正确的写法：
split('192.168.0.1','\\.')
得到的结果：
["192","168","0","1"]
需要注意的是：
当然当split包含在 "" 之中时 需要加4个\
如 hive -e "....  split('192.168.0.1','\\\\.') ... "  不然得到的值是null
同样的 | 等特殊符号也需要做类似 处理。
