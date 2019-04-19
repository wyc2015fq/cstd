# The VPN client agent was unable to create the interprocess communication depot. - 三少GG - CSDN博客
2013年07月19日 23:46:04[三少GG](https://me.csdn.net/scut1135)阅读数：9665
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)

# 安裝 AnyConnect 時出現錯誤訊息「The VPN client agent was unable to create the interprocess communication depot.」
如果您在安裝 AnyConnect 軟體時，若出現「The VPN client agent was unable to create the interprocess communication
 depot.」錯誤訊息無法建立連線，請嘗試以下解決方法處理該問題。
### 解決方法
主要是透過取消「網際網路連線共用」，可以使用以下兩種方法：
- 
針對個別網路介面設定，取消勾選「允許其他網路使用者透過這台電腦的網際網路連線來連線(N)」。 
![](http://net.nthu.edu.tw/2009/_media/faq:sslvpn:nic.png?w=&h=&cache=cache)
- 
系統全域設定，**停止並取消「Internet Connection Sharing (ICS)」服務。 **
![](http://net.nthu.edu.tw/2009/_media/faq:sslvpn:isc.png?w=600&h=&cache=cache)
### 補充資料
如果您仍然需要使用「網際網路連線共用」功能，請勿將 VPN client 軟體安裝在 ICS 主機上而是安裝在一般電腦上，相關說明可以參考以下文件： 
- 
[VPN
 Client FAQ - Cisco Systems](http://cisco.biz/en/US/products/sw/secursw/ps2308/products_qanda_item09186a00801c2dbe.shtml)
參考問題：Can the Cisco VPN Client work with Microsoft Internet Connection Sharing (ICS) installed
 on the same machine? [連線示意圖](http://www.cisco.com/image/gif/paws/45102/vpnclientfaq.gif)
### 參考文件
- 
[Anyconnect
 Vista install error](https://supportforums.cisco.com/thread/264679?tstart=0&viewcondensed)
- 
[WiscVPN
 (Win) - Interprocess Communication Depot Error](http://kb.wisc.edu/helpdesk/page.php?id=12713)
- 
[How
 to Install, Configure and Use the Cisco AnyConnect VPN for Windows 7, Vista and XP - Known Issue](http://www.helpdesk.umd.edu/documents/4/4052/#issue)
- 
[Foutmeldingen
 bij installatie en gebruik van de UGent Cisco ASA VPN (Vista or Win7)](http://helpdesk.ugent.be/vpn/faq_asa.php?id=7)
- 
[Foutmeldingen
 bij installatie en gebruik van de UGent Cisco ASA VPN (Windows XP)](http://helpdesk.ugent.be/vpn/faq_asa.php?id=8)

## [Cisco AnyConnect VPN 在win8下不能正常连接解决方案](http://www.71j.cn/archives/324)[5+](http://www.71j.cn/archives/324#comments)
7,871 views / 2012.11.02 / 1:01 上午
环境: 公司VPN+win8 英文版
升级了win8后，用着正爽，结果不能连接公司的vpn，郁闷之极。查了好多资料，包括：
修改注册表法:[Cisco AnyConnect VPN在Win8预览版上的调教记](http://www.idndx.com/posts/get-cisco-anyconnect-vpn-work-on-win8-cp.html)
添加证书法: [Cisco AnyConnect and Windows 8](http://msitpros.com/?p=1204)
问题依然没有解决,检查更新后就提示连接失败了.最后得出解决方法:下载最新的包安装即可:
http://71j.cn/mupload/anyconnect-win-3.1.00495-k9.rar
安装路径：
anyconnect-win-3.1.00495-k9\binaries\anyconnect-win-3.1.00495-web-deploy-k9.exe
anyconnect-win-3.1.00495-k9\binaries\anyconnect-dart-win-3.1.00495-k9.msi
两个文件.
下面展示杜工的战果：
![](http://www.71j.cn/wp-content/uploads/2012/11/QQ%E6%88%AA%E5%9B%BE20121102010753.jpg)注意事项：
1.软件安装时候需要以管理员模式运行
2.”The VPN client agent was unable to create the interprocess communication depot”解决方法参照：[http://net.nthu.edu.tw/2009/faq:vpn_ipc_depot_error](http://net.nthu.edu.tw/2009/faq:vpn_ipc_depot_error)
