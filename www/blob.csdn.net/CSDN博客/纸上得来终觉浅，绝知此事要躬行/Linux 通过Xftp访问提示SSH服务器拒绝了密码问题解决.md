# Linux 通过Xftp访问提示SSH服务器拒绝了密码问题解决 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月27日 17:59:20[boonya](https://me.csdn.net/boonya)阅读数：9478








配置SFTP连接，使用root用户一直提示“SSH服务器拒绝了密码”。


## 错误提示效果展示

![](https://img-blog.csdn.net/20161227175211544)


## vim进行编辑处理/etc/ssh/sshd_config文件



vim编辑命令（vi filename     //打开filename文件）：



`vi /etc/ssh/sshd_config`找到如下Authentication配置，使用 i 开启编辑：




```
# Authentication:
LoginGraceTime 120
PermitRootLogin without passwd
StrictModes yes
```

注：退出编辑先按ESC按键，然后参考下面的操作命令。



:w              //保存文件

:q              //退出编辑器，如果文件已修改请使用下面的命令

:q!             //退出编辑器，且不保存

:wq            //退出编辑器，且保存文件


将上面部分改成如下所示：





```
# Authentication:
LoginGraceTime 120
PermitRootLogin yes
StrictModes yes
```



## 验证修改是否生效

重启虚拟机


`reboot`
![](https://img-blog.csdn.net/20161227175822099)


验证成功！








