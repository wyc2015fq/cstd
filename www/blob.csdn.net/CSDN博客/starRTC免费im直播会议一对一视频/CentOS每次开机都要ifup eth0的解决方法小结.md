# CentOS每次开机都要ifup eth0的解决方法小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月12日 09:07:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1223
**vim ****/**etc**/**sysconfig**/**network-scripts**/**ifcfg-eth0
把里面的 ONBOOT 设为 yes 即可。
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=CentOS%E6%AF%8F%E6%AC%A1%E5%BC%80%E6%9C%BA%E9%83%BD%E8%A6%81ifup_eth0%E7%9A%84%E8%A7%A3%E5%86%B3%E6%96%B9%E6%B3%95%E5%B0%8F%E7%BB%93&action=edit&section=1)]艺搜参考
[http://www.jbxue.com/LINUXjishu/4810.html](http://www.jbxue.com/LINUXjishu/4810.html)
