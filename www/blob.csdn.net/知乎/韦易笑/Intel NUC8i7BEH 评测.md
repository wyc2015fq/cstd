# Intel NUC8i7BEH 评测 - 知乎
# 



服务器就是程序员永远的玩具，服务器里自有颜如玉，服务器里自有黄金屋。路由器早就被玩腻了，我从小型机一路烧到树莓派，从自己组装服务器又一路烧到刀片机。

如今这把野火烧到了 Intel Nuc 上，说起 NUC 我差不多观察了三年了，今年下半年 Intel 出了两款八代 i7 新品，分别是 NUC8i7HVK （冥王峡谷）：
![](https://pic2.zhimg.com/v2-ccc6b9d28466f514e93e1b8208a9eca1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
和我入的这台 NUC8i7BEH：
![](https://pic2.zhimg.com/v2-c6038872393b350ee8cae90836a48535_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='720'></svg>)
冥王峡谷更多用作家用娱乐和游戏配的很强的显卡，而 NUC8i7BEH 一般作为办公用，别看后者颜值没前者那么高，那么狂炫酷拽，但是性能一点不含糊：
![](https://pic3.zhimg.com/v2-b0f19e071a718f145519ec656e9911de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='644' height='208'></svg>)
所以别看冥王峡谷外观很漂亮很拉风，但如果不玩游戏，办公或者做服务器的话，虽然都是 8代 i7，但 NUC8i7BEH 从性能，功耗，硬件版本，各项参数全面碾压冥王峡谷， 无愧于今年下半年 NUC 的机皇产品。

所以作为需要一台小型高性能服务器的人来讲，当然是选购后者。自己拍的照片实在太丑，网上搜了两张别人的图片：
![](https://pic4.zhimg.com/v2-80e262b9d5341c18a3471b0fda0bb283_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='576'></svg>)
实际大小就是四包烟堆在一起（下面两包，上面两包），十分紧凑。




**性能评测**

它性能有多强呢？下面是 [cpubenchmark](https://link.zhihu.com/?target=https%3A//www.cpubenchmark.net/compare/Intel-i7-8809G-vs-Intel-i7-8559U-vs-Intel-i7-7700HQ/3209vs3302vs2906) 网站的评分：
![](https://pic3.zhimg.com/v2-610d6ab56adb4adcd77c9517985cad5a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='899' height='524'></svg>)
NUC8i7BEH 的性能，比我去年底刚买的标压版本 ThinkPad T470P 还高 34%。到底有没有那么强呢？我们简单测试一下 CPU 运算能力：
- NUC8i7BEH：


```
$ python -c 'import test.pystone;print test.pystone.pystones()'
(0.15877000000000002, 314920.954840335)
```

- ThinkPad T470P：


```
> python -c 'import test.pystone;print test.pystone.pystones()'
(0.1875, 266666.6666666667)
```


相同版本的 Python 跑 pystone 性能测试，多跑几次取最高值，前者评分 314920，后者 266666，性能比我的笔记本好多了。

再测试一下内存性能：
- NUC8i7BEH：


```
$ dd if=/dev/zero of=/dev/null bs=1M count=32768
32768+0 records in
32768+0 records out
34359738368 bytes (34 GB, 32 GiB) copied, 1.30653 s, 26.3 GB/s
```

- ThinkPad T470P：


```
> dd if=/dev/zero of=/dev/null bs=1M count=32768                                                                                                                                             130
32768+0 records in
32768+0 records out
34359738368 bytes (34 GB, 32 GiB) copied, 3.29097 s, 10.4 GB/s
```


前者内存拷贝速度 26.3GB/s，后者只有 10.4GB/s ，完全不是一个量级。

NUC8i7BEH 性能已经秒杀我所有台式机笔记本了，作为服务器的性能担当，暂时无出其右者。Intel 多年 NUC 上的耕耘，稳定性问题不大，还是低压版本，十分理想的服务器设备。




**实际使用**

我测试过安装 Windows 10 ，基本没问题，网卡那些在安装时就识别出来了，当然 Windows  10 我只是安装了跑几个 Intel 在 Win 下面的烤 CPU 和烤 FPU 的软件评测下，一切正常，随后切换到 Linux 下面。

由于硬件太新，主要是有线网卡 I219-V，安装 Debian 9 的时候（内核 4.9.110）无法识别出有线网卡来，后来尝试安装 Ubuntu 18.04 Server LTS（内核 4.15.42），网卡识别出来了，但是驱动有问题，一旦给网卡配置 IP，机器立马重启。

所以最后我退回 Debian 9 ，用无网卡的模式安装完了操作系统，按说明下载了最新版本的 intel e1000e 网卡驱动（兼容 I219-V 网卡），重新编译驱动后网卡就可以用了。为了避免 apt-get upgrade 升级到内核驱动变得不兼容的情况，使用 dkms 模块将驱动管理起来，这样每次更新到内核，dkms 都会自动重新编译网卡驱动，基本不用你操心了。

Debian 9 下面我也部署了虚拟化环境，用了两周，一切使用正常，读卡器，USB，无线这些都能用得起来。后面我又尝试了 backports 源，把内核升级到了 4.18.6 ，比较新的版本了，比 Ubuntu 18.04 带的内核版本还行，期间 dkms 也能正常的工作，完全透明的自动化编译了驱动。又用了两周，7x24 小时开机，各种繁重工作，一切工作正常。




**噪音和功耗**

平时不接显示器，就放路由器边上，噪音基本听不到，平时空置时功耗 3瓦-7瓦 左右，比我的路由器还要低一倍，我家路由器平时是 7-15 瓦。用 Windows 10 测试时，CPU和显卡跑满了会冲到最高 78 瓦，随后会控制稳定到 50瓦，所以使用了 90 瓦的电源适配器基本能覆盖。

既然平均功耗 3-7 瓦，用的又是低压版 CPU/内存，长时间开机问题都不大。实在想省电还可以用 rtcwake 设定每天凌晨 00：00 - 10：00 睡眠，关闭 CPU/内存/硬盘，过了时间又自己醒过来。




**硬盘和内存**

NUC8i7BEH 内部可以插一块全尺寸 M.2 ，还可以装一块 2.5寸的 HDD/SSD，我买了条三星 EVO 的 M.2 SSD 来装系统，外加一块 2.5寸的 2T HDD 机械硬盘来寸数据。旁边的 SD 卡槽支持最新的 TF 卡标准，我插着一条 256GB 的 TF 卡做数据备份，内存配到了 32GB。

由于没有硬盘阵列，上不了 raid，我将该机器用做 NAS 服务器的补充，提供性能担当。其实如果你不在乎 raid，家里就一台 NUC8i7BEH 也能承担起 NAS 的作用，做好冷备即可。因为 Raid 看着好看，有时候一出问题都是两块硬盘同时出问题，归根结底都要冷备和远程备份。

至于内存 ECC，我的意见是有了更好，没有也行，我自己组装的服务器包括刀片机，没有 ECC 也跑了好几年了，照样 7x24 小时，照样上了软件 raid，没大家说的那么恐怖，好像缺了 ECC服务器就没法活了一样。群晖和威联通的成品 NAS 不也没有 ECC，软件 raid 不也照样跑的嘎嘎的么？真有啥问题早被人骂死了。




**后记**

所以家里已有 NAS 的，适合搞一台回来作为高性能计算方面的补充；没有 NAS 的，直接买一台回来充当 NAS 也行。自己装服务器我也装过，高性能，低功耗，小机身，三个条件很难兼顾，基本不可能做到这么小的尺寸和这么强的性能，还有低功耗和高稳定性。

有评测文章这么评价 NUC8i7BEH 的：

> 用衣冠禽兽来形容NUC8i7BEH我想是再合适不过的了，小巧精致的外壳内配备了一颗有着澎湃动力的处理器。NUC上一直被大家诟病的散热设计也在这款产品上有了大幅的改进。这款产品非常适合需要小体积高性能的非游戏用户。

作为一台理想的服务器设备，我的感觉是，随你怎么折腾，它都能安静和稳定的帮你完成任务。




--

PS：同群晖四盘顶配 [DS918+](https://link.zhihu.com/?target=https%3A//www.synology.cn/zh-cn/products/DS918%2B%23specs) 和威联通4盘顶配 [TS-453BT3](https://link.zhihu.com/?target=https%3A//www.qnap.com/zh-cn/product/ts-453bt3) 所采用的 J3455 CPU 比较下：
![](https://pic3.zhimg.com/v2-f19edc0b2e8a9ff6828b329c4b59efa6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='898' height='421'></svg>)
上图来自：[这里](https://link.zhihu.com/?target=https%3A//www.cpubenchmark.net/compare/Intel-i7-8559U-vs-Intel-Celeron-J3455/3302vs2875) J3455 也大量用于自装 Nas 系统，比如华擎主板。

我们的 NUC8i7BEH 居然比群晖和威联通的四盘位顶配 NAS 快了整整 **5.5** 倍！！

你要上虚拟化系统，做点复杂的事情，用点 zsh，终端里编辑器打开个大文件，J3455 慢得能给你急死。而 NUC8i7BEH 使用 KVM 虚拟化了以后，虚拟机里面安装 Windows 10 ，我局域网远程桌面连过去，跟操作一台台式机一样流畅。


