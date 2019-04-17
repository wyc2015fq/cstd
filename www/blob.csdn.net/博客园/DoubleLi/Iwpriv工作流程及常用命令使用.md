# Iwpriv工作流程及常用命令使用 - DoubleLi - 博客园






iwpriv工具通过ioctl动态获取相应无线网卡驱动的private_args所有扩展参数

iwpriv是处理下面的wlan_private_args的所有扩展命令,iwpriv的实现上,是这样的,
=>main
=>set_private
=>iw_get_priv_info获取wireless网卡所能处理的所有wlan_private_args类型.

dev_ioctl
=>wext_handle_ioctl
=>wireless_process_ioctl
    if (cmd == SIOCGIWPRIV && dev->wireless_handlers)
        return ioctl_standard_call(dev, ifr, cmd,
                     &iw_handler_get_private);
static int ioctl_standard_call(struct net_device *    dev,
             struct ifreq *        ifr,
             unsigned int        cmd,
             iw_handler        handler)
{
    ...
        /* Call the handler */
        ret = handler(dev, &info, &(iwr->u), extra);
            if (user_length < iwr->u.data.length) {
                kfree(extra);
                return -E2BIG;
//通知iwpriv,本wifi网卡对应的private命令还没有完,还有,这样iwpriv将会继续
//maxpriv默认为16,即将以16个为一组,一组一组的从wifi网卡驱动读取该网卡所能支持的所有private_args参数
//newpriv = realloc(priv, maxpriv * sizeof(priv[0]));继续申请,继续拷贝,知道将wifi网卡自定义的wlan_private_args参数全部
//传出到iwpriv为止.
            }
    ...
}
    /* New driver API : try to find the handler */
    handler = get_handler(dev, cmd);//获取

    if (handler) {
        /* Standard and private are not the same */
        if (cmd < SIOCIWFIRSTPRIV)
            return ioctl_standard_call(dev, ifr, cmd, handler);
        else

//如果有对应的handler,那么处理iwpriv的命令,可以我们的iwpriv都是由dev->do_ioctl完成的.
            return ioctl_private_call(dev, ifr, cmd, handler);
    }
    /* Old driver API : call driver ioctl handler */
    if (dev->do_ioctl)
//如果dev->wireless_handlers->standard和dev->wireless_handlers->private[index都不对该cmd作处理,那么由
//dev->do_ioctl = wlan_do_ioctl;我们驱动的最后处理函数wlan_do_ioctl处理.
        return dev->do_ioctl(dev, ifr, cmd);

static iw_handler get_handler(struct net_device *dev, unsigned int cmd)
{
    /* Don't "optimise" the following variable, it will crash */
    unsigned int    index;        /* *MUST* be unsigned */

    /* Check if we have some wireless handlers defined */
    if (dev->wireless_handlers == NULL)
        return NULL;

    /* Try as a standard command */
    index = cmd - SIOCIWFIRST;
    if (index < dev->wireless_handlers->num_standard)
        return dev->wireless_handlers->standard[index];

    /* Try as a private command */
    index = cmd - SIOCIWFIRSTPRIV;//

    if (index < dev->wireless_handlers->num_private)
        return dev->wireless_handlers->private[index];//该private命令的handler.
    /* Not found */
    return NULL;
}


下面wlan_private_args为本wifi网卡驱动的所能支持的所有命令,也就是iwpriv命令所能支持的所有命令

struct iw_handler_def wlan_handler_def = {
  num_standard:sizeof(wlan_handler) / sizeof(iw_handler),
  num_private:sizeof(wlan_private_handler) / sizeof(iw_handler),
  num_private_args:sizeof(wlan_private_args) / sizeof(struct iw_priv_args),
  standard:(iw_handler *) wlan_handler,
  private:(iw_handler *) wlan_private_handler,
  private_args:(struct iw_priv_args *) wlan_private_args,
#if WIRELESS_EXT > 20
  get_wireless_stats:wlan_get_wireless_stats,
#endif
};

以下为示意代码,我们的wifi网卡驱动支持如下iwpriv命令.


