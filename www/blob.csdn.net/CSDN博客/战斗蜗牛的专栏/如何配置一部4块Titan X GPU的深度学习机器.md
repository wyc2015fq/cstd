# 如何配置一部4块Titan X GPU的深度学习机器 - 战斗蜗牛的专栏 - CSDN博客





2018年02月08日 21:22:53[vbskj](https://me.csdn.net/vbskj)阅读数：2915








由于UCSB是季度制，9月下旬才开学，于是不久前我决定亲自上阵给实验室的同学们配一台多显卡的深度学习服务器。​第一次配4块Titan X Pascal的台式机器，也没什么太多经验，结合网友们和朋友们的建议，我实验性的装了一下，最后也算一切顺利。不少同学找我要配置，我不是搞硬件的，只不过中间有些经验教训，跟大家分享一下。

我的配置：

GPU: 4 x Titan X Pascal GPU。之所以不选择1080是因为只有8GB的内存，可能深度学习在处理大数据集的时候有可能会遇到点瓶颈。之前Titan X Pascal一直断货，几周前官网开始偶尔有货，我请系里的秘书帮忙刷了一周，总算买了几块（一天只能买两块）。http://www.geforce.com/hardware/10series/titan-x-pascal

主板：Asus X99-e WS。这个是workstation版本的Asus X99，支持四块显卡，USB 3.1，很多的硬盘接口。支持Intel 59xx和6字头的i7处理器。不喜欢这块板子的也可以试试rampage v edition 10，或者rampage v extreme。可能也有便宜的板子支持4 GPU，不过你要特别关注PCIe 3.0插槽的数量和布局：一块显卡通常会占据2个口的位置。

CPU: 通常来说CPU在多核GPU的深度学习系统里还是比较重要的，因为要并行处理参数。我这次选用了网友推荐的i7 5930K，一共6核12线程，性价比还算凑合，跑起来也没什么太大问题。

内存：这个基本要看CPU能支持多少了，5930K貌似只可以支持64G，我就卖了两条Kington valueRAM DDR4 32G。当然省钱的做法是买8条8G的用。

存储：SSD还是比HDD快了不少，所以在这种情况下，我选择了2块Samsung 850 EVO 1TB的SSD内存。如果数据集太大，也可以考虑搞个4TB的HDD来存一下（10TB和8TB的还是有点贵）。

CPU冷却：我选了Corsair H60水冷。注意装的时候有两套4个螺丝钉，要选短的螺钉，短的装在板子上，另一端长的接在风冷上。H60自带涂层，不过要注意水冷必须安装特别紧，一点点空气缝隙也不能留，不然估计深度学习压力测试你的CPU会到80度。不放心的可以上H100i。

电源：电源还是很重要的基本2个选择 Corsair 1500W或者EVGA 1600W，因为一个GPU可能到250W。当然实际运行的时候一般到不了那么高。我之前选了一个 Corsair 1200W，居然self-test风扇不转，只要连主板就会reboot loop，明显是次品，赶紧趁机RMA换了1600W。

机箱：不少人推荐Corsair Carbide Air 540，这是一个中塔机箱。我最后选择了一个全塔机箱Corsair 900D，通风好，但是特别重（配上所有东西超过50多斤重。。。保险箱的节奏）。大机箱可以放很多硬盘，如果你需要的话。

最后用USB 3.1启动机器，几分钟就装好了Ubuntu 16.04。注意最好UEFI BIOS配置取消Secure Boot功能，不然你装Titan X驱动和CUDA 8.0 RC会有问题。装TensorFlow也没什么问题，就是要是找不到CUDA库的错误，可以用sudo ldconfig /usr/local/cuda/lib64和LD_LIBRARY_PATH / LIBRARY_PATH来解决。

我测试了4块Titan X Pascal跑TF的CIFAR多GPU训练，训练几天时间一切都很正常，GPU的温度最高70度（设计80 C温度范围内，其他几块会低），GPU风扇也不会到50%速度。目前我也在测Supermicro的superserver多显卡配置，可能成本会更低。



