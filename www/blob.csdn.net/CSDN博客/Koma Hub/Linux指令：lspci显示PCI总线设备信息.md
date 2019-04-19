# Linux指令：lspci显示PCI总线设备信息 - Koma Hub - CSDN博客
2018年05月07日 14:10:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1695
lspci：显示所有的PCI总线设备信息。
下面试lspci的全部功能介绍：
```python
$ lspci -
Usage: lspci [<switches>]
Basic display modes:
-mm		Produce machine-readable output (single -m for an obsolete format)
		#生成机器可读的输出（单个-m用于过时的格式）
-t		Show bus tree
		#显示巴士树
Display options:
-v		Be verbose (-vv for very verbose)
		#详细（-vv非常详细）
-k		Show kernel drivers handling each device
		#显示处理每个设备的内核驱动
-x		Show hex-dump of the standard part of the config space
		#显示配置空间的标准部分的十六进制转储
-xxx		Show hex-dump of the whole config space (dangerous; root only)
		#显示整个配置空间的十六进制转储（危险;只有root）
-xxxx		Show hex-dump of the 4096-byte extended config space (root only)
		#显示4096字节扩展配置空间的十六进制转储（仅限root用户）
-b		Bus-centric view (addresses and IRQ's as seen by the bus)
		#以总线为中心的视图（巴士所看到的地址和IRQ）
-D		Always show domain numbers
		#始终显示域名编号
Resolving of device ID's to names:
-n		Show numeric ID's#将设备ID解析为名称：
-nn		Show both textual and numeric ID's (names & numbers)
		#显示文本和数字ID（姓名和号码）
-q		Query the PCI ID database for unknown ID's via DNS
		#通过DNS查询未知ID的PCI ID数据库
-qq		As above, but re-query locally cached entries
		#如上所述，但重新查询本地缓存条目
-Q		Query the PCI ID database for all ID's via DNS
		#通过DNS查询所有ID的PCI ID数据库
Selection of devices:
-s [[[[<domain>]:]<bus>]:][<slot>][.[<func>]]	Show only devices in selected slots
						#仅显示选定插槽中的设备
-d [<vendor>]:[<device>][:<class>]		Show only devices with specified ID's
						#仅显示具有指定ID的设备
Other options:
-i <file>	Use specified ID database instead of /usr/share/hwdata/pci.ids
		#使用指定的ID数据库而不是/usr/share/hwdata/pci.ids
-p <file>	Look up kernel modules in a given file instead of default modules.pcimap
		#查找给定文件中的内核模块而不是默认的modules.pcimap
-M		Enable `bus mapping' mode (dangerous; root only)
		#启用`总线映射'模式（危险;只有root）
PCI access options:
-A <method>	Use the specified PCI access method (see `-A help' for a list)
		#使用指定的PCI访问方法（请参阅`-A help'获取列表）
-O <par>=<val>	Set PCI access parameter (see `-O help' for a list)
		#设置PCI访问参数（请参阅“-O帮助”获取列表）
-G		Enable PCI access debugging
		#启用PCI访问调试
-H <mode>	Use direct hardware access (<mode> = 1 or 2)
		#使用直接硬件访问（<mode> = 1或2）
-F <file>	Read PCI configuration dump from a given file
		#从给定文件读取PCI配置转储
```
简单的几个例子：（grep指令：[https://blog.csdn.net/rong_toa/article/details/79799127](https://blog.csdn.net/rong_toa/article/details/79799127)）
```python
#查看显卡信息
$ lspci | grep -i vga
00:0f.0 VGA compatible controller: VMware SVGA II Adapter
#只显示制定槽中的设备
$ lspci -s 00:0f.0
00:0f.0 VGA compatible controller: VMware SVGA II Adapter
#用机器可读的形式输出
$ lspci -mm -s 00:0f.0
00:0f.0 "VGA compatible controller" "VMware" "SVGA II Adapter" "VMware" "SVGA II Adapter"
$ lspci -Q -s 00:0f.0
00:0f.0 VGA compatible controller: VMware SVGA II Adapter
$ lspci -D -s 00:0f.0
0000:00:0f.0 VGA compatible controller: VMware SVGA II Adapter
```
查看详细信息：
```python
$ lspci -v -s 00:0f.0
00:0f.0 VGA compatible controller: VMware SVGA II Adapter (prog-if 00 [VGA controller])
	Subsystem: VMware SVGA II Adapter
	Flags: bus master, medium devsel, latency 64, IRQ 16
	I/O ports at 1070 [size=16]
	Memory at e8000000 (32-bit, prefetchable) [size=128M]
	Memory at fe000000 (32-bit, non-prefetchable) [size=8M]
	[virtual] Expansion ROM at c0000000 [disabled] [size=32K]
	Capabilities: <access denied>
	Kernel driver in use: vmwgfx
	Kernel modules: vmwgfx
```
