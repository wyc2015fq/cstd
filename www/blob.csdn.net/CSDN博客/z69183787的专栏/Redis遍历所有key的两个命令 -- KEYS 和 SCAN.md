# Redis遍历所有key的两个命令 -- KEYS 和 SCAN - z69183787的专栏 - CSDN博客
2017年10月19日 17:39:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4261
当我们需要遍历Redis所有key或者指定模式的key时，首先想到的是KEYS命令：
[plain][view
 plain](http://blog.csdn.net/zhang197093/article/details/74615717#)[copy](http://blog.csdn.net/zhang197093/article/details/74615717#)
- KEYS pattern  
``
# ``````
官网对于KEYS命令有一个提示： KEYS
 的速度非常快，例如，Redis在一个有1百万个key的数据库里面执行一次查询需要的时间是40毫秒 。但在一个大的数据库中使用它仍然可能造成性能问题，如果你需要从一个数据集中查找特定的 `KEYS`，
 你最好还是用 Redis 的集合结构 [SETS](http://www.redis.cn/commands/sets.html) 来代替。
KEYS命令使用很简单。
```
redis> MSET one 1 two 2 three 3 four 4
OK
redis> KEYS *o*
1) "four"
2) "one"
3) "two"
redis> KEYS t??
1) "two"
redis> KEYS *
1) "four"
2) "three"
3) "one"
4) "two"
redis>
```
但由于KEYS命令一次性返回所有匹配的key，所以，当redis中的key非常多时，对于内存的消耗和redis服务器都是一个隐患，
对于Redis 2.8以上版本给我们提供了一个更好的遍历key的命令 SCAN 该命令的基本格式：
**起始版本：2.8.0**
**时间复杂度：**O(1) for every call. O(N) for a complete iteration, including enough command calls for the cursor to return back to 0. N is the number of elements inside the collection.
[SCAN](http://www.redis.cn/commands/scan.html) 命令及其相关的 [SSCAN](http://www.redis.cn/commands/sscan.html), [HSCAN](http://www.redis.cn/commands/hscan.html) 和 [ZSCAN](http://www.redis.cn/commands/zscan.html) 命令都用于增量迭代一个集合元素。
- [SCAN](http://www.redis.cn/commands/scan.html) 命令用于迭代当前数据库中的key集合。
- [SSCAN](http://www.redis.cn/commands/sscan.html) 命令用于迭代SET集合中的元素。
- [HSCAN](http://www.redis.cn/commands/hscan.html) 命令用于迭代Hash类型中的键值对。
- [ZSCAN](http://www.redis.cn/commands/zscan.html) 命令用于迭代SortSet集合中的元素和元素对应的分值
scan ：迭代库中的key 例如  scan 0 match h* （其中match为匹配的参数）
sscan：迭代集合set中的元素 sscan jihe 0 或者 sscan
 jihe 0 match f*
hscan：迭代哈希列表中的key  hscan hash 0 或者 hscan hash 0 match a*(match匹配的是键)
zscan：迭代有序集合中的元素 zscan zhash 0
以上列出的四个命令都支持增量式迭代，它们每次执行都只会返回少量元素，所以这些命令可以用于生产环境，而不会出现像 [KEYS](http://www.redis.cn/commands/keys.html) 或者 [SMEMBERS](http://www.redis.cn/commands/smembers.html) 命令带来的可能会阻塞服务器的问题。
不过，[SMEMBERS](http://www.redis.cn/commands/smembers.html) 命令可以返回集合键当前包含的所有元素， 但是对于SCAN这类增量式迭代命令来说，有可能在增量迭代过程中，集合元素被修改，对返回值无法提供完全准确的保证。
因为 [SCAN](http://www.redis.cn/commands/scan.html), [SSCAN](http://www.redis.cn/commands/sscan.html), [HSCAN](http://www.redis.cn/commands/hscan.html) 和 [ZSCAN](http://www.redis.cn/commands/zscan.html) 四个命令的工作方式都非常相似，
 所以这个文档会一并介绍这四个命令，需要注意的是[SSCAN](http://www.redis.cn/commands/sscan.html), [HSCAN](http://www.redis.cn/commands/hscan.html) ,[ZSCAN](http://www.redis.cn/commands/zscan.html)命令的第一个参数总是一个key； [SCAN](http://www.redis.cn/commands/scan.html) 命令则不需要在第一个参数提供任何key，因为它迭代的是当前数据库中的所有key。
## SCAN命令的基本用法
[SCAN](http://www.redis.cn/commands/scan.html)命令是一个基于游标的迭代器。这意味着命令每次被调用都需要使用上一次这个调用返回的游标作为该次调用的游标参数，以此来延续之前的迭代过程
当[SCAN](http://www.redis.cn/commands/scan.html)命令的游标参数被设置为 0 时， 服务器将开始一次新的迭代， 而当服务器向用户返回值为 0 的游标时， 表示迭代已结束。
以下是一个 SCAN 命令的迭代过程示例 :
```
redis 127.0.0.1:6379> scan 0
1) "17"
2)  1) "key:12"
    2) "key:8"
    3) "key:4"
    4) "key:14"
    5) "key:16"
    6) "key:17"
    7) "key:15"
    8) "key:10"
    9) "key:3"
   10) "key:7"
   11) "key:1"
redis 127.0.0.1:6379> scan 17
1) "0"
2) 1) "key:5"
   2) "key:18"
   3) "key:0"
   4) "key:2"
   5) "key:19"
   6) "key:13"
   7) "key:6"
   8) "key:9"
   9) "key:11"
```
在上面这个例子中， 第一次迭代使用 0 作为游标， 表示开始一次新的迭代。第二次迭代使用的是第一次迭代时返回的游标 17 ，作为新的迭代参数 。
显而易见，**SCAN命令的返回值** 是一个包含两个元素的数组， 第一个数组元素是用于进行下一次迭代的新游标， 而第二个数组元素则是一个数组， 这个数组中包含了所有被迭代的元素。
在第二次调用 SCAN 命令时， 命令返回了游标 0 ， 这表示迭代已经结束， 整个数据集已经被完整遍历过了。
**full iteration** ：以 0 作为游标开始一次新的迭代， 一直调用 SCAN 命令， 直到命令返回游标 0 ， 我们称这个过程为一次完整遍历。
## Scan命令的保证
SCAN命令以及其他增量式迭代命令， 在进行完整遍历的情况下可以为用户带来以下保证 ：
- 从完整遍历开始直到完整遍历结束期间， 一直存在于数据集内的所有元素都会被完整遍历返回； 这意味着， 如果有一个元素， 它从遍历开始直到遍历结束期间都存在于被遍历的数据集当中， 那么 SCAN 命令总会在某次迭代中将这个元素返回给用户。
- 同样，如果一个元素在开始遍历之前被移出集合，并且在遍历开始直到遍历结束期间都没有再加入，那么在遍历返回的元素集中就不会出现该元素。
然而因为增量式命令仅仅使用游标来记录迭代状态， 所以这些命令带有以下缺点：
- 同一个元素可能会被返回多次。 处理重复元素的工作交由应用程序负责， 比如说， 可以考虑将迭代返回的元素仅仅用于可以安全地重复执行多次的操作上。
- 如果一个元素是在迭代过程中被添加到数据集的， 又或者是在迭代过程中从数据集中被删除的， 那么这个元素可能会被返回， 也可能不会。
## SCAN命令每次执行返回的元素数量
SCAN增量式迭代命令并不保证每次执行都返回某个给定数量的元素,甚至可能会返回零个元素， 但只要命令返回的游标不是 0 ， 应用程序就不应该将迭代视作结束。
不过命令返回的元素数量总是符合一定规则的， 对于一个大数据集来说， 增量式迭代命令每次最多可能会返回数十个元素；而对于一个足够小的数据集来说， 如果这个数据集的底层表示为编码数据结构（小的sets, hashes and sorted sets）， 那么增量迭代命令将在一次调用中返回数据集中的所有元素。
如果需要的话，用户可以通过增量式迭代命令提供的**COUNT**选项来指定每次迭代返回元素的最大值。
## COUNT选项
对于增量式迭代命令不保证每次迭代所返回的元素数量，我们可以使用**COUNT**选项， 对命令的行为进行一定程度上的调整。COUNT 选项的作用就是让用户告知迭代命令， 在每次迭代中应该从数据集里返回多少元素。使用COUNT 选项对于对增量式迭代命令相当于一种提示， 大多数情况下这种提示都比较有效的控制了返回值的数量。
- COUNT 参数的默认值为 10 。
- 数据集比较大时，如果没有使用MATCH 选项, 那么命令返回的元素数量通常和 COUNT 选项指定的一样， 或者比 COUNT 选项指定的数量稍多一些。
- 在迭代一个编码为整数集合（intset，一个只由整数值构成的小集合）、 或者编码为压缩列表（ziplist，由不同值构成的一个小哈希或者一个小有序集合）时， 增量式迭代命令通常会无视 COUNT 选项指定的值， 在第一次迭代就将数据集包含的所有元素都返回给用户。
注意: **并非每次迭代都要使用相同的 COUNT 值 **，用户可以在每次迭代中按自己的需要随意改变 COUNT 值， 只要记得将上次迭代返回的游标用到下次迭代里面就可以了。
## MATCH 选项
类似于KEYS 命令，增量式迭代命令通过给定 MATCH 参数的方式实现了通过提供一个 glob 风格的模式参数， 让命令只返回和给定模式相匹配的元素。
以下是一个使用 [MATCH](http://www.redis.cn/commands/match.html) 选项进行迭代的示例:
```
redis 127.0.0.1:6379> sadd myset 1 2 3 foo foobar feelsgood
(integer) 6
redis 127.0.0.1:6379> sscan myset 0 match f*
1) "0"
2) 1) "foo"
   2) "feelsgood"
   3) "foobar"
redis 127.0.0.1:6379>
```
[MATCH](http://www.redis.cn/commands/match.html)功能对元素的模式匹配工作是在命令从数据集中取出元素后和向客户端返回元素前的这段时间内进行的， 所以如果被迭代的数据集中只有少量元素和模式相匹配， 那么迭代命令或许会在多次执行中都不返回任何元素。
以下是这种情况的一个例子:
```
redis 127.0.0.1:6379> scan 0 MATCH *11*
1) "288"
2) 1) "key:911"
redis 127.0.0.1:6379> scan 288 MATCH *11*
1) "224"
2) (empty list or set)
redis 127.0.0.1:6379> scan 224 MATCH *11*
1) "80"
2) (empty list or set)
redis 127.0.0.1:6379> scan 80 MATCH *11*
1) "176"
2) (empty list or set)
redis 127.0.0.1:6379> scan 176 MATCH *11* COUNT 1000
1) "0"
2)  1) "key:611"
    2) "key:711"
    3) "key:118"
    4) "key:117"
    5) "key:311"
    6) "key:112"
    7) "key:111"
    8) "key:110"
    9) "key:113"
   10) "key:211"
   11) "key:411"
   12) "key:115"
   13) "key:116"
   14) "key:114"
   15) "key:119"
   16) "key:811"
   17) "key:511"
   18) "key:11"
redis 127.0.0.1:6379>
```
可以看出，以上的大部分迭代都不返回任何元素。在最后一次迭代， 我们通过将 COUNT 选项的参数设置为 1000 ， 强制命令为本次迭代扫描更多元素， 从而使得命令返回的元素也变多了。
## 并发执行多个迭代
在同一时间， 可以有任意多个客户端对同一数据集进行迭代， 客户端每次执行迭代都需要传入一个游标， 并在迭代执行之后获得一个新的游标， 而这个游标就包含了迭代的所有状态， 因此， 服务器无须为迭代记录任何状态。
## 中止迭代
因为迭代的所有状态都保存在游标里面， 而服务器无须为迭代保存任何状态， 所以客户端可以在中途停止一个迭代， 而无须对服务器进行任何通知。即使有任意数量的迭代在中途停止， 也不会产生任何问题。
## 使用错误的游标
使用SCAN 命令传入间断的（broken）、负数、超出范围或者其他非正常的游标来执行增量式迭代并不会造成服务器崩溃， 但可能会让命令产生未定义的行为。未定义行为指的是， 增量式命令对返回值所做的保证可能会不再为真。 只有两种游标是合法的:
- 在开始一个新的迭代时， 游标必须为 0 。
- 增量式迭代命令在执行之后返回的， 用于延续迭代过程的游标。
## 迭代能终止的前提
增量式迭代命令所使用的算法只保证在数据集的大小有界的情况下， 迭代才会停止， 换句话说， 如果被迭代数据集的大小不断地增长的话， 增量式迭代命令可能永远也无法完成一次完整迭代。
从直觉上可以看出， 当一个数据集不断地变大时， 想要访问这个数据集中的所有元素就需要做越来越多的工作， 能否结束一个迭代取决于用户执行迭代的速度是否比数据集增长的速度更快。
## 返回值
[SCAN](http://www.redis.cn/commands/scan.html), [SSCAN](http://www.redis.cn/commands/sscan.html), [HSCAN](http://www.redis.cn/commands/hscan.html) 和 [ZSCAN](http://www.redis.cn/commands/zscan.html) 命令都返回一个包含两个元素的
 multi-bulk 回复： 回复的第一个元素是字符串表示的无符号 64 位整数（游标）， 回复的第二个元素是另一个 multi-bulk 回复， 包含了本次被迭代的元素。
- [SCAN](http://www.redis.cn/commands/scan.html) 命令返回的每个元素都是一个key。
- [SSCAN](http://www.redis.cn/commands/sscan.html) 命令返回的每个元素都是一个集合成员。
- [HSCAN](http://www.redis.cn/commands/hscan.html) 命令返回的每个元素都是一个键值对，一个键值对由一个键和一个值组成。
- [ZSCAN](http://www.redis.cn/commands/zscan.html)命令返回的每个元素都是一个有序集合元素，一个有序集合元素由一个成员（member）和一个分值（score）组成。
## 另外的例子
迭代hash中的键值对：
```
redis 127.0.0.1:6379> hmset hash name Jack age 33
OK
redis 127.0.0.1:6379> hscan hash 0
1) "0"
2) 1) "name"
   2) "Jack"
   3) "age"
   4) "33"
```
基于SCAN的这种安全性，建议大家在生产环境都使用SCAN命令来代替KEYS，不过注意，该命令是在2.8.0版本之后加入的，如果你的Redis低于这个版本，则需要升级Redis。
下面用PHP代码演示SCAN命令的使用：
[php][view
 plain](http://blog.csdn.net/zhang197093/article/details/74615717#)[copy](http://blog.csdn.net/zhang197093/article/details/74615717#)
- <?php  
- 
- $redis = new Redis();  
- 
- $redis->connect('127.0.0.1', 6379);  
- 
- 
- /* 设置遍历的特性为不重复查找，该情况下扩展只会scan一次，所以可能会返回空集合 */
- $redis->setOption(Redis::OPT_SCAN, Redis::SCAN_NORETRY);  
- 
- $it = NULL;  
- $pattern = '*';  
- $count = 50;  // 每次遍历50条，注意是遍历50条，遍历出来的50条key还要去匹配你的模式，所以并不等于就能够取出50条key
- 
- do
- {  
- $keysArr = $redis->scan($it, $pattern, $count);  
- 
- if ($keysArr)  
-     {  
- foreach ($keysArras$key)  
-         {  
- echo$key . "\n";  
-         }  
-     }  
- 
- } while ($it > 0);   //每次调用 Scan会自动改变 $it 值，当$it = 0时 这次遍历结束 退出循环
- 
- 
- echo'---------------------------------------------------------------------------------' . "\n";  
- 
- 
- /* 设置扩展在一次scan没有查找出记录时 进行重复的scan 直到查询出结果或者遍历结束为止 */
- $redis->setOption(Redis::OPT_SCAN, Redis::SCAN_RETRY);  
- 
- $it = NULL;  
- $pattern = '*';  
- $count = 50;  // 每次遍历50条，注意是遍历50条，遍历出来的50条key还要去匹配你的模式，所以并不等于就能够取出50条key
- 
- //这种用法下我们只需要简单判断返回结果是否为空即可, 如果为空说明遍历结束
- while ($keysArr = $redis->scan($it, $pattern, $count))  
- {  
- foreach ($keysArras$key)  
-     {  
- echo$key . "\n";  
-     }  
- }  
执行结果：
[plain][view
 plain](http://blog.csdn.net/zhang197093/article/details/74615717#)[copy](http://blog.csdn.net/zhang197093/article/details/74615717#)
- [root@localhost php]# /usr/local/php/bin/php scan.php   
- bm  
- bm2  
- h1  
- name  
- bit  
- bm1  
- places  
- cities  
- hhl  
- ---------------------------------------------------------------------------------  
- bm  
- bm2  
- h1  
- name  
- bit  
- bm1  
- places  
- cities  
- hhl  
注意：如果php执行报错 请升级到较新版本的Redis扩展
更多请参考：
[http://www.redis.cn/commands/keys.html](http://www.redis.cn/commands/keys.html)
[http://www.redis.cn/commands/scan.html](http://www.redis.cn/commands/scan.html)
[https://github.com/phpredis/phpredis#scan](https://github.com/phpredis/phpredis#scan)
