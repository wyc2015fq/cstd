# WannaCry勒索病毒处理指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月15日 11:26:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：381
北京时间2017年5月12日晚，勒索软件"WannaCry"感染事件在全球范围内爆发，被攻击者电脑中的文件被加密，被要求支付赎金以解密文件；

1、开机前断网
如果电脑插了网线，则先拔掉网线；如果是通过路由器连接wifi，则先关闭路由器。
2、备份重要文档
将电脑中的重要文档拷贝至移动硬盘或U盘备份。
cmd 命令后输入命令”netstat -na“
可以发现当前系统开放了135、445以及5357端口，而且从状态看都处于监听状态”Listening“
本指南提供A、B两种方案，选择任意一种方案即可。如对操作系统不熟悉，推荐使用A方案。
**>>>>**方案A：**（推荐方案）**使用腾讯电脑管家勒索病毒免疫工具
- 
下载地址： http://dlied6.qq.com/invc/xfspeed/qqpcmgr/download/VulDetector.exe
- 
使用方案：下载后直接运行。
**>>>>**方案B：手动关闭端口，下载安装补丁
- 在微软官方页面下载操作系统补丁
http://www.catalog.update.microsoft.com/Search.aspx?q=KB4012598
https://technet.microsoft.com/zh-cn/library/security/ms17-010.aspx
- 利用防火墙添加规则屏蔽入口
● 开始菜单 > 打开控制面板 > 选择Windows防火墙
● 如果防火墙没有开启，点击“启动或关闭 Windows防火墙”启用。
● 点击“高级设置”，然后左侧点击“入站规则”，再点击右侧“新建规则”。
● 在打开窗口哦选择要创建的规则类型为“端口”，并点击“下一步”。
● 在“特定本地端口”处填入445并点击“下一步”，选择“阻止连接”，一直点击“下一步”，并给规则任意命名后点击完成即可。
注：不同系统可能有些差异，不过操作类似

参考：
[http://mp.weixin.qq.com/s/ojv_ogcJXL1p6mo3Mw9EUQ](https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxcheckurl?requrl=http%3A%2F%2Fmp.weixin.qq.com%2Fs%2Fojv_ogcJXL1p6mo3Mw9EUQ&skey=%40crypt_cc7292f1_15d56e90b24bb5936dd83c080bc03762&deviceid=e329208207559353&pass_ticket=ulj4hvX55%252FIldovJDgLdnRdOLc2Gcm7SpV8whAWYqpieCIA4f7XZLZEkHutFZGVp&opcode=2&scene=1&username=@86aa798c4b5d2b8aa52d6917c6c136df3305aa850199665009a80ac55839c683)
