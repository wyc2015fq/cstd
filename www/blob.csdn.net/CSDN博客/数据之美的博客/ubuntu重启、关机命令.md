# ubuntu重启、关机命令 - 数据之美的博客 - CSDN博客
2017年09月12日 00:25:15[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：105
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)
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
