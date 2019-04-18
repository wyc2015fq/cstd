# ssh-agent使用笔记（idea terminal无法 git pull） - z69183787的专栏 - CSDN博客
2017年02月27日 17:39:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：425
ssh-agent是管理多个ssh key的代理，受管理的私钥通过ssh-add来添加
好处1：不用重复输入密码。
用 ssh-add 添加私钥时，如果私钥有密码的话，照例会被要求输入一次密码，在这之后ssh-agent可直接使用该私钥，无需再次密码认证。
好处2：不用到处部署私钥
假设私钥分别可以登录同一内网的主机 A 和主机 B，出于一些原因，不能直接登录 B。可以通过在 A 上部署私钥或者设置 Forwarding 登录 B，也可以转发认证代理连接在 A 上面使用ssh-agent私钥登录 B；可以在A上直接sftp传文件到B上。
如这边有一台机器是local，能通过公钥直接登陆server1和server2。server1和server2之间无公钥登陆。
现在要在server1上直接登陆server2，在local上执行
ssh-agent
ssh-add
接下来登陆server1，注意-A
ssh -A server1
可以发现server1上多了/tmp/ssh-xxxxxxxxx/agent.xxxxx的socket，之后神奇的事发生了，在server1上可直接进server2，只需执行如下命令，如果加了-A则可以继续ssh forwarding，以至无限的机器forwarding。
ssh (-A) server2
同样的原理可以试一下sftp, scp等基于ssh的命令。
如运行ssh-add，遇到“Could not open a connection to your authentication agent.”。
解决：需要ssh-agent启动bash，或者说把bash挂到ssh-agent下面。
ssh-agent bash --login -i
ssh-add
参考：
http://www.cnblogs.com/cheche/archive/2011/01/07/1918825.html
http://blog.pkufranky.com/2012/08/ssh-agent-forwarding-guide/
http://blog.jobbole.com/33790/
