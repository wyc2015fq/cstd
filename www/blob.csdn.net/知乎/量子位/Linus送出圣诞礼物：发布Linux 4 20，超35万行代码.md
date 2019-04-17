# Linus送出圣诞礼物：发布Linux 4.20，超35万行代码 - 知乎
# 



> 乾明 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

这是礼物吗？


圣诞之时，一则邮件让国外的极客程序员们沸腾了，尤其是那群Linux的粉丝。

他们的“神”，Linus Torvalds，发了一封邮件，主题为：

**Linux 4.20已经发布..**
![](https://pic4.zhimg.com/v2-7b8d37ab077214a2b6c7daa90218eaeb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1011' height='1031'></svg>)
我们不那么严谨地翻译一下：

> 一想到大家都准备好去休息了，就感觉推迟发布（Linux）4.20没啥意思。
就目前的情况来看，这个版本没有啥已知的问题，就是下面的短日志，比我预想的要长一些……但绝对没有那种能够让你发出哀嚎的坑。
……
祝你圣诞快乐或者你选择的其他节日快乐
Linus

Linux论坛Phoronix的创始人Michael Larabel发表评论称，Linux 4.20的发布，说明Linux又度过了充满成就的一年。

那么，Linux 4.20都有什么特点呢？

## **超35万行代码**

根据Phoronix的报道，Linux 4.20有超过35万行代码，是一年多以来最大的内核更新。

更新基本集中在显卡、CPU和其他硬件的支持上，还删除了有争议的Speck算法。



![](https://pic1.zhimg.com/v2-9d28c7ddff420fd42228644e2a709184_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)



以下是一些主要的更新：
- 支持AMD Picasso APU和Raven 2 APU。
- 支持AMD Vega 20 7nm工作站GPU。
- Raven Ridge APU 的VCN JPEG 加速与Mesa 18.3用户空间代码相结合。
- AMD GPU内核驱动程序GPUVM性能改进。
- 支持Intel Icelake“Gen 11”显示。
- Haswell / Ivy / Valley View硬件的完全PPGTT支持。
- NVIDIA Nouveau驱动程序开源初步支持HDMI 2.0和初步支持NVIDIA Xavier“Tegra194”SoC的显示。
- 虚拟KMS ( VKMS )具有初步光标和GEM支持。
- 支持高通骁龙 835 SoC。
- Linux x86 / x86_64优化。
- 初步支持AMD Zen 2 CPU。
- 许多其他DRM更改。
- 从内核中删除NSA的Speck加密算法。
- 继续修复2038年问题。
- USB驱动程序更新。
- ……

想要看更加详细的功能更新？传送门在文末。

现在，是时候谈谈你应不应该更新的问题了。

## **你应该更新吗？**
![](https://pic3.zhimg.com/v2-06130d416fb3ab39086112d2428f9562_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)
在It’s Foss上的一篇文章中，给出了更新建议。

如果你是一个普通的Linux用户，就不要尝试去手动安装新内核了。不管4.20有多香，你总不想让电脑变成砖头吧？

在基于Ubuntu的发行版中，有多种方法可以很轻松的升级内核。不过，最好的建议还是坚持使用发行版提供的内核，这样至少稳。

使用像Arch Linux这样滚动发行版的用户，应该很快就能用上内核4.20了。

如果你是高级的Linux用户，要注意4.20不是一个提供长期支持的版本，升级不升级，就看你自己了。

最后，传送门请收好~

## **传送门**

Linus的邮件：
[https://lkml.org/lkml/2018/12/23/187](https://link.zhihu.com/?target=https%3A//lkml.org/lkml/2018/12/23/187)

Linux 4.20的下载地址：
[https://kernel.org/](https://link.zhihu.com/?target=https%3A//kernel.org/)

Linux 4.20详细功能列表：

[https://www.phoronix.com/scan.php?page=article&item=linux-420-features&num=1](https://link.zhihu.com/?target=https%3A//www.phoronix.com/scan.php%3Fpage%3Darticle%26item%3Dlinux-420-features%26num%3D1)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


