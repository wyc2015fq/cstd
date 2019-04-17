# linux死机不强制关机解决方法 - miner_zhu的博客 - CSDN博客





2018年08月08日 18:06:03[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：1073








## 可尝试的解决方法

1. 进入TTY终端
- 
Ctrl+Alt+F1进入TTY1终端字符界面, 输入用户名和密码以登录

- 
输入top命令, 找到可能造成假死的进程, 用kill命令结束掉进程。然后Ctrl+Alt+F7回到桌面


2. 直接注销用户

Ctrl+Alt+F1进入TTY1终端字符界面, 输入用户名和密码以登录。

然后执行以下的任意一个命令注销桌面重新登录。

```
sudo pkill Xorg
```

或者
`sudo restart lightdm`





