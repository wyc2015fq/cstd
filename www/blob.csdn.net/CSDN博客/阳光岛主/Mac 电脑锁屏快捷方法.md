
# Mac 电脑锁屏快捷方法 - 阳光岛主 - CSDN博客

2017年06月16日 22:55:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：23165


Mac 电脑锁屏快捷方法，尝试了很多方法，最后找到了设置触发角，进行锁屏。
本文介绍三种Mac Book Pro（Touch Bar）锁屏方法，依次是触发角、Finder 钥匙锁、Automator 快捷键。

米扑科技原创出品，转载请注明出处：[Mac 电脑锁屏快捷方法](http://blog.mimvp.com/2017/05/mac-computer-locker-shortcut/)

**锁屏方法1**
**触发角设置步骤如下：**
**触发角**，是指屏幕的四个角：左上角、左下角、右上角、右下角，指定上常用的操作，很方便的。
**1. 打开系统偏好**
左上角苹果小图标 —> 系统偏好 —> 桌面和屏幕保护程序 —> 屏幕保护 —> 触发角
![mac-computer-locker-shortcut-01](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-01.jpg)
**2. 设置触发角屏保**
屏幕的四个角分别设置了常用操作，其中，右下角设置的是“启动屏幕保护程序”
![mac-computer-locker-shortcut-02](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-02.jpg)
**3. 查看屏保效果**
触发角设置保存后，鼠标指向右下角，等待大约1-2秒钟后，启动屏保程序
![mac-computer-locker-shortcut-04](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-04.jpg)

**锁屏方法2**
Finder 钥匙锁
Finder —> 应用程序 —> 实用工具 —> 钥匙串访问
![mac-computer-locker-shortcut-06](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-06.png)
如上图，双击钥匙串访问，打开左上角的 偏好设置，见下图
![mac-computer-locker-shortcut-07](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-07.png)
如上图，勾选后，Mac屏幕最上面导航栏，多了一个小锁图标，点击即可选择锁定屏幕，如下图：
![mac-computer-locker-shortcut-08](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-08.png)

**锁屏方法3**
AutoMator 快捷键锁屏
Finder —> 应用程序 —> Automator
![mac-computer-locker-shortcut-10](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-10.png)
如上图，打开Automator菜单  —> File —> Service
![mac-computer-locker-shortcut-11](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-11.png)
新建Service，如下图，依次搜索 screen —> Start Screen Saver —> 拖动放入右侧
Service receives 选择no inputinany application
![mac-computer-locker-shortcut-12](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-12.png)
command + S，保存，命名为 homer-Screen-Saver，这样锁屏服务就创建完毕了
点击左下角 Finder —> 右上角 Finder  —>  Services  —>  Services Preferences...  —>  Services  —>  homer-Screen-Saver （自己保存的service服务名）
![mac-computer-locker-shortcut-13](http://blog.mimvp.com/wp-content/uploads/2017/05/mac-computer-locker-shortcut-13.png)
如上图，设置 homer-Screen-Saver 锁屏的快捷键，例如：Shift + command + L
这样，就可以依次通过点击Finder  —>  Services  —>  homer-Screen-Saver或 快捷键Shift + command + L进行锁屏啦
以上三种方法，都经过验证有效，依个人爱好，推荐第一种（触发角）和第三种（快捷键）