static const struct iw_priv_args wlan_private_args[] = {
     "extscan"/     "hostcmd"/     "arpfilter"/     "regrdwr"/     "sdcmd52rw"/
     "sdcmd53rw"/     "setgetconf"/     "getcis"/     "scantype"/     "deauth"
     "getNF"/     "getRSSI"/     "bgscan"/     "enable11d"/     "adhocgrate"
     "sdioclock"/     "wmm"/     "uapsdnullgen"/     "setcoalescing"/     "adhocgprot"
     "setpowercons"/     "wmm_qosinfo"/     "lolisteninter"/     "fwwakeupmethod"
     "psnullinterval"/     "bcnmisto"/     "adhocawakepd"/     "moduletype"
     "autodeepsleep"/     "enhanceps"/     "wakeupmt"/     "setrxant"/     "settxant"
     "authalgs"/     "encryptionmode"/     "setregioncode"/     "setlisteninter"
     "setmultipledtim"/     "setbcnavg"/     "setdataavg"/     "associate"/     "getregioncode"
     "getlisteninter"/     "getmultipledtim"/     "gettxrate"/     "getbcnavg"
     "getdataavg"/     "getrxant"/     "gettxant"/     "gettsf"/     "wpssession"
     "deepsleep"/     "adhocstop"/     "radioon"/     "radiooff"/     "rmaeskey"
     "crypto_test"/     "reasso-on"/     "reasso-off"/     "wlanidle-on"/     "wlanidle-off"
     "sleepparams"/     "requesttpc"/     "powercap"/     "measreq"/     "bca-ts"
     "scanmode"/     "getadhocstatus"/     "setgenie"/     "getgenie"/     "qstatus"
     "ts_status"/     "setaeskey"/     "getaeskey"/     "version"/     "verext"/     "setwpaie"
     "setband"/     "setadhocch"/     "chanswann"/     "getband"/     "getadhocch"
     "getlog"/     "tpccfg"/     "scanprobes"/     "ledgpio"/     "sleeppd"/     "rateadapt"
     "getSNR"/     "getrate"/     "getrxinfo"/     "atimwindow"/     "bcninterval"/     "sdiopullctrl"
     "scantime"/     "sysclock"/     "txcontrol"/     "hscfg"/     "hssetpara"/     "inactoext"
     "dbgscfg"/     "drvdbg"/     "drvdelaymax"/     "intfctrl"/     "setquietie"/     ""
     "setuserscan"/     "getscantable"/     "setmrvltlv"/     "getassocrsp"/     "addts"
     "delts"/    "qconfig"/     "qstats"/     "txpktstats"/     "getcfptable"/     "mefcfg"
     "getmem"

};





浅析ethx网卡控制函数ioctl实现具体流程

====================
1.应用层程序iwpriv
wireless tools网络配置应用程序iwpriv命令格式:
iwpriv ethX private-command [parameters]

iwpriv部分实现源码如下:
int main(int argc, char *argv[])
{
    ...
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ...
    ioctl(sockfd, ioctl_val, &iwr);//将控制命令通过ioctl发送到无线网卡
    ...
}
====================
2.系统调用sys_ioctl
应用层通过ioctl(sockfd, ioctl_val, &iwr);触发sys_ioctl系统调用,实际流程:
sys_ioctl=>vfs_ioctl=>do_ioctl=最后调用
filp->f_op->unlocked_ioctl执行具体的ioctl操作,该操作就是sock_ioctl,至于为什么是sock_ioctl,后边作了进一步分析
sock_ioctl=>
{
    ...
    #ifdef CONFIG_WIRELESS_EXT
        if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {
            err = dev_ioctl(net, cmd, argp);//

        } else
    #endif
    ...
}
dev_ioctl=>wext_handle_ioctl
{
    ...
/* Take care of Wireless Extensions */
    if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST)
        return wext_handle_ioctl(net, &ifr, cmd, arg);
    ...
}
wext_handle_ioctl=>wireless_process_ioctl=>
然后通过if ((dev = __dev_get_by_name(net, ifr->ifr_name)) == NULL)函数,
从系统管理的net链表中,把ioctl指定的ethX对应的struct net_device摘出来,
最后调用ioctl_private_call(handler)或者调用dev->do_ioctl(dev, ifr, cmd)来处理该ioctl,
这两个函数分别指向wlan_handler_def和wlan_do_ioctl
====================
3.wifi网卡是怎么登记到kernel上的
wlan_probe()=>wlan_add_card()=>alloc_etherdev()=>
之后将操作方法添加到struct net_device *dev=alloc_etherdev()申请的dev上去,其中包括:
    ...
    /* Setup the OS Interface to our functions */
    dev->open = wlan_open;
    dev->hard_start_xmit = wlan_hard_start_xmit;
    dev->stop = wlan_close;
    dev->do_ioctl = wlan_do_ioctl;
    dev->set_mac_address = wlan_set_mac_address;

    dev->tx_timeout = wlan_tx_timeout;
    dev->get_stats = wlan_get_stats;
    dev->watchdog_timeo = MRVDRV_DEFAULT_WATCHDOG_TIMEOUT;
    dev->wireless_handlers = (struct iw_handler_def *) &wlan_handler_def;
    dev->set_multicast_list = wlan_set_multicast_list;
    ...
