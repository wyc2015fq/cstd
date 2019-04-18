# epoll 底层实现源码分析 - xiaoyu15407的博客 - CSDN博客
2015年06月24日 13:34:32[xiaoyu15407](https://me.csdn.net/xiaoyu15407)阅读数：3547
### epoll 底层实现源码分析
##### 1.  epoll相关的函数
> - ##### epoll实例创建函数epoll_create
```cpp
#include <sys/epoll.h>
int epoll_create(int size);
int epoll_create1(int flags);
```
- ##### epoll控制接口,用于向epoll实例(注册/修改/删除)感兴趣的socket的事件
```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```
- ##### epoll_wait 用于等待已经注册的感兴趣的事件的发生
```cpp
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event *events,int maxevents,
                int timeout);
int epoll_pwait(int epfd, struct epoll_event *events,int maxevents,
                int timeout,const sigset_t *sigmask);
```
##### 2.  epoll相关的数据结构
```cpp
typedef union epoll_data {
   void    *ptr;
   int      fd;
   uint32_t u32;
   uint64_t u64;
} epoll_data_t;
struct epoll_event {
   uint32_t     events; //感兴趣的事件如EPOLLIN，EPOLLOUT，EPOLLPRI等
   epoll_data_t data;   //一般使用data的fd成员表示感兴趣的socket
};
```
##### 3.epoll_create 底层实现
epoll_create对应的内核态函数如下该函数位于/fs/eventpoll.c,该文件是epoll的实现文件
```cpp
SYSCALL_DEFINE1(epoll_create, int, size)
{
    if (size <= 0)
        return -EINVAL;
    return sys_epoll_create1(0);
}
```
可以发现epoll_create的size参数是没有使用的，但是size大小必须大于0否则会返回-EINVAL的错误，接着进入sys_epoll_create1函数,下面的函数只贴主要的代码
```cpp
SYSCALL_DEFINE1(epoll_create1, int, flags)
{
    int error, fd;
    struct eventpoll *ep = NULL;
    struct file *file;
    /*
    eventpoll结构体是epoll的核心里面存放着许多信息，主要包括
    1. struct rb_root rbr;这是一颗红黑树的根节点，代表着一颗红黑树，
    红黑树下面挂的是我们感兴趣的socket的事件,当我们调用epoll_ctl向
    epoll添加感兴趣的socket事件时，系统将我们的传递的信息封装成
    struct epitem结构体，然后挂到这颗红黑树的相应节点上
    2.struct list_head rdllist;这是一个双向链表,这个双向链表中存放
    的是就绪的事件当我们调用epoll_wait的时候这些事件会返回给用户
    3.struct file *file;文件结构指针，指向epoll文件
    */
    error = ep_alloc(&ep);//分配一个eventpoll的结构体，并设置相应的初始值
    if (error < 0)
        return error;
    //在描述符表中找到一个没用使用的描述符
    fd = get_unused_fd_flags(O_RDWR | (flags & O_CLOEXEC));
    if (fd < 0) {
        error = fd;
        goto out_free_ep;
    }
    //创建一个名叫[eventpoll]的文件，并返回其文件结构指针,这个文件代表着epoll实例
    file = anon_inode_getfile("[eventpoll]", &eventpoll_fops, ep,
                 O_RDWR | (flags & O_CLOEXEC));
    if (IS_ERR(file)) {
        error = PTR_ERR(file);
        goto out_free_fd;
    }
    //将eventpoll中的file指针指向该文件
    ep->file = file;
    //将该文件描述符与文件指针关联起来，并返回epoll实例的文件描述符fd
    //epoll可以看成是一个文件
    fd_install(fd, file);
    return fd;
out_free_fd:
    put_unused_fd(fd);
out_free_ep:
    ep_free(ep);
    return error;
}
```
当epoll_create1执行完毕，我们就创建了一个代表epoll实例的文件并返回其文件描述符，但是我们要怎么找到eventpoll这个最重要的结构体呢，答案在上面代码中的anon_inode_getfile函数,我们将eventpoll的结构体变量ep作为函数的第3个参数传了进去，下面是函数anon_inode_getfile的部分代码，删掉了不相关部分
```cpp
struct file *anon_inode_getfile(const char *name,
                const struct file_operations *fops,
                void *priv, int flags)
{
    struct file *file;
    file = alloc_file(&path, OPEN_FMODE(flags), fops);
    file->f_mapping = anon_inode_inode->i_mapping;
    file->f_flags = flags & (O_ACCMODE | O_NONBLOCK);
    file->private_data = priv;
    return file;
}
```
从上面可以看出，我们将eventpoll结构体放到了文件结构file的private_data中，所以只要我们能找到file结构体就能找到eventpoll，因为上面将epoll的文件描述符fd和file结构体相关联了，所以可以通过epoll的文件描述符找到file继而找到eventpoll.
