# python itertools模块学习 - Arthur的随笔 - CSDN博客
2011年10月25日 22:09:04[largetalk](https://me.csdn.net/largetalk)阅读数：9400
一直对itertools怀有敬畏之心，其实会了就还好了，that's nothing
这里的迭代器最主要得特点就是lazy，和stl里面得迭代器还是不一样得，最明显得好处就是节省内存了。
顾名思义，itertools得函数返回得都是迭代器，为简单起事，下面就不专门说明了
count(p,q) 返回p, p+q, p+2*q, ....
cycle(p)  返回 p[0], p[1],...p[last],p[0],p[1]......
repeat(p, n) 返回 p...p (n times)
chain(p,q..) 把几个参数连接起来
compress(data, selectors)   比如compress('ABCDEF',[1,0,1,0,1,1])-->ACEF
dropwhile(pred, seq)    比如dropwhile(lambdax:x<5,[1,4,6,4,1])-->641
```python
In [101]: for n, v in itertools.groupby('aaabbbb ccc aaaaa'):
   .....:     print n, list(v)
   .....: 
a ['a', 'a', 'a']
b ['b', 'b', 'b', 'b']
  [' ']
c ['c', 'c', 'c']
  [' ']
a ['a', 'a', 'a', 'a', 'a']
```
ifilter(pred, seq)      ifilter(lambdax:x%2,range(10))-->13579
ifilterfalse(pred, seq)  ifilterfalse(lambdax:x%2,range(10))-->02468
islice(seq,
 start, stop, step)     islice('ABCDEFG',2,None)-->CDEFG
imap(fun,
 p, q)                    imap(pow,(2,3,10),(5,2,3))-->3291000
starmap(fun,
 seq)                  starmap(pow,[(2,5),(3,2),(10,3)])-->3291000
tee(it,
 n)  将迭代器it复制n份
takewhile(pred, seq)               takewhile(lambdax:x<5,[1,4,6,4,1])-->14
izip(p,q,...)                      izip('ABCD','xy')-->AxBy
izip_longest(p,q,...)              izip_longest('ABCD','xy',fillvalue='-')-->AxByC-D-
product(p,q,...)
   笛卡尔积
permutations(seq,
 r)              permutations('ABCD',2) -->ABACADBABCBDCACBCDDADBDC
combination(seq,
 r)                          combinations('ABCD',2) -->ABACADBCBDCD
combinations_with_replacement(seq,
 r)    combinations_with_replacement('ABCD',2) -->AAABACADBBBCBDCCCDDD
其实这些都是python document里面得内容了，一直没细看，今天终于抽空看了下
