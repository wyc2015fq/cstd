# wifi详解（四） - DoubleLi - 博客园






# 1        IOCTL的调用逻辑

之所以要分析这个，是因为上层wpa_supplicant和WIFI驱动打交道的方式，多半是通过ioctl的方式进行的，所以看看它的调用逻辑（这里只列出其主要的调用逻辑）：



       上面便是用户ioctl调用的流程图，它最终分为两条线即有两种支持，选择那一条或两条都选（个人感觉最好选第2条线，因为它最后也是会调用到相应的函数的，而且还有其它更多的命令支持），从实际的代码来看，如果dev->netdev_ops

->ndo_do_ioctl被初始化了，那么它一定会被调用，是否被初始化，在前面选择对net结构变量的初始化方式中有讨论过。

       下面来具体看看该调用流程，首先说明下，上面的流程主要实现在kernel/net/wireless/wext_core.c文件中，这是wireless的协议层实现，恰好我们在wpa_supplicant中通常选择的驱动类型也是wext，它的入口函数是wext_ioctl_dispatch：

/* entry point from dev ioctl*/

**static int wext_ioctl_dispatch(struct net *net, struct ifreq*ifr,**

**                              unsigned int cmd, struct iw_request_info *info,**

**                              wext_ioctl_func standard,**

                               wext_ioctl_funcprivate)

{

        int ret = wext_permission_check(cmd);



        if (ret)

                return ret;



        dev_load(net, ifr->ifr_name);

        rtnl_lock();

**ret = wireless_process_ioctl(net, ifr, cmd, info, standard,private);**

        rtnl_unlock();



        return ret;

}



它其实就是wireless_process_ioctl的封装函数，除了进行许可权限的确认，没有做什么其它内容，这里有standard和private两个函数指针的传递，其实就是两个回调函数，在后面会用到，它是由wext_handle_ioctl函数传递过来的：

int wext_handle_ioctl(structnet *net, struct ifreq *ifr, unsigned int cmd,

                      void __user *arg)

{

        struct iw_request_info info = { .cmd =cmd, .flags = 0 };

        int ret;



**ret = wext_ioctl_dispatch(net, ifr, cmd, &info,**

**                                 ioctl_standard_call,**

**                                 ioctl_private_call);     //这两个回调函数的定义之后再讨论，这里暂不理论**

        if (ret >= 0 &&

            IW_IS_GET(cmd) &&

            copy_to_user(arg, ifr, sizeof(structiwreq)))

                return -EFAULT;



        return ret;

}

实际上传递的就是ioctl_standard_call和ioctl_private_call两个函数，在看看wireless_process_ioctl函数，这个函数很重要，下面做重点分析：

static intwireless_process_ioctl(struct net *net, struct ifreq *ifr,

                                  unsigned int cmd,

                                  structiw_request_info *info,

                                 wext_ioctl_func standard,

                                 wext_ioctl_func private)

{

        struct iwreq *iwr = (struct iwreq *)ifr;

        struct net_device *dev;

        iw_handler      handler;



        /* Permissions are already checked indev_ioctl() before calling us.

         * The copy_to/from_user() of ifr isalso dealt with in there */



        /* Make sure the device exist */

**if ((dev = __dev_get_by_name(net, ifr->ifr_name)) == NULL)             //通过网络接口名获取net_device设备**

                return -ENODEV;



        /* A bunch of special cases, then thegeneric case...

         * Note that 'cmd' is already filteredin dev_ioctl() with

         * (cmd >= SIOCIWFIRST &&cmd <= SIOCIWLAST) */

**if (cmd == SIOCGIWSTATS)**

**                returnstandard(dev, iwr, cmd, info,**

**                               &iw_handler_get_iwstats);   //如果是状态查询命令，调用该函数（回调函数中的一个）**



#ifdef CONFIG_WEXT_PRIV

**if (cmd == SIOCGIWPRIV && dev->wireless_handlers)**

**                returnstandard(dev, iwr, cmd, info,**

**                               iw_handler_get_private);      //如果是专有命令，调用回调函数，同上**

#endif



        /* Basic check */

        if (!netif_device_present(dev))

                return -ENODEV;



        /* New driver API : try to find thehandler */

**handler = get_handler(dev, cmd);              //根据cmd参数，从dev成员中查询相应的处理函数**

        if (handler) {

                /* Standard and private are notthe same */

                if (cmd < SIOCIWFIRSTPRIV)

**return standard(dev, iwr, cmd, info, handler);  //调用相应命令的处理函数**

                else if (private)

**return private(dev, iwr, cmd, info, handler);     //同上**

        }

        /* Old driver API : call driver ioctlhandler */

        if(dev->netdev_ops->ndo_do_ioctl)

**return dev->netdev_ops->ndo_do_ioctl(dev,ifr, cmd);     //如果被设置就调用该函数**

        return -EOPNOTSUPP;

}

