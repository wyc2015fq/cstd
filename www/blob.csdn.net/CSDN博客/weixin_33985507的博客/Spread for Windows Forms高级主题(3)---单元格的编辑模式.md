# Spread for Windows Forms高级主题(3)---单元格的编辑模式 - weixin_33985507的博客 - CSDN博客
2011年08月17日 16:37:25[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
# 理解单元格的编辑模式
通常情况下，当终端用户双击单元格时，编辑控件将允许用户在该单元格中输入内容。在一个单元格中编辑的能力被称为编辑模式。一些属性和方法可以用来自定义编辑模式的使用。
当一个单元格处于编辑模式时，活动单元格将显示一个I型光标，如下图所示。当该单元格不处于编辑模式时，活动单元格将显示一个焦点长方形，如下图所示。
|![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570239EWFe.png)|![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_13135702393t6O.png)|
|----|----|
|处于编辑模式下的单元格|被选中但是并不处于编辑模式下的单元格|
一个单元格将进入编辑模式（开启编辑模式），当：
- 用户在单元格中输入内容
- 用户双击单元格
- EditMode属性设置为true
一个单元格离开编辑模式（关闭编辑模式），当
- 用户按下回车键
- 用户激活了另外一个单元格
- 应用程序丢失焦点
- EditMode属性设置为false
当一个单元格进入编辑模式，默认情况下，光标位于单元格中文本的末端。你可以设置EditModeReplace属性，将光标改变为选择单元格中存在的文本。
如果你愿意，你可以使用EditModePermanent属性指定一个单元格，当该单元格变为活动单元格时一直处于编辑模式。
当一个单元格进入编辑模式时，触发EditModeOn事件；当一个单元格离开编辑模式时，触发EditModeOff事件。当编辑控件得到焦点时，你可以使用SuperEditBase.EditModeCursorPosition属性设置光标在该控件中的位置。
你可以使用StartCellEditing 和 StopCellEditing方法启动和结束编辑模式。
# 阻止单元格获取焦点
你可以防止单元格获得焦点，从而禁止终端用户点击该单元格。你可以通过设置单元格的CanFocus属性控制焦点的设置，这些设置通过键盘输入和鼠标操作进行定义。
# 为单元格自定义焦点指示器
聚焦框向终端用户显示了被选中的单元格和活动的单元格。默认情况下，当单元格被选中时，该单元格有一个纯色聚焦框，如下图所示。如果一整列（或者一整行）被选中，该列的第一个单元格（或者该行的）就会得到焦点，同时该列（或者该行）会被高亮，如图所示。活动单元格的行头或者列头也会有不同的背景色。
|![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570240XqNW.png)|![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570240Gihr.png)|
|----|----|
|被选中的每一个单元格的聚焦指示器|被选中列的聚焦指示器|
你可以使用Spread的FocusRenderer属性（这个属性使用IFocusIndicatorRenderer接口）自定义活动单元格的聚焦指示器。对于动画指示符来说，你需要IAnimatedFocusRenderer接口。你也可以改变活动表头的选定颜色。
下表总结了聚焦指示器的几种类型以及与之相对应的类。
|**类型**|**类**|
|----|----|
|默认|DefaultFocusIndicatorRenderer|
|动画|AnimatedDefaultFocusIndicatorRenderer|
|自定义行|CustomFocusIndicatorRenderer|
|编辑|EditingFocusIndicatorRenderer|
|扩展|EnhancedFocusIndicatorRenderer|
|图片|ImageFocusIndicatorRenderer|
|跑马灯线|MarqueeFocusIndicatorRenderer|
|实线|SolidFocusIndicatorRenderer|
DefaultFocusIndicatorRenderer是其它类的基类。ImageFocusIndicatorRenderer允许你使用一个图片作为聚焦指示器。SolidFocusIndicatorRenderer允许你自定义 一个实心的边框围绕着选中单元格作为聚焦指示器。
在Spread设计器中，你可以使用聚焦指示编辑器自定义聚焦指示器。
# 获取点击过的单元格的信息
你可以获得行和列的单元格索引信息，这个单元格通过CellClickEventArgs类中的CellClick事件的参数e点击访问。你也可以从这个参数中得到X和Y坐标。你可以实现一个MouseDown事件，并从X和Y坐标中获取点击过的单元格的行和列索引信息。根据FpSpread类中的GetCellFromPixel方法，你可以获取CellRange类中的目标单元格信息。你也可以从各自的成员中获取行和列的信息。
你可以获取单元格中诸如位置，规格等信息，这些信息都是被行索引列索引所指定。当FpSpread类中的GetCellRectangle方法被调用，它会指定目标行或者列索引。单元格坐标信息是以.NET框架的矩形格式返回的。
对表头来说，通过访问CellClickEventArgs类中的CellClick事件参数e，你可以得到已经被点击的表头的行和列索引信息。你可以检测这些表头是否被点击。你也可以从这个参数中获取x坐标和y坐标。你可以实现一个MouseDown事件，从 X和Y坐标中获取已经被点击的表头单元格的行和列的索引信息。
通过使用FpSpread类中的GetColumnHeaderCellFromPixel方法，你可以为列单元格获取CellRange类格式中的目标单元格信息。你可以从各自的成员中获取列表头里的行和列的信息。在有行头单元格的情况下，调用GetRowHeaderCellFromPixel方法。
# 向单元格中添加备注
你可以添加一个备注到一个单元格或单元格区域。备注可以包括诸如评论、问题，或者描述单元格值的由来的文本。每一个有备注显示的单元格在单元格的右上角显示了一个单元格备注指示器（默认情况下是一个小的红色的框）。当指针在有一条备注的单元格的指示符上的时候，备注文本显示在单元格旁边的一个文本框中。另外你可以设置你的单元格备注总是显示,而不只是当鼠标移向指示符时候显示。对于弹出的单元格备注而言，它们的显示方式类似于文本提示。当指针在单元格的备注指示器上时，就会显示单元格备注文本。这一点在下面图中有所展现。
![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570241BCLs.png)
在单元格的右上角的红色方块表示该单元格的一个可用备注，如下图所示。你可以使用CellNoteIndicatorVisible属性隐藏单元格备注指示器当指针在单元格备注上时，可以使用单元格的NoteIndicatorPosition属性设置备注的位置。
![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570241MaS7.png)
**自定义单元格备注行为**
你可以允许备注保持显示的状态，就好像它们是即时贴一样。在这种情况下，他们出现在一个单元格旁边的矩形框中，以一条可伸缩的线与要说明的单元格相连，以便备注可以被用户方便地移动。一个被选中的即时贴的示例如下图所示。这个单元格的NoteStyle属性必须使用 StickyNote枚举类型以便允许此操作。即时贴在这种情况下是一个可移动的图形。
![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_1313570243wT3J.png)
为了移动备注，当指针在备注上时，单击鼠标左键 选择它，将它拖曳到目标位置，并且释放鼠标左键以将其置于正确位置。单元格备注指示器上的即时贴延伸出的线可以适应备注的任何位置。
你可以允许用户编辑单元格备注，如果备注始终显示。为了让用户对其进行编辑，设置表单中的AllowNoteEdit属性，这样的设置使得该表中的所有即时贴备注都是可以由用户编辑的。
对于终端用户来说，单元格备注可以用来保存一些额外信息。你还可以允许用户在单元格中附加他们自己的信息，该信息可以是任何对终端用户有用的信息。例如终端用户可能会利用单元格备注来说明单元格值的来源（单元备注=“这个值来源于消费者报告七月号杂志的一篇文章”）。
你可以进一步自定义备注的使用：
- 根据内容自动调整单元格备注的大小
- 自定义单元格备注的位置
- 生成即时贴标签这样它们可以粘滞在它生成的地方
- 自定义备注指示器
- 打印单元格备注
还有可用于自定义单元格备注外观的其他类 。给备注使用StickyNoteStyleInfo 类。
**了解局限性**
有一些单元格备注使用和显示的限制：
- 当单元格对象的 NoteStyle 属性 被设置为隐藏时，此备注不显示。
- 当该单元格类型的IsReservedLocation方法被设置为true时，此备注并不会显示在某些类型的单元格中。这种情况可能会发生在一个复选框单元格中，或在不可编辑的组合框单元格中，或者当光标移动到超链接单元格中的时候。
- 当单元格处于编辑模式时，单元格备注指示器并不会出现 。
- 一个锚单元格的单元格备注显示了一个单元格合并区域，但是合并区域中的任何其它单元格的单元格备注都不会显示。
- 选择红色作为单元格的背景，以显示单元格中包含一个红色的单元格备注作为警告。单元格备注指示器可能在红色的背景中是不可见的。
**自定义****单元格备注指示器**
你可以改变单元格备注指示器的大小和颜色。单元格备注指示器的默认大小是一个3x3的正方形，但是 你可以修改NoteIndicator的宽度或高度为任何 正整数。单元格备注指示器的默认颜色 是红色的，但你可以给它分配任何颜色值。下图中显示了 使用默认值的指示器和使用自定义值的自定义指示器。使用NoteIndicatorColor 和NoteIndicatorSize属性，你可以设置自定义值。
![image](http://powertoolsteam.blog.51cto.com/attachment/201108/17/2369428_13135702442g7X.png)
这段示例代码为单元格区域设置了可编辑的单元格备注，并设置单元格备注指示器的颜色为绿色（而不是默认的红色）。
- fpSpread1.Sheets[0].AllowNoteEdit = true;  
- fpSpread1.Sheets[0].Cells[1, 1, 3, 3].Note = "test";  
- fpSpread1.Sheets[0].Cells[1, 1, 3, 3].NoteIndicatorColor = Color.Green; fpSpread1.Sheets[0].Cells[1, 1, 3, 3].NoteStyle = FarPoint.Win.Spread.NoteStyle.StickyNote; 
[Spread for Windows Forms 5.0 中文版下载地址](http://gcdn.grapecity.com/showtopic-1920.html)
**附：Spread for Windows Forms高级主题系列文章**
[Spread for Windows Forms高级主题(1)---底层模型](http://www.cnblogs.com/powertoolsteam/archive/2011/08/16/2140457.html)
[Spread for Windows Forms高级主题(2)---理解单元格类型](http://www.cnblogs.com/powertoolsteam/archive/2011/08/17/2142085.html)
