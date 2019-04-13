
# to_char前有空格 - 追求卓越,做到专业 - CSDN博客


2012年09月20日 17:37:27[Waldenz](https://me.csdn.net/enter89)阅读数：1250个人分类：[Oracle																](https://blog.csdn.net/enter89/article/category/1078928)


今天发现用to_char去转换数字为字符串时,前面有个空格.
select to_char(12,'000') from dual ; result: 012(012前有空格)
经查发现,那个空格位置是放符号的，正的数字就空了，负的就是一个‘-’号而没有空格,所以使用fm就可以解决.
select to_char(12,'fm000') from dual; result:012(012前无空格)

