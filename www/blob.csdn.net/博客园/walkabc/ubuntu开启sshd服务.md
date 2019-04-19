# ubuntu开启sshd服务 - walkabc - 博客园
# [ubuntu开启sshd服务](https://www.cnblogs.com/hitfire/articles/4526181.html)
## SSH分客户端openssh-client和openssh-server
*如果你只是想登陆别的机器的SSH只需要安装openssh-client（ubuntu有默认安装，如果没有则sudo apt-get install openssh-client），如果要使本机开放SSH服务就需要安装openssh-serversudo apt-get install openssh-server然后确认sshserver是否启动了：ps -e |grep ssh如果看到sshd那说明ssh-server已经启动了。如果没有则可以这样启动：sudo /etc/init.d/ssh startssh-server配置文件位于/ etc/ssh/sshd_config，在这里可以定义SSH的服务端口，默认端口是22，你可以自己定义成其他端口号，如222。然后重启SSH服务：sudo /etc/init.d/ssh stopsudo /etc/init.d/ssh start然后使用以下方式登陆SSH：ssh tuns@192.168.0.100 tuns为192.168.0.100机器上的用户，需要输入密码。*
