# PADS四 PCB的走线----有实际电路图演示 - xqhrs232的专栏 - CSDN博客
2014年03月09日 21:27:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3153
原文地址::[http://blog.sina.com.cn/s/blog_96a11ddf0101r0p4.html](http://blog.sina.com.cn/s/blog_96a11ddf0101r0p4.html)
相关文章
1、PADS三 PCB的布局----[http://blog.sina.com.cn/s/blog_96a11ddf0101qeph.html](http://blog.sina.com.cn/s/blog_96a11ddf0101qeph.html)
2、PADS四 PCB的走线----[http://blog.sina.com.cn/s/blog_96a11ddf0101r0p4.html](http://blog.sina.com.cn/s/blog_96a11ddf0101r0p4.html)
3、PADS五 PCB的电源处理及灌铜----[http://blog.sina.com.cn/s/blog_96a11ddf0101r9cy.html](http://blog.sina.com.cn/s/blog_96a11ddf0101r9cy.html)
4、PADS六 丝印的处理----[http://blog.sina.com.cn/s/blog_96a11ddf0101rb4a.html](http://blog.sina.com.cn/s/blog_96a11ddf0101rb4a.html)
5、PADS七 输出Gerber文件----[http://blog.sina.com.cn/s/blog_96a11ddf0101rbwz.html](http://blog.sina.com.cn/s/blog_96a11ddf0101rbwz.html)
6、画好的板子PADS 看不到铺铜--解决方法----[http://blog.sina.com.cn/s/blog_96a11ddf0101rb3x.html](http://blog.sina.com.cn/s/blog_96a11ddf0101rb3x.html)

PADS四 PCB的走线
在PCB Router中，走线为F3,删除为Backspace
在PCB Layout中，走线为F2,删除为Delete
首先将原理图与PCB保持同步
对于PCB的走线，我们先将DDR走线（数据、地址的等长，部分线的差分）以及USB，HDMI等高速信号线走完毕后再走其它比较复杂的信号线，再将电源线拉完毕并铺铜！
在这里暂时不讨论DDR的走线
差分走线的设置
首先在原理图中找出差分对，将PCB LAYOUT点击进入PCB Router模式，
VIEW-Project Explorer,打开项目导航栏，在Net Objects栏目下的Nets栏目中找出差分对并将其复制，
![PADS四 <wbr>PCB的走线](http://s12.sinaimg.cn/mw690/002L1CUfgy6FdeaWBMD2b&690)
再将复制后的差分对粘贴到Net Objects下的Differential Pairs栏目下
![PADS四 <wbr>PCB的走线](http://s5.sinaimg.cn/mw690/002L1CUfgy6Fdel06age4&690)
差分对布线宽度和间距的设置
高速信号线的线宽一般设置为5-8mil,间距为线宽的宽度
USB的线宽一般设置为5-8mil,间距为线宽的宽度，在这里我们点选所有的USB差分对并右键属性，设置其线宽线距均为8mil（一般7mil以上）
HDMI的线宽一般设置为5mil,间距为5MIL，（5/6/5)
以太网的线宽一般设置为6mil,间距为6MIL，(6/8/6)
![PADS四 <wbr>PCB的走线](http://s5.sinaimg.cn/bmiddle/002L1CUfgy6FdhPgM1S84&690)
差分对连接技巧（在PADS Router中）
技巧一：选择差分对，按F3拉至合适位置后complate (半自动布线功能）
如下图，选择差分对中的一条线如(TX_D+)，按快捷键F3,拉至如下状态后点击鼠标右键，选择完成complate,此时将保留前面所保留的状态，后面的连线将自动完成。
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/mw690/002L1CUfgy6FdFXT0gp91&690)![PADS四 <wbr>PCB的走线](http://s8.sinaimg.cn/mw690/002L1CUfgy6FdGk4SrR37&690)
技巧二:选择差分对，右键直接选择Route(全自动布线功能）
拉线完毕，需对走线进行调整，选择需要调整的走线，移动到合适位置即可。
差分对走完后如下图所示
下图为USB差分对走线
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/mw690/002L1CUfgy6FdJ57T8t21&690)
下图为HDMI和RJ45的差分对走线
![PADS四 <wbr>PCB的走线](http://s10.sinaimg.cn/mw690/002L1CUfgy6FdJkAeOBb9&690)
差分对走完后，接下来开始进行包地处理
如下图，在差分对之间分别复制连个地过孔到差分对间，在过孔间连接地引线即可
![PADS四 <wbr>PCB的走线](http://s15.sinaimg.cn/bmiddle/002L1CUfgy6FePxMb4a5e&690)
设置走线与铺铜的间距，在这里设置为10mil
Setup-Design Rules.(或者按F5),进入Rules(设置）对话框，如下图，选择Net,在net rules对话框中选择需要设置的网路，再点Clearance选项，设置Trace与Copper的对话框为10mil
![PADS四 <wbr>PCB的走线](http://s16.sinaimg.cn/mw690/002L1CUfgy6FeRCubIHcf&690)
铺铜看效果
绘制铜皮并设定网络：Drafting Toolbar, 选择Copper Pour,右键矩形框选整个板框。设置网络为GND.
开始灌铜：Tools-Poor Manager,在Pour Manager对话框中选择Flood all, start开始灌铜！
![PADS四 <wbr>PCB的走线](http://s4.sinaimg.cn/mw690/002L1CUfgy6FeT125VN33&690)
走线的时候先一组一组的按照功能拉通然后再逐层的梳理调整位置
![PADS四 <wbr>PCB的走线](http://s1.sinaimg.cn/mw690/002L1CUfgy6FifZdPiMe0&690)
走线拉通完毕开始拉电源线
关于电源网络的走线宽度，硬件工程师会根据电流的需求给出各电源所需的宽度及跨层时所需的过孔个数及孔径大小建议。
高速信号线和普通信号线拉完毕， 
 检查是否还有信号线没有连接，TOOLS-Verify Design,
如下图所示，尚有信号线没被连接， 我们根据Explanation中的提示找到相应的物料
并在PCB LAYOUT中进行搜索（如SS R353）
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/mw690/002L1CUfgy6FjHWSSmR31&690)
待所有的信号线连接完毕，开始处理电源和地，我们优先处理功能电源，再处理主干电源，最后开始处理地。
如下图逐个的将功能部分的电源在Top或BOT层面将他们连起来。
![PADS四 <wbr>PCB的走线](http://s12.sinaimg.cn/mw690/002L1CUfgy6Fk90c6QHdb&690)
对于由DC-DC,LDO等产生的大电流主干电源我们留到后面进行专门的连接并在电源层进行铺地处理。
关于主干电源 OTG-PWR的处理
如图OTG-PWR电源主要集中在micro USB处，走线比较集中，而VBUS分布比较泛，需要大量普通，于是我们可以将左上脚的D18移到右边的micro USB处。
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/bmiddle/002L1CUfgy6FtqjjQBP11&690)![PADS四 <wbr>PCB的走线](http://s11.sinaimg.cn/mw690/002L1CUfgy6FtqrDfjsea&690)
经调整后，其效果如下图所示，走线宽度为40MIL,且走线非常短
![PADS四 <wbr>PCB的走线](http://s7.sinaimg.cn/bmiddle/002L1CUfgy6FtrgqAEme6&690)
先将各功能部分的电源（如VCC_HDMI，AUDIO_VDDA，VCC_RMII，HOST-PWR等）以及部分主干电源（如OTG-PWR等）连起来，这些电源的特点是比较集中在某一处，我们可以优先将他们处理。
而其它主干电源（如VSYS,VCC_IO)则比较分散，分散在电源的各处，此时我们需要针对他们所分布的范围在电源层（第3层）铺设合适的铜皮。
我们可以针对这类分散电源进行打过孔并根据过孔的分布情况来绘制第三层的铜皮网络。
对于主干电源，由于通过电流比较大，在换层时我们需要多打一些过孔。
如下图粉色所Highlight的部分，我们可以看到VSYS电源所分布的情况
![PADS四 <wbr>PCB的走线](http://s15.sinaimg.cn/mw690/002L1CUfgy6Fty8UXiSbe&690)
打过孔的时候一定要记得将过孔粘住（勾选Glued)，否则刷新的时候会消失.
![PADS四 <wbr>PCB的走线](http://s10.sinaimg.cn/mw690/002L1CUfgy6FtyHyJex59&690)
如下图为PMIC附近的VSYS网络，每个VSYS电源脚附近都挂有电容，将它们各自连接连接起来并打过孔，对于附近能连接起来的网络就用铜皮在TOP/BOT层连接起来。
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/mw690/002L1CUfgy6FtCZgeAh51&690)
对于BGA封装下的VCC_IO网络，注意每个PAD点附近都有过孔。以便后续在电源层通过过孔的方式将他们连成一片
![PADS四 <wbr>PCB的走线](http://s2.sinaimg.cn/mw690/002L1CUfgy6Fv8cStXP41&690)
**电源层的分割与灌铜**一般在电源层需要铺设的电源铜皮有：
分布广泛的主干电源及其它耗电量比较大的DDR电源或CPU电源
分布广泛的主干电源：VCC_IO，VSYS
DDR电源：VCC_DDR
CPU电源：VDD_ARM,VDD_LOG
开始进行电源层的分割与铺铜
1.PCB的板邊至銅皮的距離一般選1mm. 最少不能小於0.5mm. 但當小於1mm時, 需有其它補救措施.
2. 零件孔心與板邊的距離一般要大於3mm. 
3.铜皮与铜皮间的距离暂时设置为12mil
在PADS当中板层定义如下图所示，另外加载那些需要在电源层铺设的网络
Setup-Layer definition
![PADS四 <wbr>PCB的走线](http://s7.sinaimg.cn/bmiddle/002L1CUfgy6FvdEu5a6b6&690)
Split/Mixed Plane是PADS专门针对多电源系统内电层分割给出的内电层属性。赋予该属性的内电层，只能通过“Plane area![PADS内电层分割与铺铜](http://s8.sinaimg.cn/bmiddle/002L1CUfgy6DRaIMU3t77&690)”命令以及“auto plane separate![PADS内电层分割与铺铜](http://s11.sinaimg.cn/mw690/002L1CUfgy6DRaMcZx81a&690)”命令进行电源层分割。
采用工具栏中的Plane Area来进行内部电源网络的铺设（混合层Split/Mixed Plane）
多个电源网络交错在一起时，需先通过颜色标识下各网络区域，并重新绘制各电源网络区域，如果铜皮无法绕过去，我们想办法通过其它层面将它们连接起来
如下图用PLANE AREA![PADS练习4-6层板](http://s8.sinaimg.cn/mw690/002L1CUfgy6DUeIls2ze7&690)将相同网络的过孔圈起来。各铜皮的连接可以挨在一起，但要避免尖角和锐角的出线，如下图，**铜皮之间的衔接主要是钝角和直角。**
**不同网络的铜皮可以紧挨着，铜皮也可以紧挨这板边画。**
但是我们必须在Design Rules中预先设置铜皮与铜皮，铜皮与板边的间距。
在这里我们将铜皮与铜皮间距设置为12mil；电源铜皮与板边的间距为40mil,地铜皮与板边的间距为8mil,
设置界面：
Setup-Design Rules (Defalt/Clearance)
![PADS练习4-6层板](http://s2.sinaimg.cn/mw690/002L1CUfgy6DWZKOqrLd1&690)
针对电源网络的设置
Setup-Design Rules (Net/Clearance)
指定电源网络与板边的间距为40mil
![PADS练习4-6层板](http://s13.sinaimg.cn/mw690/002L1CUfgy6DX0A3PL69c&690)
注意：画电源区域时，我们可以将这些**电源铜皮紧挨着电源铜皮以及板边进行绘画。**
对于个别没有被包括起来的网络要在其它层面连接起来。
（keepout区域可率先绘完）
通过上面铜皮与铜皮，铜皮与板边间距的设置，在铺铜时他们将彼此互相避开为我们所设置的距离。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=96a11ddf0101nr91&url=http://album.sina.com.cn/pic/002L1CUfgy6DUedi9vFcf)
![PADS练习4-6层板](http://s10.sinaimg.cn/bmiddle/002L1CUfgy6DUfFfQ1Pf9&690)
注意：在注意绘制铜皮前，需先切换到电源层。
![PADS四 <wbr>PCB的走线](http://s11.sinaimg.cn/mw690/002L1CUfgy6FvhiElaGea&690)
刚画的并不会实心显示，而只是一个一个的外框
在第三层铺完电源网络后，我们需要再铺一层地网络来填充其它空置的地方。
再选择Plane Area,右键Rectangle(方形框选）。框选完毕，如下指定地网络，点击Options,指定地铜皮的优先级为最后，如有5块电源铜皮，则地铜皮的优先级Flood Priority为6.也即先灌完电源铜皮后再来灌地铜皮。
![PADS练习4-6层板](http://s15.sinaimg.cn/mw690/002L1CUfgy6DQJGoDYG0e&690)
优先级设置完毕，开始灌铜处理。
OK后再F7(或Tools-Power Manager),切换到Pour Manager对话框中的Plane Connect，选择layers为电源层（默认），点击Start即可灌该电源层的所有铜皮，此时电源铜皮也是实心显示了。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=96a11ddf0101nr91&url=http://album.sina.com.cn/pic/002L1CUfgy6DQLhg4Lyc4)
![PADS四 <wbr>PCB的走线](http://s9.sinaimg.cn/bmiddle/002L1CUfgy6FwAoSYwo98&690)
而对于非电源层，如TOP层，也即层类型设置为No Plane 时
![PADS四 <wbr>PCB的走线](http://s12.sinaimg.cn/mw690/002L1CUfgy6FwAZMMxd1b&690)
我们利用Copper Pour框选住板框并指定网络为GND,OK!OK完毕，准备铺铜
![PADS四 <wbr>PCB的走线](http://s8.sinaimg.cn/bmiddle/002L1CUfgy6FwBhQ9efe7&690)
F7,进入Pour Manager对话框，选择Flood all, 进入setup...，设置焊盘与铜皮的连接方式
最后start,铺铜， 注意此处切换到Pour Manager的Flood页面，选择Flood all.
![PADS练习4-6层板](http://s3.sinaimg.cn/mw690/002L1CUfgy6DX7BWdjAd2&690)
TOP层铺铜效果如下所示
![PADS四 <wbr>PCB的走线](http://s15.sinaimg.cn/bmiddle/002L1CUfgy6FwBJdrhsee&690)
