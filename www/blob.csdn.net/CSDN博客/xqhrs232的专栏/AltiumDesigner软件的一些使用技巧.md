# AltiumDesigner软件的一些使用技巧 - xqhrs232的专栏 - CSDN博客
2015年03月12日 23:58:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：844
原文地址::[http://blog.sina.com.cn/s/blog_66fc93570100qk0i.html](http://blog.sina.com.cn/s/blog_66fc93570100qk0i.html)
相关文章
1、Altium Designer使用----[http://wenku.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwWENnSuPS32QO8X0a0gHpOOzdnNt_K0mK2cucVaEHVS3i23zuxchyltyeM4A2UozctsGtN7PIdRcLG59ZwqHAS](http://wenku.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwWENnSuPS32QO8X0a0gHpOOzdnNt_K0mK2cucVaEHVS3i23zuxchyltyeM4A2UozctsGtN7PIdRcLG59ZwqHAS)
2、ALTIUM
 DESIGNER----[http://baike.baidu.com/link?url=yYloIRhg7Bg3wntlLVHIZeisSLojqxBNOi0yuZfF7_T_N8pWfNLBbPgdvkv78pIc01keYXeZajKaf4hALOHR0a](http://baike.baidu.com/link?url=yYloIRhg7Bg3wntlLVHIZeisSLojqxBNOi0yuZfF7_T_N8pWfNLBbPgdvkv78pIc01keYXeZajKaf4hALOHR0a)
3、[AD10 开始PCB项目](http://blog.csdn.net/misskissc/article/details/12625689)----[http://blog.csdn.net/misskissc/article/details/12625689](http://blog.csdn.net/misskissc/article/details/12625689)
**原文地址：**[AltiumDesigner软件的一些使用技巧](http://blog.sina.com.cn/s/blog_595296f50100ojdn.html)**作者：**[沈燕飞](http://blog.sina.com.cn/u/1498584821)
AltiumDesigner软件的一些使用技巧：
一、多引脚集成电路芯片封装SOIC、SOP、TSOP在AD7.1元器件封装库中的命名含义。
例如：SOIC库分为L、M、N三种。
L、M、N --代表芯片去除引脚后的片身宽度，即芯片两相对引脚焊盘的最小宽度。其中L宽度最大，N次之，M最小。
--这里选择名称为SOIC_127_M的一组封装为例，选择改组中名为SOIC127P600-8M的封装。
其中，127P --代表同一排相邻引脚间距为1.27mm；
      600 --代表芯片两相对引脚焊盘的最大宽度为6.00mm；
      -8 --代表芯片共有8只引脚。
二、封装库中，名为DPDT的封装含义为（Double Pole Double Throw），同理就有了封装名称SPST、DPST、SPDT；
三、让软件中作为背景的电路板外形与实际机械1层定义的尺寸（无论方圆）等大的办法。
首先，在PCB Board Wizard中按照实际尺寸初步Custom一块板子（一定要合理设置keepout间距，一般为2mm）。然后在Edit->Origin中为电路板设置坐标原点，将生成的电路板尺寸设置在机械1层，如果不喜欢板子四周的直角怕伤手，可以将四脚重新定义为弧形并标注尺寸。选定所有机械1层上电路的尺寸约束对象，然后选择Design->Board Shape->Define from select,即可完成背景电路板外形的设置。
四、关于Design->Rules的一些设置技巧。
1、如果设计中要求敷铜层（及内电层）与焊盘（无论表贴还是通孔）的连接方式采用热缓冲方式连接，而敷铜层（及内电层）与过孔则采用直接连接方式的规则设置方法：
敷铜层设置方法：
在规则中的Plane项目中找到Polygon Connect style项目，新建子项名为：PolygonConnect_Pads，设置where the first object matches为：(InPADClass('All
 Pads'))，where the second object matches为：All；并选择连接类型为45度4瓣连接。
又新建子项名为：PolygonConnect_Vias，设置where the first object matches为：All，where the second object matches为：All；并选择连接类型为直接连接方式。
在侧边栏中选中其中任何一个子项，点击坐下方Priorities按钮，将PolygonConnect_Pads子项的优先级设置为最高级别然后关闭。
内电层设置方法：
同样，在Power Plane Connect Style项目中，新建子项名为：PlaneConnect_Pads，设置where the
 first object matches为：(InPadClass('All Pads'))；连接类型为4瓣连接。
又新建子项名为：PlaneConnect_Vias，设置where the first object matches为：All；连接类型为直接连接方式。
在侧边栏中选中其中任何一个子项，点击坐下方Priorities按钮，将PolygonConnect_Pads子项的优先级设置为最高级别然后关闭。
2、敷铜层（敷铜层为铜皮）与走线过孔以及焊盘的间距设置方法：
在Electrical项目中新建子项名为：Clearance_Polygon，设置where the first object matches为：(IsRegion)，where the second object matches为：All；并设置间距一般为20mil以上，30mil合适。
3、敷铜层（敷铜层为网格敷铜方式）与走线过孔以及焊盘的间距设置方法：
需要将走线间距由原来的9、10mil设置为需要敷铜的间距30mil，然后敷网格铜。待敷铜结束后，将走线间距改回为原来的间距，系统就不会报错了。
五、带有敷铜层和内电层的四层以上板，为了显示电路板层数，需要加入层标，在每一层上用数字标识，将层标处对准明亮处可以看到每一层的标识。
由于层标处需要透光，所以该区域不能有任何敷铜以及内电层通过。所以，首先在keepout层画出一个矩形框，阻隔上下两个敷铜层通过；然后用Place->Polygon Pour Cutout命令分别在每一个内电层上切除一个矩形框区域，这些区域要完全重叠，用于透光；最后在每一层上放置相应的层标字符。
六、在发热量较大的芯片下敷网格铜，而其他区域敷铜皮方法：
还是利用keepout线在发热芯片对应区域的禁止布线层（keepout层）圈出芯片的外形来；
然后开始整板敷铜皮，看到的结果是，所有发热芯片位置的敷铜没有了。
注意：还要将芯片底部的所有接地过孔设置为NoNet，不让它接地！（以免敷铜皮时，芯片内部没有靠近keepout线的区域也被敷上了铜皮。）
接下来是删除先前在keepout层的画线；
下面就好办了，同样还是敷铜，这回是在发热芯片区域敷网格铜，不必担心，可以圈出一个较大的敷铜区域以免芯片区域敷铜不完整，即便是占用了被敷了铜皮的位置，敷铜结果还是铜皮。
