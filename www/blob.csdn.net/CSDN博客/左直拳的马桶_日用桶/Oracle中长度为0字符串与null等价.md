# Oracle中长度为0字符串与null等价 - 左直拳的马桶_日用桶 - CSDN博客
2015年04月15日 22:35:15[左直拳](https://me.csdn.net/leftfist)阅读数：3205
不试不知道，Oracle中，长度为0的字符串'' 居然与null等价！
众所周知，null代表空，什么都不存在，而一个字符串''，虽然长度为0，但毕竟已经是一个字符串，二者怎么能等价，混为一谈呢。
在SQL SERVER 中，
`SELECT ISNULL(ISNULL(null,''),'is null') AS str`
结果是 ''
但在Oracle中，
`SELECT NVL(NVL(null,''),'is null') FROM table1`
结果是 'is null'
