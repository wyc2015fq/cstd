# CMOS与TTL比较 - xqhrs232的专栏 - CSDN博客
2014年04月19日 00:34:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：706
个人分类：[各种电子元器件/硬件技术](https://blog.csdn.net/xqhrs232/article/category/6300755)
原文地址::[http://blog.csdn.net/augusdi/article/details/17203589](http://blog.csdn.net/augusdi/article/details/17203589)
相关文章
1、[常用电平标准(TTL、CMOS、LVTTL、LVCMOS、ECL、PECL、LVPECL、RS232)](http://blog.csdn.net/augusdi/article/details/17203431)----[http://blog.csdn.net/augusdi/article/details/17203431](http://blog.csdn.net/augusdi/article/details/17203431)
### [CMOS 与 TTL 比较](http://blog.csdn.net/makebuaa/article/details/7862147)
MOS是：金属-氧化物-半导体（Metal-Oxide-Semiconductor）结构的晶体管简称MOS晶体管，有P型MOS管和N型MOS管之分。由MOS管构成的集成电路称为[MOS集成电路](http://baike.baidu.com/view/45381.htm)，而由PMOS管和NMOS管共同构成的互补型MOS集成电路即为 [CMOS-IC](http://baike.baidu.com/view/243911.htm)（
 Complementary MOS Integrated Circuit）。
　　目前[数字集成电路](http://baike.baidu.com/view/381505.htm)按导电类型可分为[双极型集成电路](http://baike.baidu.com/view/295810.htm)（主要为TTL）和单极型集成电路（[CMOS](http://baike.baidu.com/view/22318.htm)、NMOS、PMOS等）。CMOS电路的单门[静态功耗](http://baike.baidu.com/view/4518781.htm)在毫微瓦（nw）数量级。
　　CMOS发展比TTL晚，但是以其较高的优越性在很多场合逐渐取代了TTL。
　　以下比较两者性能，大家就知道其原因了。
　　1.CMOS是场效应管构成，TTL为[双极晶体管](http://baike.baidu.com/view/501948.htm)构成
　　2.CMOS的[逻辑电平](http://baike.baidu.com/view/2280549.htm)范围比较大（5～15V），TTL只能在5V下工作
　　3.CMOS的高低电平之间相差比较大、抗干扰性强，TTL则相差小，抗干扰能力差
　　4.CMOS功耗很小，TTL功耗较大（1～5mA/门）
　　5.CMOS的工作频率较TTL略低，但是高速CMOS速度与TTL差不多相当。
**集成电路中详细信息：**
　　1,TTL电平：
　　输出高电平>2.4V，输出低电平<0.4V。在室温下，一般输出高电平是3.5V，输出低电平是0.2V。最小输入高电平和低电平：输入高电平>=2.0V，输入低电平<=0.8V，[噪声容限](http://baike.baidu.com/view/1045377.htm)是0.4V。
　　2，CMOS电平：
　　1逻辑电平电压接近于电源电压，0逻辑电平接近于0V。而且具有很宽的噪声容限。
　　3，电平转换电路：
　　因为TTL和CMOS的高低电平的值不一样（[ttl](http://baike.baidu.com/view/2696.htm) 5v<==>cmos 3.3v），所以互相连接时需要电平的转换：就是用两个电阻对电平分压，没有什么高深的东西。
　　4，驱动门电路
　　OC门，即集电极开路门电路，OD门，即漏极开路门电路，必须外接[上拉电阻](http://baike.baidu.com/view/1106477.htm)和电源才能将开关电平作为高低电平用。否则它一般只作为开关大电压和大电流负载，所以又叫做驱动门电路。
　　5，TTL和CMOS电路比较：
　　1）TTL电路是电流控制器件，而CMOS电路是电压控制器件。
　　2）TTL电路的速度快，传输延迟时间短（5-10ns），但是[功耗](http://baike.baidu.com/view/720038.htm)大。
　　CMOS电路的速度慢，传输延迟时间长（25-50ns），但功耗低。
　　CMOS电路本身的功耗与输入信号的脉冲频率有关，频率越高，芯片集越热，这是正常现象。
　　3）CMOS电路的[锁定效应](http://baike.baidu.com/view/1699762.htm)（擎柱效应）：
　　CMOS电路由于输入太大的电流，内部的电流急剧增大，除非切断电源，电流一直在增大。这种效应就是锁定效应。当产生锁定效应时，CMOS的内部电流能达到40mA以上，很容易烧毁芯片。
　　防御措施：
　　1）在输入端和输出端加钳位电路，使输入和输出不超过不超过规定电压。
　　2）芯片的电源输入端加去耦电路，防止VDD端出现瞬间的高压。
　　3）在VDD和外电源之间加限流电阻，即使有大的电流也不让它进去。
　　4）当系统由几个电源分别供电时，开关要按下列顺序：开启时，先开启CMOS电路得电源，再开启输入信号和负载的电源；关闭时，先关闭输入信号和[负载](http://baike.baidu.com/view/750938.htm)的电源，再关闭CMOS电路的电源。
　　6，CMOS电路的使用注意事项
　　1）CMOS电路时电压控制器件，它的输入总抗很大，对干扰信号的捕捉能力很强。所以，不用的管脚不要悬空，要接上拉电阻或者下拉电阻，给它一个恒定的电平。
　　2）输入端接低内阻的信号源时，要在输入端和信号源之间要串联限流电阻，使输入的
　　电流限制在1mA之内。
　　3）当接长信号传输线时，在CMOS电路端接匹配电阻。
　　4）当输入端接大电容时，应该在输入端和电容间接保护电阻。电阻值为R=V0/1mA.V0是外界电容上的电压。
　　5）CMOS的输入电流超过1mA，就有可能烧坏CMOS。
　　7，TTL门电路中输入端负载特性（输入端带电阻特殊情况的处理）：
　　1）悬空时相当于输入端接高电平。因为这时可以看作是输入端接一个无穷大的电阻。
　　2）在门电路输入端串联10K电阻后再输入低电平，输入端出呈现的是高电平而不是低电平。因为由TTL门电路的输入端负载特性可知，只有在输入端接的串联电阻小于910欧时，它输入来的低[电平信号](http://baike.baidu.com/view/1573140.htm)才能被门电路识别出来，串联电阻再大的话输入端就一直呈现高电平。这个一定要注意。CMOS门电路就不用考虑这些了。
　　8，TTL和CMOS电路的输出处理
　　TTL电路有集电极开路[OC门](http://baike.baidu.com/view/1400863.htm)，MOS管也有和集电极对应的漏极开路的OD门，它的输出就叫做开漏输出。OC门在截止时有漏[电流](http://baike.baidu.com/view/10897.htm)输出，那就是漏电流，为什么有漏电流呢？那是因为当三极管截止的时候，它的基极电流约等于0，但是并不是真正的为0，经过三极管的集电极的电流也就不是真正的0，而是约0。而这个就是漏电流。开漏输出：OC门的输出就是开漏输出；OD门的输出也是开漏输出。它可以吸收很大的电流，但是不能向外输出的电流。所以，为了能输入和输出电流，它使用的时候要跟电源和上拉电阻一齐用。OD门一般作为输出缓冲/驱动器、电平转换器以及满足吸收大负载电流的需要。
　　9，什么叫做图腾柱，它与开漏电路有什么区别？
　　TTL集成电路中，输出有接上拉三极管的输出叫做图腾柱输出，没有的叫做OC门。因为TTL就是一个三级关，图腾柱也就是两个三级管推挽相连。所以推挽就是图腾。一般图腾式输出，高电平400UA，低电平8MA.
[7]:
　　打开电脑的主机箱，可以在主板右侧看到一块"圆"形成扁体的电池，这块电池也称CMOS电池，保存主板信息的BIOS设置，我在网吧工作，经常碰到主机启动不了的情况，一般比较容易见效的方法是：将主机电源拔出来，意思是把电源线从电源盒拿下来，这样是完全断电状态，取下主板电脑可以看到两个金属片，成上下，也就是正\负极电路，将其对接让它短路，按着几秒钟，放电基本成功.
　　还有一种叫小COMS放电：同样将电源线从电源盒上拔下来，在这样的状态下按"开机"按钮，重试几下，系统也将小放电，一般也可以解决电脑无法开机的问题.
[http://baike.baidu.com/view/22318.htm](http://baike.baidu.com/view/22318.htm)
