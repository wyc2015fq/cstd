# 深入浅出Netty内存管理：PoolSubpage - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本系列：
- 《[深入浅出Netty内存管理：PoolChunk](http://blog.jobbole.com/106001/)》
上一节中分析了如何在poolChunk中分配一块大于pageSize的内存，但在实际应用中，存在很多分配小内存的情况，如果也占用一个page，明显很浪费。针对这种情况，Netty提供了PoolSubpage把poolChunk的一个page节点8k内存划分成更小的内存段，通过对每个内存段的标记与清理标记进行内存的分配与释放。
![01](http://jbcdn2.b0.upaiyun.com/2016/09/8d091d1e480f52529464bb39dd6fab72.png)
PoolSubpage


```
final class PoolSubpage<T> {
    // 当前page在chunk中的id
    private final int memoryMapIdx; 
    // 当前page在chunk.memory的偏移量
    private final int runOffset;    
    // page大小
    private final int pageSize;
    //通过对每一个二进制位的标记来修改一段内存的占用状态
    private final long[] bitmap; 
    PoolSubpage<T> prev;     
    PoolSubpage<T> next;
    boolean doNotDestroy;     
    // 该page切分后每一段的大小
    int elemSize;    
    // 该page包含的段数量
    private int maxNumElems;        
    private int bitmapLength;
    // 下一个可用的位置
    private int nextAvail;
    // 可用的段数量
    private int numAvail;        
    ...
}
```
假设目前需要申请大小为4096的内存：


```
long allocate(int normCapacity) {
    if ((normCapacity & subpageOverflowMask) != 0) { // >= pageSize
        return allocateRun(normCapacity);
    } else {
        return allocateSubpage(normCapacity);
    }
}
```
因为 4096 < pageSize(8192)，所以采用 allocateSubpage 进行内存分配，具体实现如下：


```
private long allocateSubpage(int normCapacity) {
    // Obtain the head of the PoolSubPage pool that is owned by the PoolArena and synchronize on it.
    // This is need as we may add it back and so alter the linked-list structure.
    PoolSubpage<T> head = arena.findSubpagePoolHead(normCapacity);
    synchronized (head) {
        int d = maxOrder; // subpages are only be allocated from pages i.e., leaves
        int id = allocateNode(d);
        if (id < 0) {
            return id;
        }
        final PoolSubpage<T>[] subpages = this.subpages;
        final int pageSize = this.pageSize;
        freeBytes -= pageSize;
        int subpageIdx = subpageIdx(id);
        PoolSubpage<T> subpage = subpages[subpageIdx];
        if (subpage == null) {
            subpage = new PoolSubpage<T>(head, this, id, runOffset(id), pageSize, normCapacity);
            subpages[subpageIdx] = subpage;
        } else {
            subpage.init(head, normCapacity);
        }
        return subpage.allocate();
    }
}
```
1、Arena负责管理PoolChunk和PoolSubpage；
2、allocateNode负责在二叉树中找到匹配的节点，和poolChunk不同的是，只匹配叶子节点；
3、poolChunk中维护了一个大小为2048的poolSubpage数组，分别对应二叉树中2048个叶子节点，假设本次分配到节点2048，则取出poolSubpage数组第一个元素subpage；
4、如果subpage为空，则进行初始化，并加入到poolSubpage数组；
subpage初始化实现如下：


```
PoolSubpage(PoolSubpage<T> head, 
    PoolChunk<T> chunk, 
    int memoryMapIdx, int runOffset, 
    int pageSize, elemSize) {
    this.chunk = chunk;
    this.memoryMapIdx = memoryMapIdx;
    this.runOffset = runOffset;
    this.pageSize = pageSize;
    bitmap = new long[pageSize >>> 10]; // pageSize / 16 / 64
    init(head, elemSize);
}
```
1、默认初始化bitmap长度为8，这里解释一下为什么只需要8个元素：其中分配内存大小都是处理过的，最小为16，说明一个page可以分成8192/16 = 512个内存段，一个long有64位，可以描述64个内存段，这样只需要512/64 = 8个long就可以描述全部内存段了。
2、init根据当前需要分配的内存大小，确定需要多少个bitmap元素，实现如下：


```
void init(PoolSubpage<T> head, int elemSize) {
    doNotDestroy = true;
    this.elemSize = elemSize;
    if (elemSize != 0) {
        maxNumElems = numAvail = pageSize / elemSize;
        nextAvail = 0;
        bitmapLength = maxNumElems >>> 6;
        if ((maxNumElems & 63) != 0) {
            bitmapLength ++;
        }
        for (int i = 0; i < bitmapLength; i ++) {
            bitmap[i] = 0;
        }
    }
    addToPool(head);
}
```
下面通过分布申请4096和32大小的内存，说明如何确定bitmapLength的值：
- 比如，当前申请大小4096的内存，maxNumElems 和 numAvail 为2，说明一个page被拆分成2个内存段，2 >>> 6 = 0，且2 & 63 ！= 0，所以bitmapLength为1，说明只需要一个long就可以描述2个内存段状态。
- 如果当前申请大小32的内存，maxNumElems 和 numAvail 为 256，说明一个page被拆分成256个内存段， 256>>> 6 = 4，说明需要4个long描述256个内存段状态。
下面看看subpage是如何进行内存分配的：


```
long allocate() {
    if (elemSize == 0) {
        return toHandle(0);
    }
    if (numAvail == 0 || !doNotDestroy) {
        return -1;
    }
    final int bitmapIdx = getNextAvail();
    int q = bitmapIdx >>> 6;
    int r = bitmapIdx & 63;
    assert (bitmap[q] >>> r & 1) == 0;
    bitmap[q] |= 1L << r;
    if (-- numAvail == 0) {
        removeFromPool();
    }
    return toHandle(bitmapIdx);
}
```
1、方法getNextAvail负责找到当前page中可分配内存段的bitmapIdx；
2、q = bitmapIdx >>> 6，确定bitmap数组下标为q的long数，用来描述 bitmapIdx 内存段的状态；
3、bitmapIdx & 63将超出64的那一部分二进制数抹掉，得到一个小于64的数r；
4、bitmap[q] |= 1L << r将对应位置q设置为1；
如果以上描述不直观的话，下面换一种方式解释，假设需要分配大小为128的内存，这时page会拆分成64个内存段，需要1个long类型的数字描述这64个内存段的状态，所以bitmap数组只会用到第一个元素。
![02](http://jbcdn2.b0.upaiyun.com/2016/09/c471d4447e81a7703f3bb293ef3565d4.png)
状态转换
getNextAvail如何实现找到下一个可分配的内存段？


```
private int getNextAvail() {
    int nextAvail = this.nextAvail;
    if (nextAvail >= 0) {
        this.nextAvail = -1;
        return nextAvail;
    }
    return findNextAvail();
}
```
1、如果nextAvail大于等于0，说明nextAvail指向了下一个可分配的内存段，直接返回nextAvail值；
2、每次分配完成，nextAvail被置为-1，这时只能通过方法findNextAvail重新计算出下一个可分配的内存段位置。


```
private int findNextAvail() {
    final long[] bitmap = this.bitmap;
    final int bitmapLength = this.bitmapLength;
    for (int i = 0; i >>= 1;
    }
    return -1;
}
```
1、~bits != 0说明这个long所描述的64个内存段还有未分配的；
2、(bits & 1) == 0 用来判断该位置是否未分配，否则bits又移一位，从左到右遍历值为0的位置；
至此，subpage内存段已经分配完成。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
