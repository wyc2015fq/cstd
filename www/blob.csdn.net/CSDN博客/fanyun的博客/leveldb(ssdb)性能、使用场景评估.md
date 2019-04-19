# leveldb(ssdb)性能、使用场景评估 - fanyun的博客 - CSDN博客
2018年06月23日 22:47:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：354
       最近有个业务场景存储压力很大，写远远大于读，读也集中在最近写入，想想这不很适合采用leveldb存储么。leveldb的话好像用ssdb比较多，花了两天时间就ssdb简单做下测试，以下总结。
[ssdb](https://github.com/ideawu/ssdb) 是对leveldb存储引擎的redis兼容协议封装，并且实现了主从同步，源码不多易读。对于支持的操作，除了get/set KV存储，由于 leveldb 是有序的，还可实现很多操作；通过scan遍历的命令，利用有序性 list、hset也通过key+fileld/seq - val 方式存储，ttl 会单独存储一个hset 保存过期时间，由一个单独线程定时轮询删除。
## 测试场景
机器：两台R720XD E5-2620 2.1G (6核12线程)*2/内存128GB/300GB机械硬盘
数据： key: key 10位顺序数字 value: 50字节
并发： 默认情况100连接并发
客户端:  使用erlang erdis模块get/set
配置：
leveldb:
        cache_size: 500
        block_size: 1
        write_buffer_size: 64
        compression: no
        max_open_files: 1000
## 数据量：2kw
- 文件大小：1.3GB
- 写速度：7w/s  CPU 250%  mem:30M
- 随机读： 5.5w/s  CPU  100%  mem:1GB
- 并发读写1:10: 读 5k/s 写:5w/s，CPU 250%
总结：总体性能和github给出的相近；leveldb数据存放十分紧凑，因为会对key开启前缀压缩，如果开启snappy后会更小，即使全量缓存到内存，内存消耗也会比redis 少很多。
## 数据量：1.5亿
- 文件大小：9.6GB
- 写速度：7w/s  cpu: 250%  mem:70M
- 随机读:  1.6w/s cpu:100% mem:70M
-并发读写：1: 10: 4k/s 读  5w/s 写 CPU: 250%
总结：读太随机的LRU cache 无法有效缓存任何数据，请求都要经过文件系统读取，性能下降；但写保性能持不变
## 数据量：10亿
- 文件大小：66GB
- 写速度：7w/s  cpu:250% mem:80M
- 随机读:  1.6w/s cpu:180% mem:80M
-并发读写：1: 10: 4k/s 读:5w/s 写:280%
总结：和1.5亿级别效果保持一致
## page cache
     leveldb默认缓存meta data、和8M的block；本次测试使用了500M block_cache，从测试效果看，因为随机读，cache只在2k数据级别上起到作用，且带来很高性能提升。
     1.5亿、10亿时，完全依赖kernel 对文件系统的page cache，机器有128GB内存，leveldb 没有使用direct io，文件都缓存，实际运行中不会有磁盘IO。
     那么使用脚本清理：     
     while true;  do echo 1 > /proc/sys/vm/drop_caches;echo clean cache ok; sleep 1; done
- 随机读 约160/s  cpu: 5%  mem: 120m  iostat 95%util
- 1并发随机读，100并发写：90/s 读， 1500/s 写，随机读取影响写入速度
总结：
     随机IO在机械硬盘上是完全无解了，只能靠cache扛，相比page cache，block_cache 更有效，应该更加需求增加bock_cache。
     相比增加内存，使用ssd硬盘成本更低。
读取系统调用：
open("./var/data/006090.ldb", O_RDONLY) = 27
stat("./var/data/006090.ldb", {st_mode=S_IFREG|0644, st_size=34349641, ...}) = 0
mmap(NULL, 34349641, PROT_READ, MAP_SHARED, 27, 0) = 0x7f0334f76000
madvise(0x7f040457e000, 737280, MADV_DONTNEED) = 0
munmap(0x7f03dc2ab000, 34349599)        = 0
close(27)                               = 0
## 多线程
ssdb 是多线程的，但上面测试效果看有明显多核利用率很低问题，从源码看可以知道：
- 1个主线程，负责网络io
- 10个读线程，负责像scan复杂操作读
- 1个写线程，负责写操作磁盘io
- 1个leveldb 的compact线程
也就是:一个主线程负责网络，一个写线程负责leveldb操作；而读 get 只主线程在工作。
ssdb相关都没有配置，简单修改源码重新编译：
- 10个线程处理读：2.5/s  CPU 450%
     60%消耗在sys，高并发读文件对内核瓶颈
- 减小至3个线程处理读：3.2w/s 280%（相比10线程，更少的CPU消耗，更高的性能）
- 使用LRUCahce 在1kw区间随机读  7w/s ，200%CPU
## 可靠性
     leveldb 更新前先写日志方式，但默认方式日志mmap是不会做msync，也就是完全依赖操作系统刷磁盘，这样存在机器掉电等意外故障时可能会丢失部分最新消息。支持leveldb:WriteOptions.sync可选参数，但ssdb默认false，改为需要修改代码。
     修改true后，奇怪性能无变化？代码上看并不是每个写都会msync，而是4kbuffer后刷一次。
     那么leveldb 在故障时可能丢时少量数量就是没办法的了，如需要强可靠需要注意。
     测测msync速度怎么样呢？
     简单单线程c程序，每写100字节做一次msync，效果：
     服务器2w/s, 我的mac pro ssd 3w/s (此时ssd也没太大优势)
     msync 每次都做的话，肯定是有较大的性能影响的，但是可以做group msync；group  msync 会增加延时，就看可接受都少了，如0.1ms，那就就可以以1w/s 脉冲式、批量sync磁盘，保证所有请求都写入磁盘再返回。
## batch
     leveldb 的写入、修改、删除都是支持优化的batch操作，使用multi_set命令。
## 最佳实战
1. 写性能
     测试中，写入速度一直维持7w/s, 可满足到多需求， leveldb 写入可以到40w/s, 这里受限于ssdb 线程模型无法利用更多的核心。需要的话通过pipline、网卡中断平衡、提高网络、leveldb写线程数 来提高写入性能。
2. 读性能
     一般业务都存在数据热点，可调整cache_size, block_size 提高缓存命中率，block_size 为缓存块大小1K~4M视具体业务value而定。
     如果业务热点度不高，那只能上ssd硬盘了。
     注意使用了page cache，不小心清空会让性能急剧下降，还是尽量配置足够大的cache_size。还有就是启动有个预热过程。
3. compaction
     本次使用的顺序key写入，因为业务上key 都是顺序的，然后一段时间从后往前顺序删除。compaction影响会很小，如果业务大量随机key写入、修改、删除会增加compaction量，需要注意另外做压力测试。
