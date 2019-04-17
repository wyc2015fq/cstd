# Python中list,tuple,dict,set的区别和用法 - miner_zhu的博客 - CSDN博客





2018年07月17日 09:48:59[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：379








Python提供多种数据类型来存放数据项集合，主要包括序列（列表list和元组tuple），映射（如字典dict），集合（set），下面对这几种数据类型分别介绍。

**Python中list，tuple，dict和set的主要区别：**

**一句话来概括四种数据类型的区别是：tuple是一个不可改变的list，set是一个没有Value的dict，list和set的数据是可变的，tuple和dict的数据是不可变的！**

list和set的数据可变的，tuple和dict的数据是不可变的。list是最自由的，可以使用索引、切片，可以进行计算和修改；tuple是不自由的，数据不能更改，但是和list一样具有序列，可以用索引和切片；dict是半自由的，自由体现在键值对的无序，可以通过key去索引value的值，不自由体现在数据操作只能通过key来实现，键值对是不可变类型；set是半自由的，自由体现在它通过list传值给它，数据类型是可变的，还可以改变list中的值，数据集是无序的，不重复的，可以进行计算，不自由体现在无法使用索引和切片准确定位。



**1 .list**

list是一个使用方括号括起来的有序元素集合;

List 可以作为以 0 下标开始的数组,任何一个非空 list 的第一个元素总是 L[0],负数索引从 list 的尾部开始向前计数来存取元素。任何一个非空的 list 最后一个元素总是 L[-1];

有分片功能，两个list可以相加；

append 向 list 的末尾追加单个元素；

insert 将单个元素插入到 list 中； 

extend 用来连接 list，使用一个 list 参数进行调用；

append 接受一个参数, 这个参数可以是任何数据类型, 并且简单地追加到 list 的尾部；

index 在 list 中查找一个值的首次出现并返回索引值；

要测试一个值是否在 list 内, 使用 in, 如果值存在, 它返回 True, 否则返为 False ；

remove 从 list 中删除一个值的首次出现；

pop 可以删除 list 的最后一个元素, 然后返回删除元素的值，用索引删除制定位置的值；



**2.tuple**

tuple是不可变的list，创建了一个tuple就不能以任何方式改变它；

定义tuple是将整个元素集是用小括号括起来，是有序集合；

tuple的索引与list一样从0开始,所以一个非空的tuple的第一个元素总是t[0]；

负数索引与 list 一样从 tuple 的尾部开始计数；

与 list 一样分片 (slice) 也可以使用。分割一个 tuple 时, 会得到一个新的 tuple；

没有 append、extend、remove或pop方法以及index方法；

可以使用in来查看一个元素是否存在于tuple 中。



**3.dict**

dict定义了键和值之间的一一对应关系，每个元素都是一个key-value对；

整个元素集合用大括号括起来，有序集合；

可以通过 key 得到value, 但不能通过vaule获取 key；

在一个 dict中不能有重复的 key, 并且 key 是大小写敏感的；

键可以是数字、字符串或者是元组等不可变类型；

用del使用key可以删除dict中的独立元素； 

用clear可以清除dict中的所有元素。



**4.set**

set是建立一系列无序的，不重复的元素；

创建set的方式是调用set()并传入一个list，list的元素将作为set的元素；

set和dict的唯一区别仅在于没有存储对应的value。



