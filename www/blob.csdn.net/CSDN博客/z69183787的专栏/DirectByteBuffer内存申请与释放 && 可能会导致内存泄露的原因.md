# DirectByteBuffer内存申请与释放 && 可能会导致内存泄露的原因 - z69183787的专栏 - CSDN博客
2018年02月11日 12:08:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：623
DirectByteBuffer类是在Java Heap外分配内存，对堆外内存的申请主要是通过成员变量unsafe来操作，下面介绍构造方法
    // Primary constructor
    //
    DirectByteBuffer(int cap) {                   // package-private
        super(-1, 0, cap, cap);
        boolean pa = VM.isDirectMemoryPageAligned();//内存是否按页分配对齐
        int ps = Bits.pageSize();//获取每页内存大小
        long size = Math.max(1L, (long)cap + (pa ? ps : 0));//分配内存的大小，如果是按页对齐方式，需要再加一页内存的容量
        Bits.reserveMemory(size, cap);//用Bits类保存总分配内存(按页分配)的大小和实际内存的大小
        long base = 0;
        try {
            base = unsafe.allocateMemory(size);//在堆外内存的基地址
        } catch (OutOfMemoryError x) {
            Bits.unreserveMemory(size, cap);
            throw x;
        }
        unsafe.setMemory(base, size, (byte) 0);//初始化堆外内存的数据为0
        if (pa && (base % ps != 0)) {
            // Round up to page boundary
            address = base + ps - (base & (ps - 1));//计算堆外内存的基地址
        } else {
            address = base;
        }
        cleaner = Cleaner.create(this, new Deallocator(base, size, cap));//释放内存会通过cleaner类操作
        att = null;
    }
假设堆外内存已经用了512K,pagesize=1024k,需要再分配 512个字节，为了使内存按页对齐，首先申请512+1024=1536个字节，新分配内存的基地址base=512+1024-(512&1023)=1024。
DirectByteBuffer类的内存里是在堆里申请的，但是通过基类Buffer的capacity，long address两个变量指向堆外的内存，如果在程序中创建了DirectByteBuffer类实例A，当A自身没有被引用时，在触发GC回收之前，jvm把A放在PhantomReference队列里，同时不断扫描PhantomReference队列，取出A,触发new Deallocator里的run方法回收堆外直接内存，同时回收A自身的堆内存.
Direct ByteBuffer是在不是在Java Heap分配内存，而是在C Heap分配内存，但Direct ByteBuffer分配出去的内存其实也是由GC负责回收的，Hotspot在GC时会扫描Direct ByteBuffer对象是否有引用，如没有则回收其占用的堆外内存。有两种可能会导致堆外的内存无法被回收:
1.在6u32前的版本里，CMS GC有bug会导致可能回收不掉
2.Direct ByteBuffer对象晋升到了Old区，只能等Full GC触发（CMS GC的情况下等CMS GC），因此在Direct ByteBuffer使用较多，存活时间较长的情况下，有可能会导致堆外内存耗光（因为Direct ByteBuffer本身对象所占用的空间是很小的）。 Direct ByteBuffer对象本身是放在堆里，但是一个对象占用很少内存，主要是它关联到堆外内存去， Direct ByteBuffer要full gc才能回收，所以如果堆内存用得少，堆外内存用得多，这样不触发full gc会导致堆外内存out of memory。如何判断是Direct ByteBuffer造成的：如碰到堆外内存占用较多的场景，可以尝试强制执行Full GC（强制的方法为执行jmap -histo:live）看看，多执行一两次，如堆外内存下降的话，很有可能就是Direct ByteBuffer造成的，对于这种情况，通常下面的启动参数就可解决。怎么解决：
最好是在启动参数上增加-XX:MaxDirectMemorySize=x[m|g]，例如-XX:MaxDirectMemorySize=500m
（在没设置MaxDirectMemorySize参数的情况下，用jinfo -flag等方式会看到默认值是-1，但VM.maxDirectMemory这个方法里发现是-1，则会以-Xmx作为默认值），此参数的含义是当Direct ByteBuffer分配的堆外内存到达指定大小后，即触发Full GC（这段逻辑请见Bits.reserveMemory的代码），如Full GC后仍然分配不出Direct ByteBuffer需要的空间，则会报OOM错误： 
java.lang.OutOfMemoryError: Direct buffer memory参考博文：http://m.blog.csdn.net/article/details?id=50317443
