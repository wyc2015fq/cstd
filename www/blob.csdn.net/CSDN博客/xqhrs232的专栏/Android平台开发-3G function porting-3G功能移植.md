# Android平台开发-3G function porting-3G功能移植 - xqhrs232的专栏 - CSDN博客
2011年12月11日 21:53:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：869标签：[android																[function																[平台																[reference																[server																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hi.baidu.com/huntington/blog/item/3eb3f4d3ad6bc4c5a9ec9aa9.html](http://hi.baidu.com/huntington/blog/item/3eb3f4d3ad6bc4c5a9ec9aa9.html)
from: [http://www.civilnet.cn/bbs/browse.php?topicno=3863&fpage=3](http://www.civilnet.cn/bbs/browse.php?topicno=3863&fpage=3)
一、pppd相关文件
   Android ril相关的代码和脚本主要有：
   android/hardware/ril/reference_ril/   (reference_ril.c)
   android/hardware/ril/rild
   android/extern/ppp/pppd
   android/extern/ppp/chat
   android/data/etc/apn-conf-sdk.xml
   android/system/core/rootdir/etc/ppp/init.gprs-pppd
   android/system/core/rootdir/etc/ppp/peers/cmnet
   android/system/core/rootdir/etc/ppp/chat/cmtc-isp
   android/vendor/xxxxx/xxxx/system.prop
   1、reference_ril.c: RIL的一些AT命令操作，通过一些onRequest接口操作，对不同的硬件，需作一
      些修改调整。
   2、apn-conf-sdk.xml: 以下是一个例子，有些不支持的APN，需要自己加上去，否则在log中会出现类
      似：No APN found for carrier: 46xxx, 的错误。一般移动的TD USIM是46007, 有些是
      46000。
        <apns version="6">
           <apn carrier="Android"
                mcc="310"
                mnc="995"
                apn="internet"
                user="*"
                server="*"
                password="*"
                mmsc="null"
           />
           <apn carrier="TelKila" 
                mcc="310"
                mnc="260"
                apn="internet"
                user="*"
                server="*"
                password="*"
                mmsc="null"
           />
           <apn carrier="CMCC"
                mcc="460"
                mnc="00"
                apn="cmnet"
                user="*"
                server="*"
                password="*"
                mmsc="null"
           />
           <apn carrier="CHINA MOBILE"
                mcc="460"
                mnc="07"
                apn="cmnet"
                user="*"
                server="*"
                password="*"
                mmsc="null"
           />
       </apns>
  3、init.gprs-pppd: 调用pppd GPRS拨号的初始化脚本。
         PPPD_PID=
         /system/bin/setprop "net.gprs.ppp-exit" ""
         /system/bin/log -t pppd "Starting pppd"
         /system/xbin/pppd call cmnet $* //调用脚本/system/etc/peers/cmnet
       或 
         /system/xbin/pppd connect 'chat -v "" "AT" "ATDT#777 CONNECT"' user \
                     CARD password CARD /dev/ttyUSB2 115200 nodetach crtscts \
                                                debug usepeerdns defaultroute
       //直接用参数，其中/dev/USB2是控制端口。
         PPPD_EXIT=$?
         PPPD_PID=$!
         /system/bin/log -t pppd "pppd exited with $PPPD_EXIT"
         /system/bin/setprop "net.gprs.ppp-exit" "$PPPD_EXIT"
  4、cmnet：pppd拨号option脚本：
         /dev/ttyUSB3
         921600
         nocrtscts
         nocdtrcts
         local
         usepeerdns
         defaultroute
         noipdefault
         ipcp-accept-local
         ipcp-accept-remote
         user cmnet
         password cmnet
         lock
         nodetach
         connect "/system/xbin/chat -v -t 50 -f /system/etc/ppp/chat/cmtc-isp"
  5、cmtc-isp：
         ABORT 'BUSY'
         ABORT 'NO CARRIER'
         ABORT 'ERROR'
         ABORT '+CME ERROR: 100'
         ""    AT
         OK    AT+CGDCONT=1,"IP","CMNET"
         OK    AT+CGEQREQ=1,2,128,384,0,0,0,0,"0E0","0E0",,0,0
         OK    AT
         OK    AT
         OK    ATS0=0
         OK    AT
         OK    AT
         OK    ATDT*98*1# 
         CONNECT
    如果要连联通的WCDMA，就用
         OK    AT+CGDCONT=1,"IP","3GNET"
         OK    ATDT*99#  //拨号命令
  6、system.prop:
         rild.libpath=/system/lib/libreference-ril.so
         rild.libargs=-d /dev/ttyS0
         wifi.interface=eth1
二、RIL配置支持
    在Android中RIL是通过pppd实现的，要启动pppd，需要在init.rc中添加：
        service pppd_gprs /system/etc/init.gprs-pppd
            user root
            group radio cache inet misc
            disabled
            oneshot
         service ril-daemon /system/bin/rild -l /system/lib/libreference-ril.so --
                                  -d /dev/ttyUSB3 -u /dev/ttyUSB0
         socket rild stream 660 root radio
         socket rild-debug stream 660 radio system
         user root
         group radio cache inet misc audio
    Modem插入USB口模拟串口，驱动会检测到两个serial端口的，一个是控制的，一个是数据的。
    这里/dev/ttyUSB3表示控制端口。
三、RIL流程分析
    一开机启动pppd后，如果检测到Modem(通过/dev/ttyUSB*设备来判断),pppd_gprs daemon就调用
    init.gprs-pppd连接，
    正常通信后，会显示连接到3G网络标志。
    然后设置APN，保存好之后重启，就会自动连接到internet。
四、手动测试3G modem
    1、加载驱动
       如果驱动加载成功，会出现/dev/ttyUSB*设备的。
    2、进入/etc/ppp/peers目录，执行
       pppd call cmnet &
       这里cmnet是一个包含拨号命令的文件。
    3、设置DNS
       把由pppd自动产生的/etc/ppp/resolv.conf复制到/etc/resolv.conf。
    4、前面都成功的话，这是应该能ping通网络的。