4.socket系统调用如何关联上ioctl和ethX设备

asmlinkage long sys_socket(int family, int type, int protocol);

sys_socket=>sock_create=>__sock_create=>sock = sock_alloc();通过sock_mnt->mnt_sb从socket文件系统的超级块上申请一个inode节点,这样也就同时获得了由 该inode描述的一个sock结构体单元,所以sokcet和dentry目录项等效,
接下来从net_families全局管理结构体中找到当前family对应的ops操作集,
net_proto_family *pf=net_families[family];
pf->create(net, sock, protocol);//核心调用,对于ipv4,就是inet_create
以ipv4为例
static struct net_proto_family inet_family_ops = {
    .family = PF_INET,
    .create = inet_create,
    .owner    = THIS_MODULE,
};
还记得上面应用层创建sokcet的函数吧,
sockfd = socket(AF_INET, SOCK_STREAM, 0);//AF_INET虽然等于PF_INET,但是因为种种原因我们提倡使用PF_INET
可见family等于AF_INET,type等于SOCK_STREAM,协议protocol为0,也就是采用IP协议,
inet_create=>inetsw[sock->type]也就是inetsw[SOCK_STREAM],
从inetsw[sock->type]中找到已经登记的protocol网络协议处理函数,
inetsw[]是怎么填充的呢?inet_init()=>inet_register_protosw(inetsw_array)=>这样inetsw_array中的所有protocol处理模块都将登记到inetsw中了,
static struct inet_protosw inetsw_array[] =
{
    {
        .type = SOCK_STREAM,
        .protocol = IPPROTO_TCP,
        .prot = &tcp_prot,
        .ops = &inet_stream_ops,
        .capability = -1,
        .no_check = 0,
        .flags = INET_PROTOSW_PERMANENT | INET_PROTOSW_ICSK,
    },

    {
        .type = SOCK_DGRAM,
        .protocol = IPPROTO_UDP,
        .prot = &udp_prot,
        .ops = &inet_dgram_ops,
        .capability = -1,
        .no_check = UDP_CSUM_DEFAULT,
        .flags = INET_PROTOSW_PERMANENT,
    },


    {
        .type = SOCK_RAW,
        .protocol = IPPROTO_IP,    /* wild card */
        .prot = &raw_prot,
        .ops = &inet_sockraw_ops,
        .capability = CAP_NET_RAW,
        .no_check = UDP_CSUM_DEFAULT,
        .flags = INET_PROTOSW_REUSE,
    }
};
至 于inet_init,则是以fs_initcall(inet_init)方式,以5号优先级被build in到了内核中,当kernel启动时会在start_kernel=>rest_init=>kernel_init=> do_basic_setup=>do_initcalls中依据优先级号优先于其他module驱动被调用.
这样sock->ops = answer->ops;对于ipv4也就等于inet_stream_ops,
接下来就是将ops填充到file操作指针中了,
sys_socket=>sock_map_fd=>sock_attach_fd=>
dentry->d_op = &sockfs_dentry_operations;
init_file(file, sock_mnt, dentry, FMODE_READ | FMODE_WRITE, &socket_file_ops);
file->private_data = sock;
其中init_file=>file->f_op = fop;也就是file->f_op = socket_file_ops;
所以read(),wirte(),poll()和ioctl()应用程序调用的file->f_op就是socket_file_ops了,
比如:
read()对应sock_aio_read网络异步读
write()对应sock_aio_write网络异步写
ioctl()对应sock_ioctl

