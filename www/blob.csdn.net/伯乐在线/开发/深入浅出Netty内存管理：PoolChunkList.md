# 深入浅出Netty内存管理：PoolChunkList - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本系列：
- 《[深入浅出Netty内存管理：PoolChunk](http://blog.jobbole.com/106001/)》
- 《[深入浅出Netty内存管理：PoolSubpage](http://blog.jobbole.com/106037/)》
前面两篇分别分析了PoolChunk和PoolSubpage的实现，本文主要分析管理PoolChunk生命周期的PoolChunkList。
### PoolChunkList
PoolChunkList负责管理多个chunk的生命周期，在此基础上对内存分配进行进一步的优化。


```
final class PoolChunkList<T> implements PoolChunkListMetric {
    private final PoolChunkList<T> nextList;
    private final int minUsage;
    private final int maxUsage;
    private PoolChunk<T> head;
    private PoolChunkList<T> prevList;
    ...
}
```
从代码实现可以看出，每个PoolChunkList实例维护了一个PoolChunk链表，自身也形成一个链表，为何要这么实现？
![2184951-c5a5bf6c03d86ce0](http://jbcdn2.b0.upaiyun.com/2016/09/d088ace9ecf0168057f2ee4065782e8c.png)
随着chunk中page的不断分配和释放，会导致很多碎片内存段，大大增加了之后分配一段连续内存的失败率，针对这种情况，可以把内存使用率较大的chunk放到PoolChunkList链表更后面，具体实现如下：


```
boolean allocate(PooledByteBuf<T> buf, int reqCapacity, int normCapacity) {
    if (head == null) {
        return false;
    }
    for (PoolChunk<T> cur = head;;) {
        long handle = cur.allocate(normCapacity);
        if (handle < 0) {
            cur = cur.next;
            if (cur == null) {
                return false;
            }
        } else {
            cur.initBuf(buf, handle, reqCapacity);
            if (cur.usage() >= maxUsage) {   // (1)
                remove(cur);
                nextList.add(cur);
            }
            return true;
        }
    }
}
```
假设poolChunkList中已经存在多个chunk。当分配完内存后，如果当前chunk的使用量超过maxUsage，则把该chunk从当前链表中删除，添加到下一个链表中。
但是，随便chunk中内存的释放，其内存使用率也会随着下降，当下降到minUsage时，该chunk会移动到前一个列表中，实现如下：


```
boolean free(PoolChunk<T> chunk, long handle) {
    chunk.free(handle);
    if (chunk.usage() < minUsage) {
        remove(chunk);
        if (prevList == null) {
            assert chunk.usage() == 0;
            return false;
        } else {
            prevList.add(chunk);
            return true;
        }
    }
    return true;
}
```
从poolChunkList的实现可以看出，每个chunkList的都有一个上下限：minUsage和maxUsage，两个相邻的chunkList，前一个的maxUsage和后一个的minUsage必须有一段交叉值进行缓冲，否则会出现某个chunk的usage处于临界值，而导致不停的在两个chunk间移动。
所以chunk的生命周期不会固定在某个chunkList中，随着内存的分配和释放，根据当前的内存使用率，在chunkList链表中前后移动。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
