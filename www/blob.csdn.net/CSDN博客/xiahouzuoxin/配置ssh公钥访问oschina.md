# 配置ssh公钥访问oschina - xiahouzuoxin - CSDN博客





2014年02月20日 13:42:15[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：12999








之前使用Github，感觉国内速度很慢，于是搜索国内的git服务器，发现了oschina，试用一下，速度和效果都很满意，于是准备迁移到oschina。但之前一直无法使用ssh配置访问oschina，尝试配置多次都没成功！使用老土的http协议，内疚。。。。。。

为此还是打算下决心配置一番，这里总结ssh的公钥配置过程及注意事项。我使用的是Cygwin环境，Cygwin中得安装好ssh工具，详细方法如下：




**===================================================**

转到.ssh目录下（若没有则新建一个），生成的公钥将放到该目录下，


cd ~/.ssh/


使用ssh-kengen工具生成公钥（public key)，


ssh-keygen-trsa-C"xiahouzuoxin@163.com"#Createsanewsshkeyusingtheprovidedemail# Generating public/private rsa key pair...


xiahouzuoxin@163.com是您注册时使用的邮箱名。注意会弹出要输入文件名及密码等，直接敲Enter即可。

![](https://img-blog.csdn.net/20140220133131468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


查看生成的公钥，并将公钥内容拷贝添加到Git @ OSC [http://git.oschina.net/keys](http://git.oschina.net/keys)


cat~/.ssh/id_rsa.pub# ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC6eNtGpNGwstc....


注意拷贝过程中不能多一个空格，也不能少一个空格，格式等必须和生成的公钥一致（不要用Windows自带的记事本打开复制）。

通过如下的命令可以查看是否设置公钥成功，


ssh -T git@git.oschina.net


如果成功则返回提示信息，

![](https://img-blog.csdn.net/20140220134003015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





有关公钥设置的详细还可参考[我也用github](http://blog.csdn.net/xiahouzuoxin/article/details/9393119)



