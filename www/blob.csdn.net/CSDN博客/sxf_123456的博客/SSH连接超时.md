# SSH连接超时 - sxf_123456的博客 - CSDN博客
2018年03月05日 11:04:02[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：146
有2个方法 
1 配置服务器 
打开 
/etc/ssh/sshd_config 
找到 
ClientAliveInterval 
参数，如果没有就自己加一行 
数值是秒，比如你设置为540，就是9分钟. 
ClientAliveInterval 540 
对于 
ClientAliveCountMax 
指如果发现客户端没有相应，则判断一次超时，这个参数设置允许超时的次数。比如10 
ClientAliveInterval 540 
ClientAliveCountMax 10; 
则代表允许超时 5400秒 = 90分钟 
2 配置客户端 
编辑 
/etc/ssh/ssh_config 
如果是Windows用户，你自己在安装目录找吧 
然后找到里面的 
ServerAliveInterval 
参数，如果没有你同样自己加一个就好了 
参数意义相同，都是秒数，比如9分钟 
ServerAliveInterval 540
最后记得要执行一遍：service sshd reload
第二种答案：
有三种情况
1、server端
你修改/etc/ssh/sshd_config，增加ServerAliveInterval 60
2、client端，不修改服务器或防火墙在客户端自动发消息最简单
如果用SecureCRT可以在Options/SessionOptions/Terminal/Anti-idel/Send string输入个空格，后面根据实际情况设置时间间隔
![](https://p.ssl.qhimg.com/t0148375fd998624113.png)
3、如果网路出口有防火墙也会存在timeout的设置。
4、也可以用screen命令解决超时退出的问题。
a）执行screen,执行你的命令，按CTRL+A+D退出，按CTRL+D退出shell
b）登录linux,screen -r 恢复
