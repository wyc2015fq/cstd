# ros自动arp绑定，并使用arp表初始化dhcp表 - weixin_33985507的博客 - CSDN博客
2009年06月25日 10:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
:foreach wbsz in=[/ip arp find dynamic=yes ] do=[/ip arp add copy-from=$wbsz]
:foreach wxy in [/ip dhcp-server lease find] do={/ip dhcp-server lease remove $wxy}
:foreach wbsz in=[/ip arp find dynamic=no ] do={/ip dhcp-server lease add address=[/ip arp get $wbsz address] mac-address=[/ip arp get $wbsz mac-address]}
