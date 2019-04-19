# vmware下设置host-only方式上网----本人亲自测试成功 - xqhrs232的专栏 - CSDN博客
2017年08月15日 15:03:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：396
原文地址::[http://jingyan.baidu.com/article/d8072ac497452aec95cefda5.html](http://jingyan.baidu.com/article/d8072ac497452aec95cefda5.html)
vmware通过桥接的方式可以上外网，但是虚拟机的IP地址必须和物理网卡在同一网段，上网环境不同虚拟机必须跟着换ip地址很麻烦，所以最好是采用host-only方式上网。实验前提如图，虚拟机采用host-only主机模式
![vmware下设置host-only方式上网](http://a.hiphotos.baidu.com/exp/w=500/sign=ba6ff7f903d162d985ee621c21dfa950/8644ebf81a4c510f619e5b276959252dd42aa5b1.jpg)
## 工具/原料
- 
vmware workstation
- 
centos6.5_x64
## 方法/步骤
- 
“编辑”>“虚拟网络编辑器”这一步是确认虚拟网卡host-only的网卡名称VMnet1以及网段192.168.4.0
![vmware下设置host-only方式上网](http://a.hiphotos.baidu.com/exp/w=500/sign=feede21bf4faaf5184e381bfbc5594ed/960a304e251f95cae13ed88bc0177f3e660952c7.jpg)
![vmware下设置host-only方式上网](http://a.hiphotos.baidu.com/exp/w=500/sign=a1bd3f3c556034a829e2b881fb1249d9/9e3df8dcd100baa1f589f1864e10b912c8fc2e61.jpg)
- 
本地连接上点击鼠标右键“属性”，图2是无线方式上网
![vmware下设置host-only方式上网](http://h.hiphotos.baidu.com/exp/w=500/sign=f7a6c32e00f79052ef1f473e3cf3d738/5243fbf2b2119313677c4f8d6c380cd791238dd4.jpg)
![vmware下设置host-only方式上网](http://e.hiphotos.baidu.com/exp/w=500/sign=ce21e03505d79123e0e094749d355917/fcfaaf51f3deb48f88f511e3f91f3a292df5786a.jpg)
- 
“共享”，选择host-only那块网卡，点击确认之后VMnet1网卡ip会变成192.168.137.1
![vmware下设置host-only方式上网](http://d.hiphotos.baidu.com/exp/w=500/sign=0922dca2ad0f4bfb8cd09e54334f788f/9f2f070828381f3062043e94a0014c086e06f0ea.jpg)
- 
鼠标右键将VMnet1还原成图1中的192.168.4.0段
![vmware下设置host-only方式上网](http://e.hiphotos.baidu.com/exp/w=500/sign=ebe2520a3cfae6cd0cb4ab613fb20f9e/3812b31bb051f819fad1b077d3b44aed2e73e713.jpg)
- 
设置虚拟机的ip为192.168.4.2，网关和DNS都指向192.168.4.1
![vmware下设置host-only方式上网](http://a.hiphotos.baidu.com/exp/w=500/sign=e6c41a1bf4faaf5184e381bfbc5594ed/960a304e251f95caf917208bc0177f3e67095228.jpg)
- 
6
虚拟机通过host-only网段上网了
![vmware下设置host-only方式上网](http://e.hiphotos.baidu.com/exp/w=500/sign=e45675124834970a4773102fa5cbd1c0/38dbb6fd5266d01645a123159e2bd40735fa3556.jpg)步骤阅读
END

