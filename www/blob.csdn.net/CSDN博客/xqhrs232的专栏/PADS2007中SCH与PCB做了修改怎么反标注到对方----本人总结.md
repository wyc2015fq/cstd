# PADS2007中SCH与PCB做了修改怎么反标注到对方----本人总结 - xqhrs232的专栏 - CSDN博客
2013年11月03日 21:05:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1672
//PADS2007中SCH与PCB做了修改怎么反标注到对方.txt
相关文章
1、[PADS中修改了封装库后怎样才能更新到PCB中](http://blog.csdn.net/xqhrs232/article/details/13507543)----[http://blog.csdn.net/xqhrs232/article/details/13507543](http://blog.csdn.net/xqhrs232/article/details/13507543)

   1、SCH中做了修改反标注到PCB中去
       经常在SCH里面我们需要修改一个元器件使用的PART 
       TYPE/修改一个元器件所使用的封装/属性-----选中一个元器件进入Part Properties对元器件的各方面的属性重新进行分配与定义，Attributes/Visibility/PCB Decals/SigPins  etc
    AAAA、第一种方法----SCH做了的修改能够直接更新到PCB，实时性的
        //1111----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Preferences选中Compare PCB Decal Assignments
        //2222----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Design/Compare PCB进行SCH与PCB之间的比较，找到不同的地方，如果完全相同就不需要执行ECO(Engineering Change Orders)
        //3333----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Design/ECO To PCB 
        //4444----按上面的操作也会产生一个ECO文件----eco2pcb.eco，注意产生的ECO文件产生的位置----E:\PADS Projects
        //5555----上面这样操作的条件是SCH与PCB之间已经建立了连接，两个都已经打开了  
   BBBB、第二种方法----产生ECO文件
      1111----执行菜单命令PADS Logic/Tools/Compare ECO...可以产生ECO文件----这个文件有什么作用，目前还不知道在PADS Layout中怎么使用
      2222----要选择一个old原理图进行比较，才可以产生一个ECO文件，注意产生的ECO文件产生的位置----E:\PADS Projects
      3333----执行Run命令产生一个ECO文件
      4444----现在不知道SCH产生的ECO文件怎么用到PCB，在PADS Layout里面执行File/Import是乎不行

  2、PCB中做了修改反标注到SCH中去
      AAAA、第一种方法----PCB做了的修改能够直接更新到SCH，实时性的
-----------PADS Layout端----------------
        //1111----PADS Layout中点ECO Toolbar菜单进入ECO状态
        //2222----选中需要更改的元器件
        //3333----点ECO Toolbar/Change Component
        //4444----回到绘图区，散布右键弹出菜单选择Library Browse
        //5555----定位到库和要更新的PART TYPE
        //6666----执行Replace命令
        //7777----这个像是只能更新PART TYPE
        //8888----当然还有其它方面的更改
.添加元件（Add component）和添加网络连线(Add connection) · 
.删除元件（Delete component）和删除网络线（Delete connection） ·
.更改元件名（Rename component）和更改网络名（Rename net） · 
.元件编号重新编排（Auto Renumber） 
.管脚和门的交换（Swap pin/Swap Gate） 

-----------PADS Logic端-------------------
        //1111----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Preferences选中Compare PCB Decal Assignments
        //2222----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Design/Compare PCB进行SCH与PCB之间的比较，找到不同的地方，如果完全相同就不需要执行ECO(Engineering Change Orders)
        //3333----PADS Logic中执行菜单命令Tools/PADS Layout.../PADS Layout Link/Design/ECO From PCB 
        //4444----按上面的操作也会产生一个ECO文件----eco2pcb.eco，注意产生的ECO文件产生的位置----E:\PADS Projects
        //5555----上面这样操作的条件是SCH与PCB之间已经建立了连接
   BBBB、第二种方法----产生ECO文件
      1111----执行菜单命令PADS Layout/Tools/Compare ECO...可以产生ECO文件
      2222----ECO Options进行必要的设置----产生的ECO文件存储的位置，产生ECO的条件
      3333----也有两个PCB文件进行比较，才可以产生一个ECO文件，注意产生的ECO文件产生的位置及名称
      4444----执行Run命令产生一个ECO文件
      5555----在PADS logic的File/Import...功能进行反注到SCH 
//================================================================================================================================
备注::
1》每一个SCH与PCB的不同都可以叫做一个ECO(Engineering Change Orders)----工程设计更改，要SCH与PCB对应起来就得把ECO反注到另外的对象里面去
2》SCH与PCB能够进行动态的连接更新，所以不用每次重新load netlist asc文件又来重新布局元器件一遍，做了ECO更改，元器件在PCB的位置也还保持不变，
这样方便设计的往下走而不用每次都再从头开始

