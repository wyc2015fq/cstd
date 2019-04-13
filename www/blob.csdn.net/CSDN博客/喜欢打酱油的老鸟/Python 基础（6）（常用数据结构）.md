
# Python 基础（6）（常用数据结构） - 喜欢打酱油的老鸟 - CSDN博客


2019年01月02日 09:20:18[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：125


[https://www.toutiao.com/a6634514748694069763/](https://www.toutiao.com/a6634514748694069763/)
2018-12-14 08:10:00
常用数据结构
**1）元组**
元组是一种静态的数据结构，无法修改，若要修改只能重新生成新的元组。
![Python 基础（6）（常用数据结构）](http://p1.pstatp.com/large/pgc-image/2bcd69b500b542fbbc77630965c8a418)
输出结果：
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/36739dbe38e744d58d5fbb5df4de7dc3)
元组元素的获取是通过索引值去获得的；例如上面的tup1[0]返回apple；另外你可以直接把tup1一次性赋给多个值，例如上面的tup1一次性赋值给a,b,c,d; tup1[1:3]是对元组的截取，跟字符串的切片是一样，返回('banana', 'grape')
当然也是可以层次嵌套的；索引方法一样; y元组还支持直接通过+进行合并元组操作，其实是生成一个新的元组。
元组的遍历也是很简单的：
![Python 基础（6）（常用数据结构）](http://p1.pstatp.com/large/pgc-image/cefdfd912c3243e6ae87320a3cb5eac0)
输出结果:
![Python 基础（6）（常用数据结构）](http://p9.pstatp.com/large/pgc-image/156a6e29152b474d8b062544d5ec3498)
直接for遍历出来的是一个个元素，但最好还是建议先查询元素个数，然后根据索引值去获取value值，尤其对多层嵌套的元组来说，这种方式更好。
**2）列表**
列表是我们常用的，所以这一块一定要好好掌握：
![Python 基础（6）（常用数据结构）](http://p9.pstatp.com/large/pgc-image/86ed03040ae840699539b122410478c8)
输出结果：
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/1b46f87bf2aa4385a32fab82650a3e74)
列表的查询，遍历，和切片截取功能跟元组一样；添加元素可以一般是直接append("XXX")就往列表后面添加值，如果要指定位置添加元素，可以像lists.insert(4,"ouyang5")，在指定的索引值位置插入值；
移除可以用remove指定的元素，如果想要弹出最后一个元素，可以用pop方法；
列表的其他常用方法：
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/fb02ebb19abd418bbab89e0284347260)
输出结果：
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/dce7eca77cba404d95460fecc209da25)
join是把列表转为字符串，用逗号隔开；sort是根据字典排序，即字母的数字编码大小排序；reverse是反转排序；最后一个clear就是清空列表；
**3）字典**
字典很多方法也是跟list是一样的：
![Python 基础（6）（常用数据结构）](http://p3.pstatp.com/large/pgc-image/787bb60bd08e44bb8ffbf475a7fe6835)
输出结果：
![Python 基础（6）（常用数据结构）](http://p3.pstatp.com/large/pgc-image/964fcf787e0043cb9c5f6f4d725a2744)
字典是一个键值对集合，可以支持修改；排序方法，这里使用lambda函数，这里就先不讲了，后面会涉及这块知识，需要知道有这一块功能就行。
字典其他一些用法：
![Python 基础（6）（常用数据结构）](http://p3.pstatp.com/large/pgc-image/c7c34e0f327a4fe1bbdfd71e00e4d1d2)
输出结果：
![Python 基础（6）（常用数据结构）](http://p9.pstatp.com/large/pgc-image/ec8c2afcd07f49d9b812a416197294d8)
字典的浅复制copy和deepcopy在3.6版本的功能是一样的了，以前的版本，浅复制修改原字典，copy出来的字典也会跟着变化，但现在都不会了；因为原先copy出来的只是做了一个指引而已，内存地址一样。deepcopy就是新开辟一个内存空间。
字典的update很好用，他会合并两个字典，去掉重复的元素。
**4）set集合**
set集合里的元素是不能重复的，list里面的元素是可以重复的。
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/84a9d53e098c4611b7593028712b7b1e)
输出结果：
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/c41bdae14afd4e1590eae6513765bb96)
set的定义是在list列表的基础上的，去掉重复的，当然它的索引取值等方式是跟列表是一样的，这里就不在说，通样，它也有一个更新的功能，跟字典很类似。
总结：
这些集合是很常用，对常用的方法一定要熟悉，用的时候可以顺手拈来。
![Python 基础（6）（常用数据结构）](http://p99.pstatp.com/large/pgc-image/0a6eec1205d847c9be766ebbda1decdd)