该函数的大意是，通过网络接口名称获得一个网络设备，然后根据命令的类型调用相应的处理函数，特别的是当dev->netdev_ops->ndo_do_ioctl或dev->wireless_handlers被设置时，则会查找执行对应的处理函数。Get_handle函数用于查询处理函数使用：

static iw_handlerget_handler(struct net_device *dev, unsigned int cmd)

{

        /* Don't "optimise" thefollowing variable, it will crash */

        unsigned int    index;          /* *MUST* be unsigned */

**const struct iw_handler_def *handlers = NULL;**



#ifdef CONFIG_CFG80211_WEXT

        if (dev->ieee80211_ptr &&dev->ieee80211_ptr->wiphy)

**handlers =dev->ieee80211_ptr->wiphy->wext;  //初始化默认的处理函数**

#endif

#ifdef CONFIG_WIRELESS_EXT

        if (dev->wireless_handlers)

** handlers= dev->wireless_handlers;    //这里的dev->wireless_handlers在net初始化时被作为扩张功能选择性的设置，前面有提到过**

#endif



        if (!handlers)

                return NULL;



        /* Try as a standard command */

        index = IW_IOCTL_IDX(cmd);

        if (index <handlers->num_standard)

** returnhandlers->standard[index];     //返回对应的标准函数**



#ifdef CONFIG_WEXT_PRIV

        /* Try as a private command */

        index = cmd - SIOCIWFIRSTPRIV;

        if (index <handlers->num_private)

**return handlers->private[index];        //返回对应的专有函数**

#endif



        /* Not found */

        return NULL;

}

那么这个dev->wireless_handlers究竟是什么，这里来揭开它的神秘面纱，在bcm4329源码src/wl/sys/wl_iw.c中，有它的定义：

static const iw_handler **wl_iw_handler**[]=

{

        (iw_handler) wl_iw_config_commit,

        (iw_handler) wl_iw_get_name,

        (iw_handler) NULL,

......

}



static const iw_handler **wl_iw_priv_handler**[]= {

        NULL,

        (iw_handler)wl_iw_set_active_scan,

        NULL,

        (iw_handler)wl_iw_get_rssi,

......

}



const struct iw_handler_def  **wl_iw_handler_def** =

{

        .num_standard =ARRAYSIZE(wl_iw_handler),

**.standard = (iw_handler *) wl_iw_handler,**

        .num_private = ARRAYSIZE(wl_iw_priv_handler),

        .num_private_args =ARRAY_SIZE(wl_iw_priv_args),

**.private = (iw_handler *)wl_iw_priv_handler,**

        .private_args = (void *)wl_iw_priv_args,



#if WIRELESS_EXT >= 19

        get_wireless_stats:dhd_get_wireless_stats,

#endif

        };

#endif

在net初始化的时候，这里把dev->wireless_handlers和dev->netdev_ops的初始化代码再贴出来：

int

dhd_net_attach(dhd_pub_t*dhdp, int ifidx)

{

……

#if (LINUX_VERSION_CODE <KERNEL_VERSION(2, 6, 31))

        ASSERT(!net->open);

**net->get_stats = dhd_get_stats;**

**        net->do_ioctl =dhd_ioctl_entry;**

**       net->hard_start_xmit = dhd_start_xmit;**

**       net->set_mac_address = dhd_set_mac_address;**

**       net->set_multicast_list = dhd_set_multicast_list;**

**        net->open =net->stop = NULL;**

#else

        ASSERT(!net->netdev_ops);

**net->netdev_ops = &dhd_ops_virt;**

#endif



……

#if WIRELESS_EXT > 12

**net->wireless_handlers = (struct iw_handler_def*)&wl_iw_handler_def;   //这里的初始化工作很重要，之后的ioctl流程会涉及到对它的使用**

#endif /* WIRELESS_EXT > 12*/





……

}

看到这里，应该可以明白相应的命令最终会在wl_iw.c中被执行，这些处理函数也是在该文件中实现。上面已经获取了命令的处理函数，那么它是如何被执行的呢？这里wireless_process_ioctl里有standard和private的回调函数的调用：

static intioctl_standard_call(struct net_device *     dev,

                               structiwreq             *iwr,

                               unsigned int             cmd,

                               structiw_request_info   *info,

                               iw_handler               handler)

