# BCB中选择文件对话框TOpenDialog过滤后缀名使用方法 - DoubleLi - 博客园






BCB中使用TOpenDialog选择对话框时，直接OpenDialog->Execute()弹出的对话框是显示所有文件的，如果我们希望过滤指定的文件后缀名就需要在Execute()前做一些初始化工作，此处主要使用Filter属性。代码如下：[喝小酒的网摘]http://blog.hehehehehe.cn/a/8453.htm

只显示txt文本文件。
OpenDialog1->Filter = "Text files (*.txt)|*.TXT";

多个下拉框，可只显示txt文件，也可只显示pas后缀结尾文件。

OpenDialog1->Filter = "Text files (*.txt)|*.TXT|Pascal files (*.pas)|*.PAS";

下面是一种格式里面显示多种后缀名的文件。

OpenDialog1->Filter = "Pascal files|*.PAS;*.DPK;*.DPR";

显示全部文件(All file)时，可用*.*，如下所示：

OpenDialog1->Filter = "All files(*.*)|*.*";

然后直接OpenDialog1->Execute()即可。

还有另外一种用法如下：

SavePictureDialog1->DefaultExt = GraphicExtension(__classid(Graphics::TBitmap));
SavePictureDialog1->Filter = GraphicFilter(__classid(Graphics::TBitmap));
if (SavePictureDialog1->Execute()) 
  // save the graphic [喝小酒的网摘]http://blog.hehehehehe.cn/a/8453.htm









