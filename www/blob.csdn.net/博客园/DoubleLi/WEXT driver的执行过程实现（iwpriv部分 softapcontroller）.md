# WEXT driver的执行过程实现（iwpriv部分/softapcontroller） - DoubleLi - 博客园






之前在看wifi driver源代码时一直有一个疑惑就是net dev的wireless_handlers中（WEXT类型的接口）提供两个iw_handler接口,怎么知道上层是调用的是private中的函数还是standard中的SIOCSIWPRIV接口和SIOCGIWPRIV接口。

问Wifi的FAE，人家也不清楚，后来没办法只好在源代码中找，现在终于有点头绪与大家分享一下。

android 中有个system/netd/目录，在netd下有个softapController.cpp文件实际上该文件实现了程序iwpriv的功能，那么这个程序是干嘛的呢？嘿嘿从名字就可看出啦是给softap下control 命令的。至于这些命令从哪里来，待后续有机会再与大家分享netd部分时再讨论。



分析代码的从入口函数开始，构造函数

SoftapController::SoftapController()

mSock = socket(AF_INET,SOCK_DGRAM, 0); //socket调用，这个我们之前有分析过，这个mSock很重要，这就是socket关联的文件描述符接口，上层通过该接口与内核沟通。

其它函数除了getPrivFuncNum外基本都是开给上层的函数接口，用于和底层沟通。我们就分析打开softap执行的第一个函数startDriver

fnum = getPrivFuncNum(iface,"START");//函数用START作为参数，并返回该函数在driver中private中的第几个

ret = ioctl(mSock, fnum, &wrq);//执行指定的（”START”所对应的）程序



getPrivFuncNum函数

**strncpy(wrq.ifr_name, iface, sizeof(wrq.ifr_name));//指定net device 比如wlan0/eth0**

**    wrq.u.data.pointer = mBuf;**

**    wrq.u.data.length = sizeof(mBuf) /sizeof(struct iw_priv_args);**

**    wrq.u.data.flags = 0;**

**    if ((ret = ioctl(mSock,SIOCGIWPRIV, &wrq)) < 0) {//获得driver private handler的iw_priv_args**

**        LOGE("SIOCGIPRIV failed: %d",ret);**

**        return ret;**

**    }**

**    priv_ptr = (struct iw_priv_args*)wrq.u.data.pointer;**

**    for(i=0;(i < wrq.u.data.length);i++) {**

**        if (strcmp(priv_ptr[i].name, fname) ==0)//找出指定CMD**

**            return priv_ptr[i].cmd;**

**}**

**之前看这段代码时真是困惑死了，SIOCGIWPRIV明明是standard提供的一个标准接口且在我要调用的wifi driver中并没有实现怎么会调用结果是获得private 的iw_priv_args,目前我先将该疑问留着到后面自然会明白。**



**如下我只分析ioctl(mSock, SIOCGIWPRIV,&wrq)的流程，其它的ioctl流程基本一致只是过程中调用不同的函数。**





**如上调用**实际上该函数最终通过系统调用调用到kernel space.如下所示

**kernel/fs/Ioctl.c**

SYSCALL_DEFINE3(ioctl, unsigned int, fd,unsigned int, cmd, unsigned long, arg)

{

……………………………………………………………………..

         error= do_vfs_ioctl(filp, fd, cmd, arg);//调用虚拟文件系统的ioctl

……………………………………………………………..

}

如上系统调用ioctl

int do_vfs_ioctl(struct file *filp,unsigned int fd, unsigned int cmd,

              unsigned long arg)

