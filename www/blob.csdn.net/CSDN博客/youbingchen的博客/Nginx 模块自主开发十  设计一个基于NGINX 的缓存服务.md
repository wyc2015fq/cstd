# Nginx 模块自主开发十: 设计一个基于NGINX 的缓存服务 - youbingchen的博客 - CSDN博客





2016年07月06日 09:10:30[youbingchen](https://me.csdn.net/youbingchen)阅读数：508








> 
计算机不仅需要对计算机理论有一定的要求，同时要学以致用，这篇博客将结合公司给我的一个问题，我自主设计一个初步的方案 ，如何 有什么bug或是想法，可以提出来，我一直秉承一个思想：good idea worth spreading.只有将自己的学习计划放在互联网上，才会进步得更快。


# 问题

> 
将磁盘文件分块加载到内存，一般来说磁盘文件远大于内存空间，所以将磁盘文件分块，部分读入内存，系统预分配内存空间用来预读。减少响应的时间，我们的目标就是要实现对这部分Cache进行管理和从磁盘快速预读块内容。关于写操作 ，我打算采用的 是异步写的方式。


# 方案

![cache_design](https://img-blog.csdn.net/20160706090942125)

### 接口代码设计

```cpp
/*这里我采用的双向队列管理cache，采用的是LRU算法替换，当cache命中 的时候，将该节点换到队列的首部，不命中的 情况分为两种，一种是cache块 达到我们分配的最大数量，
这时采用的LRU算法替换,(将这个队列的尾部的块直接删除，因为队列的尾部块就是最久未使用的块），将新读取的块加入到队列的首部，另外一种情况是
块大小还没满足我们设定的上限，直接将加入的块插入到队列的首部*/

struct ngx_queue_new_t
{
    void *data;  // 存储块文件内容

    unsigned int position; // 块起始位置

    unsigned int offset; // 块偏移量(也可以是长度)
    char *  fileName;  // 文件名
    ngx_queue_new_t *next; // 下一块
    ngx_queue_new_t *pre; // 前一块
};


//相应队列 的操作有:

//插入 头部的节点
bool ngx_queue_insert_head(ngx_queue_new_t *queueNode);

// 删除尾部的节点
bool ngx_queue_delete_tail(ngx_queue_new_t *queueNode);
// 查找相应的节点
ngx_queue_new_t * ngx_queue_search(char *filename,unsigned int position,unsigned int offset);
/*由于链表是顺序查询的，最坏情况就是 查找完了 还知道这个块在不在内存，因此采用红黑树进行快速定位，这样做的好处就是，
如果块在内存不存在的时候，我可以在O(lgn)时间内知道，但是如果存在的话就会O(lgn)+O(n)*/
// 红黑树 或是只用STL  set就行
struct ngx_red_black_tree{
    char *hashFileName; // 这里的filename不是上面队列中，这里是经过一种hash后filename，最简单的就是 filName+tostring(position)+tostring(offset);
};

//这时候红黑树的 相应的操作有

// 插入节点 ，发生在 cache不命中的时候

bool ngx_red_black_tree_insert(char *hashFileName);

// 删除节点 ，发生在替换的时候

bool ngx_red_black_tree_delete(char *hashFileName);

// 查询节点 ，每次读操作都要进行判断的

bool ngx_red_black_tree_search(char *hashFileName);




// 最后定义好cache服务的接口
class cache_server{

  public:
       cache_server(){}   // 读取配置文件，初始化块大小，以及内存块数量
       void * getServerData(char * fileName,unsigned int position,unsigned int offset); // 提供给客户端读接口
       void * writeServerData(void *data);  // 写数据，这里 写数据可能并不需要cache，可以采用异步响应的方法
  private:
       ngx_queue_new_t *head;
       ngx_queue_new_t *tail;
       ngx_red_black_tree  *root;
       unsigned int blockSize; // 内存块大小 
       unsigned int  blockCount; // 系统提供给块的数量
       unsigned int maxBlockCount; // 系统最大分给cache的内存块
};

// 这个类是专门用于实现native aio机制，负责专门异步文件的IO读操作，但是aio异步文件操作不支持缓存操作，这个在实现的时候要注意
class native_aio_read{

};
// 下面这个类就是用于专门写磁盘，此时写操作
class write_io
{

};

/*总结: 上面的方法设计只是一种，也可以利用红黑树管理真正的文件 内容，把队列用于管理替换的方法，但是从时间复杂度其实都是差不多，最坏的就是O(lgn)+O(n)*/
```

# 总结

这个是 方案和Nginx管理Cache 的思想有一个很大 的不同就是队列结构这种直接管理了文件的内容，Nginx中的数据结构是不存储文件内容，而是利用这些数据结构进行管理Cache，但是基本上我这里 使用的数据结构的灵感是来源Nginx中。



