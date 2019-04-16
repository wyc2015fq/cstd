# Nginx 模块自主开发二：Nginx的类型命名封装 - youbingchen的博客 - CSDN博客





2016年06月23日 14:29:49[youbingchen](https://me.csdn.net/youbingchen)阅读数：499标签：[nginx																[数据结构																[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)
个人分类：[nginx](https://blog.csdn.net/youbingchen/article/category/6263213)





> 
我们在看Nginx源代码的时候，看不到我们以前在写C语言的类型名称（比如int,char等），其实是Nginx为了跨平台习惯将一些 C语言的东西进行了封装，所以在编写自己的模块的 时候，建议尽量沿用Nginx的习惯。


### 整型封装

```cpp
typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
```

### ngx_str_t 数据结构体

> 
在Nginx的领域中，Nginx对ngx_str_t结构就是字符串，ngx_str_t的定义如下：


```cpp
typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;
```

> 
ngx_str_t的data成员指向的并不是 普通 的 字符串，因为这段字符串未必会以’\0’结尾的，所以会使用长度len来衡量data的具体的值。


### ngx_list_t 数据结构

> 
ngx_list_t 是Nginx封装的链表容器，HTTP的 头部就是 使用nginx_list_t来存储的


```cpp
// 文件名:ngx_list.h
typedef struct ngx_list_part_s  ngx_list_part_t;
// ngx_list_part_s  只描述链 表的一个 元素
struct ngx_list_part_s {
    void             *elts; // 指向数组的起始地址
    ngx_uint_t        nelts; //表示数组已经使用了多少个元素，nelts要小于ngx_list_t的nalloc
    ngx_list_part_t  *next;  //下一个链表元素ngx_list_part_t
}; 

/*ngx_list_t 表示的是整个链表 ，不是简单 的链表 ，存储数组的链表，每个ngx_list_part_s又是一个数组，拥有连续的空间,依赖ngx_list_t中的size和nalloc，也依赖ngx_list_part_t中的nelts来表示数组当前已经使用了 多少容量 */
typedef struct {
    ngx_list_part_t  *last; // 指向链表的 最后一个元素 
    ngx_list_part_t   part; // 链表的首个元素 
    size_t            size;  // 通过 size限制每个元素的占用空间大小，也就是用户存储一个数据所占用的字节数 必须小于或等于size
    ngx_uint_t        nalloc; //链表 的数据元素一旦分配后是 不可更改，nalloc表示每个ngx_list_part_t数组 的 容量，最多存储多少个数据 
    ngx_pool_t       *pool; //链表中管理 内存分配的内存池池对象，用户存放的数据 占用的内存都是由pool分配的
} ngx_list_t;
```

> 
链表这样设计的好处： 

  +  链表中存储元素是灵活，它可以是一种任意的数据结构。 

  +  链表中的元素需要占用的 内存用ngx_list_t管理 ，它已经通过数组分配好了  

  +  小块的内存使用链表访问效率 是低下的，使用数组通过偏移量来直接访问内存则高效多了 
![list_struct](https://img-blog.csdn.net/20160623143034475)

  对于链表，Nginx 提供的接口就是包括 ： ngx_list_create 接口用于创建新的 链表，ngx_list_init接口用于初始化一个已有的链表，ngx_list_push  接口用于添加新的 元素 
```cpp
//文件名为ngx_list.h
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size); // pool 为内存池的对象，size为 每个元素的 大小，n相当于nalloc的大小
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size);

void *ngx_list_push(ngx_list_t *list); //传进去的是ngx_list_t 链表
ngx_str_t * str = ngx_list_push(testlist);
if (str == NULL){
 return NGX_ERROR;
}
str->len=  sizeof("Hello  world");
str->value= "Hello World";
```

### ngx_table_elt_t数据结构

> 
用来解析HTTP头部


```cpp
typedef struct {
    ngx_uint_t        hash; // 用来快速检索头部
    ngx_str_t         key;  // 存储头部名称
    ngx_str_t         value; // 存储对应的值
    u_char           *lowcase_key; // 为了忽略HTTP头部名称的大小写
} ngx_table_elt_t;
```

### ngx_buf_t数据结构

> 
缓冲区是Nginx处理大数据的关键数据结构，它既用于内存数据也应用于磁盘数据 

  这个 结构本身太复杂了，等以后用到在说


### ngx_chain_t数据结构

> 
ngx_chain_t 是与ngx_buf_t配合 使用的链表数据结构


```cpp
typedef struct ngx_chain_s       ngx_chain_t;
struct ngx_chain_s {
    ngx_buf_t    *buf;  //指向当前的ngx_buf_t的缓冲区
    ngx_chain_t  *next; // 指向下一个ngx_chain_t，如果是最后一个ngx_chain_t,一定 要将next设为NULL
};
```](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)