{

        const struct iw_ioctl_description*     descr;

        int                                     ret = -EINVAL;



        /* Get the description of the IOCTL */

        if (IW_IOCTL_IDX(cmd) >=standard_ioctl_num)

                return -EOPNOTSUPP;

        descr =&(standard_ioctl[IW_IOCTL_IDX(cmd)]);



        /* Check if we have a pointer to userspace data or not */

        if (descr->header_type !=IW_HEADER_TYPE_POINT) {



                /* No extra arguments. Trivialto handle */

**ret = handler(dev, info, &(iwr->u),NULL);**



                /* Generate an event to notifylisteners of the change */

                if ((descr->flags &IW_DESCR_FLAG_EVENT) &&

                   ((ret == 0) || (ret ==-EIWCOMMIT)))

**wireless_send_event(dev, cmd, &(iwr->u),NULL);**

        } else {

                ret =ioctl_standard_iw_point(&iwr->u.data, cmd, descr,

                                             handler, dev, info);

        }



        /* Call commit handler if needed anddefined */

        if (ret == -EIWCOMMIT)

                ret =call_commit_handler(dev);



        /* Here, we will generate theappropriate event if needed */



        return ret;

}

回调函数中对传递过来的handler函数指针进行呼叫，对应的处理函数就会被执行，当然用户传送的命令还不止这些，所以才会有net->netdev_ops的存在的必要性。下面来就来看看执行到：

**return dev->netdev_ops->ndo_do_ioctl(dev, ifr, cmd);     //wireless_process_ioctl的最后一句**

就会调用dhd_ioctl函数，这是wlan驱动对ioctl调用的处理函数，就是根据用户传递过来的cmd，给它找一个最合适最合理的“归宿”。

static int

dhd_ioctl_entry(structnet_device *net, struct ifreq *ifr, int cmd)

{

......#ifdefined(CONFIG_WIRELESS_EXT)

        /* linux wireless extensions */

        if ((cmd >= SIOCIWFIRST) &&(cmd <= SIOCIWLAST)) {

                /* may recurse, do NOT lock */

**ret = wl_iw_ioctl(net, ifr, cmd);**

               DHD_OS_WAKE_UNLOCK(&dhd->pub);

                return ret;

        }

#endif /*defined(CONFIG_WIRELESS_EXT) */



#if LINUX_VERSION_CODE >KERNEL_VERSION(2, 4, 2)

        if (cmd == SIOCETHTOOL) {

**ret = dhd_ethtool(dhd,(void*)ifr->ifr_data);**

               DHD_OS_WAKE_UNLOCK(&dhd->pub);

                return ret;

        }

#endif /* LINUX_VERSION_CODE> KERNEL_VERSION(2, 4, 2) */



        if (cmd == SIOCDEVPRIVATE+1) {

**ret = wl_android_priv_cmd(net, ifr, cmd);**

                dhd_check_hang(net,&dhd->pub, ret);

                DHD_OS_WAKE_UNLOCK(&dhd->pub);

                return ret;

        }



        if (cmd != SIOCDEVPRIVATE) {

               DHD_OS_WAKE_UNLOCK(&dhd->pub);

                return -EOPNOTSUPP;

        }



        memset(&ioc, 0, sizeof(ioc));

......

**bcmerror = dhd_wl_ioctl(&dhd->pub, ifidx, (wl_ioctl_t*)&ioc, buf, buflen);**

......

}

限于篇幅，该函数处理过程不再详述，大致的命令处理方法相似，wl_iw.c中的系列处理函数只是其中的一部分，wl_android中和dhd_linux.c也有相应的处理函数。

# 2        数据的传送

## 2.1       数据传送过程简述

传送指的是通过一个网络连接发送一个报文的行为.。无论何时内核需要传送一个数据报文, 它都必须调用驱动的 hard_start_xmit 方法将数据放在外出队列上。

每个内核处理的报文都包含在一个 socket缓存结构( 结构 sk_buff )里, 定义见<linux/skbuff.h>。这个结构从 Unix 抽象中得名, 用来代表一个网络连接socket.。对于接口来说, 一个 socket 缓存只是一个报文。

传给 hard_start_xmit 的socket 缓存包含物理报文, 它应当出现在媒介上, 以传输层的头部结束。接口不需要修改要传送的数据.。skb->data指向要传送的报文，skb->len 是以字节计的长度。传送下来的sk_buff中的数据已经包含硬件需要的帧头（这是通过hard_header函数将传递进入的信息，组织成设备特有的硬件头），所以在发送方法里不需要再填充硬件帧头，数据可以直接提 交给硬件发送。sk_buff是被锁住的(locked)，确保其他程序不会存取它。

所有的网络设备驱动程序都必须有这个发送方法。在系统调用驱动程序的xmit时，发送的数据放在一个sk_buff 结构中。一般的驱动程序把数据传给硬件发出去。也有一些特殊的设备比如loopback把数据组成一个接收数据再回送给系统，或者dummy设备直接丢弃 数据。如果发送成功，hard_start_xmit方法里释放sk_buff，返回0(发送成功)。

## 2.2      Bcm4329芯片wlan驱动数据传送

当上层传送过来报文，调用hard_start_xmit函数（该方法主用于初始化数据包的传输），该函数主要用于转换sk_buf，将其组织成pktbuf数据格式，然后调用dhd_sendpkt函数将pktbuf通过dhd bus发送到wifi芯片，最后硬件wifi芯片将报文radio发送到网络上。