switch (cmd) {

…………………………………………..

default:

                   if(S_ISREG(filp->f_path.dentry->d_inode->i_mode))

                            error= file_ioctl(filp, cmd, arg);

                   else

                            error= vfs_ioctl(filp, cmd, arg);

                   break;



**staticlong vfs_ioctl(struct file *filp, unsigned int cmd,**

**                         unsigned long arg)**

**{**

**         int error = -ENOTTY;**



**         if (!filp->f_op)**

**                   goto out;**



**         if (filp->f_op->unlocked_ioctl) {**

**                   error = filp->f_op->unlocked_ioctl(filp, cmd, arg);//此处调用的文件描述符接口在创建socket时init_file中赋值file->f_op = fop;**

**                   if (error == -ENOIOCTLCMD)**

**                            error = -EINVAL;**

**                   goto out;**

在本例中如上实际上调用了socket的文件描述符，该描述符在创建socket时就提供如“[android基于Socket的系统调用实现](http://blog.csdn.net/lanbo_g/article/details/6880448)”中描述。

socket_file_ops. unlocked_ioctl = sock_ioctl



如下列出部分commond的宏定义，具体在kernel/include/linux/Wireless.h中定义。

#define SIOCGIWPRIV 0x8B0D

#define SIOCIWFIRSTPRIV 0x8BE0//第一个privatecommand对应位址

#define SIOCIWLASTPRIV 0x8BFF

#define SIOCIWFIRST 0x8B00

#define SIOCIWLAST SIOCIWLASTPRIV 



static longsock_ioctl(struct file *file, unsigned cmd, unsigned long arg)

#ifdef CONFIG_WIRELESS_EXT

         if(cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {// 很明显SIOCGIWPRIV是在条件之内的。

                   err= dev_ioctl(net, cmd, argp);

         }else

#endif



kernel/net/core/Dev.c

int dev_ioctl(struct net*net, unsigned int cmd, void __user *arg)

/* Take care of Wireless Extensions */

                   if(cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST)

                            return wext_handle_ioctl(net, &ifr, cmd, arg);



kernel/net/wireless/Wext.c

intwext_handle_ioctl(struct net *net, struct ifreq *ifr, unsigned int cmd,

                         void __user *arg)

         ret= wext_ioctl_dispatch(net, ifr, cmd, &info,

**ioctl_standard_call**,//调用执行driver提供的standard handler

** ioctl_private_call**);// 调用执行driver提供的private handler

         if(ret >= 0 &&

             IW_IS_GET(cmd) &&

             copy_to_user(arg, ifr, sizeof(structiwreq)))



static int wext_ioctl_dispatch(struct net*net, struct ifreq *ifr,

                                   unsigned int cmd, struct iw_request_info*info,

                                   wext_ioctl_func standard,

                                   wext_ioctl_func private)

dev_load(net, ifr->ifr_name);//根据名字得到net

rtnl_lock();

         ret= wireless_process_ioctl(net, ifr, cmd, info, standard, private);

         rtnl_unlock();



void dev_load(struct net *net, const char*name)

{

         structnet_device *dev;



         read_lock(&dev_base_lock);

         dev= __dev_get_by_name(net, name);

         read_unlock(&dev_base_lock);



         if(!dev && capable(CAP_NET_ADMIN))

                   request_module("%s",name);

}

//dev_load通过name获取net的dev,name就是softapcontroller.cpp中getPrivFuncNum的第一个参数iface



static int wireless_process_ioctl(structnet *net, struct ifreq *ifr,

                                       unsigned int cmd,

                                       struct iw_request_info *info,

                                       wext_ioctl_func standard,

                                       wext_ioctl_func private)

**if(cmd == SIOCGIWSTATS)**

**                   return standard(dev, iwr,cmd, info,**

**                                     &iw_handler_get_iwstats);//get status，和SIOCGIWPRIV一样是个特殊的command,从代码跟下去看，wifidriver提供了指定接口。**



**         if (cmd == SIOCGIWPRIV &&dev->wireless_handlers)**

**                   return standard(dev, iwr, cmd,info,**

**                                     &iw_handler_get_private); //get private handle的信息（句柄，参数等）**



static int ioctl_standard_call(structnet_device *  dev,

                                   struct iwreq                 *iwr,

                                   unsigned int                 cmd,

                                   struct iw_request_info      *info,

                                   iw_handler                   handler)

descr = &(standard_ioctl[cmd- SIOCIWFIRST]);//获得指定cmd的一些信息如下有描述该结构体数组



         /*Check if we have a pointer to user space data or not */

         if(descr->header_type != IW_HEADER_TYPE_POINT){



                   /*No extra arguments. Trivial to handle */

                   ret= handler(dev, info, &(iwr->u), NULL);



                   /*Generate an event to notify listeners of the change */

                   if((descr->flags & IW_DESCR_FLAG_EVENT) &&

                      ((ret== 0) || (ret == -EIWCOMMIT)))

                            wireless_send_event(dev,cmd, &(iwr->u), NULL);

         }else {//** SIOCGIWPRIV调用下面的函数，其中handler为iw_handler_get_private**

                   ret= ioctl_standard_iw_point(&iwr->u.data, cmd, descr,

                                                     handler,dev, info);

         }



static const struct iw_ioctl_description standard_ioctl[] = {

         [SIOCSIWCOMMIT  - SIOCIWFIRST] = {

                   .header_type   = IW_HEADER_TYPE_NULL,

         },

-----------------------------------省略号-----------------------------------------------------

[SIOCGIWPRIV          -SIOCIWFIRST] = { /* (handled directly by us) *///看见原注释没，实际作用是获得driver中提供的private handle的个数及对应cmd的位置，以便调用到指定private handle。

                   .header_type   = IW_HEADER_TYPE_POINT,

                   .token_size      = sizeof(struct iw_priv_args),

                   .max_tokens    = 16,

                   .flags                  = IW_DESCR_FLAG_NOMAX,

         },

         [SIOCSIWSTATS         - SIOCIWFIRST] = {

                   .header_type   = IW_HEADER_TYPE_NULL,

         },

         [SIOCGIWSTATS        -SIOCIWFIRST] = { /* (handled directly by us) */ //get status command

                   .header_type   = IW_HEADER_TYPE_POINT,

                   .token_size      = 1,

                   .max_tokens    = sizeof(struct iw_statistics),

                   .flags                  = IW_DESCR_FLAG_DUMP,

         },

-----------------------------------省略号-----------------------------------------------------





static int ioctl_standard_iw_point(structiw_point *iwp, unsigned int cmd,

                                        const struct iw_ioctl_description *descr,

                                        iw_handler handler, struct net_device *dev,

                                        struct iw_request_info *info)

err = handler(dev,info, (union iwreq_data *) iwp, extra);





/*---------------------------------------------------------------- */

/*

 * Standard Wireless Handler : get iwpriv definitions

 * Export the driver private handler definition

 * They will be picked up by tools like iwpriv...

 */

static int iw_handler_get_private(structnet_device *           dev,

                                       struct iw_request_info *     info,

                                       union iwreq_data *               wrqu,

                                       char *                     extra)

{

         /*Check if the driver has something to export */

         if((dev->wireless_handlers->num_private_args == 0) ||

            (dev->wireless_handlers->private_args== NULL))

                   return-EOPNOTSUPP;



         /*Check if there is enough buffer up there */

         if(wrqu->data.length < dev->wireless_handlers->num_private_args) {

                   /*User space can't know in advance how large the buffer

                    * needs to be. Give it a hint, so that we cansupport

                    * any size buffer we want somewhatefficiently... */

                   wrqu->data.length= dev->wireless_handlers->num_private_args;

                   return-E2BIG;

         }



         /* Set the number of available ioctls. */

         wrqu->data.length =dev->wireless_handlers->num_private_args;



         /* Copy structure to the user buffer. */

         memcpy(extra, dev->wireless_handlers->private_args,

                sizeof(structiw_priv_args) * wrqu->data.length);



         return0;

}



如下为某wifi driver中定义的wext接口

const struct iw_handler_defwl_iw_handler_def =

{

         .num_standard= ARRAYSIZE(wl_iw_handler),

         .standard= (iw_handler *) wl_iw_handler,

         .num_private= ARRAYSIZE(wl_iw_priv_handler),

         .num_private_args = ARRAY_SIZE(wl_iw_priv_args),

         .private= (iw_handler *)wl_iw_priv_handler,

         .private_args = (void *) wl_iw_priv_args,



#if WIRELESS_EXT >= 19

         get_wireless_stats:dhd_get_wireless_stats,//** SIOCGIWSTATS执行的接口**

#endif

         };

#endif



至此可知道SIOCGIWPRIV实际上是将iface作为net设备名所对应的driver num_private_args及private_args传给上层。接下来再调用对应的privatehandler中的函数.

如start **ioctl_private_call**

#if WIRELESS_EXT > 12

static const iw_handlerwl_iw_priv_handler[] = {

         NULL,//SIOCIWFIRSTPRIV+0

         (iw_handler)wl_iw_set_active_scan,//SIOCIWFIRSTPRIV+1

         NULL,

         (iw_handler)wl_iw_get_rssi,//SIOCIWFIRSTPRIV+3

         NULL,

         (iw_handler)wl_iw_set_passive_scan,//SIOCIWFIRSTPRIV+5

         NULL,

         (iw_handler)wl_iw_get_link_speed,//SIOCIWFIRSTPRIV+7

         NULL,

         (iw_handler)wl_iw_get_macaddr,//SIOCIWFIRSTPRIV+9

         NULL,

         (iw_handler)wl_iw_control_wl_off,//SIOCIWFIRSTPRIV+11

         NULL,

         (iw_handler)wl_iw_control_wl_on,//SIOCIWFIRSTPRIV+13 所以调用start就是调用driver中该函数。

#ifdef SOFTAP      





         NULL,

         (iw_handler)iwpriv_set_ap_config,







         NULL,

         (iw_handler)iwpriv_get_assoc_list,





         NULL,

         (iw_handler)iwpriv_set_mac_filters,





         NULL,

         (iw_handler)iwpriv_en_ap_bss,





         NULL,

         (iw_handler)iwpriv_wpasupp_loop_tst,



         NULL,

         (iw_handler)iwpriv_softap_stop,



         NULL,

         (iw_handler)iwpriv_fw_reload,

#endif

#if defined(CSCAN)



         NULL,

         (iw_handler)iwpriv_set_cscan

#endif     

};



//start command在如下中定义。

static const struct iw_priv_argswl_iw_priv_args[] =

{

…………………………………………………………………………………………….

         {

                   WL_IW_SET_START,//start 对应command,该command在wifi driver中有定义位址，可根据SIOCIWFIRSTPRIV（第一个privatecmd位址）来计算偏移。该driver中定义：#define WL_IW_SET_START                            (SIOCIWFIRSTPRIV+13)

                   0,

                   IW_PRIV_TYPE_CHAR| IW_PRIV_SIZE_FIXED | MAX_WX_STRING,

                   "START"

         },





from:http://blog.csdn.net/zjjdyb/article/details/20993117









