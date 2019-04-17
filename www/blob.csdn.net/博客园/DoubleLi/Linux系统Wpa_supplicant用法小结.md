# Linux系统Wpa_supplicant用法小结 - DoubleLi - 博客园






Wpa_supplicant是linux系统下一个非常强大的无线网卡管理程序。google搜索到的它似乎不支持WPA2和AES，其实不然，参考它的文档可以发现，WPA2只是RSN的别名，而AES也是CCMP的一部分，所以它们的支持完全是没有问题的。

它的文档看起来非常复杂，其实配置非常简单，下面先给出我的wpa_supplication.conf





**[python]**[view plain](http://blog.csdn.net/ziyouwa/article/details/12081009)[copy](http://blog.csdn.net/ziyouwa/article/details/12081009)



- ctrl_interface=/var/run/wpa_supplicant  
- ctrl_interface_group=wheel  
- update_config=1  
- 
- network={  
-     ssid="xxxx"  
-     scan_ssid=1  
-     psk=xxxxxxxxx37bca5cf24a345f514d319211822f568bba28f8f0b74c894e7644  
-     proto=RSN  
-     key_mgmt=WPA-PSK  
-     pairwise=CCMP  
-     auth_alg=OPEN  
- }  



解释一下上面的比较容易困惑的地方：



签名三句应该是个模板，没仔细研究过。

network=开始是无线接入点的具体配置，一般的无线接入点可以用wpa_passphrase来自动生成，语法是





**[python]**[view plain](http://blog.csdn.net/ziyouwa/article/details/12081009)[copy](http://blog.csdn.net/ziyouwa/article/details/12081009)



- yj@YJ_N ~ $ wpa_passphrase  
- usage: wpa_passphrase <ssid> [passphrase]  
- 
- If passphrase is left out, it will be read from stdin  
- 
- 接下来看个实例：  
- 
- yj@YJ_N ~ $ wpa_passphrase TPLINK 12345678  
- network={  
-         ssid="TPLINK"  
- #psk="12345678"  
-         psk=992194d7a6158009bfa25773108291642f28a0c32a31ab2556a15dee97ef0dbb  
- }  
- 这里表示名为TPLINK的接入点，密码是12345678，输出就是该接入点在wpa_supplicant.conf里面的配置内容，可以直接用下面的命令自动写入  
- 
- yj@YJ_N ~ $ wpa_passphrase TPLINK 12345678 |sudo tee -a /etc/wpa_supplicant/wpa_supplicant.conf  




继续解释最开始的例子，network=后面的内容

ssid ：接入点名称，这个没什么好说的

scan_ssid=1 ：这个很容易错过，如果你的无线接入点是隐藏的，那么这个就是必须的，亲身经历，折腾了好久才搞定，记之备忘。

psk=xx :是加密后的密码，用wpa_passphrase自动生成的

proto=RSN  WPA2就选这个，抄一下官方配置文件例子里面的：





**[python]**[view plain](http://blog.csdn.net/ziyouwa/article/details/12081009)[copy](http://blog.csdn.net/ziyouwa/article/details/12081009)



- # proto: list of accepted protocols ---支持的协议列表  
- # WPA = WPA/IEEE 802.11i/D3.0    
- # RSN = WPA2/IEEE 802.11i (also WPA2 can be used as an alias for RSN) ---也能使用WPA2，它只是RSN的一个别名而已  
- # If not set, this defaults to: WPA RSN   ---如果不设置，默认就是WPA RSN，即全部支持  




key_mgmt= 认证密钥管理协议，还是抄：





**[python]**[view plain](http://blog.csdn.net/ziyouwa/article/details/12081009)[copy](http://blog.csdn.net/ziyouwa/article/details/12081009)



- # key_mgmt: list of accepted authenticated key management protocols  ---支持的协议列表  
- # WPA-PSK = WPA pre-shared key (this requires 'psk' field)     ---一般都是这个，这就包括了WPA、WPA2开始的那些方式  
- # WPA-EAP = WPA using EAP authentication    ---这个就是WEP开头的，猜的，求验证  
- # IEEE8021X = IEEE 802.1X using EAP authentication and (optionally) dynamically  
- #   generated WEP keys  
- # NONE = WPA is not used; plaintext or static WEP could be used  ---这个是开放的，没密码，联通、电信之类的就这个  
- # WPA-PSK-SHA256 = Like WPA-PSK but using stronger SHA256-based algorithms  
- # WPA-EAP-SHA256 = Like WPA-EAP but using stronger SHA256-based algorithms  
- # If not set, this defaults to: WPA-PSK WPA-EAP ---如果未设置，默认支持WAP、WEP开头那些  




pairwise= 这个就是加密方式，继续抄





**[python]**[view plain](http://blog.csdn.net/ziyouwa/article/details/12081009)[copy](http://blog.csdn.net/ziyouwa/article/details/12081009)



- # pairwise: list of accepted pairwise (unicast) ciphers for WPA   ---WPA可用的加密方式列表  
- # CCMP = AES in Counter mode with CBC-MAC [RFC 3610, IEEE 802.11i/D7.0]  ---看到没，这个就是AES，换了马甲而已  
- # TKIP = Temporal Key Integrity Protocol [IEEE 802.11i/D7.0] ---TKIP 这个倒是没变  
- # NONE = Use only Group Keys (deprecated, should not be included if APs support  
- #   pairwise keys)   ---这个估计很少用  
- # If not set, this defaults to: CCMP TKIP  ---不设置的话是CCMP TKIP，看似正确，其实有些路由器无法自动识别，只能二选一，很坑爹。  




好了，其他的不多说了，有兴趣看英文原文吧：

[wpa_supplicant官方配置文件(英文)](http://hostap.epitest.fi/gitweb/gitweb.cgi?p=hostap.git;a=blob_plain;f=wpa_supplicant/wpa_supplicant.conf)









