# Linux图形界面正在发生根本性的变化 - 袁萌专栏 - CSDN博客
2010年11月16日 05:49:00[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1575标签：[linux																[图形																[服务器																[ubuntu																[语言																[unix](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
	
	
	
	p { margin-bottom: 0.21cm; }
    实际上，现在Linux图形界面已经非常美丽（或漂亮）了，但是，这种美丽的代价过于沉重。尤其是，Linux桌面的精美度与Win
7不相上下（两者就差那么一小点点儿）。怎么说呢？
    大家知道，Linux系统源于”仿真”Unix操作系统，体格强壮，稳健异常，很像非洲的大河马，你根本别想把它“搬倒”。但是，它的图形界面使用的却是X.org系统，笨重无比，就像QQ那样，这是客观事实。X.org是二十多年前的老系统了，比Linux本身还要“老资格”，一老一小，两者不“般配”。X.org过于复杂，过于沉重，系统文件太多，致使Linux桌面系统运行速度上不去（当然，目前已经算是很快了），拖住了Linux桌面进一步提高品质的后腿。
2008年11月3日，Kristian
Hogsberg提出一个大胆设想，给X“瘦身”，用C语言重写X服务器的全部源代码，搞一套轻型的X服务器，同时，Kristian给它其了个很好听的名字：Wayland（美国地名）显示服务器。那时，他为Wayland显示服务器搭起了一个非常粗糙的框架，只有3,200行C源代码。但是，Wayland开发计划的方向对头，思路正确，后面立即跟上一大帮追随者，大家动手干了起来。（注：请见“Wayland:
A New X Server for Linux”，2008年11月3日）
现在的互联网“可牛”了，即时通讯（ICQ、IRC）非常便利，无论你在世界何处，干起活儿来就如同在一间办公室里面，说说笑笑真愉快，多人干活不觉累。如今两年才过去了，Wayland开发计划已经初见成效，展露头角。今年11月4日，Wayland轻型显示服务器就被Ubuntu奠基人Mark
Shuttleworth先生“一眼看中”，决定吸收到Ubuntu
11.04新版本（2011.04.28发布）里面。对此，有不少人大发议论，“太冒进了”，“不可思议”，“简直是发疯”，......可见，老外也有这种“拖后退腿”的人。
刚才，我查看了几幅用Wayland制作的截图，感觉就是不一般，有一种很“轻快”的感觉，我以前从来没有见过这种类型所“人造图形”。我十分惊叹人类的智慧，竟然能够运用极为抽象的C语言编写出如此美妙的人间从来没有过的“图画”（或画面），真是绝了！11月14日，在最近召开的“Linux
Plumbers Conference”(“Linux开发者大会”)上，又传出消息，Fedora
15（2011.05.10发布）新版本亦将采用Wayland显示服务器。如此一来，各种不同的声音就要少多了。但是，摆在Wayland前面的工作（或道路）还是很艰巨的，知难而上，这才是Linux开发者的本色！
说明：在不久将来（或在1～2年之后），Linux桌面的“容貌”必定会越来越漂亮（或亮丽），比Win
7更加漂亮（比苹果也不差！），就像一位体格健美的大姑娘，不愁嫁不出去，连XP用户也得瞅上一眼，说不准儿也会偷偷地把XP黄脸婆踢出去另寻新欢呢！
