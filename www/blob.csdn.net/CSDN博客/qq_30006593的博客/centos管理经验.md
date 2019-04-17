# centos管理经验 - qq_30006593的博客 - CSDN博客





2017年10月27日 16:27:07[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：177








# 服务设置

## 问题
- 服务器变慢：尝试重启

## 命令

systemctl start/stop/restart httpd.service 

systemctl enable/disable httpd.service 

systemctl list-units –type=service
## 常见服务
- gdm: 桌面系统
- sshd：远程登录，必备。
- cups：打印系统，服务器无用。
- udisks2：应该是管理硬盘的
- packagekit：红帽提供PackageKit查看、管理、更新、安装和卸载包与你的系统兼容。PackageKit由几种图形界面可以从GNOME面板菜单中打开,或通知区域当PackageKit提醒你更新是可用的。 无用
- wpa_supplicant：无线认证，无用
- colord：管理颜色的，和桌面系统配合
- upower：能源管理，开启
- crond：计划认证管理，开启
- atd：在指定时间执行命令，与crond重复
- httpd：apache，web服务器用
- libvirtd：虚拟机KVM
- cups：通用unix打印服务，服务器无用
- tuned：系统调优工具
- postfix：邮件服务，无用
- rsyslog：log用
- ksmtuned：KSM is a new Linux feature which uses this concept in reverse. KSM enables the kernel to examine two or more already running programs and compare their memory. If any memory regions or pages are identical, KSM reduces multiple identical memory pages to a single page. 
- firewalld：防火墙，centos7代替了iptables