int

**dhd_start_xmit**(struct sk_buff *skb,struct net_device *net)

{

......

        /* Convert to packet */

        if (!(pktbuf =PKTFRMNATIVE(dhd->pub.osh, skb))) {

                DHD_ERROR(("%s:PKTFRMNATIVE failed\n",

                          dhd_ifname(&dhd->pub, ifidx)));

                dev_kfree_skb_any(skb);        //转换成功，释放skb，在通常处理中，会在中断中做该操作

                ret = -ENOMEM;

                goto done;

        }

#ifdef WLMEDIA_HTSF

        if (htsfdlystat_sz &&PKTLEN(dhd->pub.osh, pktbuf) >= ETHER_ADDR_LEN) {

                uint8 *pktdata = (uint8*)PKTDATA(dhd->pub.osh, pktbuf);

                struct ether_header *eh =(struct ether_header *)pktdata;



                if(!ETHER_ISMULTI(eh->ether_dhost) &&

                       (ntoh16(eh->ether_type) == ETHER_TYPE_IP)) {

                        eh->ether_type =hton16(ETHER_TYPE_BRCM_PKTDLYSTATS);

                }

        }

#endif



        ret = **dhd_sendpkt**(&dhd->pub, ifidx,pktbuf);     //发送pktbuf

......

}



int

**dhd_sendpkt**(dhd_pub_t *dhdp, intifidx, void *pktbuf)

{

......

#ifdef PROP_TXSTATUS

        if (dhdp->wlfc_state &&((athost_wl_status_info_t*)dhdp->wlfc_state)->proptxstatus_mode

                        != WLFC_FCMODE_NONE) {

                dhd_os_wlfc_block(dhdp);

                ret =dhd_wlfc_enque_sendq(dhdp->wlfc_state, DHD_PKTTAG_FIFO(PKTTAG(pktbuf)),

                        pktbuf);

               dhd_wlfc_commit_packets(dhdp->wlfc_state,  (f_commitpkt_t)dhd_bus_txdata,

                        dhdp->bus);

                if(((athost_wl_status_info_t*)dhdp->wlfc_state)->toggle_host_if) {

                        ((athost_wl_status_info_t*)dhdp->wlfc_state)->toggle_host_if= 0;

                }

                dhd_os_wlfc_unblock(dhdp);

        }

        else

                /* non-proptxstatus way */

        ret = **dhd_bus_txdata(dhdp->bus, pktbuf);         //在SDIO总线上传输**

#else

        ret = **dhd_bus_txdata(dhdp->bus, pktbuf);**

#endif /* PROP_TXST

......

}

传输结束后，会产生一个中断，即传输结束中断，一般的网络驱动程序都会有这个中断的注册，但还有一种轮询方式，这在后面的数据的接收部分会有介绍，而sk_buf就在这个中断处理函数中被释放。

但是，实际情况还是比较复杂，当硬件偶尔出现问题不能响应驱动时，就不能完成驱动的功能。在网络接口发送数据时也会发生一些不可预知的不响应动作，比如当网络介质因阻塞造成的冲突，而使发送报文的动作不能得到响应，但硬件通常不需要做此类的检测，需要驱动用软件的方法来实现，这就是超时传输机制。

## 2.3       传输超时

与真实硬件打交道的大部分驱动不得不预备处理硬件偶尔不能响应。接口可能忘记它们在做什么，或者系统可能丢失中断。

许多驱动通过设置定时器来处理这个问题; 如果在定时器到期时操作还没结束, 有什么不对了，网络系统, 本质上是一个复杂的由大量定时器控制的状态机的组合体。因此, 网络代码是一个合适的位置来检测发送超时, 作为它正常操作的一部分。网络驱动不需要担心自己去检测这样的问题，相反, 它们只需要设置一个超时值, 在net_device 结构的 watchdog_timeo 成员。这个超时值, 以 jiffy 计, 应当足够长以容纳正常的发送延迟(例如网络媒介拥塞引起的冲突)。

如果当前系统时间超过设备的 trans_start 时间至少 time-out 值, 网络层最终调用驱动的 tx_timeout方法。这个方法的工作是是进行清除问题需要的工作并且保证任何已经开始的发送正确地完成。特别地, 驱动没有丢失追踪任何网络代码委托给它的 socket 缓存。

当发生传送超时, 驱动必须在接口统计量中标记这个错误, 并安排设备被复位到一个干净的能发送新报文的状态，一般驱动会调用netif_wake_queue函数重新启动传输队列。

# 3        数据的接收

## 3.1       数据接收的方式和过程

