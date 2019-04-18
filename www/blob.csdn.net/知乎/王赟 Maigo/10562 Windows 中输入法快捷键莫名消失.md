# 10562 Windows 中输入法快捷键莫名消失 - 知乎
# 

.
　　这篇文章描述 Windows 中输入法快捷键莫名消失的 bug。我在使用 Windows 7 的时候就已经发现了这个 bug，在 Windows 10 中它依然存在。这个 bug 难以稳定地复现，我也没找到解决方案。尽管在网上已经有多处关于这个 bug 的报告（见文末 [1~6]），但由于描述不够细致，且需要使用多种语言和输入法的用户不多，这个 bug 一直没有引起足够的注意。本文英文版已经上报 Windows Feedback Hub [[7]](https://link.zhihu.com/?target=https%3A//aka.ms/Crlmts)。
## 一、Bug 描述

　　我的系统中安装了多种语言和输入法，如下图所示。为了方便地在各种输入法之间切换，我为常用的 7 种输入法设置了快捷键。快捷键的形式都是 Ctrl + Shift + 数字，数字 4~9 和 0 分别对应法语（加拿大键盘）、西班牙语、搜狗五笔、搜狗拼音、日语、韩语、英语。
![](https://pic3.zhimg.com/v2-74d0db391334e0ade77446a5df01b116_b.png)
*（图 1：输入法快捷键设置页面。可通过在语言栏上单击右键，然后依次选择 Settings、Advanced settings、Change language bar hot keys、Advanced Key Settings 调出）*

　　我发现，在设置快捷键后不久（一般为几天），就会有一部分快捷键失效。通常情况下，失效的是 Ctrl + Shift + 4（法语）和 Ctrl + Shift + 6（搜狗五笔）。有人发现快捷键失效往往发生在「休眠再唤醒、登出再登入、关机再开机」这几种事件之后。我的体验是，快捷键失效与这些事件有相关性，但二者互为非充分非必要条件。

　　当快捷键失效后，我就需要调出图 1 中的窗口重新设置。我发现，这个页面中快捷键的显示与当时快捷键的有效情况并不总是一致。有时，快捷键已经失效，但图 1 中显示的快捷键仍然齐全；有时，在快捷键仍有效的情况下打开图 1 的窗口，却发现有些快捷键没有显示出来。按图 1 重新设置快捷键后点 OK 关闭窗口，会有一定的概率发现快捷键仍然无效；此时再打开图 1 窗口，快捷键有可能显示齐全，也可能有一部分缺失。

## 二、注册表中与此 bug 有关的部分

　　我试图找出这个 bug 发生的原因。经过搜索，我发现输入法快捷键设置储存在注册表中 HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys 这个位置。我在重新设置快捷键之后打开注册表中的这个位置，发现了 4 个子键，其内容如下图所示。为了节省篇幅，我把 4 个子键的内容放在了同一张图里。

![](https://pic1.zhimg.com/v2-c765f0d0b5fd6cfe652e3d2854f04060_b.png)*（图 2：注册表中与输入法快捷键有关的键值）*

　　在每个子键下，有 Key Modifiers、Target IME、Virtual Key 三个二进制值。其中 Target IME 是要切换成的输入法的代号，以小端格式显示，所以读的时候要倒过来读。根据这个帖子 [[8]](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html)，在注册表中 HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts 键下可以找到各代号的含义：
- e0230804 代表搜狗五笔；
- 04090409 和 040a0c0a 没有找到，但找到了 00000409 和 0000040a，推测二者分别代表英语和西班牙语；
- f0200c0c 没有找到，也没有找到与它类似的代码。

根据同一帖子 [[8]](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html) 中的解读，Key Modifiers 和 Virtual Key 二者合起来表示快捷键。Key Modifiers 的第一个字节表明使用哪些控制键，由 Shift (04)、Ctrl (02)、Alt (01) 相加得出；第二个字节表示使用左边还是右边的控制键，由左侧 (80)、右侧 (40) 相加得出。Virtual Key 则表示与控制键一同按下的键的 ASCII 码。按此分析，图 2 中四个子键表示的快捷键分别为：
- 任何一侧的 Ctrl + ASCII 码为 0xDE 的键，莫名其妙；
- 任何一侧的 Ctrl + Shift + 0，对应英语；
- 任何一侧的 Ctrl + Shift + 4，对应法语；
- 任何一侧的 Ctrl + Shift + 5，对应西班牙语。

可以看出，注册表中并没有包含全部的快捷键。有意思的是，注册表中包含的 Ctrl + Shift + 4 是最经常消失的两个快捷键之一，而几乎从不消失的 Ctrl + Shift + 7/8/9 却不在注册表里。

　　我试过在图 1 中修改快捷键设置，然后观察图 2 中注册表里的信息有没有变化，以及反过来，修改注册表里的信息，再观察图 1 中显示的快捷键有无变化。结果发现，多数时候这二者对于对方的改变没有反应，有时有反应，但也不与对方的变化一致。试图通过注册表找出 bug 原因的努力宣告失败。

## 三、一个部分有效的解决方案

　　帖子 [[1]](https://link.zhihu.com/?target=https%3A//answers.microsoft.com/en-us/windows/forum/windows_8-desktop/custom-hotkeys-to-change-input-language-disappear/66d1d89d-e5dc-41e1-a8b3-48d596ab8e11)、[[2]](https://link.zhihu.com/?target=https%3A//superuser.com/questions/1031736/language-hotkeys-removed-by-itself-in-windows-10) 提到了一个解决方案：把语言设置复制到欢迎界面、系统账号和新用户账号，操作方法如下图所示。我发现，如此操作后，各个快捷键能够保持 1~2 个月不消失，但终究仍会失效。

![](https://pic3.zhimg.com/v2-d8dfa36220bc1b80596001014682b8e2_b.png)*（图 3：复制语言设置的界面。可通过在语言栏上单击右键，然后依次选择 Settings、Advanced settings、Apply language settings to the welcome screen, system accounts, and new user accounts、Copy Settings 调出）*

　　研究注册表发现，这个操作的效果，是把 HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys 这个键，复制到了 HKEY_USERS 下面 .DEFAULT、S-1-5-18、S-1-5-19、S-1-5-20 这几个目录下。原先这个键只存在于 S-1-5-21-XXX 这个目录下，这也是 HKEY_CURRENT_USER 所指向的地方。看起来，这步操作的效果确实是复制了语言设置。但是，为什么它能够使快捷键的存活时间变长，仍然是个谜；并且，作为一种解决 bug 的手段，也太不直观。

## 参考文献

[1] [Custom hotkeys to change input language disappear frequently](https://link.zhihu.com/?target=https%3A//answers.microsoft.com/en-us/windows/forum/windows_8-desktop/custom-hotkeys-to-change-input-language-disappear/66d1d89d-e5dc-41e1-a8b3-48d596ab8e11)
[2] [Language Hotkeys removed by itself in Windows 10](https://link.zhihu.com/?target=https%3A//superuser.com/questions/1031736/language-hotkeys-removed-by-itself-in-windows-10)
[3] [Input language and other Windows 7 hotkeys reset every time](https://link.zhihu.com/?target=https%3A//superuser.com/questions/529917/input-language-and-other-windows-7-hotkeys-reset-every-time)
[4] [https://aka.ms/W7brr2](https://link.zhihu.com/?target=https%3A//aka.ms/W7brr2)
[5] [https://aka.ms/Yifkcc](https://link.zhihu.com/?target=https%3A//aka.ms/Yifkcc)
[6] [https://aka.ms/Luk7jg](https://link.zhihu.com/?target=https%3A//aka.ms/Luk7jg)
[7] [https://aka.ms/Crlmts](https://link.zhihu.com/?target=https%3A//aka.ms/Crlmts)
[8] [彻底摆脱SHIFT的阴影！--用注册表修改输入法热键 - carprog - 博客园](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/shangdawei/archive/2012/09/18/2691673.html)

