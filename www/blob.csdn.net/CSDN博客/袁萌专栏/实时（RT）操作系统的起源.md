# 实时（RT）操作系统的起源 - 袁萌专栏 - CSDN博客
2015年04月30日 01:46:36[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1494

2014年3月25日，微软首次正式对外公开早期MS-DOS的源代码。回顾以往，1980年，[西雅图电脑产品公司](http://zh.wikipedia.org/w/index.php?title=%E8%A5%BF%E9%9B%85%E5%9B%BE%E9%9B%BB%E8%85%A6%E4%BA%A7%E5%93%81%E5%85%AC%E5%8F%B8&action=edit&redlink=1)（Seattle
 Computer Products）的一名24岁的程序员TimPaterson花软磁盘费了四个月时间编写出了软磁盘操作系统（[86-DOS](http://zh.wikipedia.org/w/index.php?title=86-DOS&action=edit&redlink=1)）。1981年7月，[微软](http://zh.wikipedia.org/wiki/%E5%BE%AE%E8%BD%AF)以五万[美元](http://zh.wikipedia.org/wiki/%E7%BE%8E%E5%85%83)的代价向西雅图公司购得本产品的全部版权，并将它更名为MS-DOS，并于1981年8月12日，正式发布MS-DOS1.0和PC-DOS1.0。
1980年，加拿大两个大学生[Gordon
 Bell](http://en.wikipedia.org/wiki/Gordon_Bell_%28QNX%29)和[Dan
 Dodge](http://en.wikipedia.org/wiki/Dan_Dodge),在学习操作系统设计课程期间，萌发了设计“实时操作系统（RTOS）”的念头，并且动手干了起来，最终搞出了一个实时操作系统的微内核。期初，他们针对的不是软盘，而是Intel的8088CPU，出发点与DOS操作系统不同。这两个小毛头成了后来大名鼎鼎的QNX实时操作系统的奠基人。
从时间上来看，DOS与QNX产生同一年代，而且都是“小毛头”年轻人搞出来的。发展至今，前者成了Win
 10，后者成了美国无人机与瑞士太阳能飞机的实时操作系统，两者的用途不同。Win 10与QNX都是无可替代的操作系统，其它操作系统都是小兄弟，我们的“国产操作系统“根本排不上”号“。
实际上，QNX实时操作系统建立在”微内核“之上，与Linux与Android的”单体内核“不同。微内核很小很小，只有CPU调度、进程通信、中断重定向以及时间系统等内容，其它都算是外围应用服务程序，据此，对系统外部的任何”风吹草动“都会立即（实时）做出反应。
阳光动力2号飞越太平洋是一项真正意义上的探险活动，如果机翼倾斜15度，飞机就会发生”翻转“失去控制的危险。机身倾斜5度，QNX实时操作系统就会发出信号，提醒飞行员注意仪表，调整飞行姿势。
说明：明日是”国际五一劳动节“，预祝大家节日快乐！
袁萌
4月30