从网络上接收报文比发送它要难一些，因为必须分配一个 sk_buff 并从一个原子性上下文中递交给上层。网络驱动可以实现 2 种报文接收的模式：中断驱动和查询，大部分驱动采用中断驱动技术。

大部分硬件接口通过一个中断处理来控制，硬件中断处理器来发出 2 种可能的信号：一个新报文到了或者一个外出报文的发送完成了。网络接口也能够产生中断来指示错误, 例如状态改变, 等等。

通常的中断过程能够告知新报文到达中断和发送完成通知的区别，通过检查物理设备中的状态寄存器，来判断是那一种中断，对于发送完成中断更新状态信息，释放skb内存。而对于接收数据中断，从数据队列中抽取一包数据，并把它传递给接收函数。

注意：这里的对设备数据的操作是在锁得保护下完成的，做一最后还要释放掉锁。

## 3.2       选择哪种接收模式

那么，既然后两种方式来处理网络接口发来的数据，选择那一种呢？一般认为中断是比较好的一种方式，不过，如果接口接收数据太频繁，甚至一秒中接收上千包数据，那么系统的中断次数就非常多，这回严重影响系统的性能。所以，在频繁接收数据的情况下，也可以考虑使用轮询的方式。

这样，为了提高linux在宽带系统上的性能，网络子系统开发者创建了一种基于轮询方式的接口NAPI，它虽然在很多情况下，并不被看好，但处理高流量的高速接口时，用这种NAPI轮询技术处理到达的每一个数据包就足够了，前提是网络设备必须能支持这种模式，就是说一个网络接口必须能保存多个数据包，而且中断能够禁止中断并能在传输和其他事件上打开中断。

## 3.3      Bcm4329芯片wlan驱动数据传送

在bcm4329芯片Wlan驱动中，在函数dhd_attach被调用时，会初始化一个内核线程或一个tasklet中断的下半部。其实这两种方式就是之前的中断和轮询方式的实现版，如果使用轮询，驱动初始化一个内核线程dhd_dpc_thread轮询网络接口接收的数据，中断下半部是中断处理程序的延续，用于处理比较复杂费时的操作，这样就能早点从中断中解放出来，防止拖累系统的性能。

下面来看看这两种方式的初始化（在dhd_attach.c）：

/* Set up the bottom halfhandler */

        if (dhd_dpc_prio >= 0) {

                /* Initialize DPC thread */

**PROC_START(dhd_dpc_thread, dhd,&dhd->thr_dpc_ctl, 0);**

        } else {

                /*  use tasklet for dpc */

**tasklet_init(&dhd->tasklet, dhd_dpc,(ulong)dhd);**

                dhd->thr_dpc_ctl.thr_pid =-1;

        }

首先来看看轮询方式的过程：

dhd_dpc_thread(void *data)

{

        tsk_ctl_t *tsk = (tsk_ctl_t *)data;

        dhd_info_t *dhd = (dhd_info_t*)tsk->parent;



        /* This thread doesn't need anyuser-level access,

         * so get rid of all our resources

         */

        if (dhd_dpc_prio > 0)

        {

                struct sched_param param;

                param.sched_priority =(dhd_dpc_prio < MAX_RT_PRIO)?dhd_dpc_prio:(MAX_RT_PRIO-1);

                setScheduler(current, SCHED_FIFO,¶m);

        }



        DAEMONIZE("dhd_dpc");

        /* DHD_OS_WAKE_LOCK is called indhd_sched_dpc[dhd_linux.c] down below  */



        /* signal: thread has started */

        complete(&tsk->completed);



        /* Run until signal received */

        while (1) {

                if (down_interruptible(&tsk->sema)== 0) {



                       SMP_RD_BARRIER_DEPENDS();

                        if (tsk->terminated){

                                break;

                        }



                        /* Call bus dpc unlessit indicated down (then clean stop) */

                        if (**dhd->pub.busstate!= DHD_BUS_DOWN**) {

** if (dhd_bus_dpc(dhd->pub.bus)) {**

                                        up(&tsk->sema);

                                }

                                else {

                                       DHD_OS_WAKE_UNLOCK(&dhd->pub);

                                }

                        } else {

                                if (dhd->pub.up)

                                       dhd_bus_stop(dhd->pub.bus, TRUE);

                               DHD_OS_WAKE_UNLOCK(&dhd->pub);

                        }

                }

                else

                        break;

        }



       complete_and_exit(&tsk->completed, 0);

}

这里是一个永真循环，直到接收到终止信号才停止，该线程就是通过不断调用dhd_bus_dpc函数调用实现轮询的，它的调用逻辑如下所示：



上面是dhd_dpc_thread的调用逻辑，最后通过netif_rx将数据提交到上层协议，那么，还有一种中断方式时如何实现的呢？上面只看到驱动初始化了一个tasklet，一个中断下半部的实例。其实在dhdsdh_probe函数中已经注册了这个中断处理函数：

static void *

