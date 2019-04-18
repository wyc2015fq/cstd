# ubuntu重启、关机命令 - z69183787的专栏 - CSDN博客
2014年05月22日 09:46:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：23183
重启命令(root用户使用) ： 
    1、reboot 
    2、shutdown -r now 立刻重启
    3、shutdown -r 10 过10分钟自动重启
    4、shutdown -r 20:35 在时间为20:35时候重启
    如果是通过shutdown命令设置重启的话，可以用shutdown -c命令取消重启 
    关机命令(root用户使用) ： 
    1、halt   立刻关机 
    2、poweroff 立刻关机 
    3、shutdown -h now 立刻关机
    4、shutdown -h 10 10分钟后自动关机 
    如果是通过shutdown命令设置关机的话，可以用shutdown -c命令取消重启
