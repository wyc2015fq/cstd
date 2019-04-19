# labview入门 - xqhrs232的专栏 - CSDN博客
2018年02月13日 14:56:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1349
原文地址::[http://www.avrvi.com/labview_start/Introduction_to_Labview.html](http://www.avrvi.com/labview_start/Introduction_to_Labview.html)
相关文章
1、labview教程—labview新手入门----[http://www.avrvi.com/index_labview_start.html](http://www.avrvi.com/index_labview_start.html)
Labview环境
运行Labview，如图：
![labview 界面](http://www.avrvi.com/labview_start/images/3_labview/image001.jpg)
![labview界面](http://www.avrvi.com/labview_start/images/3_labview/image002.jpg)
点击新建VI，即可打开一个新的Labview程序
前面板
![前面板](http://www.avrvi.com/labview_start/images/3_labview/image003.jpg)
![前面板](http://www.avrvi.com/labview_start/images/3_labview/image004.jpg)
     ①：工具条  ②：标签   ③：数值控件    ④：标尺
     ⑤：图标    ⑥：旋钮   ⑦：图例        ⑧：XY图 
前面板工具条：![](http://www.avrvi.com/labview_start/images/3_labview/image005.jpg)
![](http://www.avrvi.com/labview_start/images/3_labview/image006.jpg)：运行按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image007.jpg)：连续运行按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image008.jpg)：中断运行按钮。当编码出错使VI不能编译或运行时，中断运行按钮将替换运行按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image009.jpg)：连续运行按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image010.jpg)：暂停/继续按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image011.jpg)：异常终止执行按钮
![](http://www.avrvi.com/labview_start/images/3_labview/image012.jpg)：对齐对象按钮。用于将变量对象设置成较好的对齐方式
![](http://www.avrvi.com/labview_start/images/3_labview/image013.jpg)：分布对象按钮。用于对两个及其以上的对象设置最佳分布方式。
![](http://www.avrvi.com/labview_start/images/3_labview/image014.jpg)：调整对象大小按钮。用于将若干个前面板对象调整到同一大小。
程序框图
![labview程序框图](http://www.avrvi.com/labview_start/images/3_labview/image015.jpg)
①：数值输入控件    ②：旋钮控件     ③：数值常量     ④：乘
⑤：For循环结构     ⑥：数据连线     ⑦：捆绑函数     ⑧：XY图控件
⑨：子VI程序
框图工具条：![](http://www.avrvi.com/labview_start/images/3_labview/image016.jpg)
![](http://www.avrvi.com/labview_start/images/3_labview/image017.jpg)：加亮执行按钮。当程序执行时，在框图代码上能够看都数据流，这对于调试和校验程序的正确运行是非常有用的。在加亮执行模式下，按钮转变成一个点亮的灯泡：![](http://www.avrvi.com/labview_start/images/3_labview/image018.jpg)
![](http://www.avrvi.com/labview_start/images/3_labview/image019.jpg)：保存连线值按钮。
![](http://www.avrvi.com/labview_start/images/3_labview/image020.jpg)：单步进入按钮。允许进入节点，一旦进入节点，就可在节点内部单步执行。
![](http://www.avrvi.com/labview_start/images/3_labview/image021.jpg)：单步跳过按钮。单步跳过节点，但不执行时不进入节点内部但有效地执行节点。
![](http://www.avrvi.com/labview_start/images/3_labview/image022.jpg)：单步跳出按钮。允许跳出节点，通过跳出节点可完成该节点的单步执行并跳转到下一个节点。
![](http://www.avrvi.com/labview_start/images/3_labview/image023.jpg)：文本设置按钮。
![](http://www.avrvi.com/labview_start/images/3_labview/image024.jpg)：层叠顺序。
下拉菜单
由于Labview8.2采用了中文的界面，每个下拉菜单的功能根据字面意思就很容易理解了，在这里我不再赘述。
选项板
- 工具选项板
![](http://www.avrvi.com/labview_start/images/3_labview/image025.jpg)?左图所示便是工具选项板，各按钮的功能如下：
![](http://www.avrvi.com/labview_start/images/3_labview/image026.jpg)：自动选择工具
![](http://www.avrvi.com/labview_start/images/3_labview/image027.jpg)：操作工具
![](http://www.avrvi.com/labview_start/images/3_labview/image028.jpg)：定位/调整大小/选择工具
![](http://www.avrvi.com/labview_start/images/3_labview/image029.jpg)：编辑文本工具
![](http://www.avrvi.com/labview_start/images/3_labview/image030.jpg)：连线工具
![](http://www.avrvi.com/labview_start/images/3_labview/image031.jpg)：对象快捷菜单工具
![](http://www.avrvi.com/labview_start/images/3_labview/image032.jpg)：滚动窗口
![](http://www.avrvi.com/labview_start/images/3_labview/image033.jpg)：设置/清除断点工具
![](http://www.avrvi.com/labview_start/images/3_labview/image034.jpg)：探针工具
![](http://www.avrvi.com/labview_start/images/3_labview/image035.jpg)：颜色复制工具
![](http://www.avrvi.com/labview_start/images/3_labview/image036.jpg)：着色工具
- 控件选项板：
![](http://www.avrvi.com/labview_start/images/3_labview/image037.jpg)
和下拉菜单一样
Labview8.2采用了中文的界面
各个控件的功能一目了然
某些常用的控件的功能我们会在下面的章节中做详细的介绍
- 函数选项板
![](http://www.avrvi.com/labview_start/images/3_labview/image038.jpg)
和下拉菜单一样
Labview8.2采用了中文的界面
各个控件的功能一目了然
某些常用的控件的功能我们会在下面的章节中做详细的介绍
- 下面将手把手地教大家写一个简单的VI程序，让大家对一个基本的认识再进入下一章节的讲解。
创建第一个VI： 
本节将创建的第一个VI来完成下列功能：
- 将两个输入数字相加并显示结果
- 将同样的两个输入数字相乘并显示结果
- 比较两个输入数字，如果数字相等，则LED指示灯变亮
步骤：
- 从“文件”菜单中选择“新建”VI打开新的前面板
- 创建数字控件和指示器。使用两个前面板控件输入数字，使用两个指示器输入数字加和乘的结果。- 从控件选项板的数值子选项板中选择数值输入控件
- 单击数值输入控件，然后拖拽控件到所希望的位置，然后单击鼠标将控件放下，如图：
![](http://www.avrvi.com/labview_start/images/3_labview/image039.jpg)
- 在标签框（在控件上方）内键如字母，按前面板工具条上的Enter按钮。如果在继续编程操作（诸如将其他控件放置在前面板上）之前不键入标签内容，标签框将保留默认标签内容“数值”。如果控件或指示器没有标签，可用右键单击控件，在控件上弹出快捷菜单，从显示项菜单中选择标签，标签框就会出现，然后可使用标签工具编辑文本。
- >重复以上过程创建第二个数字控件和第二个数字指示器。可选择按任意方式排列控件和指示器，不过排列得整齐而有规律会更好些
![](http://www.avrvi.com/labview_start/images/3_labview/image040.jpg)
- 创建布尔LED。如果两个输入数字是相同的，那么指示灯打开；如果不是，则保持关闭状态。- 从控件选项板的布尔子选项板中选择圆形知识灯。将该指示器放置到前面版上并拖拽到所希望的位置，然后单击鼠标按钮。
- 在标签框内键入“Round LED”，键入完毕，单击标签框外边的任何地方，或单击Enter按钮。如图：
![](http://www.avrvi.com/labview_start/images/3_labview/image041.jpg)
”
- 从窗口下拉菜单中选择显示程序框图，切换到程序框图。
- 先在在框图上放置“加”和“乘”函数。可从框图选项板，编程选项板的数值子选项板中选择“加”函数。按照同样的步骤，将“乘”函数放置到框图上并显示标签。
- 从框图选项板，编程选项板的子选项板中选择“等于？”函数，并放到框图上，如果他们相等，返回“真”，如果不相等，则返回“假
![](http://www.avrvi.com/labview_start/images/3_labview/image042.jpg)
![程序框图](http://www.avrvi.com/labview_start/images/3_labview/image043.jpg)
- 连线，在工具选项板中选择连线工具，当把鼠标移到各个控件的上方时，能够参与连线的节点便会自动闪烁，提示用户，此处可以连线。连线完成后如图：
![](http://www.avrvi.com/labview_start/images/3_labview/image044.jpg)
- 连线完成后，单击运行按钮，便可在前面板上看到运行的结果。