**dhdsdio_probe**(uint16 venid, uint16devid, uint16 bus_no, uint16 slot,

        uint16 func, uint bustype, void*regsva, osl_t * osh, void *sdh)

{

......

if (bus->intr) {

                /* Register interrupt callback,but mask it (not operational yet). */

                DHD_INTR(("%s: disableSDIO interrupts (not interested yet)\n", __FUNCTION__));

                bcmsdh_intr_disable(sdh);      //首先禁止SDIO中断，再注册中断

** if ((ret= bcmsdh_intr_reg(sdh, dhdsdio_isr, bus)) != 0) {**

                        DHD_ERROR(("%s:FAILED: bcmsdh_intr_reg returned %d\n",

                                  __FUNCTION__, ret));

                        goto fail;

                }

                DHD_INTR(("%s: registeredSDIO interrupt function ok\n", __FUNCTION__));

        } else {

                DHD_INFO(("%s: SDIOinterrupt function is NOT registered due to polling mode\n",

                           __FUNCTION__));

        }

......

}

看看Dhdsdio_isr这个中断处理函数干了什么？在函数的最后部分是：

#if defined(SDIO_ISR_THREAD)

        DHD_TRACE(("Calling dhdsdio_dpc()from %s\n", __FUNCTION__));

        DHD_OS_WAKE_LOCK(bus->dhd);

**while (dhdsdio_dpc(bus));**

        DHD_OS_WAKE_UNLOCK(bus->dhd);

#else

        bus->dpc_sched = TRUE;

**dhd_sched_dpc(bus->dhd);**

#endif

Dhd_sched_dpc函数在最后被调用（上面的while循环调用dhdsdio_dpc，其实和下面的这个调用函数最后的作用是一样的，就不予详述），这个函数的代码如下：

void

**dhd_sched_dpc**(dhd_pub_t *dhdp)

{

        dhd_info_t *dhd = (dhd_info_t*)dhdp->info;



        DHD_OS_WAKE_LOCK(dhdp);

#ifdef DHDTHREAD

        if (dhd->thr_dpc_ctl.thr_pid >=0) {

               up(&dhd->thr_dpc_ctl.sema);

                return;

        }

#endif /* DHDTHREAD */



**tasklet_schedule(&dhd->tasklet);**

}

就是触发一个中断的下半部tasklet，让cpu选择在一个合适的时候调用dhd_dpc函数，这个函数会调用dhd_bus_dpc，然后进入上面流程图的调用逻辑。

详细的数据处理过程不详细叙述，可以参考源码来具体分析。



# 4        电源管理相关的调用逻辑

电源管理始终是手机等移动设备最重要的一个功能，尤其对于Android这种智能手机或者说手机电脑化的设备，电源管理更显得十分重要。

Linux一直在传统的PC和服务器市场上有很好的应用，也有了比较好的电源管理框架，但是对于智能手机等嵌入式设备来说，Linux标准的电源管理就显得不是很适用了，有许多需要改进的地方。Android在这方面做了一些比较好的尝试，添加了一些新的特性，包括wake_lock，early_supend等。这里对wake_lock不做介绍，只介绍WIFI模块在系统将要或正在进入休眠的一些动作，感兴趣的话可以自己查阅android的电源管理相关文章。

在介绍实质内容之前，先来看看android的电源管理的实现基础：Linux系统的电源管理Suspend框架跟Linux系统的驱动模型（Linux DriverModel）是相关的，也是基于Linux的驱动模型来实现的，下面的图描述了Linux系统电源管理的Suspend系统框架，Linux的Suspend系统分为两部分，一部分是平台无关的核心层，另一个是平台相关的平台层。操作接口都在平台无关的核心层里了，平台相关部分会使用Suspend API将自己的操作函数注册进Suspend核心层里。



根据Linux系统驱动模型，Device结构描述了一个设备，device_driver是设备的驱动，而class、type和bus分别描述了设备所属的类别、类型和总线。而设备的电源管理也根据此模型分为class级的、type级的、bus级的和驱动级的。如果一个设备的class或者bus确切的知道如何管理一个设备的电源的时候，驱动级别的suspend/resume就可以为空了。这极大的提高了电源管理的高效性和灵活性。

对于android平台上整个系统是如何一步一步进入休眠的，我这里不做详细介绍，只作出它的大致流程图：



此流程图显示了系统的休眠的全过程，对WIFI模块来说，我们主要关注early_suspend和suspend以及相应的唤醒过程。当系统屏幕超时或用户（亮屏时）按power键，系统进入休眠流程（这里不讨论可能的中途退出休眠的其它因素），即在没有进程持有wakelock情况下，首先进入early_suspend流程。