socket_file_ops结构体具体实现如下:
static const struct file_operations socket_file_ops = {
    .owner =    THIS_MODULE,
    .llseek =    no_llseek,
    .aio_read =    sock_aio_read,
    .aio_write =    sock_aio_write,
    .poll =        sock_poll,
    .unlocked_ioctl = sock_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_sock_ioctl,
#endif
    .mmap =        sock_mmap,
    .open =        sock_no_open,    /* special open code to disallow open via /proc */
    .release =    sock_close,
    .fasync =    sock_fasync,
    .sendpage =    sock_sendpage,
    .splice_write = generic_splice_sendpage,
};
网卡控制因为涉及到的知识点比较多,上面只是从宏观上对数据流程做了一个简单的介绍,深入到其中的每个知识点,都会牵扯出一系列文章,读者需要自己去一个个的慢慢深入,希望本文能够对刚刚接触网络驱动的读者有所帮助和启发【gliethttp.Leith】

wireless extention扩展接口Blog作者的回复:
wlan_add_card=>
wlan_create_thread(wlan_service_main_thread, &priv->MainThread, "wlan_main_service");
=>wlan_service_main_thread=>wlan_exec_next_cmd=>
将调用wlan_enter_ps和wlan_exit_ps


sbi_interrupt=>从sdio口上传来的中断数据,sdio_irq_thread=>process_sdio_pending_irqs=>调用func->irq_handler(func);即本.
在mmc_signal_sdio_irq=>将调用wake_up_process(host->sdio_irq_thread);来唤醒该irq处理线程,可能还有其他命令需要处理wlan_exec_next_cmd
这个pxamci_irq就是mmc的物理irq中断了,pxamci_irq=>mmc_signal_sdio_irq(host->mmc);

wlan_exec_next_cmd=>只要cmd链表上CmdNode还存在,
那么就会执行wlan_dnld_cmd_to_fw(wlan_private * priv, CmdCtrlNode * CmdNode)将CmdNode中的数据下发下去,
然后重新触发wlan_mod_timer(&Adapter->MrvDrvCommandTimer, MRVDRV_TIMER_5S);
也就是wlan_cmd_timeout_func命令超时处理函数,
在cmd已经有了恢复之后,在主线程中调用wlan_process_cmdresp,立即调用wlan_cancel_timer(&Adapter->MrvDrvCommandTimer);来删除定时器

wlan_service_main_thread=>每次唤醒都会检查
====
    /* Execute the next command */
    if (!priv->wlan_dev.cmd_sent && !Adapter->CurCmd)
        wlan_exec_next_cmd(priv);
====


wlan_prepare_cmd=>
wlan_hostcmd_ioctl=>
获取一个空闲的CmdNode节点wlan_get_cmd_node,当完成赋值之后,执行如下语句,将CmdNode节点添加到处理队列中:
wlan_insert_cmd_to_pending_q(Adapter, CmdNode, TRUE);
wake_up_interruptible(&priv->MainThread.waitQ);
另外在数组中
/*
 * iwconfig settable callbacks 
 */
static const iw_handler wlan_handler[]这个数组中全部是回调函数,

/** wlan_handler_def */
struct iw_handler_def wlan_handler_def = {
  num_standard:sizeof(wlan_handler) / sizeof(iw_handler),
  num_private:sizeof(wlan_private_handler) / sizeof(iw_handler),
  num_private_args:sizeof(wlan_private_args) / sizeof(struct iw_priv_args),
  standard:(iw_handler *) wlan_handler,
  private:(iw_handler *) wlan_private_handler,
  private_args:(struct iw_priv_args *) wlan_private_args,
#if WIRELESS_EXT > 20
  get_wireless_stats:wlan_get_wireless_stats,
#endif
};
在wlan_add_card函数中
dev->wireless_handlers = (struct iw_handler_def *) &wlan_handler_def;

===============在kernel的net中使用wireless extention扩展接口

static iw_handler get_handler(struct net_device *dev, unsigned int cmd)
{
    /* Don't "optimise" the following variable, it will crash */
    unsigned int    index;        /* *MUST* be unsigned */

    /* Check if we have some wireless handlers defined */
    if (dev->wireless_handlers == NULL)
        return NULL;

    /* Try as a standard command */
    index = cmd - SIOCIWFIRST;
    if (index < dev->wireless_handlers->num_standard)
        return dev->wireless_handlers->standard[index];

    /* Try as a private command */
    index = cmd - SIOCIWFIRSTPRIV;
    if (index < dev->wireless_handlers->num_private)
        return dev->wireless_handlers->private[index];

    /* Not found */
    return NULL;
}


