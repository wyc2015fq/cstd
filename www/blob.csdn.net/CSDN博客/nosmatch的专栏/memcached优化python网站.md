# memcached优化python网站 - nosmatch的专栏 - CSDN博客
2011年10月19日 15:47:41[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：644标签：[memcached																[python																[优化																[数据库																[php																[sqlite](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=memcached&t=blog)
本来我一直不知道怎么来更好地优化网页的性能，然后最近做python和php同类网页渲染速度比较时，意外地发现一个很简单很白痴但是我一直没发现的好方法（不得不BS我自己）：直接像某些php应用比如Discuz论坛那样，在生成的网页中打印出“本页面生成时间多少多少秒”，然后在不停地访问网页测试时，很直观地就能发现什么操作会导致瓶颈，怎样来解决瓶颈了。
于是我发现[SimpleCD](http://www.simplecd.org/)在生成首页时，意外地竟然需要0.2秒左右，真真不能忍：对比Discuz论坛首页平均生成才0.02秒，而Discuz论坛的首页页面无疑比SimpleCD的主页要复杂不少；这让我情何以堪啊，因为这必然不是Python语言导致的差距，只能说是我完全没做优化而Discuz程序优化得很好的后果。
# 优化分析
其实不用分析也能知道肯定是数据库在拖累，SimpleCD在生成首页时需要在sqlite的三个数据库中进行42多次查询，是历史原因导致的极其低效的一个设计；但是这40多次查询中，其实大部分是非常快的查询，仔细分析一下就有两个是性能大户，其他都不慢。
**第一个大户就是：获取数据个数**
`SELECT COUNT(*) FROM verycd`
这个操作每次都要花不少时间，这是因为每次数据库都要锁住然后遍历一遍主键统计个数的缘故，数据量越大耗时就越大，耗时为O(N)，N为数据库大小；实际上解决这个问题非常容易，只要随便在哪存一个当前数据的个数，只有在增删数据的时候改动就行了，这样时间就是O(1)的了。
**第二个大户就是：获取最新更新的20个数据列表**
```
SELECT verycdid,title,brief,updtime FROM verycd
```
`    ORDER BY updtime DESC LIMIT 20;`
因为在updtime上面做了索引，所以其实真正查询时间也就是搜索索引的时间而已。然则为什么这个操作会慢呢？因为我的数据是按照publish time插入的，按update time进行显示的话就肯定需要在至少20个不同的地方做I/O，这么一来就慢了。解决的方法就是让它在一个地方做I/O。也就是，除非数据库加入新数据/改变原有数据，否则把这条语句的返回结果缓存起来。这么一来又快了20倍：）
**接下来的是20条小case：取得发布人和点击数信息**
```
SELECT owner FROM LOCK WHERE id=XXXX;
```
`SELECT hits FROM stat WHERE id=XXXX;`
这里为什么没用sql的join语句来省点事呢？因为架构原因这些数据放在不同的数据库里，stat是点击率一类的数据库，因为需要频繁的插入所以用mysql存储；而lock和verycd是需要大量select操作的数据库，因为mysql悲剧的索引使用情况和分页效率而存放在了sqlite3数据库，所以无法join -.-
总之这也不是问题，跟刚才的解决方法一样，统统缓存
**所以纵观我这个例子，优化网页性能可以一言以蔽之，缓存数据库查询，即可。我相信大部分网页应用都是这样：）**
# Memcached终于出场
终于轮到memcached了，既然打算缓存，用文件做缓存的话还是有磁盘I/O，不如直接缓存到内存里面，内存I/O可就快多了。于是memcached顾名思义就是这么个东东。
memcached是很强大的工具，因为它可以支持分布式的共享内存缓存，大站都用它，对小站点来说，只要出得起内存，这也是好东西；首页所需要的内存缓冲区大小估计不会超过10K，更何况我现在也是内存土豪了，还在乎这个？
安装：ubuntu下还是方便啊
apt-get install memcached
apt-get install python-memcached
配置运行：因为是单机没啥好配的，改改内存和端口就行了
vi /etc/memcached.conf
/etc/init.d/memcached restart
在python的网页应用中使用之
```python
import memcache
```
```python
mc = memcache.Client(['127.0.0.1:11211'], debug=0)
```
memcache其实就是一个map结构，最常使用的就是两个函数了：
- 第一个就是***set(key,value,timeout)***，这个很简单就是把key映射到value，timeout指的是什么时候这个映射失效
- 第二个就是***get(key)***函数，返回key所指向的value
于是对一个正常的sql查询可以这么干
```python
sql = 'select count(*) from verycd'
```
```python
c = sqlite3.connect('verycd.db').cursor()
 
#原来的处理方式
c.execute(sql)
```
```python
count = c.fetchone()[0]
 
#现在的处理方式
from hashlib import md5
```
```python
key=md5(sql)
```
```python
count = mc.get(key)
```
```python
if not count:
```
```python
c.execute(sql)
```
```python
count = c.fetchone()[0]
```
```python
mc.set(key,count,60*5) #存5分钟
```
其中md5是为了让key分布更均匀，其他代码很直观我就不解释了。
# 优化结果和结论
优化过语句1和语句2后，首页的平均生成时间已经降低到0.02秒，和discuz一个量级了；再经过语句3的优化，最终结果是首页生成**时间降低到了0.006秒**左右，经过memcached寥寥几行代码的优化，**性能提高了3300%**。终于可以挺直腰板来看Discuz了：）
说memcached是妖孽，并不是因为memcached应用了之后性能狂升——这本是意料之中的事情，不这样反而才奇怪——而是因为我基本上没花多少时间就实现了这么妖孽的效果，至少我花的时间并不比写这篇blog的时间多。那么方便的使用方法和那么显著的性能提升状况真是让人咋舌。
# 题外话
话说最近的一些优化测试比较中，我被php的性能吓了一跳，完全和python是一个级别的，以前因为浮点性能鄙视过php是我不对，太片面了。
不过想想这样才是合理啊呵呵，随便什么应用IO才是瓶颈，语言的性能真的太不重要了而且太容易改进了；特别是解释性脚本语言，解释器的版本进步就会对性能带来很大提升，单纯的比较性能没有多大意义。比如随着java解释器的发展java已经快到让人瞠目结舌了，比如pypy就比CPython在科学计算中快5-10倍（实测）。
如果真要比的话，易用性和资源占用才是值得比较的对象，因为易用意味着更高的开发效率，而更少内存意味着更多并发的可能。关于易用性我就不说了，从内存来说，通过简单的测试来看php比python多消耗50%-100%的内存，但这很大程度上是因为php某些时候过于傻瓜化的设定，并不能真的说明php这门语言的内存控制烂。不多说了，都是一些我觉得挺有趣的小发现吧。
虽说我这么说好像是拼命想证明python比php优越的样子，但是其实刚好相反，用过不少php下面很方便的工具后，我越来越觉得php不错了，至少用的人多网页方面的模块很多，有时候真的会省很多事。也许我应该找时间专门写一篇比较php和python在实际应用中的文——而不是网上充斥的一些无意义的浮点数计算性能比较，就像我之前做过的那样——如果有人想看的话：）
转载[http://obmem.info/?p=717](http://obmem.info/?p=717)