Early_suspend流程的实现基础是：android电源管理系统中定义了一个early_suspend结构链表，里面存放了所有系统中注册了的early_suspend实例，即如果一个模块要在系统进入early_suspend状态有所动作，就必须注册一个early_suspend实例。在WIFI驱动模块中，当驱动流程走到dhd_attach函数时，有相应的early_suspend注册代码：

Path: dhd/sys/dhd_linux.c

dhd_pub_t *

dhd_attach(osl_t *osh, structdhd_bus *bus, uint bus_hdrlen)

{

......

#ifdef CONFIG_HAS_EARLYSUSPEND

        dhd->early_suspend.level =EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 20;

**dhd->early_suspend.suspend = dhd_early_suspend;**

**       dhd->early_suspend.resume = dhd_late_resume;**

**register_early_suspend(&dhd->early_suspend);**

        dhd_state |= DHD_ATTACH_STATE_EARLYSUSPEND_DONE;

#endif

......

}

红色区域初始化了dhd结构的两个early_suspend函数，并将其注册到电源管理系统。early_suspend的休眠函数的代码如下：

static void **dhd_early_suspend**(structearly_suspend *h)

{

        struct dhd_info *dhd = container_of(h,struct dhd_info, early_suspend);



        DHD_TRACE(("%s: enter\n",__FUNCTION__));



        if (dhd)

** dhd_suspend_resume_helper(dhd, 1);**

}

调用dhd_suspend_resume_helper函数，别看函数名中有resume单词，其实early_suspend和late_resume都是通过这个函数实现功能的：

static void **dhd_suspend_resume_helper**(structdhd_info *dhd, int val)

{

        dhd_pub_t *dhdp = &dhd->pub;



        DHD_OS_WAKE_LOCK(dhdp);

        /* Set flag when early suspend wascalled */

        dhdp->in_suspend = val;

        if ((!dhdp->suspend_disable_flag)&& (dhd_check_ap_wfd_mode_set(dhdp) == FALSE))

**dhd_set_suspend(val, dhdp);**

        DHD_OS_WAKE_UNLOCK(dhdp);

}



**#if defined(CONFIG_HAS_EARLYSUSPEND)          //看这里，如果系统配置了EARLYSUSPEND       ，则系统会使用这部分代码，其实early_suspend是android对linux内核的电源管理的优化，所以如果你使用的是android平台，一定要配置该选项**

static int **dhd_set_suspend**(intvalue, dhd_pub_t *dhd)

{

......

        if (dhd && dhd->up) {

** if(value && dhd->in_suspend) {        //early_suspend**



                                /* Kernelsuspended */

                               DHD_ERROR(("%s: force extra Suspend setting \n",__FUNCTION__));



**dhd_wl_ioctl_cmd(dhd,WLC_SET_PM, (char *)&power_mode,**

**                                                sizeof(power_mode), TRUE, 0);**



                                /* Enablepacket filter, only allow unicast packet to send up */

                               dhd_set_packet_filter(1, dhd);



                                /* If DTIM skipis set up as default, force it to wake

                                 * each thirdDTIM for better power savings.  Note that

                                 * one sideeffect is a chance to miss BC/MC packet.

                                 */

                                bcn_li_dtim =dhd_get_dtim_skip(dhd);

                                bcm_mkiovar("bcn_li_dtim",(char *)&bcn_li_dtim,

                                        4,iovbuf, sizeof(iovbuf));

**dhd_wl_ioctl_cmd(dhd,WLC_SET_VAR, iovbuf, sizeof(iovbuf), TRUE, 0);**



                                /* Disable firmwareroaming during suspend */

                               bcm_mkiovar("roam_off", (char *)&roamvar, 4,

                                        iovbuf,sizeof(iovbuf));

                               dhd_wl_ioctl_cmd(dhd, WLC_SET_VAR, iovbuf, sizeof(iovbuf), TRUE, 0);

** } else {          //late_resume**



                                /* Kernelresumed  */

                               DHD_TRACE(("%s: Remove extra suspend setting \n",__FUNCTION__));



                                power_mode =PM_FAST;

                               dhd_wl_ioctl_cmd(dhd, WLC_SET_PM, (char *)&power_mode,

                                                sizeof(power_mode), TRUE, 0);



                                /* disable pktfilter */

                                dhd_set_packet_filter(0,dhd);



                                /* restorepre-suspend setting for dtim_skip */

                               bcm_mkiovar("bcn_li_dtim", (char *)&dhd->dtim_skip,

                                        4, iovbuf, sizeof(iovbuf));



**dhd_wl_ioctl_cmd(dhd,WLC_SET_VAR, iovbuf, sizeof(iovbuf), TRUE, 0);**

                                roamvar =dhd_roam_disable;

                               bcm_mkiovar("roam_off", (char *)&roamvar, 4, iovbuf,

                                       sizeof(iovbuf));

**dhd_wl_ioctl_cmd(dhd,WLC_SET_VAR, iovbuf, sizeof(iovbuf), TRUE, 0);**

                        }

        }



  return 0;

}

