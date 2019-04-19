# WIN7不能被远程桌面问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月07日 15:59:58[左直拳](https://me.csdn.net/leftfist)阅读数：629
不知从何时起，我的机器不能被远程桌面。在其他机器远程我，最后都提示“凭据不工作”，账号和密码肯定是正确的。
我是开了远程桌面的： 
![这里写图片描述](https://img-blog.csdn.net/20170207155905965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也许是新近开了防火墙的缘故？检查防火墙，3389是开的；关掉防火墙，问题依然存在。
在网上寻寻觅觅，有人说是要设置“本地组策略编辑器”，其实呢，这个是对客户机而言，而不是对被远程桌面的主机而言的。我设了果然是不行。 
![这里写图片描述](https://img-blog.csdn.net/20170207155727495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最后找到这个有用的答案：
```
最重要一点, 主机上要允许用户以非guest身份登录:
主机上运行gpedit.msc, 
计算机配置/windows设置/安全设置/本地策略/安全选项/
网络访问:本地帐户的共享和安全模型
选择: 经典-对本地用户进行身份验证,不改变其本来身份
```