=>sock_ioctl
=>dev_ioctl
+++/* Take care of Wireless Extensions */
+++if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST)
+++return wext_handle_ioctl(net, &ifr, cmd, arg);
=>wext_handle_ioctl
=>wireless_process_ioctl(struct net *net, struct ifreq *ifr, unsigned int cmd)
=>get_handler(dev, cmd);如果没有实现该cmd,那么将调用dev->do_ioctl来处理,


wlan_reassoc_timer_func=>
wmm_start_queue=>
wlan_tx_packet=>
wlan_tx_timeout=>
wlan_remove_card=>
wlan_hostcmd_ioctl=>
wlan_auto_deep_sleep=>
wlan_set_deep_sleep=>
wlan_prepare_cmd=>
wlan_cmd_timeout_func=>
将调用wake_up_interruptible(&priv->MainThread.waitQ);唤醒wlan_service_main_thread主处理线程.

wlan_hard_start_xmit=>wlan_tx_packet发送数据包
dev->tx_timeout = wlan_tx_timeout;
wlan_initialize_timer(&Adapter->MrvDrvCommandTimer, wlan_cmd_timeout_func, priv);

int wlan_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd)
{
    ...
        case WLAN_WAKEUP_MT:
            if (wrq->u.data.length > 0)
                Adapter->IntCounter++;
            wake_up_interruptible(&priv->MainThread.waitQ);
            break;
    ...
}


在wlan_process_cmdresp()处理完该cmd之后,调用
wlan_insert_cmd_to_free_q=>wlan_clean_cmd_noder,从命令链表上删除已经处理完成的cmd_node,
wlan_clean_cmd_noder然后pTempNode->CmdWaitQWoken = TRUE;同时如果该cmd_node是一个被阻塞等待的,那么唤醒等待的程序.
wake_up_interruptible(&pTempNode->cmdwait_q);



Available private ioctls :
          set              (8BE2) : set 1024 char  & get   0
          connStatus       (0004) : set 1024 char  & get 2047 char
          driverVer        (0005) : set 1024 char  & get 2047 char
          bainfo           (0006) : set 1024 char  & get 2047 char
          descinfo         (0007) : set 1024 char  & get 2047 char
          radio_off        (000A) : set 1024 char  & get 2047 char
          radio_on         (000B) : set 1024 char  & get 2047 char
          show             (0015) : set 1024 char  & get 2047 char
          adhocEntry       (0016) : set 1024 char  & get 2047 char
          bbp              (8BE3) : set 2047 char  & get 2047 char
          mac              (8BE5) : set 1024 char  & get 1024 char
          rf               (8BF3) : set 2047 char  & get 2047 char
          e2p              (8BE7) : set 1024 char  & get 1024 char
          stat             (8BE9) : set   0       & get 2047 char

‍          get_site_survey  (8BED) : set   0       & get 1024 char    查看网络状态         



Connect to stongest open AP

设置命令          
iwpriv ra0 set SSID="" 
iwpriv ra0 set Channel=0 
iwpriv ra0 set NetworkType=Infra 
iwpriv ra0 set AuthMode=SHARED 
iwpriv ra0 set EncrypType=WEP 
iwpriv ra0 set DefaultKeyID=1 
iwpriv ra0 set Key1="whatever" 
iwpriv ra0 set SSID="some_ssed" 
iwpriv ra0 set WPAPSK="wpa_key"

...

‍显示命令

iwpriv ra0 show SSID

iwpriv ra0 show Channel 
iwpriv ra0 show NetworkType

iwpriv ra0 show AuthMode

iwpriv ra0 show EncrypType

iwpriv ra0 show DefaultKeyID 
iwpriv ra0 show Key1 
iwpriv ra0 show WPAPSK

...



./iwpriv ra0 show    无线网卡功能参数
ra0       show:
SSID
WirelessMode
TxBurst
TxPreamble
TxPower
Channel
BGProtection
RTSThreshold
FragThreshold
HtBw
HtMcs
HtGi
HtOpMode
HtExtcha
HtMpduDensity
HtBaWinSize
HtRdg
HtAmsdu
HtAutoBa
CountryRegion
CountryRegionABand
CountryCode
PktAggregate
WmmCapable
IEEE80211H
NetworkType
WPAPSK
AutoReconnect
AuthMode
EncrypType
DefaultKeyID
Key1
Key2
Key3
Key4
PMK









