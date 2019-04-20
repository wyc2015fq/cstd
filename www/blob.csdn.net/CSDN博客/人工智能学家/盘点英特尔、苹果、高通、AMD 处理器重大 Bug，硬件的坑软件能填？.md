# 盘点英特尔、苹果、高通、AMD 处理器重大 Bug，硬件的坑软件能填？ - 人工智能学家 - CSDN博客
2018年09月15日 23:47:24[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：189
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibrELdyibqeSt9OmBzgalDMLJCbgjTTxtfCgxyMtomnHe0hEIciaHgpUwA/640?wx_fmt=jpeg)
来源：21ic电子网
英特尔的CPU存在两大漏洞 ——Meltdown 和 Spectre，波及自 1995 年以来所有使用英特尔芯片的硬件设备。不止 Linux、Android、Windows、苹果 OS X 等操作系统，AWS、Azure、谷歌云等云计算提供商均受到影响，其威胁范围之广，令人咋舌。
其中，Meltdown 可以允许低权限的用户和应用“越界”访问系统级内存，从而造成数据泄露; Spectre 可骗过安全检查程序，直接通过应用访问内存任意位置。对于以往的 软件安全 Bug 仅需修复打个补丁即可，但是此次的漏洞存在于处理器端，与硬件相关，Intel 无法通过微代码的方式修补漏洞，需要操作系统厂商进行弥补，然而，据悉，不同的操作系统都要从内核层面进行漏洞修复，而修补该漏洞之后会让处理器的性能最高下降 30%。
如今微软、谷歌、苹果、亚马逊等公司已经相继发布了解决方案，其他一些厂商也正在加急的修复中，不得不说，巨头的一个 Bug，整个业界都要抖三抖。事实上，这并非是第一次 CPU 出现 Bug，那么以往 CPU 出现 Bug 都是如何处理的呢?接下来，我们将一一盘点曾经的 CPU Bug 事件。
**不会算数的奔腾 FDIV Bug**
1994 年 6 月，在英特尔首次推出奔腾处理器的前几天，其技术人员发现奔腾芯片的除法运算会发生一些偏差，即 FDIV 缺陷。奔腾 CPU 的 FPU 单元有严重缺陷的产品，影响到 FDIV(浮点除法运算)指令。例如：
> 
正常电脑计算器运算结果：962306957033÷11010046=87402.6282027341
带有 Bug 的奔腾 CPU 运算结果：962306957033÷11010046=87339.5805831329
对于以上偏差，Intel 测试人员表示 90 亿次除法运算中才可能出现 1 次错误，因此怀着侥幸心理无视了该问题，但是严谨的学术人员还是在这个几亿分之一几率中找到这个除法 Bug。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibjnmBHSXdXuNDVocQZ0FCprxjWS4fRm1zJMcP0icPkwcHnrzdib5leGmA/640?wx_fmt=png)
图片来源：太平洋电脑网
**解决方案**
起初，英特尔只决定为部分已经证明受影响的用户更换 CPU。但是，这引起了用户的不满，因为连最基本运算的正确都不能百分之百的保证，何来对 CPU 的信任。后来，在用户以及竞争对手(IBM等公司)的压力下，英特尔于 1994 年的 12 月对外宣布自身 CPU 的缺陷并召回了受影响的 CPU。
显然，奔腾 FDIV Bug 并不能通过软件的方法修补，为了解决问题，英特尔召回的成本甚至高达 4.75 亿美元，结合通货膨胀率，这大致相当于现在的 7.88 亿美元。
**死机的奔腾 F00F bug**
这是一个在 1997 年发现的 CPU Bug，影响了奔腾 MMX 和奔腾 OverDrive 处理器等基于 P5 微架构的 CPU。这个 Bug 会导致电脑出现死机之类的情况，需要重启才能继续使用电脑。这也是一个影响深远的 Bug，以至于现在很多硬件设计的缺陷都用 F00F 来命名。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibDiaic10IZlsMKav2PfsTKb7NXpY3iac0JDjn2TUI9GibyB4fOJGELn7C0Q/640?wx_fmt=png)
**解决方案**
英特尔通过更新处理器的步进修复了奔腾 F00F bug，B2 步进已经解决了相关的问题。不过对于之前出现错误的 CPU，也可以通过操作系统的更新来修复，因此这是一个可以通过软件修复的 CPU Bug。
**矿渣奔腾 III 1.13GHz**
2000 年，英特尔为抗衡 AMD 发布的 1GHz CPU，迅速的推出 Pentium III 1.13GHz。却未料，大部分用户在安装 Pentium III 1.13GHz 之后，发现处理器在使用中发生了执行部分软件时死机、不稳定等情况。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibHlCat9N3CMclJd2zPdJ1AtfktX3DkKIA1BGtZS80IzegJesRAkmuXw/640?wx_fmt=png)
图片来源：太平洋电脑网
**解决方案**
因英特尔草率地推出 Coppermine 架构的奔腾 III CPU，其在发售不到一个月后便召回了所有的 CPU。
**蓝屏及死机的英特尔 Broadwell MCE Bug**
随着英特尔 Broadwell 的广泛推广，不少 Broadwell 处理器用户发现在使用时，会经常出现蓝屏死机，Linux、Windows 系统报错是 MCE( Machine Check Exception，机器异常检查)。这个问题是有 SpeedStep 节能技术导致的，关闭了 SpeedStep 后，问题就不会出现。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibycOqVB20QT19guGAAZrQLmmHLic6siaRL1Tf7vYic8pmIclHiaNwpSUZCQ/640?wx_fmt=jpeg)
**解决方案**
当然，就算可以关闭 SpeedStep 来避免 Bug，用户也不会当是没事发生过，而且某些设备例如笔记本、平板电脑关闭了节能技术的话，会发热、能耗大增，甚至已经影响正常使用了。英特尔通过微码更新来修复了这一问题，只要更新 BIOS，Broadwell MCE Bug 就能够得到修复，因此这也是一个可以通过软件更新来解决的 CPU Bug。
**黑客可控制计算机的英特尔 ME 漏洞**
在 2008 年后，英特尔在 CPU 中内置了一个低功耗子系统 Management Engine 也就是 ME。ME 可以协助专业人员远程管理计算机，深受企业用户欢迎。不过，这个 ME 系统却爆出了问题，它原本设计用于远程维护，但由于存在漏洞反倒让黑客有可能通过 ME 后门进而控制计算机。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibFvAKLqoRPt1zJx9HRhw9RJxBE29yjbiaZYPaOGohb18fV2qDvA9OIsA/640?wx_fmt=jpeg)
**解决方案**
英特尔很快就公布了相应的修复方法，释放了相应的代码，用户可以通过更新主板 BIOS、操作系统和相应的软件、驱动来修复 ME 相关的漏洞。通过软件就能修复这个 CPU 漏洞，也算是不幸中的大幸吧。
以上是作为全球知名芯片制造商的英特尔近些年 CPU 发生的一些重大 Bug，然而，不仅仅是英特尔，AMD、高通、苹果也曾遇到过一些大的处理器漏洞。
**折翼的 Phenom TLC Bug**
AMD 的第一代 Phenom(羿龙)处理器被赋予了迎击 Intel 酷睿处理器的重任，首次使用了三级缓存的设计。然而不幸的是，率先登场的 B2 步进的 Phenom 竟然存在 TLB 的 Bug。TLB 是用来连接内存和 CPU 缓存的桥梁，在有 Bug 的 Phenom 处理器中，TLB 会导致 CPU 读取页表出现错误，出现死机等情况。出了 Bug 后，AMD 就提供了软件方案来解决问题。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibB4CK5fWtPB0QBzn19oao6JvCvHVkPVxdh6DSjcDsicZQ0JrC0r13aMQ/640?wx_fmt=jpeg)
图片来源：太平洋电脑网
**解决方案**
AMD 通过系统补丁及 BIOS 的方法避开了这个硬件 Bug，但会造成性能下降 AMD 发布了一个新 BIOS，也为 Win 系统提供了一个补丁，无论是那种方法，其作用都是屏蔽某段页表乃至 CPU 缓存。这当然可以避免 Bug 的出现，不过也会造成性能降低。可以说，这个软件修复的方案只是权宜之计，其实并不完美。此后 AMD 推出了 B3 步进的 Phenom 处理器，从硬件上修改，才彻底解决了问题，步进改动后的 CPU 还从 9X00 改名为 9X50，可见此次修补之重要。
**火炉的高通骁龙 810**
2015 年，高通旗下高端 CPU 骁龙 810 发生发热问题。事实上，骁龙 810 芯片推出的主要原因是为了跟进苹果 A7 处理器的 64 位寻址的竞争，骁龙 810 使用 ARM 公版 64 位A 57/A53 架构，而自家一直研发的 Krait 架构 64 位寻址在短期内尚未实现，于是只能用公版四个高性能核心+四个低性能核心的组合，高通在 Krait 架构上积累的经验无法套用到 ARM 公版上来，于是导致了 810 惨淡收场的后果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibI3aJYneRDsbsib8dpKh6iamQzRxl8jdn1k4Iu3f0pWIZer0ibA884kBpA/640?wx_fmt=png)
图片来源：太平洋电脑网
**引发退货潮流的 iPhone 6S A9 处理器**
2015 年，iPhone 6S A9 处理器上有两个版本，一个是出自台积电，而另外一个是三星，前者基于 16nm 制程，而后者则是 14nm 制程，由于工艺上的不同，导致它们性能、续航上有了不同，由此引发的“芯片门”备受关注。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuhSRoiaWyQRumHLubslBKUYibTXa6yfAHbFicicUUsPiaXV9sUB8ibyCyTDZXnIcZibfYEDVcTsouwzbic6DQ/640?wx_fmt=jpeg)
**解决方案**
后续苹果官方迅速表态，不同 iPhone 6S 芯片之间功耗差距极小，仅 2%~3%。但部分用户并不买账，要求退货，在中国，不少港台地区用户换货成功，大陆用户似乎并未受到太大影响。
**总结**
Bug 不仅仅会出现在软件方面，在硬件上也会时常发生。整体而言，硬件上的问题似乎要比软件漏洞更为严重，而作为程序员， CPU 上的部分 Bug 也完全可用软件来修补。虽然缺陷会不可避免的存在，但是厂商还是应该加强自身管理及测试，避免牵一发而动全身的两难局面出现。同时作为普通消费者，要时刻关注软件更新，将已有的漏洞带来的威胁降到最低。
参考来源
http://pcedu.pconline.com.cn/1064/10641944_all.html
http://diy.pconline.com.cn/707/7074402_all.html#content_page_4
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
