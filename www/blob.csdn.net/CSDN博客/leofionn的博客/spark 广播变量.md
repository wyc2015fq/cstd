
# spark 广播变量 - leofionn的博客 - CSDN博客


2018年03月27日 20:33:51[leofionn](https://me.csdn.net/qq_36142114)阅读数：162


Spark执行一个阶段（stage）时，会为待执行函数建立闭包，也就是阶段所有任务所需信息的二进制形式。这个闭包包括驱动程序里函数引用的所有数据结构。Spark把这个闭包发送到集群的每个executor上。
当许多任务需要访问同一个（不可变的）数据结构时，我们应该使用广播变量。它对任务闭包的常规处理进行扩展，使我们能够：
1. 在每个executor上将数据缓存为原始的Java对象，这样就不用为每个任务执行反序列化；
2. 在多个作业和阶段之间缓存数据。

对每个executor只要执行一次传输数据：
val a = ...
val bb = sc.broadcast(a)
...
def query(path: String) = {
sc.textFile(path).map(l => score(l, bb.value))
...
}


【来自@若泽大数据】


