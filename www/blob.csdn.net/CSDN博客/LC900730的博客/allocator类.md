# allocator类 - LC900730的博客 - CSDN博客
2017年09月26日 18:09:47[lc900730](https://me.csdn.net/LC900730)阅读数：153
allocator是一个模板。为了定义这个allocator对象，我们必须指明这个allocator可以分配的对象类型。 
allocatoralloc;
allocator分配的内存是未构造的。
```
allocator<string >alloc;            //可以分配string的allocator对象
auto const p=alloc.allocate(n);     //分配n个未初始化的string
auto q=p;
alloc.constructor(q++);                  //*q为空字符串
alloc.constructor(q++,10,'c');          //*q为cccccc
alloc.constructor(q++,'hi');            //*q为hi
```
为了使用allocate返回的内存，我们必须使用constructor构造对象。使用未构造的内存，其行为是未定义的。
### 拷贝和填充未初始化内存的算法
|unintialized_copy(b,e,b2)|从迭代器b和e指出的输入范围中拷贝元素到迭代器b2指定的未构造的原始内存中。b2指向的内存必须足够大，能容纳元素的拷贝|
|----|----|
|unintialized_copy_n(b,n,b2)|从迭代器b指向的元素开始，拷贝n个元素到b2开始的内存中。|
|unintialized_fill(b,e,t)|从迭代器b和e指出的原始内存范围中创建对象，对象值均为t的拷贝。|
|unintialized_fill_n(b,n,t)|从迭代器b指向的内存地址开始创建n个对象。b必须指向足够大的未构造的原始内存，能容纳给定数量的对象|
如
```
//vector<int> v1;
//分配比v1元素中所占用空间大一倍的动态内存
auto p=alloc.allocate(v1.size()*2);
//拷贝v1中元素来构造p开始的元素
auto p=uninitialized_copy(v1.begin(),v1.end(),p);
//将剩余元素初始化为42
uninitialized_fill_n( q,vi.size(),42);
```
unintialized_copy调用会返回一个指针，指向最后一个构造的元素之后的位置。
