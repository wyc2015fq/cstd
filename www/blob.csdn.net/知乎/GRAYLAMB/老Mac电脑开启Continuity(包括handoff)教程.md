# 老Mac电脑开启Continuity(包括handoff)教程 - 知乎
# 

苹果Mac电脑使用Mac OS操作系统，从Mac OS X 10.10（Yomemite）开始，支持与其他苹果设备的Continuity功能。
这个功能简单理解就是打通你的所有苹果设备，比如：手机上看的一个网页、一个短信、一个邮件，都可以穿越到Mac电脑上来看，反之亦然。对于经常带着Mac的文艺青年来说，确实非常的实用，毕竟Mac屏幕大、有键盘，穿越过来编辑、阅读，用起来更加舒服些。

但是，如果你是一台老Mac，可能会变成2B青年。。因为Continuity的互联互通功能是借助蓝牙4.0版模块实现的。如果你购买的Mac时，蓝牙4.0还未普及，那你很可能就无法使用这个功能：

![](https://pic3.zhimg.com/313c59175e38020fe9a6dfd4318aaf8a_b.png)而文艺青年的Mac一般都是这个样子：
![](https://pic2.zhimg.com/f82be5c64f7811755fa0d01e697578a5_b.png)

本人有一台Mac Air，是2011年购买的，那会正是蓝牙4.0刚刚普及的时候，不出所料的，装完Mac OS Yomemite版本后，悲剧了，设置里没有“使用Handoff”的小勾。但不太死心，去网上查了查，发现2011年的部分Mac其实是蓝牙4.0模块，只是苹果公司把Continuity功能给屏蔽掉了。具体情况如下：
![](https://pic3.zhimg.com/343d2b966269a70cad664a773e4b530a_b.png)

我的Mac Air刚好是2011年下半年买的，也就是上图中的Mid 2011，于是开始尝试激活Continuity功能。

下载上图中提到的CAT软件（Continuity Activation Tool），就可以激活了，激活的办法很简单，运行这个软件：

![](https://pic3.zhimg.com/3fcbb07b90fb7cbe86f5e2eb3524e2a2_b.png)主菜单中，先选择2，进行系统诊断：

然后只要诊断结果中没有not OK（见下图），就可以回到主菜单选择1进行激活了。
![](https://pic1.zhimg.com/4df1b54aff34cd4491df682e775b8d48_b.png)
激活基本是一键操作，激活完成后重启一下，“使用Handoff”的小勾就出现了，这里就不做太多介绍了。

当然故事没有完，去年苹果公司发布了新版的操作系统Mac OS 10.11 EI Capitan，好消息是，针对新操作系统的CAT工具也发布了，但在激活上有一个小变化。

首先，还是进入CAT工具，选择2进行诊断。

这时会发现，有两个内容没有写ok

![](https://pic1.zhimg.com/38a3831490475270b168a20413058680_b.png)第一个是OS X版本，是个warning（警告），这个基本可以忽略，主要是提示你，你现在Mac上的OS版本CAT工具官方不承诺支持。但我已经测试过最新的10.11.5版本，都是完美支持的。

第二个是SIP，写的是not ok。这个需要我们特殊设置一下才能进行下一步激活操作。

SIP是什么呢？SIP是System Integrity Protection的缩写，是10.11版本OS中新增加的安全机制，简单解释就是限制了root用户的权限，这样你即便已经使用了root用户，但部分对于操作系统核心目录和文件的操作仍然是没有权限做的。好在有办法把它关掉。

关闭SIP的办法：

重启电脑，同时按住command+R键，进入Mac电脑的恢复模式，

恢复模式下，打开终端功能：

![](https://pic1.zhimg.com/574076f20172ad500b60621ae29a0028_b.png)然后在终端窗口中输入：csrutil disable

这时，会提示关闭了System Integrity Protection，请重新启动。

![](https://pic1.zhimg.com/ae1b5c542b488b4155cb48a0c842f828_b.png)重启后，再运行CAT工具进行检查：

![](https://pic1.zhimg.com/1e4a96b0c59618796e2b727090a160d4_b.png)

现在已经正常了，然后如以往，会主菜单选择1激活即可。

（建议激活后，进入恢复模式，将SIP重新启用，这样保证你的电脑是安全的~

重新启用SIP的命令是：csrutil enable）

最后提示一点，如果你已经关闭了SIP，但是在激活过程中仍然提示你要关闭SIP（遇到如下提示）：

To continue you need to disable System Integrity Protection and come back here.
1. Reboot and hold CMD + R
2. Utilities - Terminal
3. enter 'csrutil disable'
4. reboot

那说明你没有使用最新版本的CAT工具（这个问题困扰了我很久，最后在github论坛找到了答案），最新版工具地址如下

[https://github.com/dokterdok/Continuity-Activation-Tool/archive/master.zip](https://link.zhihu.com/?target=https%3A//github.com/dokterdok/Continuity-Activation-Tool/archive/master.zip)

最新版工具版本号是2.2.3
![](https://pic2.zhimg.com/9e0c8b1033abeb97d439f2b978a4a7a5_b.png)
（以上版本描述及下载地址写于16年6月11日，Mac OS 10.11.5版本下已成功激活。**此外需说明一点，本人在Mac OS 10.10版本下激活时，使用的是CAT 2.0.0版本，对于CAT 2.2.3版本是否能够成功激活Mac OS 10.10的continuity功能，本人不做保证**）

好啦，现在可以用你的老Mac电脑享受Continuity（包括handoff）和备忘录云同步（Mac OS 10.11 EI Capitan的新特性）等功能啦~

（完）

