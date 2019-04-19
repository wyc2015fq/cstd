# 查看linux系统常用的命令，Linux查看系统配置常用命令 - 数据之美的博客 - CSDN博客
2018年01月17日 10:41:56[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：105
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)

一、linux CPU大小
  cat /proc/cpuinfo |grep "model name" && cat /proc/cpuinfo |grep "physical id"
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714191834014-964804946.png)
说明：Linux下可以在/proc/cpuinfo中看到每个cpu的详细信息。但是对于双核的cpu，在cpuinfo中会看到两个cpu。常常会让人误以为是两个单核的cpu。
其实应该通过Physical Processor ID来区分单核和双核。而Physical Processor ID可以从cpuinfo或者dmesg中找到. flags 如果有 ht 说明支持超线程技术 判断物理CPU的个数可以查看physical id 的值，相同则为
二、内存大小
cat /proc/meminfo |grep MemTotal
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714192242342-87677541.png)
三、硬盘大小
fdisk -l |grep Disk
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714192353357-889932853.png)
四、 
uname -a # 查看内核/操作系统/CPU信息的linux系统信息命令
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194041920-2079682098.png)
五、head -n 1 /etc/issue # 查看操作系统版本，是数字1不是字母L
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194609576-1832936070.png)
六、cat /proc/cpuinfo # 查看CPU信息的linux系统信息命令
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194658889-256302652.png)
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194733186-1852291147.png)
七、hostname # 查看计算机名的linux系统信息命令
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194813779-2080167308.png)
八、lspci -tv # 列出所有PCI设备
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714194921920-870010221.png)
九、lsusb -tv # 列出所有USB设备的linux系统信息命令
![](https://images2015.cnblogs.com/blog/945786/201607/945786-20160714195010311-1391346792.png)
十、lsmod # 列出加载的内核模块
```
Module                  Size  Used by
autofs4                26513  3 
sunrpc                260521  1 
8021q                  25317  0 
garp                    7152  1 8021q
stp                     2218  1 garp
llc                     5546  2 garp,stp
cpufreq_ondemand       10544  2 
powernow_k8            18171  1 
freq_table              4936  2 cpufreq_ondemand,powernow_k8
mperf                   1557  1 powernow_k8
ip6t_REJECT             4628  2 
nf_conntrack_ipv6       8748  2 
nf_defrag_ipv6         11182  1 nf_conntrack_ipv6
xt_state                1492  2 
nf_conntrack           79357  2 nf_conntrack_ipv6,xt_state
ip6table_filter         2889  1 
ip6_tables             19458  1 ip6table_filter
ipv6                  321422  47 ip6t_REJECT,nf_conntrack_ipv6,nf_defrag_ipv6
ppdev                   8537  0 
parport_pc             22690  0 
parport                36209  2 ppdev,parport_pc
asus_atk0110           14216  0 
sg                     29350  0 
microcode             112653  0 
serio_raw               4594  0 
k10temp                 3609  0 
edac_core              46581  0 
edac_mce_amd           14705  0 
snd_hda_codec_realtek    68136  1 
snd_hda_intel          27823  2 
snd_hda_codec         121518  2 snd_hda_codec_realtek,snd_hda_intel
snd_hwdep               6652  1 snd_hda_codec
snd_seq                55759  1 
snd_seq_device          6500  1 snd_seq
snd_pcm                86635  2 snd_hda_intel,snd_hda_codec
snd_timer              22411  3 snd_seq,snd_pcm
snd                    69907  14 snd_hda_codec_realtek,snd_hda_intel,snd_hda_codec,snd_hwdep,snd_seq,snd_seq_device,snd_pcm,snd_timer
soundcore               7958  1 snd
snd_page_alloc          8470  2 snd_hda_intel,snd_pcm
i2c_piix4              12608  0 
r8169                  60023  0 
mii                     5376  1 r8169
shpchp                 32778  0 
ext4                  363408  3 
mbcache                 8193  1 ext4
jbd2                   90230  1 ext4
sr_mod                 15177  0 
cdrom                  39085  1 sr_mod
sd_mod                 38976  3 
crc_t10dif              1541  1 sd_mod
ahci                   41127  2 
pata_acpi               3701  0 
ata_generic             3837  0 
pata_atiixp             4211  0 
radeon                917108  0 
ttm                    80957  1 radeon
drm_kms_helper         40087  1 radeon
drm                   265638  3 radeon,ttm,drm_kms_helper
i2c_algo_bit            5935  1 radeon
i2c_core               31084  5 i2c_piix4,radeon,drm_kms_helper,drm,i2c_algo_bit
dm_mirror              14133  0 
dm_region_hash         12085  1 dm_mirror
dm_log                  9930  2 dm_mirror,dm_region_hash
dm_mod                 82839  11 dm_mirror,dm_log
```
十一、env # 查看环境变量资源
```
LESS_TERMCAP_mb=
HOSTNAME=localhost.localdomain
LESS_TERMCAP_md=
LESS_TERMCAP_me=
TERM=xterm
SHELL=/bin/bash
HISTSIZE=1000
NLS_LANG=AMERICAN_AMERICA.AL32UTF8
SSH_CLIENT=192.168.1.104 2465 22
QTDIR=/usr/lib64/qt-3.3
OLDPWD=/home
LESS_TERMCAP_ue=
QTINC=/usr/lib64/qt-3.3/include
SSH_TTY=/dev/pts/1
USER=root
LD_LIBRARY_PATH=/usr/lib/oracle/11.2/client64/lib:
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;05;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.tbz=01;31:*.tbz2=01;31:*.bz=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.axa=01;36:*.oga=01;36:*.spx=01;36:*.xspf=01;36:
ORACLE_BASE=/usr/lib/oracle/11.2
LESS_TERMCAP_us=
MAIL=/var/spool/mail/root
PATH=/usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/erlang/bin:/home/rabbitmq_server-3.3.1/sbin/:/usr/lib/oracle/11.2/client64/bin:/root/bin
PWD=/
LANG=zh_CN.UTF-8
KDE_IS_PRELINKED=1
MODULEPATH=/usr/share/Modules/modulefiles:/etc/modulefiles
LOADEDMODULES=
KDEDIRS=/usr
SSH_ASKPASS=/usr/libexec/openssh/gnome-ssh-askpass
HISTCONTROL=ignoredups
SHLVL=1
```
十二、free -m # 查看内存使用量和交换区使用量
```
total       used       free     shared    buffers     cached
Mem:          3706       3534        171          0         31       3258
-/+ buffers/cache:        244       3461
Swap:         3711          0       3711
[root@localhost /]#
```
十三、df -h # 查看各分区使用情况
```
[root@localhost /]# df -h
文件系统          容量  已用  可用 已用%% 挂载点
/dev/mapper/VolGroup-lv_root
                       50G  6.4G   41G  14% /
tmpfs                 1.9G   72K  1.9G   1% /dev/shm
/dev/sda1             485M   38M  422M   9% /boot
/dev/mapper/VolGroup-lv_home
                      406G  2.8G  382G   1% /home
```
                                 下面就不一一演示了
十四、du -sh # 查看指定目录的大小
十五、grep MemTotal /proc/meminfo # 查看内存总量
十六、grep MemFree /proc/meminfo # 查看空闲内存量
十七、uptime # 查看系统运行时间、用户数、负载
十八、cat /proc/loadavg # 查看系统负载磁盘和分区
十九、mount | column -t # 查看挂接的分区状态
二十、fdisk -l # 查看所有分区
二十一、swapon -s # 查看所有交换分区
二十二、hdparm -i /dev/hda # 查看磁盘参数(仅适用于IDE设备)
二十三、dmesg | grep IDE # 查看启动时IDE设备检测状况网络
二十四、ifconfig # 查看所有网络接口的属性
二十五、iptables -L # 查看防火墙设置
二十六、route -n # 查看路由表
二十七、netstat -lntp # 查看所有监听端口
二十八、netstat -antp # 查看所有已经建立的连接
二十九、netstat -s # 查看网络统计信息进程
三十、ps -ef # 查看所有进程
三十一、top # 实时显示进程状态用户
三十二、w # 查看活动用户
三十三、id # 查看指定用户信息
三十四、last # 查看用户登录日志
三十五、cut -d: -f1 /etc/passwd # 查看系统所有用户
三十六、cut -d: -f1 /etc/group # 查看系统所有组
三十七、crontab -l # 查看当前用户的计划任务服务
三十七、chkconfig –list # 列出所有系统服务
三十八、chkconfig –list | grep on # 列出所有启动的系统服务程序
三十九、rpm -qa # 查看所有安装的软件包
四十、cat /proc/cpuinfo ：查看CPU相关参数的linux系统命令
四十一、cat /proc/partitions ：查看linux硬盘和分区信息的系统信息命令
四十二、cat /proc/meminfo ：查看linux系统内存信息的linux系统命令
四十三、cat /proc/version ：查看版本，类似uname -r
四十四、cat /proc/ioports ：查看设备io端口
四十五、cat /proc/interrupts ：查看中断
四十六、cat /proc/pci ：查看pci设备的信息
四十七、cat /proc/swaps ：查看所有swap分区的信息
今天不努力，明天就会落后
