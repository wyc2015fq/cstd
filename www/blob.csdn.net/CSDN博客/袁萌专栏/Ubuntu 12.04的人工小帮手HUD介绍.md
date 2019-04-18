# Ubuntu 12.04的人工小帮手HUD介绍 - 袁萌专栏 - CSDN博客
2012年05月07日 05:23:48[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1377标签：[ubuntu																[firefox																[search																[function																[浏览器																[idea](https://so.csdn.net/so/search/s.do?q=idea&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
    Ubuntu12.04版本有什么新特点（或新面貌）？这个版本使计算机仿佛有了“灵性”。为什么？
进入12.04版本，不论你在何处，只要按一下开关键“Alt”，在屏幕左上角就会出现一个小帮手HUD如下：（注意：再按一下“Alt”键，它就会立即消失。）
![](http://news.bbcimg.co.uk/media/images/59980000/png/_59980494_delete.png)
由上图可见，HUD的”图标“就像是航空驾驶员的头盔，表明了HUD的由来。HUD就是“Head-UpDisplay”，意思是“抬头显示”，即只要航空驾驶员目视前方，专注目标，不必低头查看仪表盘数据，而仪表读数投射在视窗玻璃上，呈半透明状，可供驾驶员参考。由此就有了“抬头显示”（HUD）的说法。把HUD引入计算机领域，也算是一种“奇思妙想”吧？
半透明的HUD“面板”（panel）有是什么用处？只要你把心中的“意图”用文字（不论哪一种文字）键入“HUD面板”，你们，在该面板的下方立即就会出现一个“建议列表”（List)，而且，第一个选项瞬时增加亮度，以便引起你的注意，只要你觉得此选项内容比较符合自己的”意图“，立即按下”回车键“，计算机就会”领旨而行“。用英文表达这个意思就是：”The
 HUD tries to predict（预测）which function or file the user wants as they type it
 in“，其基本思想就是："It's driven by the idea that search or expressing your intent（意图）has
 become really powerful.”
很明显，HUD采用了人工智能技术（Fuzzy
 Matching模糊匹配以及机器学习）。但是，此时的HUD小帮手是不是很聪敏了呢?它能不能猜透你的”意图“？实际上，HUD开始不够聪敏，但是，它会不断地自动学习，摸索着猜透你的习惯与意图，会变得越来越聪敏。
比如说，由于你的粗心大意，把字母的顺序搞颠倒了，比如把“Firefox”打成了“Firefxo”。这不要紧，HUD会立即觉察出来，知道你的真正意图是“Firefox”，它会建议你使用“火狐浏览器”。做到这一点是很不容易的。HUD背后的理论根据是俄国学者Levenshtein于1965年提出的所谓“编辑”距离，即如何定义两个字符串之间的”编辑”距离。我们不难想象，给定一个字符串，要求在其他的100个字符串中间找出一个“最像它”的字符串，计算工作量是何等的巨大。不知Ubuntu开发者是怎么解决这个问题的。HUD技术不是一般的查找、搜索与匹配的简单“拷贝”。

