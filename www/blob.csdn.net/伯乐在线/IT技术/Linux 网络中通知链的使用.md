# Linux 网络中通知链的使用 - 文章 - 伯乐在线
原文出处： [yangyefeng820803](http://blog.chinaunix.net/uid-31087949-id-5752533.html)
Linux 网络内核代码中使用了通知链（Notification Chains）来使相关的子系统对感兴趣的事件作出反应。下面从头介绍一下通知链的使用。
# 数据结构定义
通知链使用的数据结构如下：


```
struct notifier_block {
  int (*notifier_call)(struct notifier_block *, unsigned long, void *);
  struct notifier_block *next;
  int priority;
};
```
其 中notifier_call 是通知链要执行的函数指针，后面会介绍它的参数和返回值；next 用来连接下一个notifier_block；priority是这个通知的优先级，同一条链上的notifier_block是按优先级排列的，一般都 初始化为0，这样就按照注册到通知链上的顺序执行。
内核代码中一般把通知链命名成xxx_chain，xxx_notifier_chain 这种形式的变量名
# 注册一个notifier_block
在通知链上增加一个notifier_block是用notifier_chain_register() 函数完成的


```
static int notifier_chain_register(struct notifier_block **nl, struct notifier_block *n)
{
  while ((*nl) != NULL) {
   if (n->priority > (*nl)->priority)
     break;
   nl = &((*nl)->next);
  }
  n->next = *nl;
  rcu_assign_pointer(*nl, n);
  return 0;
}
```
# 注销一个notifier_block


```
static int notifier_chain_unregister(struct notifier_block **nl, struct notifier_block *n)
{
  while ((*nl) != NULL) {
    if ((*nl) == n) {
      rcu_assign_pointer(*nl, n->next);
      return 0;
    }
    nl = &((*nl)->next);
  }
  return -ENOENT;
}
```
# 调用通知链上的函数
当需要通知特定的事件发生时，通过调用notifier_call_chain()函数来完成。注意，通知链上的回调函数是在调用notifier_call_chain()的上下文中执行的。


```
static int __kprobes notifier_call_chain(struct notifier_block **nl,
unsigned long val, void *v,
int nr_to_call, int *nr_calls)
{
   int ret = NOTIFY_DONE;
   struct notifier_block *nb, *next_nb;
   nb = rcu_dereference(*nl);
   while (nb && nr_to_call) {
      next_nb = rcu_dereference(nb->next);
      ret = nb->notifier_call(nb, val, v);
   if (nr_calls)
      (*nr_calls)++;
   if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK)
      break;
   nb = next_nb;
   nr_to_call--;
  }
  return ret;
}
```
参 数nl是通知链的名称，val表示事件类型，例如NETDEV_REGISTER，v用来指向通知链上的函数执行时需要用到的参数，一般不同的通知链，参 数类型也不一样，例如当通知一个网卡被注册时，v就指向net_device结构，nr_to_call 表示准备最多通知几个，-1表示整条链上的都调用，nr_calls 非空的话，返回通知了几个。
每个被执行的notifier_block回调函数一般返回下面几个可能的值：
NOTIFY_DONE：表示对相关的事件类型不关心
NOTIFY_OK：顺利执行
NOTIFY_BAD：执行有错
NOTIFY_STOP：停止执行后面的回调函数
NOTIFY_STOP_MASK：停止执行的掩码
关于这几个值的定义，请参考include/linux/notifier.h。
notifier_call_chain() 函数把最后一个被调的回调函数的返回值作为它的返回值。
# 内核网络代码中对通知链的使用
明白了通知链的原理后，我们看一下内核网络中使用的一些通知链
inetaddr_chain ipv4地址变动时的通知链
netdev_chain 网络设备状态变动时通知链
网络代码中对通知链的调用一般都有一个包装函数，例如对netdev_chain的注册就是由register_netdevice_notifier() 函数来完成


```
int register_netdevice_notifier(struct notifier_block *nb)
{
  struct net_device *dev;
  struct net_device *last;
  struct net *net;
  int err;
  rtnl_lock();
  err = raw_notifier_chain_register(&netdev_chain, nb);
  if (err)
    goto unlock;
  if (dev_boot_phase)
    goto unlock;
  for_each_net(net) {
    for_each_netdev(net, dev) {
        err = nb->notifier_call(nb, NETDEV_REGISTER, dev);
    err = notifier_to_errno(err);
    if (err)
      goto rollback;
    if (!(dev->flags & IFF_UP))
      continue;
   nb->notifier_call(nb, NETDEV_UP, dev);
  }
 }
  unlock:
  rtnl_unlock();
  return err;
rollback:
  last = dev;
  for_each_net(net) {
    for_each_netdev(net, dev) {
    if (dev == last)
      break;
    if (dev->flags & IFF_UP) {
      nb->notifier_call(nb, NETDEV_GOING_DOWN, dev);
      nb->notifier_call(nb, NETDEV_DOWN, dev);
    }
    nb->notifier_call(nb, NETDEV_UNREGISTER, dev);
  }
}
  raw_notifier_chain_unregister(&netdev_chain, nb);
  goto unlock;
}
```
这个函数看似复杂，其实就主要作两件事：
1） 把参数struct notifier_block *nb 注册到netdev_chain通知链上去
2） 系统中所有已经被注册过或激活的网络设备的事件都要被新增的这个通知的回调函数重新调用一遍，这样让设备更新到一个完整的状态。
# 一个例子
在 路由子系统初始化时，系统会调用ip_fib_init() 函数，ip_fib_init() 中会注册一个回调函数到netdev_chain通知链，这样当别的子系统通知netdev_chain上有特定的事件类型发生时，路由子系统的相应回调 函数就可以作一些反应。


```
void __init ip_fib_init(void)
{
... ...
  register_netdevice_notifier(&fib_netdev_notifier);
... ...
}
```
看一下fib_netdev_notifier的定义：


```
static struct notifier_block fib_netdev_notifier = {
.notifier_call =fib_netdev_event,
};
```
fib_netdev_notifier就是一个struct notifier_block，其中.priority默认初始化为０，.next由注册时设定。
再来大致看一下函数fib_netdev_event()做一些什么


```
static int fib_netdev_event(struct notifier_block *this, unsigned long event, void *ptr)
{
  struct net_device *dev = ptr;
  struct in_device *in_dev = __in_dev_get_rtnl(dev);
  if (event == NETDEV_UNREGISTER) {
    fib_disable_ip(dev, 2);
    return NOTIFY_DONE;
  }
  if (!in_dev)
    return NOTIFY_DONE;
  switch (event) {
    case NETDEV_UP:
      for_ifa(in_dev) {
        fib_add_ifaddr(ifa);
      } endfor_ifa(in_dev);
#ifdef CONFIG_IP_ROUTE_MULTIPATH
  fib_sync_up(dev);
#endif
    rt_cache_flush(-1);
    break;
  case NETDEV_DOWN:
    fib_disable_ip(dev, 0);
    break;
  case NETDEV_CHANGEMTU:
  case NETDEV_CHANGE:
    rt_cache_flush(0);
    break;
  }
  return NOTIFY_DONE;
}
```
