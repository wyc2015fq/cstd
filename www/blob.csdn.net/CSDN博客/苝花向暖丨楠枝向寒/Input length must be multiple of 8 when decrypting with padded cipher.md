# Input length must be multiple of 8 when decrypting with padded cipher - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月04日 20:01:29[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1537


今天给数据库的用户和密码加密的时候 报错 Input length must be multiple of 8 when decrypting with padded cipher

原因   ： 自己在红色箭头的位置 加了个分号，导致无论怎样都会执行![](https://img-blog.csdn.net/2018040419562235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180404195529539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分析： 报这个 错误，应该是 将未被加密过的字符串解密。  

比如root 被加密成 WMYfCpui1FY=，调用自己写的deCode（WMYfCpui1FY=） ，但由于我if加了分号，导致驱动和url也被解码了。

