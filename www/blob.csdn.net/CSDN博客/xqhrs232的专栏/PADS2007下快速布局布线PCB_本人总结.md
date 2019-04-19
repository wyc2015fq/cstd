# PADS2007下快速布局布线PCB_本人总结 - xqhrs232的专栏 - CSDN博客
2013年10月01日 12:19:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2214
                
//PADS2007下快速布局布线PCB_本人总结.txt
1、从sch文件导出pcb可用的xxx.asc文件
     1111----执行PADS Logic下Tools菜单下的Layout Nestlist...命令，这样就会产生sch文件对应的xxx.asc+xxx.err文件，
这一步要确保xxx.err文件没有错误，有错误的排查，否则影响在PADS Layout对xxx.asc文件的使用
2、从PADS Layout导入产生的xxx.asc文件
     1111----执行PADS Layout下File菜单下的Import...命令，
这个时候就可以在pcb文件里面看到sch文件里面所有元器件的封装及电气连接了
3、散开所有的元器件并隐藏元器件之间的电气连接
     1111----前面一个步骤完成后，pcb文件里面的所有的元器件都叠在一起，要为布局做准备必须先散开
     2222----执行PADS Layout下Tools菜单下的Disperse Components命令，散开所有的元器件
     3333----被散开的元器件之间存在着密密麻麻的电气连接，还不利于元器件的布局
     4444----执行View菜单下的Nets...命令对所有的网络可以进行设置----电气连接也是通过网络名称连接到一起的
           在View Nets窗口下把左边Net List下所有的网络名称添加到右边的View List下，再选中View List下所有的网络名称，
设置他们的View Details就可以控制电气连接的显示与隐藏了
4、根据你的机械尺寸定出PCB板的外形Outline
      1111----先设置PCB图使用的单位----mm/mil
      2222----Setup/set origin设置原点++++modeless window (s 160 136)----定位绝对位置
      3333----Drafting toolbar/Board Outline and Cut Out----选择画Polygon类型，圈出PCB板的外框
5、根据元器件的大小，特性，位置要求放置元器件到PCB板的外形Outline里面去
      1111----首先肯定是放对位置要求很严格的大的元器件了----特别是安装要求/孔位/信号输入/天线布局/电源芯片/排线
      2222----然后就是放置逻辑比较多的元器件----比如PCB板核心的CPU/RAM什么的----对时序/阻抗要求很高的
      3333----然后就是逻辑比较少的元器件了----小的IC/电阻/电容/电感/磁珠
      4444----根据sch文件里面块状结构的电路一块块地布局每一块电路的元器件------每块电路的元器件尽量靠拢
      5555----输入S+你要找的元件位号，如R1，回车，就会高亮发表找到你要布局的元器件----S R1
6、执行Setup菜单下的Layer Definition命令设置PCB板的层数，每层的定义及走线方向/材料/厚度
7、执行Setup菜单下的Design Rules...命令设置PCB走线的限制----注意单位
       1111----各种东西之间允许的间距等等----过孔与过孔之间/过孔与走线之间/走线之间等等
       2222----差分线对(Differential Pairs)的走线要求
8、执行Setup菜单下的Display Colors...命令设置PCB每一层的对象在PCB中显示的颜色
      1111----焊盘/过孔/走线/文字/外框
9、下一步应该是按要求布线了----Add Route/Unroute----布线/不布(恢复成飞线)
      1111----点View菜单Toolbars/Design Toolbar将显示很多可以用于布线的工具
      2222----Select/Move/Radial Move/Rotate/Spin/Swap part/
      3333----Add corner/Split/Add Route/Add Jumper/Add Test Point/Make Like Reuse
      4444----Design Options-----可以设置走线的一些参数与规则
      5555----选中Add Route----记得从其它模式切(Select)到Add Route要先释放(点视图区域释放)，否则Output Windows输出Nothing is selected to start routing. 
      6666----选中要布的飞线的某个端点/焊盘，拖动的线路/轨迹就是你布线的走向
      7777----从电气特性/美观大气的角度考虑你的全局布线
10、为了板子可靠覆铜是少不了的-----让板子充分接地/降低各个功能模块电路的阻抗/提高板子的性能与稳定性
11、标注板子的外形尺寸/增加说明文字
12、设计验证
13、输出PCB文件与报告
//====================================================================================
备注::
 1>上面还有很多步骤都还没真正实践过,还有待进一步的验证与体会

