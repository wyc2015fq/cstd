# Win10怎样关闭自动维护 - fighting！！！ - CSDN博客
2018年07月04日 19:38:33[dujiahei](https://me.csdn.net/dujiahei)阅读数：489
转自：https://jingyan.baidu.com/article/3065b3b697d59fbecff8a43a.html
在默认情况下，Win10 中的自动维护是处于打开状态的，它的作用是根据预定的计划对系统进行维护，比如说系统更新，整理磁盘碎片，或者是进行安全扫描等等。
不过，在有些时候，我们可能更愿意自己手动来进行这些维护工作，以便做到完全掌控电脑。但不幸的是，Win10 的开发者们似乎根本就没有考虑普通百姓的这个需求，在设计时并没有提供相应的关闭选项。
那么，Win10 怎样才能关闭自动维护呢？
## 工具/原料
- 
Windows 10
## 关闭方法：
- 
首先，按下快捷键 Win+R 键，打开运行窗口。
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=310610f201fa513d51aa6cde0d6c554c/5ab5c9ea15ce36d375b0061e33f33a87e950b16c.jpg)
- 
在运行窗口中输入 regedit，再点击“确定”打开注册表。
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=02e411958ed6277fe912323818391f63/472309f790529822f7ebbceddeca7bcb0b46d4c0.jpg)
- 
打开注册表以后，进入到以下位置：
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Schedule\Maintenance
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=497c782379cf3bc7e800cdece101babd/9f510fb30f2442a7f746798bd843ad4bd1130257.jpg)
- 
然后在该处右击鼠标，选择新建一个 DWORD 类型的 32 位值。
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=ca7e631eb3a1cd1105b672208913c8b0/c995d143ad4bd1134152301c53afa40f4bfb0504.jpg)
- 
再将新建的值改名为 MaintenanceDisabled。
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=8d207b09763e6709be0045ff0bc69fb8/34fae6cd7b899e512515bd3a4ba7d933c8950d00.jpg)
- 
双击这个新建的 MaintenanceDisabled，并将其值改为 1。现在，一切就大功告成了，保存即可关闭自动维护功能。
![Win10怎样关闭自动维护](https://imgsa.baidu.com/exp/w=500/sign=849e01a9d9a20cf44690fedf46094b0c/3b292df5e0fe9925612def7e3da85edf8db171ff.jpg)
- 
