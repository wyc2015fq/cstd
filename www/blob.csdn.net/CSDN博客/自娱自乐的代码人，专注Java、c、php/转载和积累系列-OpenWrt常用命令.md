# 转载和积累系列 - OpenWrt常用命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月18日 16:09:19[initphp](https://me.csdn.net/initphp)阅读数：15363








uci set network.lan.ipaddr=[lan ip]  



使用pppoe设置 

Shell代码  

uci set network.wan.proto=pppoe    //设置wan口类型为pppoe  

uci set network.wan.username=[上网帐户]  

uci set network.wan.password=[上网密码]    //这两行设置pppoe用户名和密码  



如果要挂在上级路由下面,就需要进行下面的设置 

Shell代码  

uci set network.wan.proto=none    //关掉wan  

uci set network.lan.gateway=[上级路由ip]    //网关指向上级路由  

uci set network.lan.dns=[上级路由ip]    //dns指向上级路由  

uci set dhcp.lan.ignore=1    //关掉lan的dhcp  



最后对无线网络进行配置 

Shell代码  

uci set wireless.@wifi-device[0].disabled=0    //打开无线  

uci set wireless.@wifi-device[0].txpower=17    //设置功率为17dbm 太高会烧无线模块  

uci set wireless.@wifi-device[0].channel=6    //设置无线信道为6  

uci set wireless.@wifi-iface[0].mode=ap    //设置无线模式为ap  

uci set wireless.@wifi-iface[0].ssid=[自己设置SSID]    //设置无线SSID  

uci set wireless.@wifi-iface[0].network=lan    //无线链接到lan上  

uci set wireless.@wifi-iface[0].encryption=psk2    //设置[加密](http://www.2cto.com/Article/jiami/)为[WP](http://www.2cto.com/kf/yidong/wp/)A2-PSK
  

uci set wireless.@wifi-iface[0].key=[密码]    //设置无线密码  



提交应用配置 

Shell代码  

uci commit    //应用  

/etc/init.d/network restart    //重启网络服务  





安装luci管理界面 

Shell代码  

opkg update // 更新软件列表  

opkg list-installed // 查看已安装软件  

opkg install luci // 安装LUCI  

opkg install luci-i18n-chinese // 支持中文  


luci-app-firewall - 0.10.0-1

luci-i18n-english - 0.10.0-1

luci-lib-core - 0.10.0-1

luci-lib-ipkg - 0.10.0-1

luci-lib-lmo - 0.10.0-1

luci-lib-nixio - 0.10.0-1

luci-lib-sys - 0.10.0-1

luci-lib-web - 0.10.0-1

luci-mod-admin-core - 0.10.0-1

luci-mod-admin-full - 0.10.0-1

luci-proto-core - 0.10.0-1

luci-proto-ppp - 0.10.0-1

luci-sgi-cgi - 0.10.0-1

luci-theme-base - 0.10.0-1

luci-theme-openwrt - 0.10.0-1




即可完成LUCI的安装。 

输入以下命令开启支持web服务的uhttpd，并设置其为自启动： 

Shell代码  

/etc/init.d/uhttpd enable # 开机自启动  

/etc/init.d/uhttpd start # 启动uhttpd  


- Wifidog

你可以尝试执行以下命令安装Wifidog：
  opkg update # Optional

  opkg install wifidog


--sftp安装

opkg update

opkg install vsftpd openssh-sftp-server

/etc/init.d/vsftpd enable

/etc/init.d/vsftpd start






