# 【RemoteJoy】PSP图像采集方案 - CD's Coding - CSDN博客





2018年03月25日 23:08:47[糖果天王](https://me.csdn.net/okcd00)阅读数：184








## 0x00 前言

只是简单的想给别人看看自己PSP1000（没错就是很元老级）里的命运石之门和刀剑神域里的画面。 

图像采集？难道很难么？ 

试试看吧，好久没有探索新技能了
## 0x01 准备
- RemoteJoyLite 
- [https://download.csdn.net/download/okcd00/10308078](https://download.csdn.net/download/okcd00/10308078)
- 可以自行下载，感觉网上还是有蛮多版本的，我这里用的是019

- 一台PSP（本人的是PSP1000）
- 一台电脑（本人的是Win10 x64）
- 如果同时需要采集音频，一根音频线

## 0x02 实际操作
- 使用USB接口链接PSP和电脑
- 将以下文件拷入PSP根目录下的seplugins文件夹 
- RemoteJoyLite.prx
- usb.prx

- 然后在seplugins文件夹的 `GAME.TXT` 和 `VSH.TXT` 中添加如下两行 
- ms0:/seplugins/usb.prx 1
- ms0:/seplugins/RemoteJoyLite.prx 1

- 然后拔掉USB线，重启（这一步如果你是高贵的PSP2K或者PSP3K似乎不需要） 
- 这里的重启指的是长推电源关机，然后按住R键不放开机进入恢复模式
- 然后在plugins中找到`RemoteJoyLite`和`usb`这两项，改成Enable
- 好了可以开机了，随便点开一个游戏吧

- 进了游戏之后，用USB线连上电脑，然后会发现新硬件请装驱动 
- 这里的驱动叫做`PSP TypeB`，可以在管理—设备管理器中看到
- 驱动在Driver文件夹里，更新驱动—手动选择驱动程序—选择Driver（勾上包括子文件夹）
- 这时如果你是高贵的Win10，保护措施会让你无法安装没有数字签名的驱动的
- 嗯，别找了我也找了一年，PSP的驱动就是没有数字签名的，哪里来的驱动都没有

- 没办法，管理员权限调用开始 
- 打开命令提示符（管理员），键入`bcdedit /set testsigning on`
- 重启电脑，会发现进入了测试模式（屏幕右下角有测试模式几个白字）
- 然后你再装装看，是不是这次不会一口咬定不让安装了？两个选项： 
- 哇好危险我还是不装了
- 我不管我就要装

- 选择后一个，安装完成 
- 如果你觉得测试模式这几个字看的难受 
- 键入`bcdedit /set testsigning off`，重启

- 安装完成之后PSP就可以被电脑识别了

- 然后是电脑端，打开RemoteJoyLite.exe 
- 如果没什么问题的话你已经可以看到PSP游戏画面了

- 哦然后是声音 
- 需要声音的可以买一个音频线，就是两头都是插耳机的那种头的线，一头给PSP，一头给电脑


## 0x03 实际效果

![Test1](https://img-blog.csdn.net/20180325230633334?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![这里写图片描述](https://img-blog.csdn.net/20180325230752332?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





