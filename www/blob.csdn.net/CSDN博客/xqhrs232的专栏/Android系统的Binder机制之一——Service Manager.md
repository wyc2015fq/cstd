# Android系统的Binder机制之一——Service Manager - xqhrs232的专栏 - CSDN博客
2010年12月22日 14:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1432标签：[service																[manager																[android																[server																[服务器																[linux内核](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.unix-center.net/~Simon_fu/?p=875](http://my.unix-center.net/~Simon_fu/?p=875)
Android虽然构建在Linux上面，但是在IPC（进程间）机制方面，没有利用Linux提供IPC机制，而是自己实现了一套轻量级的IPC机制——binder机制。并且Android Binder机制之上，Android框架提供了一套封装，可以实现对象代理（在本地进程中代理远程进程的对象）。本文简单分析一下Android Binder机制。
### Binder情景分析
    一个IPC通讯我们可以理解成客户端-服务器模式，因此我们先在这里分析一下典型的Binder应用模式：
> 
1、客户端通过某种方式（后文会详细介绍）得到服务器端的代理对象。从客户端角度看来代理对象和他的本地对象没有什么差别。它可以像其他本地对象一样调用其方法，访问其变量。
2、客户端通过调用服务器代理对象的方法向服务器端发送请求。 
3、代理对象把用户请求通过Android内核（Linux内核）的Binder驱动发送到服务器进程。 
4、服务器进程处理用户请求，并通过Android内核（Linux内核）的Binder驱动返回处理结果给客户端的服务器代理对象。 
5、客户端收到服务器端的返回结果。
    如果你对COM或者Corba熟悉的话，以上的情景是否会让你联想到什么呢？没错！都是对象代理。以上的情景，在Android中经常会被用到。如果你还没有注意到这点儿，那么本文非常适合你。
### Binder机制的组成
#### 1、Binder驱动
    binder是内核中的一个字符驱动设备位于：/dev/binder。这个设备是Android系统IPC的核心部分，客户端的服务代理用来通过它向服务器（server）发送请求，服务器也是通过它把处理结果返回给客户端的服务代理对象。我们只需要知道它的功能就可以了，本文我们的重点不在这里，所以后面不会专门介绍这部分，因为很少会有人会显示打开这个设备去开发Android程序。如果想深入了解的话，请研究内核源码中的binder.c。
#### 2、Service Manager
    负责管理服务。对应于第一步中，客户端需要向Service Manager来查询和获得所需要服务。服务器也需要向Service Manager注册自己提供的服务。可以看出Service Manager是服务的大管家。
#### 3、服务（Server）
    需要强调的是这里服务是指的是System Server，而不是SDK server，请参考《[（转）高焕堂——Android框架底层结构知多少？](http://my.unix-center.net/~Simon_fu/?p=865)》关于两种Server的介绍（其实应该是三种，丢掉了init调用的server，在init.rc中配置）。
#### 4、客户端
    一般是指Android系统上面的应用程序。它可以请求Server中的服务。
#### 5、对象代理
    是指在客户端应用程序中生成的Server代理（proxy）。从应用程序角度看代理对象和本地对象没有差别，都可以调用其方法，方法都是同步的，并且返回相应的结果。
### 大内总管——Service Manager
    Android系统Binder机制的总管是Service Manager，所有的Server（System Server）都需要向他注册，应用程序需要向其查询相应的服务。可见其作用是多么的重要，所以本文首先介绍Service Manager。
    通过上面介绍我们知道Service Manager非常重要，责任重大。那么怎样才能成为Service Manager呢？是不是谁都可以成为Service Manager呢？怎样处理server的注册和应用程序的查询和获取服务呢？为了回答这些问题先查看，Android中Service Manager的源码，其源码位于：
frameworks/base/cmds/servicemanager/service_manager.c
我们发现了main函数，说明他自己就是一个进程，在init.rc中我们发现：
service servicemanager /system/bin/servicemanager
    user system
    critical
    onrestart restart zygote
    onrestart restart media
说明其是Android核心程序，开机就会自动运行。
    下面我们在研究一下它的代码，main函数很简单：
int main(int argc, char **argv)
{
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;
    bs = binder_open(128*1024);
    if (binder_become_context_manager(bs)) {
        LOGE("cannot become context manager (%s)/n", strerror(errno));
        return -1;
    }
    svcmgr_handle = svcmgr;
    binder_loop(bs, svcmgr_handler);
    return 0;
}
我们看到它先调用binder_open打开binder设备（/dev/binder），其次它调用了binder_become_context_manager函数，这个函数使他自己变为了“Server大总管”，其代码如下：
int binder_become_context_manager(struct binder_state *bs)
{
    return ioctl(bs->fd, BINDER_SET_CONTEXT_MGR, 0);
}
也就是通过ioctl向binder设备声明“我就是server大总管”。
    Service Manager作为一个Server大总管，本身也是一个server。既然是一个server就要时刻准备为客户端提供服务。最好Service Manager调用binder_loop进入到循环状态，并提供了一个回调函数，等待用户的请求。注意他的Service Manager的客户端既包括应用程序（查询和获取服务），也包括Server（注册服务）。
    Service Manager的客户怎样才能请求其服务呢？答案是上文我们提到的情景一样。客户需要在自己进程中创建一个服务器代理。现在没有地方去查询服务，那么怎样它的客户怎样生成他的服务代理对象呢？答案是binder设备（/devbinder）为每一个服务维护一个句柄，调用binder_become_context_manager函数变为“Server大总管”的服务，他的句柄永远是0，是一个“众所周知”的句柄，这样每个程序都可以通过binder机制在自己的进程空间中创建一个
Service Manager代理对象了。其他的服务在binder设备在设备中的句柄是不定的，需要向“Server大总管”查询才能知道。
    现在我们需要研究Server怎样注册服务了，还是在其源码中，我们可以看到在其服务处理函数中（上文提到binder_loop函数注册给binder设备的回调函数）有如下代码：
case SVC_MGR_ADD_SERVICE:
        s = bio_get_string16(msg, &len);
        ptr = bio_get_ref(msg);
        if (do_add_service(bs, s, len, ptr, txn->sender_euid))
            return -1;
        break;
有server向binder设备写入请求注册Service时，Service Manager的服务处理回调函数将会被调用。我们在仔细看看do_add_service函数的实现：
int do_add_service(struct binder_state *bs,
                   uint16_t *s, unsigned len,
                   void *ptr, unsigned uid)
{
    struct svcinfo *si;
//    LOGI("add_service('%s',%p) uid=%d/n", str8(s), ptr, uid);if (!ptr || (len == 0) || (len > 127))
        return -1;
    if (!svc_can_register(uid, s)) {
        LOGE("add_service('%s',%p) uid=%d - PERMISSION DENIED/n",
             str8(s), ptr, uid);
        return -1;
    }
    si = find_svc(s, len);
    if (si) {
        if (si->ptr) {
            LOGE("add_service('%s',%p) uid=%d - ALREADY REGISTERED/n",
                 str8(s), ptr, uid);
            return -1;
        }
        si->ptr = ptr;
    } else {
        si = malloc(sizeof(*si) + (len + 1) * sizeof(uint16_t));
        if (!si) {
            LOGE("add_service('%s',%p) uid=%d - OUT OF MEMORY/n",
                 str8(s), ptr, uid);
            return -1;
        }
        si->ptr = ptr;
        si->len = len;
        memcpy(si->name, s, (len + 1) * sizeof(uint16_t));
        si->name[len] = '/0';
        si->death.func = svcinfo_death;
        si->death.ptr = si;
        si->next = svclist;
        svclist = si;
    }
    binder_acquire(bs, ptr);
    binder_link_to_death(bs, ptr, &si->death);
    return 0;
}
我们看到首先检查是否有权限注册service，没权限就对不起了，出错返回；然后检查是否已经注册过，注册过的service将不能再次注册。然后构造一个svcinfo对象，并加入一个全局链表中svclist中。最后通知binder设备：有一个service注册进来。
    我们再来看看客户端怎样通过Service Manager获得Service，还是在服务处理函数中（上文提到binder_loop函数注册给binder设备的回调函数）有如下代码：
case SVC_MGR_GET_SERVICE:
    case SVC_MGR_CHECK_SERVICE:
        s = bio_get_string16(msg, &len);
        ptr = do_find_service(bs, s, len);
        if (!ptr)
            break;
        bio_put_ref(reply, ptr);
        return 0;
    我们可以看到通过do_find_service查找Service如果查找到的话，写入reply中返回给客户端。
    本文我们简单分析了一下Service Manager，后续我们会继续分析Android binder机制的其他部分。

