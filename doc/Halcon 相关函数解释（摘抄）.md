# Halcon 相关函数解释（摘抄）

2018年10月11日 19:03:15 [weixin_42895895](https://me.csdn.net/weixin_42895895) 阅读数：202



@[Halcon 相关函数解释（摘抄）](Halcon 相关函数解释（摘抄)

一. dev_clear_obj 删除一图像对象
例如:
dev_close_window()
read_image(Image,‘fabrik’)
dev_open_window(0,0, 512, 512, ‘black’, WindowHandle)
dev_display(Image)
dev_clear_obj (Image)
二. dev_error_var( : : ErrorVar, Mode : )
定义一个错误变量，即一个包含最后一个函数调用的结果状态的变量。ErrorVar将是2（h_msg_true）如果没有错误发生。参数 mode指定是否使用错误变量（1）或不（0）。如果一个错误变量是激活状态，它将在每次执行操作函数完成时被更新。因此，值只有在函数下一次调用之前才有效。该变量可以赋值给另一个变量用于保存;
例程:
dev_close_window ()
dev_open_window (0, 0, 512, 512,‘black’, WindowHandle)
dev_error_var (Error, 1)
dev_set_check (’~give_error’)
FileName := ‘wrong_name’
read_image (Image, FileName)
ReadError := Error
if (ReadError != H_MSG_TRUE)
write_string (WindowHandle, 'wrong file name: '+FileName)
endif

三．dev_get_exception_data( : : Exception, Name : Value)
函数dev_get_exception_data能够访问一个被TRYcatch捕获的异常数组的元素。除了错误代码数组的第一个元素外，所有其他的必须完全通过函数dev_get_exception_data访问。这是因为在将来的版本中，所提供的数据的顺序和程度可能会发生变化，并且可能因不同的代码导出而有所不同。
如果函数错误发生在HDevelop或hdevengine，被捕获的异常数组包含的数据项如下。这个数组必须传递给参数异常。请求的数据槽的名称必须传递给参数名。
请求的数据在参数值中返回。通过一个数组的槽的名字来命名可能要求一些异常数据，通过调用一次dev_get_exception_data。在这种情况下，相应的数据数组以值的方式返回。对于不可用的请求项，返回空字符串（“”）
例程:
for Index := 0 to |ImageNames| - 1 by1
try
read_image (Image, ImageNames[Index])
catch (Exception)
dev_get_exception_data (Exception, ‘error_code’,ErrorCode)
if (ErrorCode == 5200)
throw ([Exception,ImageNames[Index]])
endif
throw (Exception)

endtry
endfor
四，dev_get_system( : : SystemQuerys : SystemInformations)
返回关于HDevelop系统参数信息。SystemQuerys值为engine_environment’，haocon内部调用，返回HdevEngine
五。dev_set_check( : : Mode : )
在HALCON中函数返回值不为H_MSG_TRUE (2)时，dev_set_check可以设置HALCON处理的方式；
如果mode值为give_error”—这是系统默认的—
一个函数参数传递不对，调用时会抛出一个异常，可以在HDevelop程序由catch语句捕获。
然而，如果没有使用try catch块，在HDevelop执行程序，程序执行停在错误的函数位及错误消息框会打开显示错误文本。
此外，相应的算子函数被输入到“算子输入窗口”中，以便用户可以轻松地编辑并可能纠正错误操作符调用的参数。
如果程序是在hdevengine内调，和exception不是在HDevelop程序内捕获，一个hdevengineexception对象抛出和函数停止。
如果模式设置为“~ give_error，错误将被忽略，程序继续下一个操作。
dev_set_check（’ ~ give_error”）是用于与dev_error_var的连接，来检查函数返回结果的状态；
测试例程:
dev_close_window ()
dev_open_window (0, 0,512, 512, ‘black’, WindowHandle)
dev_error_var (Error, 1)
dev_set_check(’~give_error’)
FileName := ‘wrong_name’
*参数出错，程序不信停止与报错，继续执行
read_image (Image,FileName)
ReadError := Error
if (ReadError !=H_MSG_TRUE)
write_string (WindowHandle, ‘wrong file name:’+FileName)
endif
*现在程序就会停止，发出错误信息
dev_set_check(‘give_error’)
read_image(Image, FileName)
六.dev_update_pc( : : DisplayMode : )
在程序执行时，开关PC刷新功能；
dev_update_pc指定程序执行期间PC的行为。如果DisplayMode设置为（开”）这是默认的设置，在选定的程序里，函数执行后PC问题会显示。此外，程序文本滚动，如果有必要，使目前的操作函数也可见
如果模式是关PC程序执行过程中的程序文本不会自动滚动，不可见。当程序停止时，PC变得可见,列表滚动到当前PC机的位置。
测量一系列函数的执行时间，所有更新选项应该关闭，以减少HDevelop的GUI更新运行的影响。dev_update_pc，dev_update_time，dev_update_var，和dev_update_window或dev_update_on和dev_update_off。就可以用上；
这个选项也可以通过首选项对话框来控制：编辑>参数设置>运行时设置>更新程序计数器；
七. dev_update_time( : : DisplayMode : )
开关函数执行时间的显示，会在HALCON左下角显示；
测量一系列函数的执行时间，所有更新选项应该关闭，以减少HDevelop的GUI更新运行的影响。dev_update_pc，dev_update_time，dev_update_var，和dev_update_window或dev_update_on和dev_update_off。就可以用上；
这个选项也可以通过首选项对话框来控制：编辑>设置>运行时设置>显示处理时间
八dev_clear_window( : : : )
清空当前使用的图形窗口内容
dev_clear_window清除活动图形窗口的显示内容，包括图像对象。
已通过窗口上下文菜单，可视化参数对话框，或适当的操作（例如，与dev_set_color，dev_set_draw，等）设置的输出参数，保持不变。
运算符相当于按下当前操作的图形窗口的清除按钮。
一个图形窗口可以通过调用dev_set_window或按下在选定的图形窗口的工具条上的启动按钮激活。
敬告
基于HDevelop代码导出功能，这个算子生成的代码可能比相关halcon算子有不同行为。对于HDevelop图形函数导出到不同的编程语言的代码的详细说明，参考“HDevelop用户指南”章节的代码出口->总的方面代码生成->图形窗口
九. dev_close_inspect_ctrl( : : Variable : )
dev_close_inspect_ctrl与算子dev_inspect_ctrl对应。它关闭变量检查窗口或与变量中指定的变量相对应的窗口。参数变量可以包含表达式（例如，数组）。首先，函数dev_close_inspect_ctrl尝试找到与列出的全部变量完全匹配的检查窗口。如果发现，它将被关闭。如果找不到精确匹配，则运算符试图从第一个匹配检查窗口中的每个列中删除每个列出的变量。通过删除检查窗口的最后一列，窗口就关闭了。
另一种方法是，通过在标题栏中按下关闭按钮，可以关闭检查窗口。
例程:
Var := 1
dev_inspect_ctrl(Var)
Var :=[1,2,3,9,5,6,7,8]
Var[3] :=4
stop ()
dev_close_inspect_ctrl (Var)
十. dev_close_tool( : : ToolId : )
关闭指定工具窗口
dev_close_tool关闭指定toolid工具窗口。而不是使用toolid，TOOLID为函数dev_open_tool返回值。
您可以使用已被使用在dev_open_tool函数内的名称。在这种情况下，任意与参数toolid匹配的工具窗口都会被使用。
十一. dev_close_window( : : : )
关闭当前使用的图形窗口
dev_close_window关闭事先由dev_open_window，合适的菜单项，或由HDevelop程序开始时，或者在加载新程序（默认窗口）所打开的窗口；
效果相当于按下活动窗口的标题栏中的关闭按钮，或从可视化菜单中选择适当的菜单项。
一个图形窗口可以通过调用dev_set_window或按下启动按钮在选定的图形窗口的工具条激活。
敬告
HDevelop代码导出后，这函数与在HALCON中的行为可能不同。
对于halcon图形函数代码导出到不同语言细节描述，参考“HDevelop用户指南”的章节，代码导出->总的方面代码生成->图形窗口。
十二. dev_display(Object : : : )
在当前操作窗口上显示图像对象
十三. dev_get_preferences( : : PreferenceNames : PreferenceValues)
在一个程序中查询Hdevelop用户喜好参数设置
dev_get_preferences允许查询在HDevelop程序中选定的参数。到目前为止，支持以下参数：
“graphics_window_context_menu”：
返回右键单击图形窗口是否打开上下文菜单。默认情况下启用上下文菜单。
“graphics_window_mouse_wheel”：
返回鼠标滚轮是否可以用来缩放图形窗口的内容。默认情况下启用鼠标轮。
“graphics_window_tool_tip”：
返回是否在图形窗口上按下Ctrl键显示当前光标位置和鼠标光标下的灰度值的工具提示。默认情况下启用工具提示。
“suppress_handled_exceptions_dlg”：
返回错误对话框是否被禁止，默认是在程序执行期间抛出的异常，并且在一个catch catch块内使用，因此可以由异常处理程序处理。
这个选项存储在hdevelop.ini文件，可以在菜单中进行设置：编辑->参数选择->一般属性->高级用户->高级用户选项
敬告
代码导出不支持此运算符。
十四. dev_get_window( : : : WindowHandle)
返回当前使用的图形窗口句柄，无图形窗口，返回-1;
当导出时，函数行为与在halcon中有所不同
代码的详细说明，可以看“HDevelop用户指南”章节的代码导出->总的方面代码生成->图形窗口。
十五. dev_inspect_ctrl( : : Variable : )
打开一个窗口查看一个或多个变量 ；
dev_inspect_ctrl打开变量检查对话框显示一个或多个控制变量的内容。
函数效果与从变量窗口中控制变量的上下文菜单中选择检查相同。
标准变量检查窗口显示表中的一些可选统计数据。对于某些语义类型，例如矩阵，存在一种特殊的表示，默认使用。更多信息，见本章“在HDevelop用户指南变量检验。
可以在变量中传递数组变量以打开多个变量的检查窗口。还可以通过将字符串文字作为数组的第一个元素来覆盖默认的检查模式：
数组：
即使变量的语义类型有特殊表示，也可以在默认检查窗口中显示值。
“细节”：
根据它们的数组索引绘制数值变量的值。
“plot_xy”：
图为X-Y对数值变量值。变量的数量必须是偶数，每对变量具有相同的长度。
通常，每当变量改变时，对话框的内容将被更新。然而，更新行为可以由函数dev_update_var和更新变量的偏好的影响。
对话框可以通过按下关闭按钮或通过调用dev_close_inspect_ctrl封闭。
十六. dev_map_par ()
打开可视化参数的设置窗口
这也可以通过菜单 可视化>设置参数，或相应的工具栏按钮。该对话框用于配置用于显示图标对象的可视化参数
十七. dev_map_prog( : : : )
让Hdevelop的主窗口可见
dev_map_prog是用来重新显示（或映射）Hdevelop的主窗口，事先由函数dev_unmap_prog隐藏
十八。dev_map_var( : : : )
打开HDevelop变量窗口。
是用来重新显示变量窗口，事先由dev_unmap_var隐藏
十九. dev_open_dialog( : : DialogName : )
打开一个Hdevelop的模态对话框

DialogName

Dialog
‘about_dialog’
关于对话框
‘duplicate_procedure_dialog’
函数复制对话框
‘export_dialog’
导出对话框
‘open_graphics_windows_dialog’
图形窗口打开对话框
‘insert_visualization_code_dialog’
插入可视代码对话框
‘read_image_dialog’
图像读取对话框
‘print_dialog’
Print dialog
‘properties_dialog’
Properties dialog
‘save_window_dialog’
Save Graphic Window dialog
二十. dev_open_file_dialog( : : Filter, Mode, Path : Selection)
dev_open_file_dialog打开一个文件选择对话框（Mode=‘read’），读取一个或多个文件（Mode=“read_multi '），写（Mode=“write”）文件或选择（Mode=dir）目录。
如果模式设置为“default”和过滤器设置为一个halcon算子,类型设置为算子可以使用的值。
该过滤器可用于选择应用于读写文件类型。过滤器可以设置为halcon算子，例如read_image，应读取或写入所选文件。在这种情况下，过滤器设置适当的HALCON算子使用的文件。选择式JPEG和TIFF过滤器可以设置为“图像（*. jpg，tif，TIFF）或JPEG文件（*. JPG）；；TIFF文件（*. tif，TIFF）”。在第一种情况下，通过选择条目“图像”，所有支持的图像文件都显示在一起。在第二种情况下，可以在JPEG文件或TIFF文件之间进行选择。过滤器不是区分大小写的，因此“JPEG文件（* JPG）”和“JPEG文件”（* JPG）结果相同。会在尾部添加all files（*）。要组合一个以上的过滤器，你必须用“;;”。
例程:
dev_open_file_dialog (‘read_image’, ‘default’,‘c:/’,Selection)read_image(ToolId, Selection)
二十一dev_open_tool( : : ToolName, Row, Column, Width, Height, GenParamNames, GenParamValues : ToolId)
打开HDevelop的工具窗口，一个非模态对话框，或助手；
参数toolname包含应打开工具的名称。这个函数返回新创建的toolid，可以通过函数dev_set_tool_geometry，dev_show_tool访问，打开的工具窗口自动激活。
对于toolname。'halcon_news '返回一个空的ToolId，因为这只是要查看信息页。
通过参数genparamnames和genparamvalues可以指定哪页显示等等。具体查阅编程手册。对genparamnames和genparamvalues的使用是可选的。如果将其设置为[ ]，则工具将以默认设置打开。
二十二。dev_open_window( : : Row, Column, Width, Height, Background : WindowHandle)
打开一个新一图形窗口
二十三dev_set_color( : : ColorName : )
dev_set_color定义在图形窗口中用于显示区域,XLDS其他几何对象的颜色。可用的颜色可以用函数query_color查询。
此外，ColorName 还可以设定为RGB值。格式为”#rrggbb”，其中“RR”、“GG”、和“BB”是十六进制数，值在“00”和“FF’间
更多信息查看set_color的描述。然而，与这函数不同的是，dev_set_color颜色设置效果也用于随后打开的所有新图形窗口。
这些颜色设置一直有效，直到dev_set_color或dev_set_colored调用，或直到颜色设置由用户修改了；
二十四: dev_set_colored( : : NumColors : )
dev_set_colored允许regions，xlds数组和其他几何对象在图形窗口中使用一组预定义的颜色 numcolors。对于numcolors有效值可以查看query_colored介绍。
更多信息见操作者的描述set_colored。然而，与该操作符相反，颜色设置也用于随后打开
更多信息查看set_color的描述。然而，与这函数不同的是，dev_set_color颜色设置效果也用于随后打开的所有新图形窗口。
二十五. dev_set_draw( : : DrawMode : )
设置区域的填充模式。如果DrawMode为“‘fill’”区域显示填充，如果设置为“margin”，只显示轮廓。在“margin”模式的轮廓外观会受到dev_set_line_width，set_line_approx和set_line_style影响。
二十六. dev_set_line_width( : : LineWidth : )
定义线的宽度（像素），是用来显示区域的轮廓（在“margin”模式），xlds，和其他几何输出（例如，disp_region，disp_line，等）。
二十七. dev_set_lut( : : LutName : )
设置活动图形窗口的 look-up-table。 look-up-table定义屏幕上从一个通道图像到“灰色值”图像的 “gray value’转换。query_lut列出所有的名字查找表
二十八. dev_set_paint( : : Mode : )
定义图形窗口上图像对象输出的模式
二十九.dev_set_part( : : Row1, Column1, Row2, Column2 : )
修改图片显示的区域
dev_set_part用于设置在图形窗口中显示的图像的区域。参数row1和column1表示图片区域的左上角，row2和column2右下角。
三十. dev_set_preferences( : : PreferenceNames, PreferenceValues : )
在程序内设置Hdevelop用户操作属性
三十一. dev_set_shape( : : Shape : )
定义区域输出外形
dev_set_shape定义用于显示区域的形状。可用的形状可以用query_shape查询，缺省original，
三十二. dev_set_tool_geometry( : : ToolId, Row, Column, Width, Height : )
设置工具窗口ToolId的位置与尺寸
三十三。dev_set_window( : : WindowHandle : )
激活指定图形窗口WindowHandle
三十四. dev_set_window_extents( : : Row, Column, Width, Height : )
设置当前操作图形窗口的位置与大小
三十五.dev_show_tool( : : ToolId, Action : )
显示指定的工具窗口
不用toolid的话，还可以使用dev_open_tool.函数传递进的名称，这样的话任意的工具窗口只要满足TOOLID就会被使用 ；
三十六.dev_unmap_par( : : : )
关闭可视化参数对话框
三十七.dev_unmap_prog( : : : )
隐藏主窗口，对应函数dev_map_prog可以再次显示主窗口
三十八. dev_unmap_var( : : : )
隐藏变量窗口，dev_map_var可再次显示变量窗口；
三十九. dev_update_var( : : DisplayMode : )
变量窗口的更新开关；
四十. dev_update_window( : : DisplayMode : )
在程序执行时，将图像输出对象自动输出到图形窗口中的功能打开或关闭；