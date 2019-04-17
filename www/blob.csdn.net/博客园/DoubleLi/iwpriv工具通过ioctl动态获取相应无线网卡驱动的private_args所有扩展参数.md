# iwpriv工具通过ioctl动态获取相应无线网卡驱动的private_args所有扩展参数 - DoubleLi - 博客园







```
iwpriv工具通过ioctl动态获取相应无线网卡驱动的private_args所有扩展参数

iwpriv是处理下面的wlan_private_args的所有扩展命令,iwpriv的实现上,是这样的,
=>main
=>set_private
=>iw_get_priv_info获取wireless网卡所能处理的所有wlan_private_args类型.

dev_ioctl
=>wext_handle_ioctl
=>wireless_process_ioctl
    if (cmd == SIOCGIWPRIV && dev->wireless_handlers)
        return ioctl_standard_call(dev, ifr, cmd,
                     &iw_handler_get_private);
static int ioctl_standard_call(struct net_device *    dev,
             struct ifreq *        ifr,
             unsigned int        cmd,
             iw_handler        handler)
{
    ...
        /* Call the handler */
        ret = handler(dev, &info, &(iwr->u), extra);
            if (user_length < iwr->u.data.length) {
                kfree(extra);
                return -E2BIG;
//通知iwpriv,本wifi网卡对应的private命令还没有完,还有,这样iwpriv将会继续
//maxpriv默认为16,即将以16个为一组,一组一组的从wifi网卡驱动读取该网卡所能支持的所有private_args参数
//newpriv = realloc(priv, maxpriv * sizeof(priv[0]));继续申请,继续拷贝,知道将wifi网卡自定义的wlan_private_args参数全部
//传出到iwpriv为止.
            }
    ...
}
    /* New driver API : try to find the handler */
    handler = get_handler(dev, cmd);//获取

    if (handler) {
        /* Standard and private are not the same */
        if (cmd < SIOCIWFIRSTPRIV)
            return ioctl_standard_call(dev, ifr, cmd, handler);
        else
```



```
//如果有对应的handler,那么处理iwpriv的命令,可以我们的iwpriv都是由dev->do_ioctl完成的.
            return ioctl_private_call(dev, ifr, cmd, handler);
    }
    /* Old driver API : call driver ioctl handler */
    if (dev->do_ioctl)
//如果dev->wireless_handlers->standard和dev->wireless_handlers->private[index都不对该cmd作处理,那么由
//dev->do_ioctl = wlan_do_ioctl;我们驱动的最后处理函数wlan_do_ioctl处理.
        return dev->do_ioctl(dev, ifr, cmd);

static iw_handler get_handler(struct net_device *dev, unsigned int cmd)
{
    /* Don't "optimise" the following variable, it will crash */
    unsigned int    index;        /* *MUST* be unsigned */

    /* Check if we have some wireless handlers defined */
    if (dev->wireless_handlers == NULL)
        return NULL;

    /* Try as a standard command */
    index = cmd - SIOCIWFIRST;
    if (index < dev->wireless_handlers->num_standard)
        return dev->wireless_handlers->standard[index];

    /* Try as a private command */
    index = cmd - SIOCIWFIRSTPRIV;//

    if (index < dev->wireless_handlers->num_private)
        return dev->wireless_handlers->private[index];//该private命令的handler.
    /* Not found */
    return NULL;
}
```


`下面wlan_private_args为本wifi网卡驱动的所能支持的所有命令,也就是iwpriv命令所能支持的所有命令`


```
struct iw_handler_def wlan_handler_def = {
  num_standard:sizeof(wlan_handler) / sizeof(iw_handler),
  num_private:sizeof(wlan_private_handler) / sizeof(iw_handler),
  num_private_args:sizeof(wlan_private_args) / sizeof(struct iw_priv_args),
  standard:(iw_handler *) wlan_handler,
  private:(iw_handler *) wlan_private_handler,
  private_args:(struct iw_priv_args *) wlan_private_args,
#if WIRELESS_EXT > 20
  get_wireless_stats:wlan_get_wireless_stats,
#endif
};
```



```
以下为示意代码,我们的wifi网卡驱动支持如下iwpriv命令.
```


static const struct iw_priv_args wlan_private_args[] = {
     "extscan"
     "hostcmd"
     "arpfilter"
     "regrdwr"
     "sdcmd52rw"
     "sdcmd53rw"
     "setgetconf"
     "getcis"
     "scantype"
     "deauth"
     "getNF"
     "getRSSI"
     "bgscan"
     "enable11d"
     "adhocgrate"
     "sdioclock"
     "wmm"
     "uapsdnullgen"
     "setcoalescing"
     "adhocgprot"
     "setpowercons"
     "wmm_qosinfo"
     "lolisteninter"
     "fwwakeupmethod"
     "psnullinterval"
     "bcnmisto"
     "adhocawakepd"
     "moduletype"
     "autodeepsleep"
     "enhanceps"
     "wakeupmt"
     "setrxant"
     "settxant"
     "authalgs"
     "encryptionmode"
     "setregioncode"
     "setlisteninter"
     "setmultipledtim"
     "setbcnavg"
     "setdataavg
     "associate"
     "getregioncode"
     "getlisteninter"
     "getmultipledtim"
     "gettxrate"
     "getbcnavg"
     "getdataavg"
     "getrxant"
     "gettxant"
     "gettsf"
     "wpssession"
     "deepsleep"
     "adhocstop"
     "radioon"
     "radiooff"
     "rmaeskey"
     "crypto_test"
     "reasso-on"
     "reasso-off"
     "wlanidle-on"
     "wlanidle-off"
     "sleepparams"
     "requesttpc"
     "powercap"
     "measreq"
     "bca-ts"
     "scanmode"
     "getadhocstatus"
     "setgenie"
     "getgenie"
     "qstatus"
     "ts_status"
     "setaeskey"
     "getaeskey"
     "version"
     "verext"
     "setwpaie"
     "setband"
     "setadhocch"
     "chanswann"
     "getband"
     "getadhocch"
     "getlog"
     "tpccfg"
     "scanprobes"
     "ledgpio"
     "sleeppd"
     "rateadapt"
     "getSNR"
     "getrate"
     "getrxinfo"
     "atimwindow"
     "bcninterval"
     "sdiopullctrl"
     "scantime"
     "sysclock"
     "txcontrol"
     "hscfg"
     "hssetpara"
     "inactoext"
     "dbgscfg"
     "drvdbg"
     "drvdelaymax"
     "intfctrl"
     "setquietie"
     ""
     "setuserscan"
     "getscantable"
     "setmrvltlv"
     "getassocrsp"
     "addts"
     "delts"
     "qconfig"
     "qstats"
     "txpktstats"
     "getcfptable"
     "mefcfg"
     "getmem"
};
``











