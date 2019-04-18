# Cisco IOS 设备的操作与使用（以Cisco Packet Tracer  Student为例）思科路由器密码忘记怎么办？ - csdn_baotai的博客 - CSDN博客

2019年03月07日 20:06:38[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：58


## 思科路由器密码忘记怎么办？
- 路由器密码遗忘恢复步骤

路由器断电
- 启动的时候按住ctrl+C进入监视模式

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019030720061827.png)
- 
输入confreg 0x2142 然后 reset重启设备

- 
等设备正常重启完成后，跳过配置向导

- 
进入特权模式输入Router#copy startup-config running-config

- 
然后可以修改原来的密码后，保存（write）

- 
进入全局配置模式修改原来的寄存器值：config-register 0x2102

