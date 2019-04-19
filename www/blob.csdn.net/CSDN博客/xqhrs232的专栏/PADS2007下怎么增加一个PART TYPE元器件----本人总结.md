# PADS2007下怎么增加一个PART TYPE元器件----本人总结 - xqhrs232的专栏 - CSDN博客
2013年04月06日 00:35:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2053

相关网帖
1、[http://bbs.21ic.com/icview-57996-1-1.html](http://bbs.21ic.com/icview-57996-1-1.html)

//1111----PADS要加一个元器件，这个元器件必须有CAE DECAL++++PCB DECAL----就算是PCB DECAL指定一个错误的
//2222----怎么画一个CAE DECAL-----CAE DECAL可能有不同的PIN  DECAL，所以有必要可以先画个PIN DECAL
  //3333----增加一个PIN  DECAL
      A、TOOLS/Part Editor----元器件编辑器
      B、File/new----Select type of....Part Type/Connector/Pin Decal/CAE Decal选Pin Decal出现如下字符于编辑区
        1>#E
       2>#0:TYP=U SWP=0------在原理图里面不会出现 
      3>PNAME 
      4>NETNAME
     C、用Decal Editor Toolbar里面的2D工具画线/画圆
     D、然后就是保存了，然后就是在画CAE DECAL的时候引用刚画的PIN DECAL了
  //4444----增加一个CAE DECAL   
      A、TOOLS/Part Editor----元器件编辑器
      B、File/new----Select type of....Part Type/Connector/Pin Decal/CAE Decal选CAE Decal出现如下字符于编辑区
         1>#REF 
        2>PART-TYPE
        3>*Free Label1 1 
        4>*Free Label1 2
     C、用Decal Editor Toolbar里面的CAE Decal Wizard通过向导很容易创建一个有规则的CAE DECAL，指定大小跟上下左右的管脚数，然后就在编辑区会出现如下字符
（通过向导制作出来的CAE DECAL不需要添加端点----因为已经有端点了，只是有可能电气特性不一样，修改使用的PIN DECAL   ）
      （不通通过向导整出来的CAE DECAL不好写文字，可能文字会相互盖住，最好还是自己去画那个外形，加几个文字看看会不会存在盖住的情况，增加端点也手动去加，也可以通过  step and repeat工具一次增加一排的端点！！！）
        1>#E 
       2>NETNAME 3>#3:TYP=U SWP=0------在原理图里面不会出现
    注意其中的PIN的PINAME和PINNUMBER都没有指定----这个在编辑PART TYPE时才指定
     D、修改PIN的电气逻辑----如果是低电平的就用低电平的PIN DECAL
      E、调整PIN的位置，同类的与其它的区别开来-----两边摆不下要不要放到第三边上;通过删除一些PIN可以空出位置，然后再增加一些自己需要的PIN,通过MOVE调整PIN的位置
      F、然后就是保存了，然后就是在创建PART TYPE的时候引用刚画的CAE DECAL了 
     特别的CAE DECAL就要手动一点点地画了
 //5555----增加一个PART TYPE
      A、TOOLS/Part Editor----元器件编辑器
      B、File/new----Select type of....Part Type/Connector/Pin Decal/CAE Decal选PART TYPE编辑区无字符出现
      C、Edit electric----编辑PART TYPE属性
  0>在General表单项要先指定Logic Family，也就是指定你新建的PART TYPE属于那个类型
         比如：：电容属于CAP，电阻属于RES，电感属于IND，集成电路属于UND
      1>Gates-----指定CAE DECAL---比如刚才创建的
     2>PCB Decal----指定PCB封装----可以随便指定一个，但管脚数必须匹配----
      3>然后切到PINS属性页-----分配VCC+GND的管脚号(不一定非这样，这样的话VCC+GND 在原理图里面不会显示出来 )
      4>attributes----指定PART TYPE描述性的东西
 (对于一些需要指定数值大小的元器件，需要在"Browse Lib. Attr"处增加一个VALUE属性  )
      5>点OK
      D、Edit Graphics/Select Gate Decal/Gate A-----会弹出警告,直接点OK,然后编辑区出现如下字符
1>REF 
2>NEW_PART
 3>*Free Label1 1
 4>*Free Label1 2
 5>PIN脚----NETNAME #5:TYP=U SWP=0

     E、然后就是Decal Editing Toolbar/Set Pin Number++++Set Pin name
       Set Pin Number------1/2/3这样的序号
       Set Pin name------文字功能-----CE/D0/A0这样的
< 也可以在编辑GATE A的电气特性的时候，直接从EXCEL表格/CSV文件里面自己COPY Pin name/Pin number,这样可以快很多！ >

    F、保存执行File/Return to 
    Part-----这个时候要保存对GATE的修改的对话框弹出-----必须选择YES，然后编辑区出现如下字符
        1>U999
       2>NEW_DECAL
      3>*Free Label1 1 
      4>*Free Label1 2 
       5>PIN脚----D0|1 #5:TYP=U SWP=0-----注意这个时候NETNAME字符已经消失不见了
      G、然后就是保存，选择保存的名字----最后就是在原理图里面ADD PART找到这个PART TYPE并加进原理图了-----加进原理图要求指定开头字母---比如U
      H、上面就是PADS2007下完成一个PART TYPE元器件的大致整体过程，当然具体的细节的慢慢一步步对照去体会
//=====================================================================================================================
备注::
1》PADS Layout地线飞线集中在了一点的消除方法
     //1111----按ctrl+m就可以了以最短的显示

