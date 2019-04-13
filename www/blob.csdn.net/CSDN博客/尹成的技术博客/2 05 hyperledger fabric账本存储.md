
# 2.05 hyperledger fabric账本存储 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:39:00[尹成](https://me.csdn.net/yincheng01)阅读数：394个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**1.账本存储概述**
peer节点账本存储图如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170149334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
左边区块链是狭义上的区块存储，底层是一个文件系统，区块并不是存储在数据库，而是直接存储为文件
右下角的区块索引用于查询区块，将区块属性与区块位置关联，例如根据区块哈希、高度、交易ID查询区块
区块索引的实现使用了levelDB，是一个内嵌的数据库
fabric中不是一个区块单独存一个文件，所以需要区块索引去查找
右上角状态数据库是区块链上的最新数据
2.交易读写集
交易读写集（RWSet）有三个概念读集
写集
版本号
交易验证阶段需要对交易读写集进行验证
交易模拟&验证过程例子，现在假设有T1,T2,T3,T4和T5共5笔交易，所有模拟基于同一个世界状态世界状态：（k1,1,v1）（k2,1,v2）（k3,1,v3）（k4,1,v4）——key，version和value
T1通过验证，因为它并没有任何读操作，并且世界状态中k1和k2键的值分别更新为(k1,2,v1’)和(k2,2,v2’)
T2不能通过验证，因为它读取了k1键的值，该值在T1中被修改了
T3通过验证，因为它并没有任何读操作。并且世界状态中k2的值被更新为(k2,3,v2”)
T4不能通过验证，因为它读取了k2键的值，该值在T1中被修改了
T5通过验证，虽然读取了k5的值，但是k5的值之前交易并未修改过

3.账本存储其他概念
世界状态交易执行后所有键的最新值
可以显著提升链码执行效率
历史数据索引启用与否取决于智能合约是否有查询历史的需求
记录某个键在某区块的某条交易中被改变，只记录改变动作，不记录具体改变为了什么
区块存储区块在fabric中以文件块存储，文件名是blocfile_xxxxxx（加一个6位数的编号）
现阶段，每个文件块大小是64M，需要更改的话，需要修改peer源码重新编译
区块读取区块文件流
区块流
区块迭代器
区块索引快速定位区块
索引键：区块高度、区块哈希、交易哈希
区块提交保存区块文件
更新数据状态
4.账本存储源码
看core/ledger下的ledger_interface.go
读写集分为交易读写集生成和交易读写集验证两个部分
状态数据库core/ledger/kvledger/txmgmt/statedb/stateleveldb/stateleveldb.go
历史数据库core/ledger/kvledger/history/historydb/historyleveldb/historyleveldb.go
区块文件读取common/ledger/blkstorage/fsblkstorage/ fs_blockstore.go
5.账本存储总结
账本存储的接口定义，从总体上把握
交易读写集校验，防止双花攻击，比特币是用最长链来防止的
状态数据库及历史状态数据库
区块文件存储及区块索引
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

