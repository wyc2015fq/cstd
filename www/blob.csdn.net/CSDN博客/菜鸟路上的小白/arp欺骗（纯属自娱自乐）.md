# arp欺骗（纯属自娱自乐） - 菜鸟路上的小白 - CSDN博客





2017年01月11日 15:36:00[somnus_小凯](https://me.csdn.net/u012486840)阅读数：321











# 1局域网IP地址查询

![](https://img-blog.csdn.net/20170111152455271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


使用方法: fping [选项] [目标...]


-a显示是活着的目标


-A 显示目标地址


-b n 大量 ping 数据要发送，以字节为单位 (默认 56)


-B f 将指数退避算法因子设置为 f


-c n 的计数的 ping 命令发送到每个目标 (默认为 1)


-C n-c，相同报告结果在冗长的格式


-D 打印前每个输出行的时间戳


-e 显示经过的时间上返回数据包


-f 文件读取列表中的目标是从一个文件 (-意味着 stdin) (仅当没有-g 指定)


-g 生成目标列表 (仅当没有-f 指定)


(指定的开始和结束 IP 在目标列表中，或提供一个 IP 子网掩码)


(如: fping-g 192.168.1.0 192.168.1.255 或 fping-g 192.168.1.0/24)


-H n 设置 IP TTL (生存时间跃点)


-i n (在毫秒的时间内) 发送 ping 数据包之间的间隔 (默认为 25)


-我如果将绑定到特定的接口


-l 循环永远发送 ping 信号


-m ping 目标主机上的多个接口


-n 显示目标的名称 (-d 是等效)


-O n 设置的服务 (tos) 标志类型的 ICMP 数据包


-p n 间隔 ping 数据包到一个目标 (在毫秒的时间内)


(在循环和计数模式，默认值 1000年)


-q 安静 (不显示每个目标/每-平结果)


-Q n-q，相同但显示摘要每隔 n 秒钟


-r n 试次数 (默认 3)


-s 打印最终统计


-S 地址设置源地址


-t n 个人目标初始超时 (以毫秒的时间内) (默认 500)


-T n 忽略 (为了兼容 fping 2.4)


-u 显示是遥不可及的目标


-v 显示版本

要检查 (如果没有-f 指定) 的目标的目标列表

实例：

![](https://img-blog.csdn.net/20170111152733922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# 2arp欺骗

![](https://img-blog.csdn.net/20170111152918401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


arpspoof <ip1> <ip2> <port> <adpnum> <mode> /[r|s] <file> 

ip1,ip2 分别被欺骗的两个ip 

port 要针对哪个端口数据进行修改或嗅探 

adpnum 本机网卡的序号可以用arpspoof -l列出来 适合电脑又多个网卡时选择 

mode 可以用参数0或1 0代表单向欺骗仅欺骗从ip1发往ip2的数据1代表双向欺

骗 

/r 表示replace数据包并转发

/s 表示仅嗅探不修改数据 





实例：

![](https://img-blog.csdn.net/20170111153245472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


就是这么简单，打不开网页了，剩下的就看自己想做继续什么啦