#endif

    具体做什么内容，可以不用过多理会，一般只是会对该模块做些最基本的低功耗设置，其实真正的低功耗设置时在suspend中完成的。一般的模块也不需要注册early_suspend实例，但是背光灯，键盘LED和LCD屏是一定要在注册的。

       Early_suspend注册成功后，会被挂接到电源管理系统中的一个链表上，当系统进入early_suspend流程时，会逐一调用该链表中的每一个实例的early_suspend回调函数，使设备进入相应的状态。在完成early_suspend流程后，系统检测wake_lock（也是被链表管理，其实不止一个），如果没有进程持有wake_lock包括main_wake_lock，系统进入suspend流程。

       同样，suspend流程的实施也是需要系统支持的，需要实现电源管理的模块需要实现suspend和resume两个函数，并注册到系统中，对于WIFI设备的电源管理函数的注册在调用wifi_add_dev函数时被注册：

Path：wl/sys/wl_android.c

static struct platform_driverwifi_device = {

        .probe          = wifi_probe,

**.remove         =wifi_remove,**

**        .suspend        = wifi_suspend,**

        .resume         = wifi_resume,

        .driver         = {

        .name  = "bcmdhd_wlan",

        }

};



static struct platform_driverwifi_device_legacy = {

        .probe          = wifi_probe,

        .remove         = wifi_remove,

**.suspend        =wifi_suspend,**

**        .resume         = wifi_resume,**

        .driver         = {

        .name  = "bcm4329_wlan",

        }

};



static int wifi_add_dev(void)

{

        DHD_TRACE(("## Callingplatform_driver_register\n"));

** platform_driver_register(&wifi_device);**

**       platform_driver_register(&wifi_device_legacy);**

        return 0;

}

    Wifi_suspend和wifi_resume随着wifi_device设备的注册而注册，这样当系统进入suspend流程后，就可以调用每个设备上的电源管理函数来使设备进入休眠状态了。

       Wifi设备的休眠：

static int **wifi_suspend**(structplatform_device *pdev, pm_message_t state)

{

        DHD_TRACE(("##> %s\n",__FUNCTION__));

#if (LINUX_VERSION_CODE <=KERNEL_VERSION(2, 6, 39)) && defined(OOB_INTR_ONLY)

**bcmsdh_oob_intr_set(0);**

#endif

        return 0;

}



static int **wifi_resume**(structplatform_device *pdev)

{

        DHD_TRACE(("##> %s\n",__FUNCTION__));

#if (LINUX_VERSION_CODE <=KERNEL_VERSION(2, 6, 39)) && defined(OOB_INTR_ONLY)

        if (dhd_os_check_if_up(bcmsdh_get_drvdata()))

** bcmsdh_oob_intr_set(1);**

#endif

        return 0;

}

       上面的两个电源管理函数都调用bcmsdh_oob_intr_set函数，但是传递的参数不同，在wifi_suspend函数中传递0，表示禁止wifi设备对应的oob中断，而wifi_resume的作用恰恰相反。

       Bcmsdh_oob_intr_set函数的定义如下：

PATH：bcmsdio/sys/bcmsdh_linux.c

**#if defined(OOB_INTR_ONLY)   //该中断的使用需要配置**

**void bcmsdh_oob_intr_set(bool enable)**

{

        static bool curstate = 1;

        unsigned long flags;



       spin_lock_irqsave(&sdhcinfo->irq_lock, flags);

**if (curstate != enable) {**

**                if (enable)**

**                       enable_irq(sdhcinfo->oob_irq);**

**                else**

**                       disable_irq_nosync(sdhcinfo->oob_irq);**

**                curstate =enable;**

**        }**

       spin_unlock_irqrestore(&sdhcinfo->irq_lock, flags);

}



此中断是在打开wifi网络设备的时候被注册的，流程如下：

static int

**dhd_open(struct net_device *net)**

{

......

               if (dhd->pub.busstate !=DHD_BUS_DATA) {



                        /* try to bring up bus*/

**if ((ret = dhd_bus_start(&dhd->pub)) !=0) {**

                               DHD_ERROR(("%s: failed with code %d\n", __FUNCTION__, ret));

                                ret = -1;

                                goto exit;

                        }



                }

......

}





**dhd_bus_start(dhd_pub_t *dhdp)**

{

......

#if defined(OOB_INTR_ONLY)

        /* Host registration for OOB interrupt*/

** if(bcmsdh_register_oob_intr(dhdp)) {**

......

}

在系统进入suspend状态后，wifi设备进入禁止中断状态，不再接收处理网络发来的数据，系统进入sleep状态，当然还有很多cpu在suspend之后进入sleep状态，但此时系统clock中断并没有被禁止，而且pmu还正常工作，以期对power键和充电器连接的检测。









