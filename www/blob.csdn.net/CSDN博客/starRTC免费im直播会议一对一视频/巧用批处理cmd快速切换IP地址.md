# 巧用批处理cmd快速切换IP地址 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月29日 09:58:03[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1082
如果你的笔记本经常在不同的地方使用，有些地方需要自动获取IP，而有些地方需要配置固定IP，每换一个地方都需要重新配置一遍，是不是感觉很麻烦呢？
下面介绍一种通过建立批处理文件来快速切换IP的方法：
@echo **off**
title 【IP地址快速切换】
color 84
set /p mode=请选择上网模式：1为自动获取，2为手动配置：
if%mode% EQU 1(
echo 正在配置IP地址为动态获取，请耐心等候......
netsh interface ip set address name="本地连接" source=DHCP
echo 正在配置DNS为动态获取，请耐心等候......
netsh interface ip set dns name="本地连接" source=DHCP
)elseif%mode% EQU 2(
set /p myip=请输入IP地址最后一位：
goto a
**:a**
echo 正在配置IP，请耐心等候......
netsh interface ip set address name="本地连接" source=static 192.168.0.%myip% 255.255.255.0 192.168.0.1 1@echo 正在设置DNS，请耐心等候......
netsh interface ip set dns name="本地连接" static 192.168.0.1 primary
)pause
把上面的内容复制到记事本里保存，然后把扩展名由txt改为bat，换IP时双击这个文件即可。
效果图：
手动配置IP前，IP地址为自动获取：
开始手动配置IP：
配置完成后：
自动获取IP：
自动获取IP之后：
