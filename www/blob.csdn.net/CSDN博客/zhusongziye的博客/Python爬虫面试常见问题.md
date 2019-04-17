# Python爬虫面试常见问题 - zhusongziye的博客 - CSDN博客





2018年03月12日 20:51:03[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：658









是否了解线程的同步和异步？1线程同步：
多个线程同时访问同一资源，等待资源访问结束，浪费时间，效率低  
2线程异步：
在访问资源时在空闲等待时同时访问其他资源，实现多线程机制
是否了解网络的同步和异步？1同步：
提交请求->等待服务器处理->处理完毕返回 这个期间客户端浏览器不能干任何事
2异步：
请求通过事件触发->服务器处理（这是浏览器仍然可以作其他事情）->处理完毕 
链表和顺序表储存时各自有什么优点？1顺序表存储：
原理：顺序表存储是将数据元素放到一块连续的内存存储空间，存取效率高，速度快。但是不可以动态增加长度

优点：存取速度高效，通过下标来直接存储

缺点：1.插入和删除比较慢，2.不可以增长长度    

                比如：插入或者删除一个元素时，整个表需要遍历移动元素来重新排一次顺序



2链表存储：
原理：链表存储是在程序运行过程中动态的分配空间，只要存储器还有空间，就不会发生存储溢出问题


优点：插入和删除速度快，保留原有的物理顺序，比如：插入或者删除一个元素时，只需要改变指针指向即可


缺点：查找速度慢，因为查找时，需要循环链表访问




使用redis搭建分布式系统时如何处理网络延迟和网络异常？





由于网络异常的存在，分布式系统中请求结果存在“三态”的概念，即三种状态：“成功”、“失败”、“超时（未知）”




当出现“超时”时可以通过发起读取数据的操作以验证 RPC 是否成功（例如银行系统的做法）




另一种简单的做法是，设计分布式协议时将执行步骤设计为可重试的，即具有所谓的“幂等性”



数据仓库是什么？


数据仓库是一个面向主题的、集成的、稳定的、反映历史变化的、随着时间的流逝发生变化的数据集合。它主要支持管理人员的决策分析。

数据仓库收集了企业相关内部和外部各个业务系统数据源、归档文件等一系列历史数据，最后转化成企业需要的战略决策信息。

特点：

面向主题：根据业务的不同而进行的内容划分；

集成特性：因为不同的业务源数据具有不同的数据特点，当业务源数据进入到数据仓库时，需要采用统一的编码格式进行数据加载，从而保证数据仓库中数据的唯一性；

非易失性：数据仓库通过保存数据不同历史的各种状态，并不对数据进行任何



你是否了解谷歌的无头浏览器？
无头浏览器即headless browser，是一种没有界面的浏览器。既然是浏览器那么浏览器该有的东西它都应该有，只是看不到界面而已。





Python中selenium模块中的PhantomJS即为无界面浏览器（无头浏览器）:是基于QtWebkit的无头浏览器



你是否了解MySQL数据库的几种引擎？


InnoDB：

InnoDB是一个健壮的事务型存储引擎，这种存储引擎已经被很多互联网公司使用，为用户操作非常大的数据存储提供了一个强大的解决方案。





在以下场合下，使用InnoDB是最理想的选择：


1.更新密集的表。InnoDB存储引擎特别适合处理多重并发的更新请求。


2.事务。InnoDB存储引擎是支持事务的标准MySQL存储引擎。

3.自动灾难恢复。与其它存储引擎不同，InnoDB表能够自动从灾难中恢复。

4.外键约束。MySQL支持外键的存储引擎只有InnoDB。

5.支持自动增加列AUTO_INCREMENT属性。

一般来说，如果需要事务支持，并且有较高的并发读取频率，InnoDB是不错的选择。


MEMORY：


　　使用MySQL Memory存储引擎的出发点是速度。为得到最快的响应时间，采用的逻辑存储介质是系统内存。

　　虽然在内存中存储表数据确实会提供很高的性能，但当mysqld守护进程崩溃时，所有的Memory数据都会丢失。

　　获得速度的同时也带来了一些缺陷。　　

一般在以下几种情况下使用Memory存储引擎：


1.目标数据较小，而且被非常频繁地访问。在内存中存放数据，所以会造成内存的使用，可以通过参数max_heap_table_size控制Memory表的大小，设置此参数，就可以限制Memory表的最大大小。


2.如果数据是临时的，而且要求必须立即可用，那么就可以存放在内存表中。


3.存储在Memory表中的数据如果突然丢失，不会对应用服务产生实质的负面影响。



redis数据库有哪几种数据结构？
5种数据结构

string



使用string时，redis**大多数情况下**并不会理解或者解析其含义，无论使用json、xml还是纯文本在redis看来都是一样的，只是一个字符串，只能进行strlen、append等对字符串通用的操作，无法针对其内容进一步操作。其基本操作命令有set、get、strlen、getrange、append：

> 
1 SET key value

2 GET key

3 STRLEN key

4 GETRANGE  key start end

5 APPEND key value


在大多数情况之外，就是string中存储的为纯数字的情况，redis可以将字符串当做数字进行进一步操作，这些操作包括decr、decrby、incr、incrby和incrbyfloat。





hash

　　使用hash时，在我看来，就是value本身就是一组key-value对，不过redis将这里的key称为field（但是hkeys命令为什么不叫hfields命令呢哈哈），也就是value是一组field-value对。其基本操作命令有hset、hget、hmset、hmget、hgetall、hkeys和hdel：

> 
1 HSET key field value

2 HGET key field

3 HMSET key field value [field value ...]

4 HMGET key field [field ...]

5 HGETALL key

6 HKEYS key

7 HDEL key field [field ...]





list

使用list时，value就是一个string数组，操作这组string时，可以像对待栈一样使用pop和push操作，但是这个栈两端都能进行操作；也可以像对待数组一样使用一个index参数来操作。list的操作命令略杂，主要分为两类：L开头的和R开头的，L代表LEFT或者LIST，进行一些从列表左端进行的操作，或者一些与端无关的操作；R代表RIGHT，进行一些从列表右端进行的操作。  

set

　　set用于存储一组不重复的值，也可以进行一些集合的操作，就像数学上的集合，它是无序的。基本操作有sadd和sismember：

> 
1 SADD key member [member ...]

2 SISMEMBER key member




集合操作有：求交sinter、求并sunion和求差sdiff：


> 
1 SINTER key [key ...]


2 SUNION key [key ...]

3 SDIFF key [key ...]





sorted set

　　sorted set类似set，但是sorted set里每个元素都有一个score，这个score可用于排序和排名。基本操作有zadd、zcount、zrank：  

> 
1 ZADD key score member [score member ...]

2 ZCOUNT key min max

3 ZRANK key member






作者：默行

源自：https://www.cnblogs.com/zdong0103/p/8492779.html

声明：文章著作权归作者所有，如有侵权，请联系小编删除


是否了解线程的同步和异步？1线程同步：
多个线程同时访问同一资源，等待资源访问结束，浪费时间，效率低  
2线程异步：
在访问资源时在空闲等待时同时访问其他资源，实现多线程机制
是否了解网络的同步和异步？1同步：
提交请求->等待服务器处理->处理完毕返回 这个期间客户端浏览器不能干任何事
2异步：
请求通过事件触发->服务器处理（这是浏览器仍然可以作其他事情）->处理完毕 
链表和顺序表储存时各自有什么优点？1顺序表存储：
原理：顺序表存储是将数据元素放到一块连续的内存存储空间，存取效率高，速度快。但是不可以动态增加长度

优点：存取速度高效，通过下标来直接存储

缺点：1.插入和删除比较慢，2.不可以增长长度    

                比如：插入或者删除一个元素时，整个表需要遍历移动元素来重新排一次顺序



2链表存储：
原理：链表存储是在程序运行过程中动态的分配空间，只要存储器还有空间，就不会发生存储溢出问题


优点：插入和删除速度快，保留原有的物理顺序，比如：插入或者删除一个元素时，只需要改变指针指向即可


缺点：查找速度慢，因为查找时，需要循环链表访问




使用redis搭建分布式系统时如何处理网络延迟和网络异常？





由于网络异常的存在，分布式系统中请求结果存在“三态”的概念，即三种状态：“成功”、“失败”、“超时（未知）”




当出现“超时”时可以通过发起读取数据的操作以验证 RPC 是否成功（例如银行系统的做法）




另一种简单的做法是，设计分布式协议时将执行步骤设计为可重试的，即具有所谓的“幂等性”



数据仓库是什么？


数据仓库是一个面向主题的、集成的、稳定的、反映历史变化的、随着时间的流逝发生变化的数据集合。它主要支持管理人员的决策分析。

数据仓库收集了企业相关内部和外部各个业务系统数据源、归档文件等一系列历史数据，最后转化成企业需要的战略决策信息。

**特点：**

面向主题：根据业务的不同而进行的内容划分；

集成特性：因为不同的业务源数据具有不同的数据特点，当业务源数据进入到数据仓库时，需要采用统一的编码格式进行数据加载，从而保证数据仓库中数据的唯一性；

非易失性：数据仓库通过保存数据不同历史的各种状态，并不对数据进行任何



你是否了解谷歌的无头浏览器？
无头浏览器即headless browser，是一种没有界面的浏览器。既然是浏览器那么浏览器该有的东西它都应该有，只是看不到界面而已。





Python中selenium模块中的PhantomJS即为无界面浏览器（无头浏览器）:是基于QtWebkit的无头浏览器



你是否了解MySQL数据库的几种引擎？


InnoDB：

InnoDB是一个健壮的事务型存储引擎，这种存储引擎已经被很多互联网公司使用，为用户操作非常大的数据存储提供了一个强大的解决方案。





在以下场合下，使用InnoDB是最理想的选择：


1.更新密集的表。InnoDB存储引擎特别适合处理多重并发的更新请求。


2.事务。InnoDB存储引擎是支持事务的标准MySQL存储引擎。

3.自动灾难恢复。与其它存储引擎不同，InnoDB表能够自动从灾难中恢复。

4.外键约束。MySQL支持外键的存储引擎只有InnoDB。

5.支持自动增加列AUTO_INCREMENT属性。

一般来说，如果需要事务支持，并且有较高的并发读取频率，InnoDB是不错的选择。


MEMORY：


　　使用MySQL Memory存储引擎的出发点是速度。为得到最快的响应时间，采用的逻辑存储介质是系统内存。

　　虽然在内存中存储表数据确实会提供很高的性能，但当mysqld守护进程崩溃时，所有的Memory数据都会丢失。

　　获得速度的同时也带来了一些缺陷。　　

一般在以下几种情况下使用Memory存储引擎：


1.目标数据较小，而且被非常频繁地访问。在内存中存放数据，所以会造成内存的使用，可以通过参数max_heap_table_size控制Memory表的大小，设置此参数，就可以限制Memory表的最大大小。


2.如果数据是临时的，而且要求必须立即可用，那么就可以存放在内存表中。


3.存储在Memory表中的数据如果突然丢失，不会对应用服务产生实质的负面影响。



redis数据库有哪几种数据结构？
5种数据结构

string



使用string时，redis**大多数情况下**并不会理解或者解析其含义，无论使用json、xml还是纯文本在redis看来都是一样的，只是一个字符串，只能进行strlen、append等对字符串通用的操作，无法针对其内容进一步操作。其基本操作命令有set、get、strlen、getrange、append：

> 
1 SET key value

2 GET key

3 STRLEN key

4 GETRANGE  key start end

5 APPEND key value


在大多数情况之外，就是string中存储的为纯数字的情况，redis可以将字符串当做数字进行进一步操作，这些操作包括decr、decrby、incr、incrby和incrbyfloat。





hash

　　使用hash时，在我看来，就是value本身就是一组key-value对，不过redis将这里的key称为field（但是hkeys命令为什么不叫hfields命令呢哈哈），也就是value是一组field-value对。其基本操作命令有hset、hget、hmset、hmget、hgetall、hkeys和hdel：

> 
1 HSET key field value

2 HGET key field

3 HMSET key field value [field value ...]

4 HMGET key field [field ...]

5 HGETALL key

6 HKEYS key

7 HDEL key field [field ...]





list

使用list时，value就是一个string数组，操作这组string时，可以像对待栈一样使用pop和push操作，但是这个栈两端都能进行操作；也可以像对待数组一样使用一个index参数来操作。list的操作命令略杂，主要分为两类：L开头的和R开头的，L代表LEFT或者LIST，进行一些从列表左端进行的操作，或者一些与端无关的操作；R代表RIGHT，进行一些从列表右端进行的操作。  

set

　　set用于存储一组不重复的值，也可以进行一些集合的操作，就像数学上的集合，它是无序的。基本操作有sadd和sismember：

> 
1 SADD key member [member ...]

2 SISMEMBER key member




**集合操作有：求交sinter、求并sunion和求差sdiff：**


> 
1 SINTER key [key ...]


2 SUNION key [key ...]

3 SDIFF key [key ...]





sorted set

　　sorted set类似set，但是sorted set里每个元素都有一个score，这个score可用于排序和排名。基本操作有zadd、zcount、zrank：  

> 
1 ZADD key score member [score member ...]

2 ZCOUNT key min max

3 ZRANK key member




![](https://mmbiz.qpic.cn/mmbiz_jpg/Kad3LZzM7n50rDmehPbMxqLD0Sib1ag9Aibx4WSsoXOickgW2RAlP6ORrCAh4jhsl8PkjTOjianQv9yrdSc28gN9tA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
作者：默行

源自：https://www.cnblogs.com/zdong0103/p/8492779.html

声明：文章著作权归作者所有，如有侵权，请联系小编删除
![](https://mmbiz.qpic.cn/mmbiz_jpg/Kad3LZzM7n50rDmehPbMxqLD0Sib1ag9Aibx4WSsoXOickgW2RAlP6ORrCAh4jhsl8PkjTOjianQv9yrdSc28gN9tA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)



