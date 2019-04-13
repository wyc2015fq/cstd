
# Android系统init进程启动及init.rc全解析 - 嵌入式Linux - CSDN博客

2018年11月20日 20:04:12[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2


这是一篇用心写的博客，也希望大家用心看并帮忙找到文章的改进之处，谢谢；
服务启动机制
system/core/init/init.c文件main函数中parse_config_file(init.rc)读取并解析init.rc文件内容。将service信息放置到system/core/init/init_parser.cpp的service_list中
system/core/init/init.c文件main函数继续执行restart_servie_if_needed(…) -> service_start(…) -> Execve(…)建立service进程；
为了让大伙看得更明白，上个图先《总体启动框架图》：
init.rc 简介
目前Linux有很多通讯机制可以在用户空间和内核空间之间交互，例如设备驱动文件（位于/dev目录中）、内存文件（/proc、/sys目录等）。了解Linux的同学都应该知道Linux的重要特征之一就是一切都是以文件的形式存在的，例如，一个设备通常与一个或多个设备文件对应。这些与内核空间交互的文件都在用户空间，所以在Linux内核装载完，需要首先建立这些文件所在的目录。而完成这些工作的程序就是本文要介绍的init。Init是一个命令行程序。其主要工作之一就是建立这些与内核空间交互的文件所在的目录。当Linux内核加载完后，要做的第一件事就是调用init程序，也就是说，init是用户空间执行的第一个程序。
尽管init完成的工作不算很多，不过代码还是非常复杂的。Init程序并不是由一个源代码文件组成的，而是由一组源代码文件的目标文件链接而成的。这些文件位于如下的目录。
需要明白的是，这些init.rc只是语法文件，并不是程序，真正的入口则是上面提到的system/core/init/init.c
因为init.c文件比较大，在文章的第二部分我会简要的通过main函数分析init启动流程；
init.rc有两个，分别位于：
./system/core/rootdir/init.rc
./bootable/recovery/etc/init.rc
从目录上大致可以猜测，这两个init.rc使用场景不一样，一个是刷机用到的，也就是进入recorvery模式，一个是正常启动用到的；我们这里重点分析的是上面那个，也是init.c关联的那个；
init.rc语法结构解析
要了解init.rc是怎么解析的，我们需要先看看说明文档，说明文档在，当然也可以看下热心网友的中文对照版本；
init.rc位于/bootable/recovery/etc/init.rc
Android初始化语言包含了四种类型的声明：
Actions（行为）、Commands（命令）、Services（服务）和Options（选项）
所有这些都是以行为单位的，各种记号由空格来隔开。
C语言风格的反斜杠号可用于在记号间插入空格。
双引号也可用于防止字符串被空格分割成多个记号。
行末的反斜杠用于折行，注释行以井号（\#）开头（允许以空格开头）。
需要注意的是，这个只是一个语法文件，就像一个xml文件一样，没有执行顺序的，解析器通过读这个文件获取想要的数据，包括service，action等
Actions和Services声明一个新的分组Section。所有的命令或选项都属于最近声明的分组。位于第一个分组之前的命令或选项将会被忽略。
Actions和Services有唯一的名字。如果有重名的情况，第二个申明的将会被作为错误忽略。
Actions
Actions（行为）是一系列命令的开始
Actions代表一些Action.Action代表一组命令(Commands),Actions都有一个trigger（触发器）,该触发器决定了何时执行这个Action,即在什么情况下才能执行该Action中的定义命令.当一些条件满足触发器的条件时,该Action中定义的命令会被添加到要执行命令队列的尾部(如果这组命令已经在队列中,则不会再次添加).
队列中的每一个action都被依次提取出，而这个action中的每个command（命令）在一个Action从队列移除时,该Action定义的命令会依次被执行.
Action的格式如下:
on <trgger> [&& <trigger>]*
<command1>
<command2>
<command3>
...
1
2
3
4
5
on后面跟着一个触发器，当trigger被触发时，command1，command2，command3，会依次执行，直到下一个Action或下一个Service。
简单来说，Actions就是Android在启动时定义的一个启动脚本，当条件满足时，会执行该脚本，脚本里都是一些命令commands，不同的脚本用on来区分。
Triggers（触发器）
trigger即我们上面所说的触发器,本质上是一个字符串,能够匹配某种包含该字符串的事件.
trigger又被细分为事件触发器(event trigger)和属性触发器(property trigger).
Triggers（触发器）是一个用于匹配特定事件类型的字符串，用于使Actions发生。
事件触发器可由”trigger”命令或初始化过程中通过QueueEventTrigger()触发,通常是一些事先定义的简单字符串,例如:boot,late-init
属性触发器是当指定属性的变量值变成指定值时触发,其格式为property:=*
一个Action可以有多个属性触发器,但是最多有一个事件触发器.下面我们看两个例子:
on boot && property:a=b
1
该Action只有在boot事件发生时,并且属性a和b相等的情况下才会被触发.
on property:a=b && property:c=d
1
该Action会在以下三种情况被触发:
在启动时,如果属性a的值等于b并且属性c的值等于d
在属性c的值已经是d的情况下,属性a的值被更新为b
在属性a的值已经是b的情况下,属性c的值被更新为d
当前AIL中常用的有以下几种事件触发器:
类型                      说明
-------------------------------------------------
boot                    init.rc被装载后触发
device-added-<path>     指定设备被添加时触发
device-removed-<path>   指定设备被移除时触发
service-exited-<name>   在特定服务(service)退出时触发
early-init              初始化之前触发
late-init               初始化之后触发
init                    初始化时触发（在 /init.conf （启动配置文件）被装载之后）
1
2
3
4
5
6
7
8
9
Init的触发是由init.c里的函数action_for_each_trigger来决定的（在main函数中被调用）。
Services
Services（服务）是一个程序，以 service开头，由init进程启动，一般运行于另外一个init的子进程，所以启动service前需要判断对应的可执行文件是否存在。init生成的子进程，定义在rc文件，其中每一个service，在启动时会通过fork方式生成子进程。Services（服务）的形式如下：
service <name> <pathname> [ <argument> ]*
<option>
<option>
...
1
2
3
4
其中：
name:服务名
pathname:当前服务对应的程序位置
option：当前服务设置的选项
argument 可选参数
init.rc文件详解
为了方便理解，我把整个init.rc解析一边，便于大家了解整个流程；如果想要了解recovery下的init语法解析，参考这篇文章《recovery下的init.rc语法解析》
代码量比较大，如果觉得看起来费劲，可以挑绿色部分看；
\# Copyright (C) 2012 The Android Open Source Project
\#
\# IMPORTANT: Do not create world writable files or directories.
\# This is a common source of Android security bugs.
\#
"【import <filename>一个init配置文件，扩展当前配置。】"
import /init.environ.rc
import /init.usb.rc
import /init.${ro.hardware}.rc
import /init.${ro.zygote}.rc
import /init.trace.rc
"【触发条件early-init，在early-init阶段调用以下行】"
on early-init
\# Set init and its forked children's oom_adj.
write /proc/1/oom_score_adj -1000
"【打开路径为<path>的一个文件，并写入一个或多个字符串】"
\# Apply strict SELinux checking of PROT_EXEC on mmap/mprotect calls.
write /sys/fs/selinux/checkreqprot 0
\# Set the security context for the init process.
\# This should occur before anything else (e.g. ueventd) is started.
"【这段脚本的意思是init进程启动之后就马上调用函数setcon将自己的安全上下文设置为“u:r:init:s0”，即将init进程的domain指定为init。】"
setcon u:r:init:s0
\# Set the security context of /adb_keys if present.
"【恢复指定文件到file_contexts配置中指定的安全上线文环境】"
restorecon /adb_keys
"【执行start ueventd的命令。ueventd是一个service后面有定义】 "
start ueventd
"【mkdir <path> [mode] [owner] [group]   //创建一个目录<path>，可以选择性地指定mode、owner以及group。如果没有指定，默认的权限为755，并属于root用户和root组。】"
\# create mountpoints
mkdir /mnt 0775 root system
on init
"【设置系统时钟的基准,比如0代表GMT,即以格林尼治时间为准】"
sysclktz 0
"【设置kernel日志等级】"
loglevel 6 \#\#\#\#
write /proc/bootprof "INIT: on init start" \#\#\#\#
"【symlink <target> <path>    //创建一个指向<path>的软连接<target>。】"
\# Backward compatibility
symlink /system/etc /etc
symlink /sys/kernel/debug /d
\# Right now vendor lives on the same filesystem as system,
\# but someday that may change.
symlink /system/vendor /vendor
"【创建一个目录<path>，可以选择性地指定mode、owner以及group。】"
\# Create cgroup mount point for cpu accounting
mkdir /acct
mount cgroup none /acct cpuacct
mkdir /acct/uid
"【mount <type> <device> <dir> [ <mountoption> ]   //在目录<dir>挂载指定的设备。<device> 可以是以 mtd@name 的形式指定一个mtd块设备。<mountoption>包括 ro、rw、remount、noatime、 ...】"
\# Create cgroup mount point for memory
mount tmpfs none /sys/fs/cgroup mode=0750,uid=0,gid=1000
mkdir /sys/fs/cgroup/memory 0750 root system
mount cgroup none /sys/fs/cgroup/memory memory
write /sys/fs/cgroup/memory/memory.move_charge_at_immigrate 1
"【chown <owner> <group> <path>   //改变文件的所有者和组。】"
"【后面的一些行因为类似，就省略了】"
.....
\# Healthd can trigger a full boot from charger mode by signaling this
\# property when the power button is held.
on property:sys.boot_from_charger_mode=1
"【停止指定类别服务类下的所有已运行的服务】"
class_stop charger
"【触发一个事件,将该action排在某个action之后(用于Action排队)】"
trigger late-init
\# Load properties from /system/ + /factory after fs mount.
on load_all_props_action
"【从/system，/vendor加载属性。默认包含在init.rc】"
load_all_props
\# Indicate to fw loaders that the relevant mounts are up.
on firmware_mounts_complete
"【删除指定路径下的文件】"
rm /dev/.booting
\# Mount filesystems and start core system services.
on late-init
"【触发一个事件。用于将一个action与另一个 action排列。】"
trigger early-fs
trigger fs
trigger post-fs
trigger post-fs-data
\# Load properties from /system/ + /factory after fs mount. Place
\# this in another action so that the load will be scheduled after the prior
\# issued fs triggers have completed.
trigger load_all_props_action
\# Remove a file to wake up anything waiting for firmware.
trigger firmware_mounts_complete
trigger early-boot
trigger boot
on post-fs
...
"【一些创造目录，建立链接，更改权限的操作，这里省略】"
on post-fs-data
...
"【一些创造目录，建立链接，更改权限的操作，这里省略】"
"【恢复指定文件到file_contexts配置中指定的安全上线文环境】"
restorecon /data/mediaserver
"【将系统属性<name>的值设置为<value>,即以键值对的方式设置系统属性】"
\# Reload policy from /data/security if present.
setprop selinux.reload_policy 1
"【以递归的方式恢复指定目录到file_contexts配置中指定的安全上下文中】"
\# Set SELinux security contexts on upgrade or policy update.
restorecon_recursive /data
\# If there is no fs-post-data action in the init.<device>.rc file, you
\# must uncomment this line, otherwise encrypted filesystems
\# won't work.
\# Set indication (checked by vold) that we have finished this action
\#setprop vold.post_fs_data_done 1
on boot
"【初始化网络】"
\# basic network init
ifup lo
"【设置主机名为localhost】"
hostname localhost
"【设置域名localdomain】"
domainname localdomain
"【设置资源限制】"
\# set RLIMIT_NICE to allow priorities from 19 to -20
setrlimit 13 40 40
"【这里省略了一些chmod,chown,等操作，不多解释】"
...
\# Define default initial receive window size in segments.
setprop net.tcp.default_init_rwnd 60
"【重启core服务】"
class_start core
on nonencrypted
class_start main
class_start late_start
on property:vold.decrypt=trigger_default_encryption
start defaultcrypto
on property:vold.decrypt=trigger_encryption
start surfaceflinger
start encrypt
on property:sys.init_log_level=*
loglevel ${sys.init_log_level}
on charger
class_start charger
on property:vold.decrypt=trigger_reset_main
class_reset main
on property:vold.decrypt=trigger_load_persist_props
load_persist_props
on property:vold.decrypt=trigger_post_fs_data
trigger post-fs-data
on property:vold.decrypt=trigger_restart_min_framework
class_start main
on property:vold.decrypt=trigger_restart_framework
class_start main
class_start late_start
on property:vold.decrypt=trigger_shutdown_framework
class_reset late_start
class_reset main
on property:sys.powerctl=*
powerctl ${sys.powerctl}
\# system server cannot write to /proc/sys files,
\# and chown/chmod does not work for /proc/sys/ entries.
\# So proxy writes through init.
on property:sys.sysctl.extra_free_kbytes=*
write /proc/sys/vm/extra_free_kbytes ${sys.sysctl.extra_free_kbytes}
\# "tcp_default_init_rwnd" Is too long!
on property:sys.sysctl.tcp_def_init_rwnd=*
write /proc/sys/net/ipv4/tcp_default_init_rwnd ${sys.sysctl.tcp_def_init_rwnd}
"【守护进程】"
\#\# Daemon processes to be run by init.
\#\#
service ueventd /sbin/ueventd
class core
critical
seclabel u:r:ueventd:s0
"【日志服务进程】"
service logd /system/bin/logd
class core
socket logd stream 0666 logd logd
socket logdr seqpacket 0666 logd logd
socket logdw dgram 0222 logd logd
seclabel u:r:logd:s0
"【Healthd是android4.4之后提出来的一种中介模型，该模型向下监听来自底层的电池事件，向上传递电池数据信息给Framework层的BatteryService用以计算电池电量相关状态信息】"
service healthd /sbin/healthd
class core
critical
seclabel u:r:healthd:s0
"【控制台进程】"
service console /system/bin/sh
"【为当前service设定一个类别.相同类别的服务将会同时启动或者停止,默认类名是default】"
class core
"【服务需要一个控制台】"
console
"【服务不会自动启动,必须通过服务名显式启动】"
disabled
"【在执行此服务之前切换用户名,当前默认的是root.自Android M开始,即使它要求linux capabilities,也应该使用该选项.很明显,为了获得该功能,进程需要以root用户运行】"
user shell
seclabel u:r:shell:s0
on property:ro.debuggable=1
start console
\# adbd is controlled via property triggers in init.<platform>.usb.rc
service adbd /sbin/adbd --root_seclabel=u:r:su:s0
class core
"【创建一个unix域下的socket,其被命名/dev/socket/<name>. 并将其文件描述符fd返回给服务进程.其中,type必须为dgram,stream或者seqpacke,user和group默认是0.seclabel是该socket的SELLinux的安全上下文环境,默认是当前service的上下文环境,通过seclabel指定】"
socket adbd stream 660 system system
disabled
seclabel u:r:adbd:s0
\# adbd on at boot in emulator
on property:ro.kernel.qemu=1
start adbd
"【内存管理服务，内存不够释放内存】"
service lmkd /system/bin/lmkd
class core
critical
socket lmkd seqpacket 0660 system system
"【ServiceManager是一个守护进程，它维护着系统服务和客户端的binder通信。
在Android系统中用到最多的通信机制就是Binder，Binder主要由Client、Server、ServiceManager和Binder驱动程序组成。其中Client、Service和ServiceManager运行在用户空间，而Binder驱动程序运行在内核空间。核心组件就是Binder驱动程序了，而ServiceManager提供辅助管理的功能，无论是Client还是Service进行通信前首先要和ServiceManager取得联系。而ServiceManager是一个守护进程，负责管理Server并向Client提供查询Server的功能。】"
service servicemanager /system/bin/servicemanager
class core
user system
group system
critical
onrestart restart healthd
"【servicemanager 服务启动时会重启zygote服务】"
onrestart restart zygote
onrestart restart media
onrestart restart surfaceflinger
onrestart restart drm
"【Vold是Volume Daemon的缩写,它是Android平台中外部存储系统的管控中心,是管理和控制Android平台外部存储设备的后台进程】"
service vold /system/bin/vold
class core
socket vold stream 0660 root mount
ioprio be 2
"【Netd是Android系统中专门负责网络管理和控制的后台daemon程序】"
service netd /system/bin/netd
class main
socket netd stream 0660 root system
socket dnsproxyd stream 0660 root inet
socket mdns stream 0660 root system
socket fwmarkd stream 0660 root inet
"【debuggerd是一个daemon进程，在系统启动时随着init进程启动。主要负责将进程运行时的信息dump到文件或者控制台中】"
service debuggerd /system/bin/debuggerd
class main
service debuggerd64 /system/bin/debuggerd64
class main
"【Android RIL (Radio Interface Layer)提供了Telephony服务和Radio硬件之间的抽象层】"
\# for using TK init.modem.rc rild-daemon setting
\#service ril-daemon /system/bin/rild
\#    class main
\#    socket rild stream 660 root radio
\#    socket rild-debug stream 660 radio system
\#    user root
\#    group radio cache inet misc audio log
"【提供系统 范围内的surface composer功能，它能够将各种应用 程序的2D、3D surface进行组合。】"
service surfaceflinger /system/bin/surfaceflinger
class core
user system
group graphics drmrpc
onrestart restart zygote
"【DRM可以直接访问DRM clients的硬件。DRM驱动用来处理DMA，内存管理，资源锁以及安全硬件访问。为了同时支持多个3D应用，3D图形卡硬件必须作为一个共享资源，因此需要锁来提供互斥访问。DMA传输和AGP接口用来发送图形操作的buffers到显卡硬件，因此要防止客户端越权访问显卡硬件。】"
\#make sure drm server has rights to read and write sdcard \#\#\#\#
service drm /system/bin/drmserver
class main
user drm
\# group drm system inet drmrpc \#\#\#\#
group drm system inet drmrpc sdcard_r \#\#\#\#
"【媒体服务，无需多说】"
service media /system/bin/mediaserver
class main
user root \#\#\#\#
\#   google default \#\#\#\#
\#   user media    \#\#\#\#
group audio camera inet net_bt net_bt_admin net_bw_acct drmrpc mediadrm media sdcard_r system net_bt_stack \#\#\#\#
\#   google default \#\#\#\#
\#   group audio camera inet net_bt net_bt_admin net_bw_acct drmrpc mediadrm \#\#\#\#
ioprio rt 4
"【设备加密相关服务】"
\# One shot invocation to deal with encrypted volume.
service defaultcrypto /system/bin/vdc --wait cryptfs mountdefaultencrypted
disabled
"【当服务退出时,不重启该服务】"
oneshot
\# vold will set vold.decrypt to trigger_restart_framework (default
\# encryption) or trigger_restart_min_framework (other encryption)
\# One shot invocation to encrypt unencrypted volumes
service encrypt /system/bin/vdc --wait cryptfs enablecrypto inplace default
disabled
oneshot
\# vold will set vold.decrypt to trigger_restart_framework (default
\# encryption)
"【开机动画服务】"
service bootanim /system/bin/bootanimation
class core
user graphics
\#    group graphics audio \#\#\#\#
group graphics media audio \#\#\#\#
disabled
oneshot
"【在Android系统中，PackageManagerService用于管理系统中的所有安装包信息及应用程序的安装卸载，但是应用程序的安装与卸载并非PackageManagerService来完成，而是通过PackageManagerService来访问installd服务来执行程序包的安装与卸载的。】"
service installd /system/bin/installd
class main
socket installd stream 600 system system
service flash_recovery /system/bin/install-recovery.sh
class main
seclabel u:r:install_recovery:s0
oneshot
"【vpn相关的服务】"
service racoon /system/bin/racoon
class main
socket racoon stream 600 system system
\# IKE uses UDP port 500. Racoon will setuid to vpn after binding the port.
group vpn net_admin inet
disabled
oneshot
"【android中有mtpd命令可以连接vpn】"
service mtpd /system/bin/mtpd
class main
socket mtpd stream 600 system system
user vpn
group vpn net_admin inet net_raw
disabled
oneshot
service keystore /system/bin/keystore /data/misc/keystore
class main
user keystore
group keystore drmrpc
"【可以用dumpstate 获取设备的各种信息】"
service dumpstate /system/bin/dumpstate -s
class main
socket dumpstate stream 0660 shell log
disabled
oneshot
"【mdnsd 是多播 DNS 和 DNS 服务发现的守护程序。】"
service mdnsd /system/bin/mdnsd
class main
user mdnsr
group inet net_raw
socket mdnsd stream 0660 mdnsr inet
disabled
oneshot
"【触发关机流程继续往下走】"
service pre-recovery /system/bin/uncrypt
class main
disabled
"【当服务退出时,不重启该服务】"
oneshot
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
259
260
261
262
263
264
265
266
267
268
269
270
271
272
273
274
275
276
277
278
279
280
281
282
283
284
285
286
287
288
289
290
291
292
293
294
295
296
297
298
299
300
301
302
303
304
305
306
307
308
309
310
311
312
313
314
315
316
317
318
319
320
321
322
323
324
325
326
327
328
329
330
331
332
333
334
335
336
337
338
339
340
341
342
343
344
345
346
347
348
349
350
351
352
353
354
355
356
357
358
359
360
361
362
363
364
365
366
367
368
369
370
371
372
373
374
375
376
377
378
379
380
381
382
383
384
385
386
387
388
389
390
391
392
393
394
395
396
397
398
399
400
401
402
403
404
405
406
407
408
409
410
411
412
413
414
init.c全解析
接下来我们具体分析以下这个main函数的执行过程；可能比较长，大家耐心看一下：
int main( int argc, char **argv )
{
\#创 建一些linux根文件系统中的目录
mkdir( "/dev", 0755 );
mkdir( "/proc", 0755 );
mkdir( "/sys", 0755 );
mount( "tmpfs", "/dev", "tmpfs", 0, "mode=0755" );
mkdir( "/dev/pts", 0755 );
mkdir( "/dev/socket", 0755 );
mount( "devpts", "/dev/pts", "devpts", 0, NULL );
mount( "proc", "/proc", "proc", 0, NULL );
mount( "sysfs", "/sys", "sysfs", 0, NULL );
\#init的 标准输入，标准输出，标准错误文件描述符定向到__null__，意味着没有输入和输出，它的输入和输出全部写入到Log中
open_devnull_stdio();
\#初始化 log 写入init进 信息
log_init();
\#读取并 且解析init.rc文件（这个文件在根目录下）
parse_config_file( "/init.rc" );
\#取得硬件 为打印我们的设备名fs100
get_hardware_name();
snprintf( tmp, sizeof(tmp), "/init.%s.rc", hardware );
\#读取并 且解析硬件相关的init脚本文件,
parse_config_file( tmp );
\#触发在init脚本文件中名字为early-init的action，并且执行其commands，其实是: on early-init
action_for_each_trigger( "early-init", action_add_queue_tail );
drain_action_queue();
\#初始化动态设备管理，设备文件有变化时反应给内核，后面具体解释
device_fd = device_init(); \# 初 始 化 设 备 管 理 务
\#加载启动动画，如果动画打开失败，则在屏幕上打印： A N D R O I D字样。
if ( load_565rle_image( INIT_IMAGE_FILE ) )
{
fd = open( "/dev/tty0", O_WRONLY );
if ( fd >= 0 )
{
const char *msg;
msg = "\n"
"\n"
"\n"
879         "\n"
"\n"
"\n"
"\n" /* console is 40 cols x 30 lines */
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
/* "             A N D R O I D ";开机动画 */
write( fd, msg, strlen( msg ) );
close( fd );
}
}
\#触发 在init脚本文件中名字为init的action，并且执行其commands，其实是：on init
action_for_each_trigger( "init", action_add_queue_tail );
drain_action_queue();
\#启动系统属性服务： system property service
property_set_fd = start_property_service();
\#创建socket用来处理孤儿进程信号
if ( socketpair( AF_UNIX, SOCK_STREAM, 0, s ) == 0 )
{
signal_fd   = s[0];
signal_recv_fd  = s[1];
fcntl( s[0], F_SETFD, FD_CLOEXEC );
fcntl( s[0], F_SETFL, O_NONBLOCK );
fcntl( s[1], F_SETFD, FD_CLOEXEC );
fcntl( s[1], F_SETFL, O_NONBLOCK );
}
\#触发 在init脚本文件中名字为early-boot和boot的action，并且执行其commands，其实是：on early-boot和on boot
action_for_each_trigger( "early-boot", action_add_queue_tail );
action_for_each_trigger( "boot", action_add_queue_tail );
drain_action_queue();
\#启动所有属性变化触发命令，其实是： on property:ro.xx.xx=xx
queue_all_property_triggers();
drain_action_queue();
\#进入 死循环（）
for (;; )
{
\#启 动所有init脚本中声明的service，
\#如 ：266 service servicemanager /system/bin/servicemanager
\#user system
\#critical
\#onrestart restart zygote
\#onrestart restart media
restart_processes();
\#多路监听设备管理，子进程运行状态，属性服务
nr = poll( ufds, fd_count, timeout );
if ( nr <= 0 )
continue;
if ( ufds[2].revents == POLLIN )
{
read( signal_recv_fd, tmp, sizeof(tmp) );
while ( !wait_for_one_process( 0 ) )
;
continue;
}
if ( ufds[0].revents == POLLIN )
handle_device_fd( device_fd );
if ( ufds[1].revents == POLLIN )
handle_property_set_fd( property_set_fd );
if ( ufds[3].revents == POLLIN )
handle_keychord( keychord_fd );
}
return(0);
}
---------------------
作者：疯人院的院长大人
来源：CSDN
原文：https://blog.csdn.net/zhonglunshun/article/details/78615980
版权声明：本文为博主原创文章，转载请附上博文链接！

