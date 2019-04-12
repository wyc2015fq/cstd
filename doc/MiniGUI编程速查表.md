# MiniGUI编程速查表

  

# 前言

自己学习MiniGui也快一年了，粗浅的学习了一下，本文是根据《MINIGUI-PROG-GUIDE-V3.0-C》中的内容整理而来，根据自己的理解做了一些分类，大部分都是一些函数的说明，方便自己以后学习MiniGui时查阅。现在分享出来，供大家一起学习。当然在学习的过程中也遇到过一些坑，像自定义控件，主窗口透明背景，窗口弹出动画啊，控件动画等等，有时间也会分享出来。

------

# 1  窗口的创建

## 1.1 创建函数

| 函数                            | 含义                         | 备注                           |
| ------------------------------- | ---------------------------- | ------------------------------ |
| CreateMainWindowEx              | 主窗口创建函数               | 以Ex结尾的函数都是新增加的函数 |
| CreateWindowEx                  | 控件的创建函数               | 以Ex结尾的函数都是新增加的函数 |
| DialogBoxIndirectParamEx        | 模态对话框的创建函数         | 以Ex结尾的函数都是新增加的函数 |
| CreateMainWindowIndirectParamEx | 非模态对话框的创建函数       | 以Ex结尾的函数都是新增加的函数 |
| CreateMainWindow                | 无扩展主窗口创建函数         |                                |
| CreateWindow                    | 无扩展控件的创建函数         |                                |
| DialogBoxIndirectParam          | 无扩展模态对话框的创建函数   |                                |
| CreateMainWindowIndirectParam   | 无扩展非模态对话框的创建函数 |                                |
| CreateWindowEx2                 | 对CreateWindowEx的封装       |                                |

## 1.2 主窗口创建属性

| 属性                                  | 含义                                        |
| ------------------------------------- | ------------------------------------------- |
| CreateInfo.dwStyle                    | 窗口风格                                    |
| CreateInfo.spCaption                  | 窗口的标题                                  |
| CreateInfo.dwExStyle                  | 窗口的附加风格                              |
| CreateInfo.hMenu                      | 附加在窗口上的菜单句柄                      |
| CreateInfo.hCursor                    | 在窗口中所使用的鼠标光标句柄                |
| CreateInfo.hIcon                      | 程序的图标                                  |
| CreateInfo.MainWindowProc             | 该窗口的消息处理函数指针                    |
| CreateInfo.lx                         | 窗口左上角相对屏幕的绝对横坐标,以象素点表示 |
| CreateInfo.ty                         | 窗口左上角相对屏幕的绝对纵坐标,以象素点表示 |
| CreateInfo.rx                         | 窗口右下角相对屏幕的绝对横坐标,以象素点表示 |
| [CreateInfo.by](http://CreateInfo.by) | 窗口右下角相对屏幕的绝对纵坐标,以象素点表示 |
| CreateInfo.iBkColor                   | 窗口背景颜色                                |
| CreateInfo.dwAddData                  | 附带给窗口的一个 32 位值                    |
| CreateInfo.hHosting                   | 窗口消息队列的托管窗口                      |

# 2  预定义的控件类

| 控件类             | 类名称          | 宏定义             |
| ------------------ | --------------- | ------------------ |
| 静态框             | “static”        | CTRL_STATIC        |
| 按钮               | “button”        | CTRL_BUTTON        |
| 列表框             | “listbox”       | CTRL_LISTBOX       |
| 单行编辑框         | “sledit”        | CTRL_SLEDIT        |
| 多行编辑框         | “mledit”        | CTRL_MLEDIT        |
| 文本编辑框         | “textedit”      | CTRL_TEXTEDIT      |
| 单行双向文本编辑框 | “bidisledit”    | CTRL_BIDISLEDIT    |
| 组合框             | “combobox”      | CTRL_COMBOBOX      |
| 菜单按钮           | “menubutton”    | CTRL_MENUBUTTON    |
| 进度条             | “progressbar”   | CTRL_PROGRESSBAR   |
| 滑块               | “trackbar”      | CTRL_TRACKBAR      |
| 工具栏             | “newtoolbar”    | CTRL_NEWTOOLBAR    |
| 属性表             | “propsheet”     | CTRL_PROPSHEET     |
| 滚动窗口控件       | “ScrollWnd”     | CTRL_SCROLLWND     |
| 滚动型控件         | “ScrollView”    | CTRL_SCROLLVIEW    |
| 树型控件           | “treeview”      | CTRL_TREEVIEW      |
| 列表型控件         | “listview”      | CTRL_LISTVIEW      |
| 月历               | “MonthCalendar” | CTRL_MONTHCALENDAR |
| 旋钮控件           | “SpinBox”       | CTRL_SPINBOX       |
| 酷工具栏           | “CoolBar”       | CTRL_COOLBAR       |
| 动画控件           | “Animation”     | CTRL_ANIMATION     |
| 网格控件           | “gridview”      | CTRL_GRIDVIEW      |
| 图标型控件         | “IconView”      | CTRL_ICONVIEW      |
| 滚动条控件         | “ScrollBar”     | CTRL_SCROLLBAR     |

\#3  风格

## 3.1 窗口通用风格

| 风格                  | 含义                                                         | 备注                                                         |
| --------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| WS_NONE               | 未指定任何风格                                               |                                                              |
| WS_CHILD              | 创建子窗口                                                   |                                                              |
| WS_VISIBLE            | 创建初始可见的窗口                                           |                                                              |
| WS_DISABLED           | 创建初始被禁止的窗口                                         |                                                              |
| WS_CAPTION            | 创建含标题栏的主窗口                                         | 仅用于主窗口                                                 |
| WS_SYSMENU            | 创建含系统菜单的主窗口                                       | 仅用于主窗口                                                 |
| WS_BORDER             | 创建有边框的窗口                                             |                                                              |
| WS_THICKFRAME         | 创建具有厚边框的窗口                                         |                                                              |
| WS_THINFRAME          | 创建具有薄边框的窗口                                         |                                                              |
| WS_VSCROLL            | 创建带垂直滚动条的窗口                                       |                                                              |
| WS_HSCROLL            | 创建带水平滚动条的窗口                                       |                                                              |
| WS_MINIMIZEBOX        | 标题栏上带最小化按钮                                         | 仅用于主窗口                                                 |
| WS_MAXIMIZEBOX        | 标题栏上带最大化按钮                                         | 仅用于主窗口                                                 |
| WS_EX_NONE            | 无扩展风格                                                   |                                                              |
| WS_EX_USEPRIVATECDC   | 使用私有DC                                                   | 仅用于主窗口                                                 |
| WS_EX_TOPMOST         | 建立始终处于顶层的主窗口                                     | 仅用于主窗口                                                 |
| WS_EX_TOOLWINDOW      | 建立Tooltip主窗口                                            | 仅用于主窗口。Tooltip主窗口将不会拥有输入焦点,但仍接收鼠标消息 |
| WS_EX_TRANSPARENT     | 透明窗口风格	仅用于控件                                   |                                                              |
| WS_EX_USEPARENTFONT   | 使用父窗口字体作为默认字体                                   |                                                              |
| WS_EX_USEPARENTCURSOR | 使用父窗口光标作为默认光标                                   |                                                              |
| WS_EX_NOCLOSEBOX      | 主窗口标题栏上不带关闭按钮                                   |                                                              |
| WS_EX_CTRLASMAINWIN   | 建立可显示在主窗口之外的控件                                 | 仅用于控件                                                   |
| WS_EX_TROUNDCNS       | 左上角和右上角是圆角的窗口                                   |                                                              |
| WS_EX_BROUNDCNS       | 左下角和右下角是圆角的窗口                                   |                                                              |
| WS_EX_CLIPCHILDREN    | 调用BegainPaint获得DC并刷新窗口客户区时, 子窗口所占区域将被剪切;也就是说,向窗口客户区的输出不会输出到子窗口所在位置。 | 该风格将导致额外的内存占用并影响绘制效率。只有窗口的输出和其子窗口的输出发生重叠时才应使用该风格,一般的对话框窗口、属性页控件无需使用该风格。 |
| WS_EX_LEFTSCROLLBAR   | 创建垂直滚动条在左侧的窗口                                   |                                                              |
| WS_EX_AUTOSECONDARYDC | 双缓冲窗口扩展风格                                           | 仅用于主窗口                                                 |

\##3.2  控件风格

### 3.2.1 仅用于对话框控件的风格

| 风格       | 含义                                     | 备注                                                         |
| ---------- | ---------------------------------------- | ------------------------------------------------------------ |
| WS_GROUP   | 具有该风格的控件将成为同组控件的打头控件 | 从该控件到下一个WS_GROUP风格的控件之前的,或者下一个不同种类的控件之前的控件属于同一组 |
| WS_TABSTOP | 具有TAB键停止功能                        | 当用户在对话框中使用TAB键和Shift-TAB键切换输入焦点时,具有该风格的控件可获得焦点 |

### 3.2.2 静态框风格

| 风格              | 含义                               | 备注                                                         |
| ----------------- | ---------------------------------- | ------------------------------------------------------------ |
| SS_SIMPLE         | 显示单行文本                       | 控件文本不会自动换行显示,并且文本永远是左对齐                |
| SS_LEFT           | 左对齐                             | 用来显示多行文本                                             |
| SS_CENTER         | 中对齐                             | 用来显示多行文本                                             |
| SS_RIGHT          | 右对齐                             | 用来显示多行文本                                             |
| SS_LEFTNOWORDWRAP | 扩展文本中的TAB符                  | 但是不做自动换行处理                                         |
| SS_BITMAP         | 显示一幅位图                       |                                                              |
| SS_ICON           | 显示一幅图标                       |                                                              |
| SS_CENTERIMAGE    | 居中显示                           | 与SS_REALSIZEIMAGE一起使用，则取消缩放，居中显示             |
| SS_REALSIZEIMAGE  | 消缩放操作，并显示在静态框的左上方 | 默认情况下,位图和图标要经过适当的缩放充满整个静态框          |
| SS_GROUPBOX       | 分组框                             | 是一个矩形框,分组框标题在其顶部显示,分组方块常用来包含其他的控件 |
| SS_WHITERECT      | 以白色填充静态框矩形               |                                                              |
| SS_GRAYRECT       | 以灰色填充静态框矩形               |                                                              |
| SS_BLACKRECT      | 以黑色填充静态框矩形               |                                                              |
| SS_GRAYFRAME      | 灰色边框                           |                                                              |
| SS_WHITEFRAME     | 白色边框                           |                                                              |
| SS_BLACKFRAME     | 黑色边框                           |                                                              |
| SS_NOTIFY         | 接收静态框消息                     |                                                              |

### 3.2.3 按钮风格

| 风格               | 含义                                    | 备注                                                         |
| ------------------ | --------------------------------------- | ------------------------------------------------------------ |
| BS_PUSHBUTTON      | 边框要细一些                            |                                                              |
| BS_DEFPUSHBUTTON   | “DEF”代表“默认”，边框要粗一些           | 默认接收ENTER键输入的按钮,而不管当前的输入焦点处于哪个控件上 |
| BS_MULTLINE        | 指定显示多行文本                        | 通常情况下,按钮文本会以单行的形式在垂直和水平方向居中显示,不会自动换行 |
| BS_LEFTTEXT        | 文本置于复选框的左边                    |                                                              |
| BS_LEFT            | 文本在左边                              |                                                              |
| BS_CENTER          | 文本在中间                              |                                                              |
| BS_RIGHT           | 文本在右边                              |                                                              |
| BS_TOP             | 文本在上边                              |                                                              |
| BS_BOTTOM          | 文本在下边                              |                                                              |
| BS_VCENTER         | 文本居中垂直                            |                                                              |
| BS_BITMAP          | 普通按钮上显示位图                      |                                                              |
| BS_ICON            | 普通按钮上显示图标                      |                                                              |
| BS_REALSIZEIMAGE   | 使位图或图标显示在控件中部,不作任何缩放 | 默认情况下位图或图标会缩放显示以充满整个按钮窗口范围         |
| BS_CHECKBOX        | 复选框                                  | 应用程序需要自己向该控件发送消息来设定选中标记               |
| BS_AUTOCHECKBOX    | 复选框                                  | 控件会自动在选中和非选中状态之间切换                         |
| BS_3STATE          | 复选框                                  | 功能同上，复选框内是灰色的,这种状态表明该复选框不能被选择或者禁止使用 |
| BS_AUTO3STATE      | 复选框                                  | 功能同上，复选框内是灰色的,这种状态表明该复选框不能被选择或者禁止使用 |
| BS_PUSHLIKE        | 使复选框以普通按钮的形式显示            | 选中时显示为按下状态,未选中时显示为正常状态                  |
| BS_RADIOBUTTON     | 单选钮                                  | 它的行为很像复选框，圆圈内的加重圆点表示该单选按钮已经被选中 |
| BS_AUTORADIOBUTTON | 单选钮                                  | 自动显示用户的选择情况                                       |
| BS_PUSHLIKE        | 使单选按钮以普通按钮的形式显示          | 选中时显示为按下状态,未选中时显示为正常状态                  |
| BS_NOTIFY          | 接收按钮消息                            |                                                              |

### 3.2.4 列表风格

| 风格            | 含义                         |
| --------------- | ---------------------------- |
| LBS_MULTIPLESEL | 多选列表框                   |
| LBS_USEICON     | 附带位图或者图标             |
| LBS_CHECKBOX    | 在列表项旁边显示一个检查框   |
| LBS_AUTOCHECK   | 自动切换选中状态             |
| LBS_NOTIFY      | 选中某个列表项时产生通知消息 |
| LBS_SORT        | 列表排序                     |

### 3.2.5 编辑框风格

| 风格          | 含义                                                         |
| ------------- | ------------------------------------------------------------ |
| ES_UPPERCASE  | 可以使编辑框只显示大写字母                                   |
| ES_LOWERCASE  | 可以使编辑框只显示小写字母                                   |
| ES_PASSWORD   | 编辑框用来输入密码,但用星号(*)显示输入的字符                 |
| ES_READONLY   | 建立只读编辑框,用户不能修改编辑框中的内容,但插入符仍然可见   |
| ES_BASELINE   | 在编辑框文本下显示虚线                                       |
| ES_AUTOWRAP   | 用于多行编辑框,当文本超过控件边界时,将自动换行               |
| ES_LEFT       | 指定非多行编辑框的对齐风格,实现文本的左对齐风格              |
| ES_NOHIDESEL  | 编辑框在失去焦点时保持被选择文本的选中状态                   |
| ES_AUTOSELECT | 编辑框在得到焦点时自动选中所有的文本内容(仅针对单行编辑框)   |
| ES_TITLE      | 在编辑框的第一行显示指定的标题,只适用于多行编辑框控件        |
| ES_TIP        | 当编辑框的内容为空时,在其中显示相关的提示信息;只适用于SLEDIT控件 |
| ES_CENTER     | 指定非多行编辑框的对齐风格,实现文本的居中对齐风格            |
| ES_RIGHT      | 指定非多行编辑框的对齐风格,实现文本的右对齐风格              |

其中适用于多行编辑框的风格有：ES_UPPERCASE, ES_LOWERCASE, ES_READONLY, ES_BASELINE, ES_AUTOWRAP, ES_NOHIDESEL,ES_TITLE
 其中适用于单行编辑框的风格有：ES_UPPERCASE, ES_LOWERCASE, ES_READONLY, ES_BASELINE,  ES_LEFT, ES_CENTER,  ES_RIGHT,ES_PASSWORD,ES_NOHIDESEL,ES_AUTOSELECT,ES_TIP

### 3.2.6 组合框风格

| 风格                    | 含义                                              |
| ----------------------- | ------------------------------------------------- |
| CBS_SIMPLE              | 创建一个简单组合框                                |
| CBS_DROPDOWNLIST        | 创建下拉式组合框                                  |
| CBS_SPINLIST            | 创建旋钮组合框                                    |
| CBS_SPINARROW_TOPBOTTOM | 箭头在内容的上下                                  |
| CBS_SPINARROW_LEFTRIGHT | 箭头在内容的左右                                  |
| CBS_READONLY            | 使组合框的输入域成为只读区域                      |
| CBS_UPPERCASE           | 使键入编辑框中的文本自动变成大写                  |
| CBS_LOWERCASE           | 使键入编辑框中的文本自动变成小写                  |
| CBS_EDITBASELINE        | 使编辑框带有文本基线                              |
| CBS_SORT                | 使用该风格的组合框将自动对插入的条目进行排序      |
| CBS_EDITNOBORDER        | 使得输入域不带边框                                |
| CBS_AUTOFOCUS           | 组合框在获得输入焦点之后,编辑框将自动获得输入焦点 |
| CBS_AUTOSPIN            | 创建旋钮数字框,默认的最小值和最大值为0和100       |
| CBS_AUTOLOOP            | 框中的数字将自动循环显示                          |

### 3.2.7 菜单按钮风格

| 风格            | 含义                           |
| --------------- | ------------------------------ |
| MBS_SORT        | 对菜单按钮中的条目进行排序显示 |
| MBS_LEFTARROW   | 箭头显示在菜单按钮的左侧       |
| MBS_NOBUTTON    | 不显示按钮                     |
| MBS_ALIGNLEFT   | 菜单按钮上的文字向左对齐       |
| MBS_ALIGNRIGHT  | 菜单按钮上的文字向右对齐       |
| MBS_ALIGNCENTER | 菜单按钮上的文字居中对齐       |

### 3.2.8 进度条风格

| 风格         | 含义                                 |
| ------------ | ------------------------------------ |
| PBS_NOTIFY   | 使用该风格的进度条控件会产生通知消息 |
| PBS_VERTICAL | 竖直显示进度条                       |

### 3.2.9 滑块风格

| 风格         | 含义                                                 |
| ------------ | ---------------------------------------------------- |
| TBS_NOTIFY   | 滑块产生通知消息                                     |
| TBS_VERTICAL | 创建竖直的滑块，默认情况下,滑块是水平的              |
| TBS_TIP      | 滑块两端显示文字说明，还将在控件的中部显示当前刻度值 |
| TBS_NOTICK   | 不显示刻度                                           |
| TBS_BORDER   | 风格可使滑块带有边框,该风格不常用                    |

### 3.2.10 工具栏风格

| 风格               | 含义                                                         |
| ------------------ | ------------------------------------------------------------ |
| NTBS_HORIZONTAL    | 水平显示工具栏，这是默认风格                                 |
| NTBS_VERTICAL      | 垂直显示工具栏                                               |
| NTBS_MULTLINE      | 工具栏可多行显示，当工具项类型为NTBIF_NEWLINE时,将另起一行显示其后添加的工具项 |
| NTBS_WITHTEXT      | 将在按钮下方或者按钮右边显示文本,默认显示在按钮位图的下方。这时,应用程序必须在添加按钮时指定按钮对应的文本。当文字在图标下方显示且按钮处于被激活状态时,按钮图片将突出显示 |
| NTBS_TEXTRIGHT     | 配合NTBS_WITHTEXT风格使用时,该风格指定将文本显示在按钮位图的右边。当文字在图标右侧且按钮处于被激活状态时,按钮图片和文字都将突出显示 |
| NTBS_DRAWSTATES    | 不使用按钮的高亮、按下以及灰化状态的位图单元,而改用三维风格的边框来表示这些状态 |
| NTBS_DRAWSEPARATOR | 绘制分隔条。默认情况下,工具栏上用来分隔按钮的分隔条是不会被绘制的,而只会加大两个按钮之间的间距。具有该风格之后,将绘制窄的分隔条 |

### 3.2.11 属性表风格

| 风格           | 含义                                                         |
| -------------- | ------------------------------------------------------------ |
| PSS_SIMPLE     | 所有的属性页凸舌具有相同的宽度                               |
| PSS_COMPACTTAB | 属性页凸舌的宽度取决于属性页标题文本的长度                   |
| PSS_SCROLLABLE | 属性页凸舌的宽度取决于属性页标题文本的长度,当属性页凸舌的数目过多时,将自动出现左右箭头用来调节当前可见的属性页凸舌 |
| PSS_BOTTOM     | 属性页凸舌显示在属性表的下方,可以和上面三种风格同时配合使用  |

### 3.2.12 滚动型控件风格

| 风格           | 含义                                                 |
| -------------- | ---------------------------------------------------- |
| SVS_AUTOSORT   | 对列表项进行自动排序，前提是先使用SVM_SETITEMCMP消息 |
| SVM_SETITEMCMP | 设置滚动型控件的列表项比较函数                       |
| SVM_SORTITEMS  | 来对列表项进行一次性的排序                           |

### 3.2.13 树型控件风格

| 风格         | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| TVS_WITHICON | 使用图标来显示每项的折叠和展开状态,相应的图标可以在创建节点项时指定，没有该风格使用一个带方框的“+”号来表示一个折叠的节点项，用带方框的“-”号来表示展开的节点项 |
| TVS_SORT     | 对节点项进行自动排序                                         |
| TVS_NOTIFY   | 将在响应用户操作时产生相应的通知消息和通知码                 |

### 3.2.14 列表控件风格

| 风格         | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| LVS_TREEVIEW | 支持以树型的方式来显示列表项                                 |
| LVS_UPNOTIFY | 指定列表型控件的在响应用户鼠标点击操作时的响应方式，默认情况下,如果没有指定LVS_UPNOTIFY风格,列表型控件将在鼠标按下时发出通知消息;如果指定了该风格,控件将在鼠标抬起时发出通知消息 |

### 3.2.15 月历控件风格

| 风格       | 含义                                         |
| ---------- | -------------------------------------------- |
| MCS_CHN    | 控件以中文显示日期信息                       |
| MCS_ENG_L  | 以英文显示日期信息                           |
| MCS_ENG_S  | 以简写的英文格式显示                         |
| MCS_NOTIFY | 将在响应用户操作时等情况下产生相应的通知消息 |

### 3.2.16 旋钮控件风格

| 风格           | 含义                                                         |
| -------------- | ------------------------------------------------------------ |
| SPS_AUTOSCROLL | 自动判断旋钮控件目前的滚动状态,在滚动到最大值和最小值时分别把向上和向下箭头禁止掉(变灰)，没有该风格的旋钮控件的滚动状态由应用程序掌握 |

### 3.2.17 酷工具栏风格

| 风格           | 含义                                 |
| -------------- | ------------------------------------ |
| CBS_BMP_16X16  | 酷工具栏的按钮项分别显示16x16的位图  |
| CBS_BMP_32X32  | 酷工具栏的按钮项分别显示32x32的位图  |
| CBS_BMP_CUSTOM | 酷工具栏的按钮项使用自定义大小的位图 |
| CBS_USEBKBMP   | 酷工具栏有背景位图                   |

### 3.2.18 动画控件风格

| 风格         | 含义                                      |
| ------------ | ----------------------------------------- |
| ANS_AUTOLOOP | 使用该风格之后,动画控件将自动重复播放动画 |
| ANS_SCALED   | 根据控件大小缩放动画对象                  |
| ANS_FITTOANI | 根据动画对象大小调整控件尺寸              |

### 3.2.19 独立滚动条风格

| 风格                | 含义                                                         |
| ------------------- | ------------------------------------------------------------ |
| SBS_HORZ            | 创建一个水平滚动条。当没有指定SBS_BOTTOMALIGN或SBS_TOPALIGN,滚动条的范围根据CreateWindowEx2参数x,y,w,h决定 |
| SBS_VERT            | 创建一个垂直滚动条。当没有指定SBS_LEFTALIGN或SBS_RIGHTALIGN,滚动条的范围根据CreateWindowEx2参数x,y,w,h决定 |
| SBS_BOTTOMALIGN     | 与SBS_HORZ一起使用。放置水平滚动条在CreateWindowEx2指定范围的底部 |
| SBS_TOPALIGN        | 与SBS_HORZ一起使用。放置水平滚动条在CreateWindowEx2指定范围的顶部 |
| SBS_LEFTALIGN       | 与SBS_VERT一起使用。放置垂直滚动条在CreateWindowEx2指定范围的左边 |
| SBS_RIGHTALIGN      | 与SBS_VERT一起使用,放置垂直滚动条在CreateWindowEx2指定范围的右边 |
| SBS_NOARROWS        | 没有箭头,不能与SBS_NOSHAFT一起使用                           |
| SBS_NOSHAFT         | 没有shaft,不能与SBS_NOARROWS一起使用                         |
| SBS_FIXEDBARLEN     | 水平滚动条的thumb长度固定,或者垂直滚动条的thumb长度固定      |
| SBS_NOTNOTIFYPARENT | 向父窗口的通知发送方式不是发送通知码,而是发送消息;默认发送通知码 |

\#4  通用窗口操作函数

| 函数                     | 含义                             | 备注                                                         |
| ------------------------ | -------------------------------- | ------------------------------------------------------------ |
| UpdateWindow             | 立即更新某个窗口                 |                                                              |
| ShowWindow               | 显示或隐藏某个窗口               |                                                              |
| IsWindowVisible          | 判断某个窗口是否可见             | 控件和主窗口均可用                                           |
| EnableWindow             | 使能或禁止某个窗口               |                                                              |
| IsWindowEnabled          | 判断某个窗口是否可用             |                                                              |
| GetClientRect            | 获取窗口客户区矩形               |                                                              |
| GetWindowRect            | 获取窗口矩形                     | 屏幕坐标系中的窗口尺寸                                       |
| GetWindowBkColor         | 获取窗口背景色                   |                                                              |
| SetWindowBkColor         | 设置窗口背景色                   |                                                              |
| GetWindowFont            | 获取窗口默认字体                 |                                                              |
| SetWindowFont            | 设置窗口默认字体                 |                                                              |
| GetWindowCursor          | 获取窗口光标                     |                                                              |
| SetWindowCursor          | 设置窗口光标                     |                                                              |
| GetWindowStyle           | 获取窗口风格                     |                                                              |
| GetWindowExStyle         | 获取窗口扩展风格                 |                                                              |
| GetFocusChild            | 获取拥有输入焦点的子窗口         |                                                              |
| SetFocusChild            | 设置焦点子窗口                   |                                                              |
| GetWindowCallbackProc    | 获取窗口过程函数                 |                                                              |
| SetWindowCallbackProc    | 设置窗口过程函数                 |                                                              |
| GetWindowAdditionalData  | 获取窗口附加数据一               |                                                              |
| SetWindowAdditionalData  | 设置窗口附加数据一               |                                                              |
| GetWindowAdditionalData2 | 获取窗口附加数据二               | 对话框和控件在内部已使用附加数据二,保留附加数据一给应用程序使用 |
| SetWindowAdditionalData2 | 设置窗口附加数据二               |                                                              |
| GetWindowCaption         | 获取窗口标题                     | 通常用于主窗口                                               |
| SetWindowCaption         | 设置窗口标题                     |                                                              |
| InvalidateRect           | 使窗口的给定矩形区域无效         | 将引发窗口重绘                                               |
| GetUpdateRect            | 获取窗口当前的无效区域外包矩形   |                                                              |
| ClientToScreen           | 将窗口客户区坐标转换为屏幕坐标   |                                                              |
| ScreenToClient           | 将屏幕坐标转换为客户区坐标       |                                                              |
| WindowToScreen           | 将窗口坐标转换为屏幕坐标         |                                                              |
| ScreenToWindow           | 将屏幕坐标转换为窗口坐标         |                                                              |
| IsMainWindow             | 判断给定窗口是否为主窗口         |                                                              |
| IsControl                | 判断给定窗口是否为控件           |                                                              |
| IsDialog                 | 判断给定窗口是否为对话框         |                                                              |
| GetParent                | 获取窗口的父窗口句柄             | 主窗口的父窗口永远为HWND_DESKTOP                             |
| GetMainWindowHandle      | 返回包含某个窗口的主窗口句柄     |                                                              |
| GetNextChild             | 获取下一个子窗口                 | 用于遍历某个窗口的所有子窗口                                 |
| GetNextMainWindow        | 获取下一个主窗口句柄             | 用于遍历所有主窗口                                           |
| GetHosting               | 获取某个主窗口的托管窗口         |                                                              |
| GetFirstHosted           | 获取某个主窗口的第一个被托管窗口 | 用于遍历某个主窗口的所有被托管窗口                           |
| GetNextHosted            | 获取下一个被托管窗口             |                                                              |
| GetActiveWindow          | 获取当前活动主窗口               |                                                              |
| SetActiveWindow          | 设置当前活动主窗口               |                                                              |
| GetCapture               | 获取当前捕获鼠标的窗口           |                                                              |
| SetCapture               | 捕获鼠标                         |                                                              |
| ReleaseCapture           | 释放鼠标                         |                                                              |
| MoveWindow               | 移动窗口或改变窗口大小           |                                                              |
| ScrollWindow             | 滚动窗口客户区的内容             | ScrollWindow函数可根据客户区的滚动情况自动调整窗口内子窗口的位置 |
| IncludeWindowStyle       | 增加指定的窗口属性               |                                                              |
| ExcludeWindowStyle       | 去除指定的窗口属性               |                                                              |
| GetNotificationCallback  | 获取控件的通知消息回调函数       | 在MiniGUI 1.2.6 版本中出现                                   |
| SetNotificationCallback  | 设置控件的通知消息回调函数       |                                                              |
| SetTimer                 | 创建定时器                       | 指定的窗口会收到MSG_TIMER消息                                |
| KillTimer                | 删除定时器                       |                                                              |
| DestroyAllControls       | 销毁所有的子窗口                 |                                                              |

# 5  对话框操作函数

| 函数                | 含义                         | 备注                                            |
| ------------------- | ---------------------------- | ----------------------------------------------- |
| GetDlgCtrlID        | 根据控件句柄获取控件标识符   |                                                 |
| GetDlgItem          | 根据控件标识符获取控件句柄   |                                                 |
| GetDlgItemInt       | 获取控件文本并转换为整数值   |                                                 |
| SetDlgItemInt       | 根据整数值设置控件文本       |                                                 |
| GetDlgItemText      | 获取子控件文本               | 功能同GetWindowText                             |
| GetDlgItemText2     | 获取子控件文本               | 根据文本长度自动分配内存,应用程序负责释放该内存 |
| SetDlgItemText      | 设置子控件文本               | 功能同SetWindowText                             |
| GetNextDlgGroupItem | 获取下一个同组子控件         | 用于遍历同组控件,参阅WS_GROUP风格               |
| GetNextDlgTabItem   | 获取下一个“TAB键停止”子控件  | 用于TAB键游历控件,参阅WS_TABSTOP风格            |
| SendDlgItemMessage  | 向子控件发送消息             | 功能同SendMessage                               |
| CheckDlgButton      | 设置检查框子控件的选中状态   |                                                 |
| CheckRadioButton    | 设置单选按钮子控件的选中状态 |                                                 |
| IsDlgButtonChecked  | 检查子按钮是否选中           |                                                 |
| GetDlgDefPushButton | 获取当前默认子按钮           |                                                 |

# 6  控件操作函数

## 6.1 菜单操作函数

| 函数            | 含义                     | 备注                                                         |
| --------------- | ------------------------ | ------------------------------------------------------------ |
| CreateMenu      | 创建一个空的菜单         |                                                              |
| InsertMenuItem  | 添加菜单项               |                                                              |
| CreatePopupMenu | 创建弹出式菜单           |                                                              |
| TrackPopupMenu  | 显示弹出式菜单           |                                                              |
| StripPopupHead  | 删除弹出式菜单的头部     |                                                              |
| GetMenuItemInfo | 获得感兴趣的菜单项属性   |                                                              |
| SetMenuItemInfo | 设置感兴趣的菜单项属性   |                                                              |
| RemoveMenu      | 从菜单中删除指定的菜单项 | 如果菜单项含有子菜单,则会解除子菜单和该菜单项的关联,但并不删除子菜单 |
| DeleteMenu      | 从菜单中删除指定的菜单项 | 如果菜单项含有子菜单,则同时会删除子菜单                      |
| DestroyMenu     | 删除整个菜单             |                                                              |

## 6.2 滚动条操作函数

| 函数            | 含义                 | 备注                                                         |
| --------------- | -------------------- | ------------------------------------------------------------ |
| EnableScrollBar | 使能、禁止滚动条     |                                                              |
| SetScrollRange  | 设置滚动条的滚动范围 |                                                              |
| SetScrollInfo   | 设置滚动条的信息     | 如果在调用SetScrollRange后,调用了影响滚动条位置的其他函数,则应该将bRedraw设定为FALSE以避免过多地重画 |

## 6.3 键盘/鼠标操作函数

| 函数              | 含义                                               | 备注                             |
| ----------------- | -------------------------------------------------- | -------------------------------- |
| GetShiftKeyStatus | 获取键状态值                                       |                                  |
| GetKeyStatus      | 确定键盘上某个键的状态                             |                                  |
| GetFocusChild     | 获得某个窗口中具有输入焦点的子窗口的句柄           |                                  |
| SetFocusChild     | 父窗口可以调用该函数来把输入焦点赋予它的某个子窗口 |                                  |
| SetCapture        | 用该函数来使某个窗口捕获鼠标                       | 在某一时刻只能有一个窗口捕获鼠标 |
| ReleaseCapture    | 恢复正常的鼠标处理方式                             |                                  |
| GetCapture        | 确定当前哪个窗口捕获了鼠标                         |                                  |

## 6.4 图标/光标操作函数

| 函数               | 含义                                                         | 备注                                                         |
| ------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| LoadIconFromFile   | 从一个Windows图标文件(*.ico)中载入图标,图标可以是单色、16色或256色的 |                                                              |
| LoadIconFromMem    | 从内存中装载图标                                             |                                                              |
| DrawIcon           | 装载完图标对象之后,就可以调用该函数在指定的位置绘制图标      |                                                              |
| DestroyIcon        | 销毁图标句柄,并释放它所占用的内存                            |                                                              |
| CreateIcon         | 运行时动态创建图标，同样需要用DestroyIcon函数来销毁          |                                                              |
| GetLargeSystemIcon | 来获取内存中的系统图标来使用                                 | 32x32像素的系统大图标                                        |
| GetSmallSystemIcon | 来获取内存中的系统图标来使用                                 | 16x16像素的系统小图标                                        |
| LoadSystemIcon     | 直接从MiniGUI.cfg配置文件中定义的图标文件中载入所需的系统图标 | 实际上是通过调用 LoadIconFromFile来载入图标的，不再需要的时候也必须使用DestroyIcon函数来销毁 |
| LoadCursorFromFile | 从一个Windows光标文件中载入光标                              |                                                              |
| LoadCursorFromMem  | 从内存中读入光标                                             |                                                              |
| CreateCursor       | 动态创建光标                                                 |                                                              |
| DestroyCursor      | 销毁由LoadCursorFromFile函数和CreateCursor函数创建的光标,并释放光标对象所占用的内存 |                                                              |
| GetCursorPos       | 获取光标的当前屏幕位置                                       |                                                              |
| SetCursorPos       | 把光标移动到屏幕上的指定位置                                 |                                                              |
| GetCurrentCursor   | 获取当前光标句柄                                             |                                                              |
| SetCursorEx        | 设置当前光标                                                 |                                                              |
| SetCursor          | 设置当前光标,不改变缺省光标                                  |                                                              |
| SetDefaultCursor   | 把给定光标设置为当前光标和缺省光标                           |                                                              |
| GetWindowCursor    | 获取给定窗口的当前光标                                       |                                                              |
| SetWindowCursor    | 设置新的窗口光标                                             |                                                              |
| GetSystemCursor    | 获得的系统缺省光标                                           | GetSystemCursor(0)、GetSystemCursor(IDC_ARROW)是一样的       |
| ShowCursor         | 显示或隐藏光标                                               |                                                              |
| ClipCursor         | 把光标限定在屏幕的某个矩形区域内                             | 常用于响应某一特定的限制矩形区域内的事件                     |
| GetClipCursor      | 获取当前的光标限定矩形                                       | 该函数可以用来在设置新的限定矩形之前保存原始限定矩形,需要时把它用于恢复原始区域 |

### 6.4.1光标类型

| 类型            | 含义                        | 值   |
| --------------- | --------------------------- | ---- |
| IDC_ARROW       | 系统缺省的箭头光标          | 0    |
| IDC_IBEAM       | ‘I’形光标,指示输入区域      | 1    |
| IDC_PENCIL      | 笔形光标                    | 2    |
| IDC_CROSS       | 十字光标                    | 3    |
| IDC_MOVE        | 移动光标                    | 4    |
| IDC_SIZENWSE    | 西北-东南方向的调整大小光标 | 5    |
| IDC_SIZENESW    | 东北-西南方向的调整大小光标 | 6    |
| IDC_SIZEWE      | 东西方向的调整大小光标      | 7    |
| IDC_SIZENS      | 南北方向的调整大小光标      | 8    |
| IDC_UPARROW     | 向上箭头光标                | 9    |
| IDC_NONE        | 空光标                      | 10   |
| IDC_HELP        | 带问号光标                  | 11   |
| IDC_BUSY        | 忙光标                      | 12   |
| IDC_WAIT        | 等待光标                    | 13   |
| IDC_RARROW      | 右箭头光标                  | 14   |
| IDC_COLOMN      | 列光标                      | 15   |
| IDC_ROW         | 行光标                      | 16   |
| IDC_DRAG        | 拖动光标,用于拖放操作       | 17   |
| IDC_NODROP      | 不可放下光标,用于拖放操作   | 18   |
| IDC_HAND_POINT  | 手形指点光标                | 19   |
| IDC_HAND_SELECT | 手形选择光标                | 20   |
| IDC_SPLIT_HORZ  | 水平分割光标                | 21   |
| IDC_SPLIT_VERT  | 垂直分割光标                | 22   |

## 6.5 插入符操作函数

| 函数              | 含义                                | 备注                                               |
| ----------------- | ----------------------------------- | -------------------------------------------------- |
| CreateCaret       | 创建一个插入符,并把它赋给指定的窗口 |                                                    |
| DestroyCaret      | 销毁由创建的插入符                  |                                                    |
| ShowCaret         | 显示插入符                          |                                                    |
| HideCaret         | 把插入符隐藏掉                      |                                                    |
| GetCaretPos       | 获取插入符的所在位置                |                                                    |
| SetCaretPos       | 在一个窗口之内移动插入符            |                                                    |
| GetCaretBlinkTime | 获取插入符的反转时间                | 该时间以毫秒计数,系统缺省的插入符反转时间为500毫秒 |
| SetCaretBlinkTime | 定义插入符的反转时间                | 插入符反转时间最小不能小于100毫秒                  |

## 6.6 剪贴板操作函数

| 函数                | 含义                                                     | 备注                                                |
| ------------------- | -------------------------------------------------------- | --------------------------------------------------- |
| CreateClipBoard     | 创建自定义的剪贴板，包括系统的最多有NR_CLIPBOARDS（4）个 | MiniGUI提供了一个默认的文本剪贴板,名字为CBNAME_TEXT |
| DestroyClipBoard    | 销毁自定义的剪贴板                                       |                                                     |
| SetClipBoardData    | 把数据传送到指定的剪贴板                                 |                                                     |
| GetClipBoardDataLen | 获取剪贴板上数据的大小                                   |                                                     |
| GetClipBoardData    | 把剪贴板上的数据复制到指定的数据缓冲区中                 |                                                     |
| GetClipBoardByte    | 从剪贴板数据的指定位置获取一个字节                       |                                                     |

## 6.7 配置文件操作函数

| 函数                   | 含义                                                    | 备注                                                         |
| ---------------------- | ------------------------------------------------------- | ------------------------------------------------------------ |
| GetValueFromEtcFile    | 从指定的配置文件当中获取指定的键值,键值以字符串形式返回 |                                                              |
| GetIntValueFromEtcFile | 从指定的配置文件当中获取指定的整数型键值                |                                                              |
| SetValueToEtcFile      | 将给定的键值保存到指定的配置文件当中                    | 如果配置文件不存在,则将新建配置文件                          |
| LoadEtcFile            | 把指定的配置文件读入内存,返回一个配置对象句柄           |                                                              |
| UnloadEtcFile          | 释放内存中的配置文件信息                                |                                                              |
| GetValueFromEtc        | 使用方法和第一个函数类似                                | 注意第一个参数是句柄                                         |
| GetIntValueFromEtc     | 使用方法和第二个函数类似                                | 注意第一个参数是句柄                                         |
| SetValueToEtc          | 使用方法和第三个函数类似                                | 只改变内存中的配置值                                         |
| RemoveSectionInEtc     | 从内存中的配置文件信息中删除指定的段信息                |                                                              |
| RemoveSectionInEtcFile | 从指定的配置文件当中删除指定的段信息                    |                                                              |
| GetValueFromEtcSec     | 从内存中的配置文件信息的指定段中获取指定的键值          |                                                              |
| GetIntValueFromEtcSec  | 从内存中的配置文件信息的指定段中获取指定的整数型键值    |                                                              |
| SetValueToEtcSec       | 保存键值到内存中的配置文件信息的指定段中                |                                                              |
| SaveEtcToFile          | 将内存中的配置文件信息保存到指定的文件中                |                                                              |
| FindSectionInEtc       | 在内存中的配置文件信息中查找指定的字段                  | 如果没有指定的字段存在,当参量bCreateNew为TRUE时将新建一空字段 |

## 6.8 编辑框操作函数

| 函数                | 含义                        |
| ------------------- | --------------------------- |
| GetWindowTextLength | 获取文本的长度,以字节为单位 |
| GetWindowText       | 获取编辑框中的文本          |
| SetWindowText       | 设置编辑框中的文本内容      |

# 7  消息的种类

## 7.1 窗口消息

| 消息             | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| MSG_NCCREATE     | 消息在MiniGUI建立主窗口的过程中发送到窗口过程                |
| MSG_SIZECHANGING | 消息窗口尺寸发生变化时,或者建立窗口时发送到窗口过程          |
| MSG_SIZECHANGED  | 消息在窗口尺寸发生变化后发送到窗口过程                       |
| MSG_CSIZECHANGED | 消息是窗口客户区的尺寸发生变化后发送到窗口过程               |
| MSG_CREATE       | 消息在窗口成功创建并添加到MiniGUI的窗口管理器之后发送到窗口过程 |
| MSG_FONTCHANGING | 当应用程序调用SetWindowFont改变窗口的默认字体时,将发送该消息到窗口过程 |
| MSG_FONTCHANGED  | 当应用程序调用SetWindowFont改变了窗口的默认字体后,将发送该消息到窗口过程 |
| MSG_ERASEBKGND   | 当系统需要清除窗口背景时,将发送该消息到窗口过程              |
| MSG_PAINT        | 该消息在需要进行窗口重绘时发送到窗口过程                     |
| MSG_CLOSE        | 当用户点击窗口上的“关闭”按钮时, MiniGUI向窗口过程发 送MSG_CLOSE消息 |
| MSG_DESTROY      | 消息在应用程序调DestroyMainWindow或者DestroyWindow时发送到窗口过程当中 |
| MSG_INITDIALOG   | 消息是在MiniGUI根据对话框模板建立对话框以及控件之后,发送到对话框回调函数的 |
| MSG_COMMAND      | 控件内部发生某种事件时,会通过通知消息通知其父窗口            |
| MSG_ACTIVEMENU   | 在用户激活菜单栏中的某个弹出式菜单后, MiniGUI将给菜单栏所在的窗口过程发送MSG_ACTIVEMENU消息 |
| MSG_TIMER        | 创建定时器后触发的消息                                       |

一般来说,在以下情况下,MiniGUI程序的窗口过程会接收到一个MSG_PAINT 消息

- 用户移动窗口或显示窗口时,MiniGUI向先前被隐藏的窗口发送 MSG_PAINT 消息
- 程序使用InvalidateRect函数来更新窗口的无效区域,这将产生一个 MSG_PAINT 消息
- 程序调用UpdateWindow函数来重绘窗口
- 覆盖程序窗口的对话框或消息框被消除
- 下拉或弹出菜单被消除

## 7.2 键鼠消息

| 消息              | 含义                                                         |
| ----------------- | ------------------------------------------------------------ |
| MSG_KEYDOWN       | 一个键被按下，如果用户按住某个键不放手，系统将会产生一系列按下的消息 |
| MGS_SYSKEYDOWN    | 一个键被按下，如果用户按住某个键不放手，系统将会产生一系列按下的消息 |
| MSG_KEYUP         | 释放一个键                                                   |
| MGS_SYSKEYUP      | 释放一个键                                                   |
| MSG_CHAR          | 字符键的字符消息                                             |
| MSG_KILLFOCUS     | 系统向将要失去焦点的窗口发送该消息                           |
| MSG_SETFOCUS      | 发送给将要得到焦点的窗口                                     |
| MSG_LBUTTONDOWN   | 鼠标左按钮被按下，MSG_NCLBUTTONDOWN                          |
| MSG_LBUTTONUP     | 鼠标左按钮被释放，MSG_NCLBUTTONUP                            |
| MSG_RBUTTONDOWN   | 鼠标右按钮被按下，MSG_NCRBUTTONDOWN                          |
| MSG_RBUTTONUP     | 鼠标右按钮被释放，MSG_NCRBUTTONUP                            |
| MSG_LBUTTONDBLCLK | 鼠标左按钮被双击，MSG_NCLBUTTONDBLCLK                        |
| MSG_RBUTTONDBLCLK | 鼠标右按钮被双击，MSG_NCRBUTTONDBLCLK                        |
| MSG_HITTEST       | 如果发生鼠标事件,系统会向含有光标热点的窗口或捕捉鼠标的窗口发送该消息，MSG_NCHITTEST |
| MSG_MOUSEMOVE     | 确定光标的当前位置                                           |
| MSG_SETCURSOR     | 当用户移动鼠标时,MiniGUI 将把MSG_SETCURSOR消息发送给光标下面的窗口,光标应用程序可以在处理MSG_SETCURSOR消息时改变当前的光标 |

## 7.3 控件消息

### 7.3.1 滚动条消息

| 消息        | 含义                     |
| ----------- | ------------------------ |
| MSG_VSCROLL | 垂直滚动条上下移动的消息 |
| MSG_HSCROLL | 水平滚动条左右移动的消息 |

### 7.3.2 静态框消息

| 消息         | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| STM_GETIMAGE | 返回位图的指针或者图标句柄                                   |
| STM_SETIMAGE | 通过wParam参数重新设置位图指针或者图标句柄,并且返回原来的指针 |

### 7.3.3 按钮消息

| 消息        | 含义                                   |
| ----------- | -------------------------------------- |
| BM_GETCHECK | 查询复选框或者单选钮的选中状态         |
| BM_SETCHECK | 设置复选框或者单选钮的选中状态         |
| BM_GETSTATE | 查询普通按钮或者复选框的按下或释放状态 |
| BM_SETSTATE | 设置普通按钮或者复选框的按下或释放状态 |
| BM_GETIMAGE | 获取位图按钮上的位图或者图标           |
| BM_SETIMAGE | 设置位图按钮上的位图或者图标           |
| BM_CLICK    | 发送BM_CLICK模拟用户鼠标的单击操作     |

### 7.3.4 列表消息

| 消息               | 含义                                                         |
| ------------------ | ------------------------------------------------------------ |
| LB_ERRSPACE        | 当列表框控件所能存储的内容超过了可用内存空间时返回该消息     |
| LB_ERR             | 因为其他原因而出错返回该消息                                 |
| LB_OKAY            | 操作成功返回该消息                                           |
| LB_ADDSTRING       | 将新的字符串追加为列表框的最后一项                           |
| LB_INSERTSTRING    | 将字符串插入到列表框中的指定位置                             |
| LB_DELETESTRING    | 从列表框中删除指定的条目                                     |
| LB_RESETCONTENT    | 清空列表框中的所有内容                                       |
| LB_GETCOUNT        | 可获得列表框中的条目个数                                     |
| LB_GETTEXTLEN      | 获得列表框中指定条目的字符串长度                             |
| LB_GETTEXT         | 将该条目复制到文字缓冲区中                                   |
| LB_SETTEXT         | 设置列表框条目的字符串                                       |
| LB_GETITEMDATA     | 获得列表框条目的其他信息，如位图对象或图标句柄、检查框状态   |
| LB_SETITEMDATA     | 设置列表框条目的其他信息，如位图对象或图标句柄、检查框状态   |
| LB_SETCURSEL       | 控制当前的选中项，对于多项选择列表框，只能用来设置当前高亮项 |
| LB_GETCURSEL       | 获得当前选定的索引项，对于多项选择列表框，只能用来获取当前高亮项,无法获得所有具有选中状态的条目 |
| LB_SETSEL          | 设定某特定条目的选择状态,而不影响其他项                      |
| LB_GETSEL          | 获取某特定条目的选择状态                                     |
| LB_GETSELCOUNT     | 获得多选列表框中当前被选中的条目个数                         |
| LB_GETSELITEMS     | 获得所有被选中条目的索引值                                   |
| LB_FINDSTRING      | 查找含有字符串的条目，返回模糊匹配字符串string的条目索引值   |
| LB_FINDSTRINGEXACT | 将进行严格精确匹配查找                                       |
| LB_GETCHECKMARK    | 设置某条目的检查框的当前状态,如果没有找到相应条目,则返回LB_ERR,CMFLAG_CHECKED处于选择状态,CMFLAG_PARTCHECKED处于部分选择状态,CMFLAG_BLANK处于未选择状态 |
| LB_SETCHECKMARK    | 指定索引处条目的检查框的状态为status中指定的值               |
| LB_SETITEMBOLD     | 指定索引处条目的检查框的状态为加粗显示状态,根据lParam的值判断是否进行加粗设置,如果为1则加粗显示,为0则正常显示 |
| LB_GETITEMDISABLE  | 获取指定索引处条目的检查框是否处于禁止选中状态，返回1表示该条目的检查框处于禁止选中状态，0表示该条目的检查框处于可选择状态 |
| LB_SETITEMDISABLE  | 设置指定索引处条目的检查框的状态为禁止选中状态，根据lParam的值设置，为1则设置为禁止选中状态,为0则为可选择状态 |
| LB_MULTIADDITEM    | 用于向列表框一次添加多个条目                                 |
| LB_SETSTRCMPFUNC   | 重载默认的排序方式                                           |
| LB_SETITEMADDDATA  | 设置附加的32位数据                                           |
| LB_GETITEMADDDATA  | 获取附加的32位数据                                           |
| LB_SETITEMHEIGHT   | 设定条目所占的高度                                           |
| LB_GETITEMHEIGHT   | 返回条目所占的高度                                           |

### 7.3.5 编辑框消息

| 消息                  | 含义                                                         |
| --------------------- | ------------------------------------------------------------ |
| MSG_GETTEXTLENGTH     | 获取文本的长度,以字节为单位                                  |
| MSG_GETTEXT           | 获取编辑框中的文本                                           |
| MSG_SETTEXT           | 设置编辑框中的文本内容                                       |
| EM_GETCARETPOS        | 获得当前的插入符位置                                         |
| EM_SETCARETPOS        | 设置插入符的位置                                             |
| EM_GETSEL             | 获取当前被选中的文本                                         |
| EM_SETSEL             | 设置当前被选中的文本                                         |
| EM_GETSELPOS          | 获取当前的选择点位置                                         |
| EM_GETSELPOS          | 用法和EM_GETCARETPOS消息类似                                 |
| EM_SELECTALL          | 使编辑框所有的文本都被选中                                   |
| EM_COPYTOCB           | 把编辑框控件当前选中的文本复制到剪贴板                       |
| EM_CUTTOCB            | 把剪贴板的文本内容剪切到编辑框                               |
| EM_INSERTCBTEXT       | 把剪贴板的文本内容复制到编辑框                               |
| EM_GETLINEHEIGHT      | 获取行的高度                                                 |
| EM_SETLINEHEIGHT      | 设置行的高度                                                 |
| EM_GETLINECOUNT       | 获取行的数量                                                 |
| EM_LIMITTEXT          | 设置编辑框控件的文本上限,以字节为单位                        |
| EM_SETREADONLY        | 在wParam参数传递TRUE,将使编辑框置于只读状态,而wParam参数传递FALSE,将使编辑框置于正常编辑状态 |
| EM_SETPASSWORDCHAR    | 修改密码字符                                                 |
| EM_GETPASSWORDCHAR    | 获得当前的密码字符                                           |
| EM_SETTIPTEXT         | 设置编辑框的提示文字                                         |
| EM_GETTIPTEXT         | 获取编辑框的提示文字                                         |
| EM_SETTITLETEXT       | 设置编辑框的标题文字                                         |
| EM_GETTITLETEXT       | 获取编辑框的标题文字                                         |
| EM_SETLFDISPCHAR      | 设置了用于行结束符的显示符号,编辑框将把行结束符显示为所设的显示符号 |
| EM_SETLINESEP         | 改变编辑框使用的换行符号                                     |
| EM_GETNUMOFPARAGRAPHS | 获取文本的段落数目                                           |
| EM_GETPARAGRAPHLENGTH | 获取某个特定段落的长度,获取成功返回指定段落的长度,失败返回-1 |
| EM_GETPARAGRAPHTEXT   | 获取特定段落的文本                                           |

### 7.3.6 组合框消息

| 消息                     | 含义                                                         |
| ------------------------ | ------------------------------------------------------------ |
| CB_ADDSTRING             | 用来向内部列表框中添加条目                                   |
| CB_INSERTSTRING          | 用来向内部列表框中插入条目                                   |
| CB_DELETESTRING          | 用来从内部列表框中删除条目                                   |
| CB_FINDSTRING            | 用于模糊匹配列表框中的条目                                   |
| CB_FINDSTRINGEXACT       | 用于精确匹配列表框中的条目                                   |
| CB_GETCOUNT              | 用于获取内部列表框中的条目个数                               |
| CB_GETCURSEL             | 用于获取内部列表框的当前选中项                               |
| CB_SETCURSEL             | 用于设置内部列表框的选中项                                   |
| CB_RESETCONTENT          | 用于清空内部列表框                                           |
| CB_GETITEMADDDATA        | 用于获取内部列表框条目的附加数据                             |
| CB_SETITEMADDDATA        | 用于设置内部列表框条目的附加数据                             |
| CB_GETITEMHEIGHT         | 用于获取内部列表框条目的高度                                 |
| CB_SETITEMHEIGHT         | 用于设置内部列表框条目的高度                                 |
| CB_SETSTRCMPFUNC         | 用于设置内部列表框排序用的字符串对比函数                     |
| CB_GETLBTEXT             | 用于获取内部列表框条目的文本内容                             |
| CB_GETLBTEXTLEN          | 用于获得内部列表框条目的文本长度                             |
| CB_GETCHILDREN           | 获得组合框的子控件,wParam返回编辑框控件指针,lParam返回列表框控件指针 |
| CB_LIMITTEXT             | 用于限制内部编辑框的文本长度                                 |
| CB_SETEDITSEL            | 用来设置编辑框选中的文本                                     |
| CB_GETEDITSEL            | 用来获取编辑框选中的文本                                     |
| CB_SPIN                  | 发送该消息将使旋钮框向前或向后步进,相当于用户单击编辑框旁边的向上或向下箭头(在编辑框中键入向上或向下箭头键,也可取得一样的效果)。wParam控制步进方向,取0为向下,取1为向上 |
| CB_FASTSPIN              | 发送该消息将使旋钮框快速向前步进,相当于用户在编辑框中键入PageUp/PageDown键。wParam控制步进方向,取0为向上,取1为向下 |
| CB_GETDROPPEDCONTROLRECT | 获得组合框的下拉列表对应矩形位置                             |
| CB_GETDROPPEDSTATE       | 检查组合框的下拉列表是否为显示状态                           |
| CB_GETSPINRANGE          | 获得可取的最大值和最小值,它们分别存储在wParam参数和lParam参数指向的地址中 |
| CB_SETSPINRANGE          | 设定可取的最大值和最小值,分别取wParam参数和lParam参数的值    |
| CB_SETSPINVALUE          | 参数设置编辑框的当前数值,通过wParam参数传递要设置的值        |
| CB_GETSPINVALUE          | 该消息返回当前编辑框内的数值                                 |
| CB_SPIN                  | 发送该消息将使旋钮框向前或向后步进,相当于用户单击编辑框旁边的向上或向下箭头(在编辑框中键入向上或向下箭头键,也可取得一样的效果)。wParam控制步进方向,取1为向上,取0为向下。步进值取决于CB_SETSPINPACE的设置值 |
| CB_FASTSPIN              | 发送该消息将使旋钮框快速向前步进,相当于用户在编辑框中键入PageUp/PageDown键。wParam控制步进方向,取0为向上,取1为向下。步进值取决于CB_SETSPINPACE的设置值 |
| CB_GETSPINPACE           | 获得步进值(wParam)和快速步进值(lParam)                       |
| CB_SETSPINPACE           | 设置步进值(wParam)和快速步进值(lParam)                       |
| CB_SETSPINFORMAT         | 设定整数的格式化字符串。MiniGUI 在内部使用sprintf和sscanf函数在编辑框的文本字符串和整数值之间互相转换。设定格式化字符串之后,MiniGUI在调用sprintf和sscanf函数时将使用这个格式化字符串,使之具有特定的显示格式 |

### 7.3.7 菜单按钮消息

| 消息              | 含义                                                    |
| ----------------- | ------------------------------------------------------- |
| MBM_ADDITEM       | 向菜单按钮添加条目，当内存空间不足时,则返回MB_ERR_SPACE |
| MBM_DELITEM       | 从菜单按钮中删除条目                                    |
| MBM_RESETCTRL     | 删除所有条目                                            |
| MBM_SETCURITEM    | 设置选中条目,被选中的条目文本将显示在菜单按钮上         |
| MBM_GETCURITEM    | 获得当前选中条目的索引号                                |
| MBM_GETITEMDATA   | 获取菜单项条目的数据                                    |
| MBM_SETITEMDATA   | 设置菜单项条目的数据                                    |
| MBM_SETSTRCMPFUNC | 设定一个定制的排序函数                                  |

### 7.3.8 进度条消息

| 消息         | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| PBM_SETRANGE | 设置进度条的范围，默认情况下的进度条范围是0到100             |
| PBM_SETSTEP  | 设置步进长度，默认的进度条步进值是10，当进度条的步进值为负值时,需要设置进度条的位置为进度条范围的最大值 |
| PBM_SETPOS   | 设置进度条位置                                               |
| PBM_DELTAPOS | 在当前进度基础上偏移                                         |
| PBM_STEPIT   | 使进度条前进一个步进值                                       |

### 7.3.9 滑块消息

| 消息            | 含义                                                         |
| --------------- | ------------------------------------------------------------ |
| TBM_SETRANGE    | 通过wParam和lParam参数分别设置滑块的最小值和最大值。默认的范围是0~10 |
| TBM_GETMIN      | 获得滑块的最小值                                             |
| TBM_GETMAX      | 获得滑块的最大值                                             |
| TBM_SETMIN      | 设置滑块的最小值                                             |
| TBM_SETMAX      | 设置滑块的最大值                                             |
| TBM_SETLINESIZE | 通过wParam参数设置滑块的步进值。当用户在滑块拥有输入焦点时按下向上或向下光标键,将使滑块向上或向下移动该步进值。默认的步进值是1 |
| TBM_GETLINESIZE | 获得滑块的步进值                                             |
| TBM_SETPAGESIZE | 通过wParam参数设置滑块的快速步进值。当用户在滑块拥有输入焦点时按下PageUp和PageDown键,将使滑块分别向上或向下移动该快速步进值。默认的快速步进值是5 |
| TBM_GETPAGESIZE | 获得滑块的快速步进值                                         |
| TBM_SETPOS      | 设置滑块的位置                                               |
| TBM_GETPOS      | 获得滑块的位置                                               |
| TBM_SETTICKFREQ | 设置刻度间距,默认间距是1                                     |
| TBM_GETTICKFREQ | 获得刻度间距                                                 |
| TBM_SETTIP      | 设置最小值及最大值处的文字说明                               |
| TBM_GETTIP      | 获取最小值及最大值处的文字说明                               |

### 7.3.10 工具栏消息

| 消息            | 含义                                   |
| --------------- | -------------------------------------- |
| NTBM_ADDITEM    | 向工具栏中添加一个工具项               |
| NTBM_GETITEM    | 获取具有指定标识符的工具项信息         |
| NTBM_SETITEM    | 设置具有指定标识符的工具项信息         |
| NTBM_ENABLEITEM | 使能或者禁止某个具有指定标识符的工具项 |
| NTBM_SETBITMAP  | 改变工具栏上的按钮位图                 |

### 7.3.11 属性表消息

| 消息               | 含义                                                         |
| ------------------ | ------------------------------------------------------------ |
| PSM_ADDPAGE        | 向属性表中添加属性页                                         |
| MSG_INITPAGE       | 类似对话框的MSG_INITDIALOG消息                               |
| MSG_SHOWPAGE       | 在属性页被隐藏和重新显示时发送到属性页过程中                 |
| PSM_SHEETCMD       | 属性表控件将向其拥有的所有属性页广播MSG_SHEETCMD消息         |
| PSM_REMOVEPAGE     | 删除某个属性页                                               |
| PSM_GETPAGE        | 获得具有某个索引值的属性页的窗口句柄                         |
| PSM_GETPAGEINDEX   | 根据属性页句柄返回属性页索引值                               |
| PSM_GETPAGECOUNT   | 返回属性页总个数                                             |
| PSM_GETTITLELENGTH | 根据wParam参数传入的属性页索引值获得该属性页标题的长度,类似窗口的MSG_GETTEXTLENGTH消息 |
| PSM_GETTITLE       | 根据wParam参数传入的属性页索引值获得该属性页标题,并保存在lParam参数传递的缓冲区中,类似窗口的 MSG_GETTEXT消息 |
| PSM_SETTITLE       | 根据lParam参数传入的文本字符串设置由wParam指定的属性页标题,类似窗口的MSG_SETTEXT消息 |
| PSM_GETACTIVEPAGE  | 返回活动属性页的窗口句柄                                     |
| PSM_GETACTIVEINDEX | 返回活动属性页的索引值                                       |
| PSM_SETACTIVEINDEX | 根据wParam传入的属性页索引值设置活动属性页                   |

### 7.3.12 滚动窗口消息

| 消息                  | 含义                                                         |
| --------------------- | ------------------------------------------------------------ |
| SVM_SETCONTRANGE      | 设置滚动窗口的内容区域的大小                                 |
| SVM_SETCONTWIDTH      | 设置滚动窗口的宽度                                           |
| SVM_SETCONTHEIGHT     | 设置滚动窗口的高度                                           |
| SVM_GETCONTWIDTH      | 获取内容区域的宽度                                           |
| SVM_GETCONTHEIGHT     | 获取内容区域的高度                                           |
| SVM_GETVISIBLEWIDTH   | 获取内容区域的可视区域宽度                                   |
| SVM_GETVISIBLEHEIGHT  | 获取内容区域的可视区域高度                                   |
| SVM_GETCONTENTX       | 获取内容区域的当前位置X值                                    |
| SVM_GETCONTENTY       | 获取内容区域的当前位置Y值                                    |
| SVM_SETCONTPOS        | 设置内容区域的当前位置值,也就是在可视区域中移动内容区域到某个指定位置 |
| SVM_MAKEPOSVISIBLE    | 使内容区域中的某个位置点成为可见                             |
| SVM_GETHSCROLLVAL     | 获取滚动窗口的当前水平滚动值                                 |
| SVM_GETVSCROLLVAL     | 获取滚动窗口的当前垂直滚动值                                 |
| SVM_GETHSCROLLPAGEVAL | 获取滚动窗口的当前水平页滚动值                               |
| SVM_GETVSCROLLPAGEVAL | 获取滚动窗口的当前垂直页滚动值                               |
| SVM_SETSCROLLVAL      | 设置滚动窗口的水平和垂直滚动值                               |
| SVM_SETSCROLLPAGEVAL  | 设置滚动窗口的水平和垂直页滚动值                             |
| SVM_ADDCTRLS          | 往其中添加子控件                                             |
| SVM_RESETCONTENT      | 重置滚动窗口控件,包括清空其中的子控件和设置内容区域的范围和位置值为默认值 |
| SVM_GETCTRL           | 获取滚动窗口控件中的子控件的句柄                             |
| SVM_GETFOCUSCHILD     | 获取滚动窗口控件中具有键盘焦点的子控件                       |
| SVM_SETCONTAINERPROC  | 设置新的容器窗口过程,从而达到定制滚动窗口的目的              |

### 7.3.13 滚动型控件消息

| 消息                | 含义                                                         |
| ------------------- | ------------------------------------------------------------ |
| SVM_SETITEMDRAW     | 设置列表项的绘制函数                                         |
| SVM_SETITEMOPS      | 设置列表项相关操作的一些回调函数,包括初始化、绘制和结束函数  |
| SVM_ADDITEM         | 添加一个列表项                                               |
| SVM_DELITEM         | 删除一个列表项                                               |
| SVM_REFRESHITEM     | 刷新一个列表项区域                                           |
| SVM_GETITEMADDDATA  | 获取列表项的附加数据                                         |
| SVM_SETITEMADDDATA  | 设置列表项的附加数据                                         |
| SVM_GETITEMCOUNT    | 获取当前列表项的数量                                         |
| SVM_RESETCONTENT    | 删除掉控件中所有的列表项                                     |
| SVM_SETCURSEL       | 设置控件的高亮列表项                                         |
| SVM_GETCURSEL       | 获取控件的当前高亮列表项                                     |
| SVM_SELECTITEM      | 设置列表项的选中状态                                         |
| SVM_SHOWITEM        | 显示一个列表项                                               |
| SVM_CHOOSEITEM      | 是SVM_SELECTITEM和SVM_SHOWITEM消息的组合,用来选中一个列表项并使之可见 |
| MSG_FREEZE          | 在操作之前冻结控件,操作之后解冻                              |
| SVM_SETMARGINS      | 对滚动型控件的边缘范围进行设置                               |
| SVM_GETMARGINS      | 获取滚动型控件的边缘范围值                                   |
| SVM_GETLEFTMARGIN   | 获取左边缘值                                                 |
| SVM_GETTOPMARGIN    | 获取上边缘值                                                 |
| SVM_GETRIGHTMARGIN  | 获取右边缘值                                                 |
| SVM_GETBOTTOMMARGIN | 获取下边缘值                                                 |

### 7.3.14 树型控件消息

| 消息               | 含义                                     |
| ------------------ | ---------------------------------------- |
| TVM_ADDITEM        | 插入一个节点项                           |
| TVM_DELTREE        | 删除一个节点及其所有子项(包括子项的子项) |
| TVM_GETITEMINFO    | 获取某个节点项的属性信息                 |
| TVM_SETITEMINFO    | 设置某个节点项的属性                     |
| TVM_GETITEMTEXT    | 获取某个节点项的文字标题                 |
| TVM_GETITEMTEXTLEN | 获取节点项的文字标题的长度               |
| TVM_SETSELITEM     | 选择某个节点项                           |
| TVM_GETSELITEM     | 获取当前被选择的节点项                   |
| TVM_GETROOT        | 获取树型控件的根节点                     |
| TVM_GETRELATEDITEM | 获取指定节点的相关节点项                 |
| TVIR_PARENT        | 获取item节点的父节点                     |
| TVIR_FIRSTCHILD    | 获取item节点的第一个子节点               |
| TVIR_NEXTSIBLING   | 获取item节点的下一个兄弟节点             |
| TVIR_PREVSIBLING   | 获取item节点的前一个兄弟节点             |
| TVM_SEARCHITEM     | 查找某个特定的节点项，整个节点树中查找   |
| TVM_FINDCHILD      | 查找节点项的特定子节点，只在子节点中查找 |
| TVM_SETSTRCMPFUNC  | 设置新的树型控件字符串比较函数           |

### 7.3.15 列表型控件消息

| 消息                | 含义                                                         |
| ------------------- | ------------------------------------------------------------ |
| LVM_ADDCOLUMN       | 添加一列或依次添加多列                                       |
| LVM_GETCOLUMN       | 获取列表型控件中某一列的属性                                 |
| LVM_SETCOLUMN       | 设置和修改列的各项属性                                       |
| LVM_MODIFYHEAD      | 是LVM_SETCOLUMN的简化,可以用来设置列表头的标题               |
| LVM_GETCOLUMNWIDTH  | 获取某列的宽度                                               |
| LVM_GETCOLUMNCOUNT  | 获取列表型控件中列的数量                                     |
| LVM_DELCOLUMN       | 删除列表型控件中的一列                                       |
| LVM_SETHEADHEIGHT   | 设置列表头的高度                                             |
| LVM_ADDITEM         | 往列表型控件中添加列表项                                     |
| LVM_FILLSUBITEM     | 设置列表项中各个子项的内容                                   |
| LVM_SETSUBITEM      | 设置列表项中各个子项的内容                                   |
| LVM_GETITEM         | 获取一个列表项的信息                                         |
| LVM_GETITEMCOUNT    | 获取列表型控件的列表项数量                                   |
| LVM_GETITEMADDDATA  | 获取列表项的附加数据                                         |
| LVM_SETITEMADDDATA  | 设置列表项的附加数据                                         |
| LVM_SETITEMHEIGHT   | 设置一个列表型控件的列表项高度，如果不设置的话,列表型控件的列表项高度将采用缺省值 |
| LVM_DELITEM         | 在列表型控件中删除一个列表项                                 |
| LVM_DELALLITEM      | 删除所有的列表项                                             |
| LVM_GETSUBITEMTEXT  | 获取子项的文字内容                                           |
| LVM_SETSUBITEMTEXT  | 设置子项的文字内容                                           |
| LVM_GETSUBITEMLEN   | 获取子项字符串的长度                                         |
| LVM_SETSUBITEMCOLOR | 设置子项的文字颜色                                           |
| LVM_SELECTITEM      | 选择一个列表项,被选中的项将高亮显示                          |
| LVM_GETSELECTEDITEM | 用来确定当前被选中的列表项                                   |
| LVM_SHOWITEM        | 使一个列表项在列表型控件中成为可见的条目                     |
| LVM_CHOOSEITEM      | 是LVM_SELECTIEM和LVM_SHOWITEM功能的组合,它使一个列表项被选中而且成为可见的项 |
| LVM_FINDITEM        | 在列表型控件中查找一个特定的列表项                           |
| LVM_SETCOLUMN       | 设置新的比较函数                                             |
| LVM_SORTIEMS        | 使之对列表项进行排序                                         |
| LVM_COLSORT         | 使列表型控件依据某列来进行比较排序                           |
| LVM_SETSTRCMPFUNC   | 设置自定义的字符串比较函数                                   |
| LVM_GETRELATEDITEM  | 获取一个节点的相关树型节点                                   |
| LVM_GETRELATEDITEM  | 返回所获取到的相关节点的句柄                                 |
| LVM_FOLDITEM        | 折叠或者展开一个包含子节点的节点项                           |

### 7.3.16 月历控件消息

| 消息                | 含义                                 |
| ------------------- | ------------------------------------ |
| MCM_GETCURDAY       | 获取当前选择的日期中是当月中的第几天 |
| MCM_GETCURMONTH     | 获取当前选择的日期中的月份值         |
| MCM_GETCURYEAR      | 获取当前选择日期中的年份             |
| MCM_GETFIRSTWEEKDAY | 确定当前月份中的第一天是星期几       |
| MCM_GETCURDATE      | 获取月历控件中当前选择的日期         |
| MCM_GETTODAY        | 获取“今天”的日期                     |
| MCM_SETCURDAY       | 设置当前选择的“天”                   |
| MCM_SETCURMONTH     | 设置当前的月                         |
| MCM_SETCURYEAR      | 设置当前的年                         |
| MCM_SETCURDATE      | 设置当前选择的日期                   |
| MCM_SETTODAY        | 把“今天”设为当前选择的日期           |
| MCM_GETCOLOR        | 获取月历控件中各部分的颜色设置       |
| MCM_SETCOLOR        | 改变月历控件中各部分的颜色设置       |
| MCM_GETMINREQRECTW  | 获取最小宽度值                       |
| MCM_GETMINREQRECTH  | 获取最小高度值                       |

### 7.3.17 旋钮控件消息

| 消息            | 含义                                                         |
| --------------- | ------------------------------------------------------------ |
| SPM_SETINFO     | 设置控件的属性和状态                                         |
| SPM_GETINFO     | 获取旋钮控件的属性                                           |
| SPM_SETCUR      | 设置旋钮控件的当前位置值                                     |
| SPM_GETCUR      | 获取当前的位置值                                             |
| SPM_DISABLEDOWN | 禁止下箭头的滚动能力，仅对没有SPS_AUTOSCROLL风格的旋钮控件有效 |
| SPM_ENABLEDOWN  | 恢复下箭头的滚动能力，仅对没有SPS_AUTOSCROLL风格的旋钮控件有效 |
| SPM_DISABLEUP   | 禁止上箭头的滚动能力，仅对没有SPS_AUTOSCROLL风格的旋钮控件有效 |
| SPM_ENABLEUP    | 恢复上箭头的滚动能力，仅对没有SPS_AUTOSCROLL风格的旋钮控件有效 |
| SPM_SETTARGET   | 设置旋钮控件的目标窗口                                       |
| SPM_GETTARGET   | 获取旋钮控件的目标窗口                                       |

### 7.3.18 网格控件消息

| 消息                  | 含义                                         |
| --------------------- | -------------------------------------------- |
| GRIDM_ADDCOLUMN       | 往控件中增加列                               |
| GRIDM_ADDCOLUMN       | 增加网格控件的一列                           |
| GRIDM_DELCOLUMN       | 删除网格控件中的一列                         |
| GRIDM_GETCOLWIDTH     | 获取控件列的宽度                             |
| GRIDM_GETCOLCOUNT     | 获取网格控件中列的数量                       |
| GRIDM_SETROWHEIGHT    | 设置控件行的高度                             |
| GRIDM_GETROWHEIGHT    | 获取控件行的高度                             |
| GRIDM_ADDROW          | 往网格控件中添加一行                         |
| GRIDM_DELROW          | 删除网格控件中的一行                         |
| GRIDM_GETROWCOUNT     | 获取网格控件中行的数量                       |
| GRIDM_SETCELLPROPERTY | 设置一个或多个单元格                         |
| GRIDM_GETCELLPROPERTY | 获得单元格的属性                             |
| GRIDM_SETNUMFORMAT    | 设置数字单元格(GRIDCELLDATANUMBER)的数字格式 |
| GRIDM_SETSELECTED     | 设置高亮的单元格                             |
| GRIDM_GETSELECTED     | 得到所有高亮的单元格                         |
| GRIDM_ADDDEPENDENCE   | 往网格控件中添加一个单元格关联               |
| GRIDM_DELDEPENDENCE   | 删除网格控件中已有的一个单元格关联           |

### 7.3.18 酷工具栏消息

| 消息        | 含义                 |
| ----------- | -------------------- |
| CBM_ADDITEM | 往工具栏中添加按钮项 |
| CBM_ENABLE  | 禁止或恢复某个按钮项 |

### 7.3.19 动画控件消息

| 消息             | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| ANM_SETANIMATION | 设置ANIMATION对象                                            |
| ANM_GETANIMATION | 获取当前的ANIMATION对象                                      |
| ANM_STARTPLAY    | 开始播放。在发送ANM_STARTPLAY消息给动画控件之前,动画控件将仅仅显示ANIMATION对象的第一帧图象;只有发送了ANM_STARTPLAY消息之后,动画控件才会按ANIMATION对象中的信息播放动画 |
| ANM_PAUSE_RESUME | 暂停/继续播放。用来暂停动画的播放(正在播放时),或者用来继续动画的播放(已被暂停时) |
| ANM_STOPPLAY     | 停止动画的播放。动画控件将返回到ANIMATION的第一帧图象        |

### 7.3.20 图标型控件消息

| 消息                    | 含义                                                         |
| ----------------------- | ------------------------------------------------------------ |
| IVM_ADDITEM             | 往该控件中添加图标项                                         |
| IVM_RESETCONTENT        | 对应SVM_RESETCONTENT,用于清空图标型控件中的图标项            |
| IVM_DELITEM             | 对应SVM_DELITEM,用于删除图标型控件中的图标项                 |
| IVM_SETITEMDRAW         | 对应SVM_SETITEMDRAW,用于设置图标项的绘制函数                 |
| IVM_SETCONTWIDTH        | 对应SVM_SETCONTWIDTH,用于设置滚动窗口的宽度                  |
| IVM_SETCONTHEIGHT       | 对应SVM_SETCONTHEIGHT,用于设置滚动窗口的高度                 |
| IVM_SETITEMOPS          | 对应SVM_SETITEMOPS,用于设置图标项相关操作的一些回调函数      |
| IVM_GETMARGINS          | 对应SVM_GETMARGINS,用于获取图标型控件的边缘范围值            |
| IVM_SETMARGINS          | 对应SVM_SETMARGINS,用于设置图标型控件的边缘范围值            |
| IVM_GETLEFTMARGIN       | 对应SVM_GETLEFTMARGIN,用于获取图标型控件中的左边缘值         |
| IVM_GETTOPMARGIN        | 对应SVM_GETTOPMARGIN,用于获取图标型控件中的上边缘值          |
| IVM_GETRIGHTMARGIN      | 对应SVM_GETRIGHTMARGIN,用于获取图标型控件中的右边缘值        |
| IVM_GETBOTTOMMARGIN     | 对应SVM_GETBOTTOMMARGIN,用于获取图标型控件中的下边缘值       |
| IVM_GETCONTWIDTH        | 对应SVM_GETCONTWIDTH,用来获取内容区域的宽度                  |
| IVM_GETCONTHEIGHT       | 对应SVM_GETCONTHEIGHT,用来获取内容区域的高度                 |
| IVM_GETVISIBLEWIDTH     | 对应SVM_GETVISIBLEWIDTH,用来获取可视区域的宽度               |
| IVM_GETVISIBLEHEIGHT    | 对应SVM_GETVISIBLEHEIGHT,用来获取可视区域的高度              |
| IVM_SETCONTRANGE        | 对应SVM_SETCONTRANGE,用于设置滚动窗口的内容区域的大小        |
| IVM_GETCONTENTX         | 对应SVM_GETCONTENTX,用于获取内容区域的当前位置值             |
| IVM_GETCONTENTY         | 对应SVM_GETCONTENTY,用于获取内容区域的当前位置值             |
| IVM_SETCONTPOS          | 对应SVM_SETCONTPOS,用于设置内容区域的当前位置值,也就是在可视区域中移动内容区域到某个指定位置 |
| IVM_GETCURSEL           | 对应SVM_GETCURSEL,用于获取控件的当前高亮图标项               |
| IVM_SETCURSEL           | 对应SVM_SETCURSEL,用于设置控件的当前高亮图标项               |
| IVM_SELECTITEM          | 对应SVM_SELECTITEM,用于选择一个列表项,被选中的项将高亮显示   |
| IVM_SHOWITEM            | 对应SVM_SHOWITEM,用于显示一个图标项                          |
| IVM_CHOOSEITEM          | 对应SVM_CHOOSEITEM,是IVM_SELECTITEM和IVM_SHOWITEM消息的组合,用来选中一个图标项并使之可见 |
| IVM_SETITEMINIT         | 对应SVM_SETITEMINIT,用于设置图标项的初始操作                 |
| IVM_SETITEMDESTROY      | 对应SVM_SETITEMDESTROY,用于设置图标项的销毁操作              |
| IVM_SETITEMCMP          | 对应SVM_SETITEMCMP,用于设置图标型控件图标项的比较函数        |
| IVM_MAKEPOSVISIBLE      | 对应VM_MAKEPOSVISIBLE,用于使内容区域中的某个位置点成为可见   |
| IVM_GETHSCROLLVAL       | 对应SVM_GETHSCROLLVAL,用来获取滚动窗口的当前水平滚动值(点击滚动条箭头的滚动范围大小) |
| IVM_GETVSCROLLVAL       | 对应SVM_GETVSCROLLVAL,用来获取滚动窗口的当前垂直滚动值(点击滚动条箭头的滚动范围大小) |
| IVM_GETHSCROLLPAGEVAL   | 对应SVM_GETHSCROLLPAGEVAL,用来获取滚动窗口的当前水平页滚动值(翻页操作时的滚动范围大小) |
| IVM_GETVSCROLLPAGEVAL   | 对应SVM_GETVSCROLLPAGEVAL,用来获取滚动窗口的当前垂直页滚动值(翻页操作时的滚动范围大小) |
| IVM_SETSCROLLVAL        | 对应SVM_SETSCROLLVAL,用于设置滚动窗口的水平和(或者)垂直滚动值 |
| IVM_SETSCROLLPAGEVAL    | 对应SVM_SETSCROLLPAGEVAL,用于设置滚动窗口的水平和(或者)垂直页滚动值 |
| IVM_SORTITEMS           | 对应SVM_SORTITEMS,用于对图标项进行一次性的排序               |
| IVM_GETITEMCOUNT        | 对应SVM_GETITEMCOUNT,用于获取当前图标项的数量                |
| IVM_GETITEMADDDATA      | 对应SVM_GETITEMADDDATA,用于获取当前图标项的附加信息          |
| IVM_SETITEMADDDATA      | 对应SVM_SETITEMADDDATA,用于设置当前图标项的附加信息          |
| IVM_REFRESHITEM         | 对应SVM_REFRESHITEM,用于刷新一个图标项区域                   |
| IVM_GETFIRSTVISIBLEITEM | 对应SVM_GETFIRSTVISIBLEITEM,用于获取第一个可见的图标项       |

### 7.3.21 独立滚动条消息

| 消息               | 含义                           |
| ------------------ | ------------------------------ |
| SBM_GETSCROLLINFO  | 获取滚动条控件的最大值、最小值 |
| SBM_SETSCROLLINFO  | 进行滚动条信息设置             |
| SBM_GETPOS         | 获取当前游标的位置             |
| SBM_SETPOS         | 设置当前游标的位置             |
| SBM_GETRANGE       | 获取滚动条滚动范围             |
| SBM_SETRANGE       | 设置滚动条滚动范围             |
| SBM_SETRANGEREDRAW | 设置滚动条滚动范围,并立即重绘  |
| SBM_ENABLE_ARROW   | 启用或者禁用滚动条箭头         |

# 8  通知码

## 8.1 滚动条通知码

| 标识符           | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| SB_LINEUP        | 鼠标点击竖直按钮的上箭头1次                                  |
| SB_LINEDOWN      | 鼠标点击竖直滚动条的下箭头1次                                |
| SB_LINELEFT      | 鼠标点击水平滚动条的左箭头1次                                |
| SB_LINERIGHT     | 鼠标点击水平滚动条的右箭头1次                                |
| SB_PAGEUP        | 鼠标点击竖直按钮的上箭头与滑块之间的区域1次                  |
| SB_PAGEDOWN      | 鼠标点击竖直按钮的下箭头与滑块之间的区域1次                  |
| SB_PAGELEFT      | 鼠标点击水平按钮的左箭头与滑块之间的区域1次                  |
| SB_PAGERIGHT     | 鼠标点击水平按钮的右箭头与滑块之间的区域1次                  |
| SB_THUMBTRACK    | 鼠标拖动滑块移动时窗口不断地收到的消息,注意,该值表示对鼠标位置经换算后对应的数值(通过lParam传递的),该值有可能大于设置的最大值,也有可能<小于设置的最小值,在程序中需要作相应的判断 |
| SB_THUMBPOSITION | 用户拖动滑块结束                                             |

## 8.2 静态框通知码

| 标识符      | 含义                         |
| ----------- | ---------------------------- |
| STN_DBLCLK  | 用户在静态框内双击了鼠标左键 |
| STN_CLICKED | 用户在静态框内单击了鼠标左键 |

## 8.3 按钮通知码

| 标识符       | 含义                                       |
| ------------ | ------------------------------------------ |
| BN_CLICKED   | 表明用户单击此按钮                         |
| BN_PUSHED    | 表明用户将此按钮按下                       |
| BN_UNPUSHED  | 表明用户将此按钮释放                       |
| BN_DBLCLK    | 表明用户在此按钮上进行了鼠标左键的双击操作 |
| BN_SETFOCUS  | 表明按钮获得了输入焦点                     |
| BN_KILLFOCUS | 表明按钮失去了输入焦点                     |

## 8.4 列表通知码

| 标识符             | 含义                               |
| ------------------ | ---------------------------------- |
| LBN_ERRSPACE       | 内存分配失败                       |
| LBN_SELCHANGE      | 单项选择列表框的当前选择项发生变化 |
| LBN_CLICKED        | 用户在列表框某条目上单击了鼠标左键 |
| LBN_DBLCLK         | 用户在列表框某条目上双击了鼠标左键 |
| LBN_SELCANCEL      | 用户取消了某个条目的选择           |
| LBN_SETFOCUS       | 列表框获得了输入焦点               |
| LBN_KILLFOCUS      | 列表框失去了输入焦点               |
| LBN_CLICKCHECKMARK | 用户单击了条目的检查框             |
| LBN_ENTER          | 用户在列表框中按下ENTER键          |

## 8.5 编辑框通知码

| 标识符       | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| EN_SETFOCUS  | 编辑控件已经获得输入焦点                                     |
| EN_KILLFOCUS | 编辑控件已经失去输入焦点                                     |
| EN_CHANGE    | 编辑控件的内容已经改变                                       |
| EN_UPDATE    | 编辑控件在接收到MSG_SETTEXT,EM_RESETCONTENT或EM_SETLINEHEIGHT消息后,内容已经改变 |
| EN_ENTER     | 用户在编辑框中按下了Enter键                                  |
| EN_MAXTEXT   | 编辑控件在插入时超出了限定长度                               |
| EN_DBLCLK    | 编辑控件被鼠标左键双击                                       |
| EN_CLICKED   | 编辑控件被鼠标左键点击                                       |

## 8.6 组合框通知码

| 标识符           | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| CBN_ERRSPACE     | 内存不足                                                     |
| CBN_SELCHANGE    | 条目选择变化                                                 |
| CBN_EDITCHANGE   | 方框区域的文本发生了变化                                     |
| CBN_DBLCLK       | 用户双击了组合框中的某个条目                                 |
| CBN_CLICKED      | 用户点击了组合框                                             |
| CBN_SETFOCUS     | 组合框获得了输入焦点。如果组合框具有CBS_AUTOFOCUS风格,则内部编辑框将同时获得输入焦点 |
| CBN_KILLFOCUS    | 组合框失去了输入焦点                                         |
| CBN_DROPDOWN     | 用户下拉列表框使之显示。当用户点击编辑框旁边的向下箭头按钮或者在编辑框中键入光标控制键,比如向下、向上箭头键,PageDown或者PageUp等键时,也会下拉并显示列表框 |
| CBN_CLOSEUP      | 下拉的列表框被隐藏(关闭)                                     |
| CBN_SELENDOK     | 用户从下拉列表框中选择了某个条目                             |
| CBN_SELENDCANCEL | 用户未选择任何条目而关闭下拉列表框                           |

## 8.7 菜单按钮通知码

| 标识符        | 含义                                                         |
| ------------- | ------------------------------------------------------------ |
| MBN_ERRSPACE  | 内存分配失败,存储空间不足                                    |
| MBN_SELECTED  | 对菜单按钮控件进行了选择。不管前后选择的菜单项是否改变,均会产生该通知消息 |
| MBN_CHANGED   | 菜单按钮控件的选择项发生了变化                               |
| MBN_STARTMENU | 用户激活了菜单按钮的弹出式菜单                               |
| MBN_ENDMENU   | 弹出式菜单关闭                                               |

## 8.8 进度条通知码

| 标识符       | 含义               |
| ------------ | ------------------ |
| PBN_REACHMAX | 已到达最大进度位置 |
| PBN_REACHMIN | 已到达最小进度位置 |

## 8.9 滑块通知码

| 标识符       | 含义                 |
| ------------ | -------------------- |
| TBN_CHANGE   | 滑块的位置发生了变化 |
| TBN_REACHMAX | 已到达了上限         |
| TBN_REACHMIN | 已到达了下限         |

## 8.10 属性表通知码

| 标识符             | 含义                                                        |
| ------------------ | ----------------------------------------------------------- |
| PSN_ACTIVE_CHANGED | 当属性表中的活动属性页发生变化时,属性表控件将产生该通知消息 |

## 8.11 滚动型控件通知码

| 标识符          | 含义                     |
| --------------- | ------------------------ |
| SVN_SELCHANGED  | 当前高亮列表项发生改变   |
| SVN_CLICKED     | 用户点击列表项           |
| SVN_SELCHANGING | 当前高亮列表项正发生改变 |

## 8.12 树型控件通知码

| 标识符        | 含义                     |
| ------------- | ------------------------ |
| TVN_SELCHANGE | 当前选择的节点项发生改变 |
| TVN_DBLCLK    | 用户双击节点项           |
| TVN_SETFOCUS  | 树型控件获得焦点         |
| TVN_KILLFOCUS | 树型控件失去焦点         |
| TVN_CLICKED   | 用户单击节点项           |
| TVN_ENTER     | 用户按下回车键           |
| TVN_FOLDED    | 节点项被折叠             |
| TVN_UNFOLDED  | 节点项被展开             |

## 8.13 列表型控件通知码

| 标识符        | 含义                                  |
| ------------- | ------------------------------------- |
| LVN_ITEMRDOWN | 用户鼠标右键在列表项上按下            |
| LVN_ITEMRUP   | 用户鼠标右键在列表项上抬起            |
| LVN_HEADRDOWN | 用户鼠标右键在表头上按下              |
| LVN_HEADRUP   | 用户鼠标右键在表头上抬起              |
| LVN_KEYDOWN   | 键按下                                |
| LVN_ITEMDBCLK | 用户双击某个列表项                    |
| LVN_ITEMCLK   | 用户单击某个列表项(保留)              |
| LVN_SELCHANGE | 当前选择的列表项改变                  |
| LVN_FOLDED    | 用户鼠标点击某个列表项,使之折叠       |
| LVN_UNFOLDED  | 用户鼠标点击某个列表项,使之展开       |
| LVN_SELCHANGE | 鼠标右键在列表项上按下时,该项将被选中 |
| LVN_ITEMRDOWN | 鼠标右键在列表项上按下时,该项将被选中 |

## 8.14 月历控件通知码

| 标识符         | 含义                                       |
| -------------- | ------------------------------------------ |
| MCN_DATECHANGE | 当用户点击月历控件并造成当前日期发生改变时 |

## 8.15 旋钮控件通知码

| 标识符       | 含义                         |
| ------------ | ---------------------------- |
| SPN_REACHMAX | 旋钮控件在大于等于最大位置时 |
| SPN_REACHMIN | 旋钮控件在小于等于最小位置时 |

## 8.16 网格控件通知码

| 标识符                | 含义                     |
| --------------------- | ------------------------ |
| GRIDN_HEADLDOWN       | 用户鼠标左键在表头上按下 |
| GRIDN_HEADLUP         | 用户鼠标左键在表头上抬起 |
| GRIDN_KEYDOWN         | 键按下                   |
| GRIDN_CELLDBCLK       | 用户双击某个单元格       |
| GRIDN_CELLCLK         | 用户单击某个单元格       |
| GRIDN_FOCUSCHANGED    | 当前选择的单元格改变     |
| GRIDN_CELLTEXTCHANGED | 单元格内容改变           |

## 8.17 图标型控件通知码

| 标识符        | 含义                                     |
| ------------- | ---------------------------------------- |
| LVN_SELCHANGE | 对应SVN_SELCHANGE,当前高亮图表项发生改变 |
| LVN_CLICKED   | 对应SVN_CLICKED,用户点击图标项           |

## 8.18 独立滚动条通知码

| 标识符           | 含义                                                         |
| ---------------- | ------------------------------------------------------------ |
| SB_LINEUP        | 垂直滚动条向上滚一行                                         |
| SB_LINEDOWN      | 垂直滚动条向下滚一行                                         |
| SB_PAGEUP        | 垂直滚动条向上滚一页                                         |
| SB_PAGEDOWN      | 垂直滚动条向下滚一页                                         |
| SB_LINELEFT      | 水平滚动条向左滚一列                                         |
| SB_LINERIGHT     | 水平滚动条向右滚一列                                         |
| SB_PAGELEFT      | 水平滚动条向左滚一列页                                       |
| SB_PAGERIGHT     | 水平滚动条向右滚一页                                         |
| SB_THUMBPOSITION | 当游标被鼠标左键按住拖动,然后释放,此时的游标位置将由此通知码传给父窗口 |
| SB_THUMBTRACK    | 当游标被鼠标左键按住,在拖动游标的过程中,游标的位置将由此通知码不断的传给父窗口 |
| SB_TOP           | 游标到了水平滚动条的最左边或者是垂直滚动条的最上边,即到了滚动条的最小值 |
| SB_BOTTOM        | 游标到了水平滚动条的最右边或者是垂直滚动条的最下边,即到了滚动条的最大值 |

# 9  发送和投递消息

## 9.1 窗口消息函数

| 函数              | 含义                                                         |
| ----------------- | ------------------------------------------------------------ |
| PostMessage       | 该函数将消息放到指定窗口的消息队列后立即返回。这种发送方式称为“邮寄”消息。如果消息队列中的邮寄消息缓冲区已满,则该函数返回错误值。在下一个消息循环中,由GetMessage函数获得这个消息之后,窗口才会处理该消息PostMessage一般用于发送一些非关键性的消息。比如在MiniGUI中,鼠标和键盘消息就是通过PostMessage函数发送的 |
| SendMessage       | 应用程序一般通过发送一条消息来通知窗口过程立即完成某项任务。该函数和PostMessage函数不同,它把一条消息发送给指定窗口的窗口过程,而且等待该窗口过程完成消息的处理之后才会返回。当需要知道某个消息的处理结果时,使用该函数发送消息,然后根据其返回值进行处理。在MiniGUI-Threads当中,如果发送消息的线程和接收消息的线程不是同一个线程,发送消息的线程将阻塞并等待另一个线程的处理结果,然后继续运行;如果发送消息的线程和接收消息的线程是同一个线程,则与MiniGUI-Processes的SendMessage一样,直接调用接收消息窗口的窗口过程函数 |
| SendNotifyMessage | 该函数和PostMessage消息类似,也是不等待消息被处理即返回。但和PostMessage消息不同,通过该函数发送的消息不会因为缓冲区满而丢失,因为系统采用链表的形式处理这种消息。通过该函数发送的消息称为“通知消息”,一般用来从控件向其父窗口发送通知消息 |
| PostQuitMessage   | 该消息在消息队列中设置一个QS_QUIT标志。GetMessage在从指定消息队列中获取消息时,会检查该标志,如果有QS_QUIT标志,GetMessage消息将返回FALSE,从而可以利用该返回值终止消息循环 |
| BroadcastMessage  | 该函数将指定消息广播给桌面上的所有主窗口                     |
| ThrowAwayMessages | 该函数丢弃和指定窗口相关的消息队列中的所有消息,并返回所丢弃的消息个数。 |
| WaitMessage       | 该函数等待主窗口消息队列中的消息,消息队列中一有消息就返回。不同于GetMessage,这个函数并不从消息队列中移走消息 |
| TranslateMessage  | 把击键消息转换为MSG_CHAR消息,然后直接发送到窗口过程函数      |
| DispatchMessage   | 最终把消息发往消息的目标窗口的窗口过程                       |
| GetMessage        | 调用从应用程序的消息队列中取出一个消息                       |
| NotifyParentEx    | 发送控件通知消息                                             |

## 10.2 进程/线程间通信

### 10.2.1 异步事件处理

MAX_NR_LISTEN_FD 宏定义了系统能够监听的最多文件描述符数,默认定义为5

| 函数               | 含义                                                         |
| ------------------ | ------------------------------------------------------------ |
| RegisterListenFD   | 函数在系统当中注册一个需要监听的文件描述符,并指定监听的事件类型(type参数,可取POLLIN、POLLOUT或者POLLERR),接收MSG_FDEVENT消息的窗口句柄以及一个上下文信息 |
| UnregisterListenFD | 函数注销一个被注册的监听文件描述符                           |

### 10.2.2 简单请求/应答处理

| 函数                   | 含义                                         |
| ---------------------- | -------------------------------------------- |
| ClientRequest          | 向服务器发送请求                             |
| ServerSendReply        | 将结果发送给客户                             |
| RegisterRequestHandler | 服务器可以通过调用该函数注册一些请求处理函数 |
| GetRequestHandler      | 获取到处理函数                               |

### 10.2.3 UNIX Domain Socket封装

| 函数         | 含义                                                         | 备注                                                         |
| ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| serv_listen  | 服务器调用该函数建立一个监听套接字,并返回套接字文件描述符    | 建议将服务器监听套接字建立在/var/tmp/目录下                  |
| serv_accept  | 服务器调用该函数接受来自客户的连接请求                       |                                                              |
| cli_conn     | 客户调用该函数连接到服务器,其中name是客户的监听套接字        | 该函数为客户建立的套接字将保存在/var/tmp/目录中,并且以-c的方式命名,其中c是用来区别不同套接字通讯用途的字母,由project参数指定。MiniGUI-Processes内部使用了 ‘a’,所以由应用程序建立的套接字,应该使用除‘a’之外的字母 |
| sock_write_t | 在建立并连接之后,客户和服务器之间就可以使用sock_write_t函数和sock_read_t函数进行数据交换 | 和系统调用write类似,但可以传递进入一个超时参数,注意该参数以10ms为单位,为零时超时设置失效,且超时设置只在mginit程序中有效 |
| sock_read_t  | 和系统调用read类似,但可以传递进入一个超时参数                | 注意该参数以10ms为单位,为零时超时设置失效,且超时设置只在mginit程序中有效 |
| sock_write   | sock_write_t的宏定义，超时时间为零                           |                                                              |
| sock_read    | sock_read_t的宏定义，超时时间为零                            |                                                              |

# 11  界面外观及特效

## 11.1 窗口属性操作函数

| 函数                            | 含义                               | 备注                                                         |
| ------------------------------- | ---------------------------------- | ------------------------------------------------------------ |
| GetWindowElementAttr            | 获取指定窗口的某个元素属性的属性值 | we_attr_id是窗口元素属性ID                                   |
| SetWindowElementAttr            | 设置窗口的某个元素属性的属性值     |                                                              |
| GetWindowElementPixelEx         | 获取窗口元素的颜色值               |                                                              |
| SetWindowElementRenderer        | 设置当前窗口的渲染器               | werdr_name为所要设置的渲染器的名字,we_attrs是修改的窗口属性的结构体数组 |
| GetWindowRendererFromName       | 通过名称获取渲染器                 |                                                              |
| AddWindowElementRenderer        | 添加渲染器到MiniGUI                |                                                              |
| RemoveWindowElementRenderer     | 从MiniGUI删除渲染器                |                                                              |
| GetDefaultWindowElementRenderer | 获取默认渲染器                     |                                                              |
| SetDefaultWindowElementRenderer | 设置默认渲染器                     |                                                              |
| SetWindowElementRenderer        | 指定窗口渲染器                     |                                                              |

## 11.2 窗口元素属性名称

| 配置文件中的名称 | 代码名称           | 说明           |
| ---------------- | ------------------ | -------------- |
| caption          | WE_METRICS_CAPTION | 窗口标题栏大小 |

```
||WE_FONT_CAPTION	|窗口标题栏字体
1
```

fgc_active_caption	|WE_FGC_ACTIVE_CAPTION	|焦点状态窗口标题栏前景色
 bgca_active_caption	|WE_BGCA_ACTIVE_CAPTION	|焦点状态窗口标题栏背景渐变起始色
 bgcb_active_caption	|WE_BGCB_ACTIVE_CAPTION	|焦点状态窗口标题栏背景渐变终止色
 fgc_inactive_caption	|WE_FGC_INACTIVE_CAPTION	|非焦点状态窗口标题栏前景色
 bgca_inactive_caption	|WE_BGCA_INACTIVE_CAPTION	|非焦点状态窗口标题栏背景色渐变起始色
 bgcb_inactive_caption	|WE_BGCB_INACTIVE_CAPTION	|非焦点状态窗口标题栏背景色渐变终止色
 menu	|WE_METRICS_MENU	|菜单项、菜单栏的高度
 ||WE_FONT_MENU	|菜单字体
 fgc_menu	|WE_FGC_MENU	|菜单前景色
 bgc_menu	|WE_BGC_MENU	|菜单背景色
 border	|WE_METRICS_WND_BORDER	|窗口边框宽度
 fgc_active_border	|WE_FGC_ACTIVE_WND_BORDER	|焦点状态窗口边框颜色
 fgc_inactive_border	|WE_FGC_INACTIVE_WND_BORDER	|非焦点状态窗口边框颜色
 scrollbar	|WE_METRICS_SCROLLBAR	|滚动条大小
 fgc_msgbox	|WE_FGC_MESSAGEBOX	|消息框前景色
 ||WE_FONT_MESSAGEBOX	|消息框字体
 fgc_tip	|WE_FGC_TOOLTIP	|提示框前景色
 bgc_tip	|WE_BGC_TOOLTIP	|提示框背景色
 ||WE_FONT_TOOLTIP	|提示框字体
 fgc_window	|WE_FGC_WINDOW	|窗口前景色
 bgc_window	|WE_BGC_WINDOW	|窗口背景色
 fgc_3dbox	|WE_FGC_THREED_BODY	|三维立体框表面上符号的颜色,如对勾、箭头等的颜色
 mainc_3dbox	|WE_MAINC_THREED_BODY	|三维立体框边框及表面颜色
 fgc_selected_item	|WE_FGC_SELECTED_ITEM	|选定菜单项(列表项)的前景色
 bgc_selected_item	|WE_BGC_SELECTED_ITEM	|选定菜单项(列表项)的背景色
 bgc_selected_lostfocus	|WE_BGC_SELECTED_LOSTFOCUS	|选定菜单项(列表项)失去焦点后的背景色
 fgc_disabled_item	|WE_FGC_DISABLED_ITEM	|无效菜单项(列表项)的前景色
 bgc_disabled_item	|WE_BGC_DISABLED_ITEM	|无效菜单项(列表项)的背景色
 fgc_hilight_item	|WE_FGC_HIGHLIGHT_ITEM	|高亮菜单项(列表项)的前景色
 bgc_hilight_item	|WE_BGC_HIGHLIGHT_ITEM	|高亮菜单项(列表项)的背景色
 fgc_significant_item	|WE_FGC_SIGNIFICANT_ITEM	|重要菜单项(列表项)的前景色
 bgc_significant_item	|WE_BGC_SIGNIFICANT_ITEM	|重要菜单项(列表项)的背景色
 bgc_desktop	|WE_BGC_DESKTOP	|桌面背景色

## 11.3 窗口皮肤属性名称

| 配置文件中的名称        | 代码名称                     | 说明                   |
| ----------------------- | ---------------------------- | ---------------------- |
| skin_bkgnd              | WE_LFSKIN_WND_BKGND          | 桌面背景皮肤图片       |
| skin_caption            | WE_LFSKIN_CAPTION            | 窗口标题栏皮肤图       |
| skin_caption_btn        | WE_LFSKIN_CAPTION_BTN        | 窗口标题栏按钮皮肤图片 |
| skin_scrollbar_hshaft   | WE_LFSKIN_SCROLLBAR_HSHAFT   | 水平滚动条滚槽皮肤图片 |
| skin_scrollbar_vshaft   | WE_LFSKIN_SCROLLBAR_VSHAFT   | 垂直滚动条滚槽皮肤图片 |
| skin_scrollbar_hthumb   | WE_LFSKIN_SCROLLBAR_HTHUMB   | 水平滚动条游标皮肤图片 |
| skin_scrollbar_vthumb   | WE_LFSKIN_SCROLLBAR_VTHUMB   | 垂直滚动条游标皮肤图片 |
| skin_scrollbar_arrows   | WE_LFSKIN_SCROLLBAR_ARROWS   | 滚动条箭头皮肤图       |
| skin_tborder            | WE_LFSKIN_BORDER_TOP         | 顶部边框皮肤图片       |
| skin_bborder            | WE_LFSKIN_BORDER_BOTTOM      | 底部边框皮肤图片       |
| skin_lborder            | WE_LFSKIN_BORDER_LEFT        | 左边边框皮肤图片       |
| skin_rborder            | WE_LFSKIN_BORDER_RIGHT       | 右边边框皮肤图片       |
| skin_arrows             | WE_LFSKIN_ARROWS             | 箭头皮肤图片           |
| skin_arrows_shell       | WE_LFSKIN_ARROWS_SHELL       | 肩头外围皮肤图片       |
| skin_pushbtn            | WE_LFSKIN_PUSHBUTTON         | PUSH 按钮皮肤图        |
| skin_radiobtn           | WE_LFSKIN_RADIOBUTTON        | 单选按钮皮肤图片       |
| skin_checkbtn           | WE_LFSKIN_CHECKBUTTON        | 复选按钮皮肤图片       |
| skin_tree               | WE_LFSKIN_TREE               | 树控件皮肤图片         |
| skin_header             | WE_LFSKIN_HEADER             | 表头皮肤图片           |
| skin_tab                | WE_LFSKIN_TAB                | 属性页皮肤图片         |
| skin_tbslider_h         | WE_LFSKIN_TBSLIDER_H         | 水平滑动条皮肤图       |
| skin_tbslider_v         | WE_LFSKIN_TBSLIDER_V         | 垂直滑动条皮肤图       |
| skin_trackbar_horz      | WE_LFSKIN_TRACKBAR_HORZ      | 水平轨迹条皮肤图       |
| skin_trackbar_vert      | WE_LFSKIN_TRACKBAR_VERT      | 垂直轨迹条皮肤图       |
| skin_progressbar_htrack | WE_LFSKIN_PROGRESS_HTRACKBAR | 水平进度条滑轨皮肤图片 |
| skin_progressbar_vtrack | WE_LFSKIN_PROGRESS_VTRACKBAR | 垂直进度条滑轨皮肤图片 |
| skin_progressbar_hchunk | WE_LFSKIN_PROGRESS_HCHUNK    | 水平进度条滑块皮肤图片 |
| skin_progressbar_vchunk | WE_LFSKIN_PROGRESS_VCHUNK    | 垂直进度条滑块皮肤图片 |

## 11.4 渲染器结构体成员

| 成员名称                | 说明                                                         |
| ----------------------- | ------------------------------------------------------------ |
| name                    | 渲染器名称,长度为LEN_RENDERER_NAM。宏LEN_RENDERER_NAME定义为15 |
| init                    | 渲染器初始化函数指针。用来初始化渲染器的基本信息,包括窗口元素的尺寸、字体、颜色等窗口属性信息和渲染器私有信息。 |
| deinit                  | 渲染器销毁函数指针。用来释放渲染器所占用的资源。             |
| calc_3dbox_color        | 三维立体颜色计算函数指针，此函数根据三维立体边框和表面颜色color和计算标志flag获取一个颜色值 |
| draw_3dbox              | 三维立体框绘制函数指针                                       |
| draw_radio              | 单选框绘制函数指针                                           |
| draw_checkbox           | 非选中复选框绘制函数指针                                     |
| draw_checkmark          | 选中复选框绘制函数指针                                       |
| draw_arrow              | 箭头绘制函数指针                                             |
| draw_fold               | 打开的或者是关闭的文件夹、树控件的缩进、展开标志的绘制函数指针 |
| draw_focus_frame        | 焦点方框绘制函数指针                                         |
| draw_normal_item        | 无效状态的列表框子项的绘制函数指针                           |
| draw_significant_item   | 需重点标识的列表框子项的绘制函数指针                         |
| draw_push_button        | PUSH按钮绘制函数指针                                         |
| draw_radio_button       | 单选按钮绘制函数指针                                         |
| draw_check_button       | 复选框按钮绘制函数指针                                       |
| draw_border             | 窗口边框绘制函数指针                                         |
| draw_caption            | 窗口标题栏绘制函数指针                                       |
| draw_caption_button     | 窗口标题栏按钮绘制函数指针。标题栏按钮包括:最小化按钮、最大化按钮、关闭按钮 |
| draw_scrollbar          | 滚动条绘制函数指针                                           |
| calc_trackbar_rect      | 轨迹条外框绘制函数指针                                       |
| draw_trackbar           | 轨迹条绘制函数指针                                           |
| calc_we_area            | 窗口各元素区域计算函数指针。元素区域包括边框、标题栏、标题栏按钮、工具栏、菜单、客户区、窗口水平滚动条和垂直滚动条 |
| calc_we_metrics         | 窗口各元素尺寸的计算函数指针。所包含的元素与calc_we_area函数所包含的元素相同 |
| hit_test                | 获取鼠标所点击的窗口元素的函数指针                           |
| on_click_hotspot        | 当鼠标点击到热点区域时,进行相应处理的函数指针                |
| draw_custom_hotspot     | 热点区域的绘制函数指针                                       |
| calc_thumb_area         | 水平滚动条和垂直滚动条游标区域的计算函数指针                 |
| disabled_text_out       | 无效区域文字的输出函数指针                                   |
| draw_tab                | 属性页控件标签的绘制函数指针                                 |
| draw_progress           | 进度条绘制函数                                               |
| draw_header             | 列表框或者网格控件的表头绘制函数指针                         |
| on_get_rdr_attr         | 渲染器私有信息获取函数指针                                   |
| on_set_rdr_attr         | 渲染器私有信息设置函数指针                                   |
| erase_background        | 窗口背景擦除绘制函数指针                                     |
| draw_normal_menu_item   | 正常状态菜单项绘制函数指针                                   |
| draw_hilite_menu_item   | 高亮状态菜单项绘制函数指针                                   |
| draw_disabled_menu_item | 无效状态菜单项绘制函数指针                                   |
| we_metrics              | 窗口尺寸属性                                                 |
| we_colors               | 窗口颜色属性                                                 |
| we_fonts                | 窗口字体属性                                                 |
| we_icon                 | 窗口所使用的图标句柄                                         |
| refcount                | 渲染器引用计数                                               |
| private_info            | 渲染器私有信息指针                                           |

## 11.5 双缓冲区操作函数

| 函数                  | 含义                                                         | 备注                                        |
| --------------------- | ------------------------------------------------------------ | ------------------------------------------- |
| CreateSecondaryDC     | 双缓冲区DC创建函数                                           | 根据传入的窗口大小,创建兼容的内存DC并返回   |
| SetSecondaryDC        | 将已经创建好的内存DC设置为目标主窗口的双缓冲区,并设置从双缓冲区的屏幕DC数据复制回调函数 |                                             |
| GetSecondaryDC        | 双缓冲区句柄获取函数                                         |                                             |
| GetSecondaryClientDC  | 该函数用于双缓冲窗体的客户区绘制DC，如果窗体不支持双缓冲,则直接返回一般的客户区DC,相当于调用GetClientDC |                                             |
| ReleaseSecondaryDC    | 该函数用于释放双缓冲DC,如果是主窗体本身的双缓冲DC,则什么也不处理;否则释该DC |                                             |
| DeleteSecondaryDC     | 该函数删除由CreateSecondaryDC创建的内存DC                    |                                             |
| GetSecondarySubDC     | 该函数仅针对双缓冲,在私有窗口DC基础上创建子DC                | 使之可以作为主窗口的客户DC,或者控件的DC使用 |
| ReleaseSecondarySubDC | 该函数释放私有DC的子DC                                       |                                             |

在调用 SetSecondaryDC 函数时要注意以下几点

- 如果主窗口具有WS_EX_AUTOSecondaryDC风格,则会取消该风格,并调用 DeleteSecondaryDC函数删除已有的双缓冲区,并返回HDC_SCREEN
- 如果主窗口没有WS_EX_AUTOSecondaryDC风格,则返回原先的双缓冲区句柄。原先的双缓冲区由应用程序自行管理
- 如果传入HDC_SCREEN,将取消窗口的双缓冲机制
- 如果调用该函数时,传递进入的ON_UPDATE_SecondaryDC回调函数为NULL,则这时MiniGUI不会自动完成屏幕DC到屏幕的显示的绘制,用户需要使用基本的块拷贝函数完成双缓冲区到屏幕DC的复制;否则调用该回调函数由应用程序负责从双缓冲区到屏幕DC的复制,应用程序可在此时完成界面特效

# 12  图形设备接口

## 12.1 设备上下文句柄

| 函数                    | 含义                                                         | 备注                                                         |
| ----------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| BeginPaint              | 接收到MSG_PAINT消息时，获取句柄                              |                                                              |
| EndPaint                | 绘制结束之后，释放设备上下文句柄                             |                                                              |
| GetDC                   | 获取的设备上下文是针对整个窗口的                             | 避免同时使用多个设备上下文,并避免在递归函数中调用            |
| GetSubDC                | 获取指定DC的兼容子DC                                         |                                                              |
| GetClientDC             | 获取的设备上下文是针对窗口客户区                             |                                                              |
| ReleaseDC               | 释放上面三个函数获取的设备上下文                             |                                                              |
| CreatePrivateDC         | 创建私有设备环境上下文，如果主窗口的扩展风格中指定了WS_EX_USEPRIVATECDC风格,则CreateMainWindow函数会自动为该窗口的客户区建立私有设备上下文 |                                                              |
| CreatePrivateSubDC      | 创建私有设备环境上下文的兼容子DC                             |                                                              |
| CreatePrivateClientDC   | 创建客户区私有设备环境上下文，如果控件类具有CS_OWNDC属性,则所有属于该控件类的控件将自动建立私有设备上下文 |                                                              |
| GetPrivateClientDC      | 获取私有设备环境上下文                                       |                                                              |
| DeletePrivateDC         | 删除私有设备环境上下文，自动创建的，系统将在销毁窗口时自动调用 |                                                              |
| SaveDC                  | 保存设备环境上下文                                           |                                                              |
| RestoreDC               | 恢复设备环境上下文                                           |                                                              |
| CreateCompatibleDCEx    | 内存设备上下文的创建，在系统内存中建立一个类似显示内存的区域,然后在该区域中进行绘图操作,结束后再复制到显示内存中 | 速度很快,减少直接操作显存造成的闪烁现象                      |
| CreateCompatibleDC      | 内存设备上下文的创建，在系统内存中建立一个类似显示内存的区域,然后在该区域中进行绘图操作,结束后再复制到显示内存中 |                                                              |
| CreateMemDC             | 指定新建内存DC的高度、宽度、颜色深度,以及必要的RGBA组成方式  |                                                              |
| ConvertMemDC            | 将一个任意的内存DC对象,根据给定的参考DC的象素格式进行转换    |                                                              |
| DeleteMemDC             | 删除创建的内存设备上下文                                     |                                                              |
| DeleteCompatibleDC      | 删除创建的内存设备上下文                                     |                                                              |
| SetMemDCAlpha           | 设定或者取消整个内存DC对象的Alpha通道值                      | Alpha通道值作用在DC的所有象素点上                            |
| SetMemDCColorKey        | 设定或者取消整个内存DC对象的ColorKey                         | 即透明象素值                                                 |
| CreateMemDCFromBitmap   | 创建一个指向设备相关位图并和指定DC兼容的内存DC               |                                                              |
| CreateMemDCFromMyBitmap | 创建指向设备无关位图的内存DC                                 |                                                              |
| LockDC                  | 锁定给定HDC的指定矩形区域,然后返回缓冲区头指针               |                                                              |
| UnlockDC                | 解开已锁定的HDC                                              |                                                              |
| InitSlaveScreen         | 创建副屏                                                     | 可以根据指定的引擎和显示模式构造打开双屏并返回一个图形设备上下文句柄 |
| TerminateSlaveScreen    | 销毁副屏                                                     | hdc 参数是InitSlaveScreen函数返回的表示副屏的图形设备上下文句柄 |

## 12.2 矩形/区域/剪切域操作

| 函数                | 含义                                                     | 备注                            |
| ------------------- | -------------------------------------------------------- | ------------------------------- |
| SetRect             | 对RECT对象的各个分量进行赋值                             |                                 |
| SetRectEmpty        | 将RECT对象设置为空                                       |                                 |
| IsRectEmpty         | 判断给定RECT对象是否为空                                 |                                 |
| NormalizeRect       | 对给定矩形进行正规化处理                                 | 满足(right>left并且 bottom>top) |
| CopyRect            | 复制矩形                                                 |                                 |
| EqualRect           | 判断两个RECT对象是否相等                                 |                                 |
| IntersectRect       | 求两个RECT对象之交集                                     | 不相交,返回FALSE                |
| DoesIntersect       | 仅仅判断两个矩形是否相交                                 |                                 |
| IsCovered           | 判断对象A是否全部覆盖对象B                               |                                 |
| UnionRect           | 求两个矩形之并                                           | 不相并,返回FALSE                |
| GetBoundRect        | 求两个矩形的外包最小矩形                                 |                                 |
| SubstractRect       | 从一个矩形中减去另外一个矩形                             |                                 |
| OffsetRect          | 对给定的RECT对象进行平移处理                             |                                 |
| InflateRect         | 对给定的RECT对象进行膨胀处理                             | 给定膨胀值的两倍                |
| InflateRectToPt     | 将给定的RECT对象膨胀到指定的点                           |                                 |
| PtInRect            | 判断给定的点是否位于指定的RECT中                         |                                 |
| SetClipRgn          | 将剪切域设为仅包含一个矩形的剪切域                       |                                 |
| ClipRgnCopy         | 复制剪切域                                               |                                 |
| ClipRgnIntersect    | 求两个剪切域的交集                                       |                                 |
| GetClipRgnBoundRect | 求剪切域的外包最小矩形                                   |                                 |
| IsEmptyClipRgn      | 判断剪切域是否为空                                       |                                 |
| EmptyClipRgn        | 释放剪切域中的剪切矩形,并清空剪切域                      |                                 |
| AddClipRect         | 将一个剪切矩形追加到剪切域中                             |                                 |
| IntersectClipRect   | 求剪切区域和给定矩形相交的剪切区域                       |                                 |
| SubtractClipRect    | 从剪切区域中减去指定的矩形                               |                                 |
| CreateClipRgn       | 创建一个剪切区域                                         |                                 |
| DestroyClipRgn      | 清空并且销毁一个剪切区域                                 |                                 |
| ExcludeClipRect     | 从设备上下文的当前可见区域中排除给定的矩形区域           | 设备上下文的可见区域将缩小      |
| IncludeClipRect     | 向当前设备上下文的可见区域中添加一个矩形区域             | 设备上下文的可见区域将扩大      |
| ClipRectIntersect   | 将设备上下文的可见区域设置为已有区域和给定矩形区域的交集 |                                 |
| SelectClipRect      | 将设备上下文的可见区域重置为一个矩形区域                 |                                 |
| SelectClipRegion    | 将设备上下文的可见区域设置为一个指定的区域               |                                 |
| GetBoundsRect       | 获取当前可见区域的外包最小矩形                           |                                 |
| PtVisible           | 判断给定的点是否可见                                     |                                 |
| RectVisible         | 判断给定的矩形是否可见                                   |                                 |
| PtInRegion          | 检查给定点是否位于给定的区域中                           |                                 |
| RectInRegion        | 检查给定矩形是否和给定区域相交                           |                                 |
| OffsetRegionEx      | 将剪切域处于两个矩形区域的重叠区的剪切矩形进行位移       |                                 |
| OffsetRegion        | 对剪切域简单的进行位移                                   |                                 |
| IntersectRegion     | 对两个给定区域进行求交运算                               |                                 |
| UnionRegion         | 合并两个不同的区域                                       |                                 |
| SubtractRegion      | 从一个区域中减去另外一个区域                             |                                 |
| XorRegion           | 对两个区域进行异或运算                                   |                                 |
| GetRasterOperation  | 获取当前的光栅操作                                       |                                 |
| SetRasterOperation  | 设置当前的光栅操作                                       |                                 |

## 12.3 像素与RGB操作函数

| 函数        | 含义                |
| ----------- | ------------------- |
| SetPixel    | 直接设置像素值      |
| SetPixelRGB | 通过RGB值设置像素值 |
| GetPixel    | 直接获取像素值      |
| GetPixelRGB | 通过RGB值获取像素值 |
| RGB2Pixel   | RGB值转换成像素值   |
| Pixel2RGB   | 像素值转换成RGB值   |

## 12.4 位图操作函数

| 函数                   | 含义                                                 | 备注                                 |
| ---------------------- | ---------------------------------------------------- | ------------------------------------ |
| FillBox                | 填充指定矩形颜色                                     | 设置刷子颜色后，受当前光栅操作的影响 |
| FillCircle             | 填充指定的圆颜色                                     | 设置刷子颜色后，受当前光栅操作的影响 |
| FillEllipse            | 填充指定的椭圆颜色                                   | 设置刷子颜色后，受当前光栅操作的影响 |
| FillPolygon            | 填充指定的多边形颜色                                 | 设置刷子颜色后，受当前光栅操作的影响 |
| FloodFill              | 指定点开始填注                                       | 设置刷子颜色后，受当前光栅操作的影响 |
| FillBoxWithBitmap      | 用设备相关位图对象填充矩形框                         | 可以用来扩大或者缩小位图             |
| FillBoxWithBitmapPart  | 用设备相关位图对象的部分填充矩形框                   | 也可以扩大或缩小位图                 |
| BitBlt                 | 用来实现两个相同或不同的设备上下文之间的显示内存复制 |                                      |
| StretchBlt             | 在BitBlt的基础上进行缩放操作                         |                                      |
| LoadBitmapEx           | 将位图对象的逐个扫描行装载成设备无关的位图对象       | 可以减少对内存资源的占用             |
| LoadBitmapFromFile     | 从文件加载位图，设备相关                             |                                      |
| LoadBitmapFromMemory   | 从内存加载位图，设备相关                             |                                      |
| LoadBitmap             | LoadBitmapFromFile函数宏定义                         |                                      |
| UnloadBitmap           | 卸载位图                                             |                                      |
| LoadMyBitmapEx         | 将位图文件装载成设备无关的位图对象                   |                                      |
| LoadMyBitmapFromFile   | 从文件加载位图，设备无关                             |                                      |
| LoadMyBitmapFromMemory | 从内存加载位图，设备无关                             |                                      |
| UnloadMyBitmap         | 卸载位图                                             |                                      |
| InitMyBitmapSL         | 为LoadMyBitmapSL函数的装载进行初始化                 |                                      |
| LoadMyBitmapSL         | 每加载完一行后,将调用传入该函数的用户定义回调函数cb  | 对装载后的一条扫描线进行处理         |
| CleanupMyBitmapSL      | LoadMyBitmapSL加载完释放资源                         |                                      |
| PaintImageEx           | 将指定的图形直接绘制到屏幕上，无需装载为BITMAP对象   | 减少图片装载和绘制中内存消耗         |
| PaintImageFromFile     | 从文件绘制                                           |                                      |
| PaintImageFromMem      | 从内存绘制                                           |                                      |
| ExpandMyBitmap         | 将设备无关位图转换为和特定设备上下文相关的位图       |                                      |
| ScaleBitmap            | 将源BITMAP对象进行伸缩处理                           |                                      |
| GetBitmapFromDC        | 将指定矩形范围内的象素复制到BITMAP对象中             |                                      |
| GetPixelInBitmap       | 获得BITMAP对象中指定位置的象素值                     |                                      |
| SetPixelInBitmap       | 设置BITMAP对象中指定位置的象素值                     |                                      |

BitBlt 函数的各参数含义如下

- hsdc:源设备上下文
- sx,sy:源设备上下文中所选矩形的左上角坐标
- sw,sh:所选矩形的宽度和高度
- hddc:目标设备上下文
- dx,dy:目标设备上下文中目标矩形的左上角坐标
- dwRop:光栅操作,目前被忽略

## 12.5 图片缩放/旋转函数

| 函数                      | 含义                                                         |
| ------------------------- | ------------------------------------------------------------ |
| StretchPaintImageFromFile | 从文件读入图片信息并进行缩放处理                             |
| StretchPaintImageFromMem  | 从内存中读取图片信息并进行缩放处理                           |
| StretchPaintImageEx       | 从数据源中读取图片信息并同时进行缩放处理                     |
| PivotScaledBitmapFlip     | 将位图进行垂直或水平翻转,并且可以缩放至指定宽度、高度,同时绕指定点旋转一指定的角度angle并将它画在DC的指定位置 |
| RotateBitmap              | 可对位图进行绕中心旋转                                       |
| PivotBitmap               | 可对位图进行绕指定旋转中心进行旋转                           |
| RotateScaledBitmap        | 将位图缩放至指定的宽度和高度并绕中心旋转指定的角度           |
| RotateBitmapVFlip         | 将位图垂直翻转并绕中心旋转                                   |
| RotateBitmapHFlip         | 将位图水平翻转并绕中心旋转                                   |
| RotateScaledBitmapVFlip   | 将位图垂直翻转,缩放到指定的宽度和高度并绕中心旋转            |
| RotateScaledBitmapHFlip   | 将位图水平翻转,缩放到指定的宽度和高度并绕中心旋转            |

## 12.6 调色板操作函数

| 函数                   | 含义                       |
| ---------------------- | -------------------------- |
| CreatePalette          | 创建一个新的调色板         |
| GetDefaultPalette      | 得到默认的调色板           |
| GetPaletteEntries      | 获取调色板的入口项         |
| SetPaletteEntries      | 设置调色板的入口项         |
| ResizePalette          | 重设调色板的大小           |
| GetNearestPaletteIndex | 得到调色板的最接近的索引值 |
| GetNearestColor        | 得到调色板的最接近的颜色   |

## 12.7 画线操作函数

| 函数                             | 含义                                    |
| -------------------------------- | --------------------------------------- |
| LineClipper                      | 直线剪切器，对给定的直线进行剪切        |
| LineGenerator                    | 直线生成器                              |
| CircleGenerator                  | 圆生成器                                |
| EllipseGenerator                 | 椭圆生成器                              |
| CircleArcGenerator               | 圆弧生成器                              |
| PolygonIsMonotoneVertical        | 判断给定的多边形是否是垂直单调多边形    |
| MonotoneVerticalPolygonGenerator | 垂直多边形生成器                        |
| PolygonGenerator                 | 一般多边形生成器                        |
| FloodFillGenerator               | 填注生成器                              |
| LineTo                           | 从当前画笔点画直线到给定点              |
| MoveTo                           | 将当前画笔的起始点移动到给定点          |
| Rectangle                        | 绘制矩形                                |
| RoundRect                        | 绘制圆角矩形                            |
| PollyLineTo                      | 利用LineTo函数画折线                    |
| SplineTo                         | 利用LineTo函数画三次样条曲线            |
| Circle                           | 绘制圆                                  |
| Ellipse                          | 绘制椭圆                                |
| CircleArc                        | 绘制圆弧                                |
| InitFreeClipRectList             | 为区域创建剪切矩形的私有堆              |
| InitClipRgn                      | 初始化区域,并指定区域使用已创建的私有堆 |
| InitCircleRegion                 | 将某个区域分别初始化为圆                |
| InitEllipseRegion                | 将某个区域分别初始化为椭圆              |
| InitPolygonRegion                | 将某个区域分别初始化为多边形            |

## 12.8 高级二维绘图函数

在配置MiniGUI时,我们可以通过–enable-adv2dapi参数来打开MiniGUI中的高级二维绘图函数接口。当MiniGUI中包含高级二维绘图函数接口时,前面提到的所有填充类函数将受到当前画刷属性的影响,这些函数包括FillBox  、FillCircle、  FillEllipse、FillPolygon、FloodFill等等,但基本的线段绘制函数却不会受画笔属性的影响,这些函数包括MoveTo  /LineTo、Rectangle、PolyLineTo、SplineTo、Circle、Ellipse、CircleArc等函数。这些基本的线段绘制函数仍将以零画笔绘制

| 函数                            | 说明                             |
| ------------------------------- | -------------------------------- |
| GetPenType/SetPenType           | 获取/设置画笔类型                |
| GetPenWidth/SetPenWidth         | 获取/设置画笔宽度,以象素为单位   |
| GetPenCapStyle/SetPenCapStyle   | 获取/设置画笔端点风格            |
| GetPenJoinStyle/SetPenJoinStyle | 获取/设置画笔接合风格            |
| SetPenDashes                    | 设定画笔的虚实                   |
| GetBrushType/SetBrushType       | 获取/设置画刷类型                |
| SetBrushInfo                    | 设定画刷所使用的位图或者点刻位图 |

| 类型               | 说明                                                         |
| ------------------ | ------------------------------------------------------------ |
| PT_SOLID           | 表示实画笔                                                   |
| PT_ON_OFF_DASH     | 开/关虚线，虚实线段中的偶数段会被绘制,而奇数段不会被绘制     |
| PT_DOUBLE_DASH     | 双虚线，虚实线段中的偶数段会被绘制,而奇数段会根据画刷的设置进行绘制 |
| PT_CAP_BUTT        | 线段的端点被绘制为矩形,并且扩展到端点的坐标处                |
| PT_CAP_ROUND       | 线段的端点被绘制为半圆形,端点是圆弧的圆心,直径是线段的宽度   |
| PT_CAP_PROJECTING  | 线段的端点被绘制为矩形,并超出端点坐标,超出的部分为线段宽度的一半 |
| PT_JOIN_MITER      | 相连两条线段的边被扩展为一个斜角,又称“斜接合”                |
| PT_JOIN_ROUND      | 相连两条线段的边被扩展为圆弧,又称“圆接合”                    |
| PT_JOIN_BEVEL      | 相连两条线段的接合形成了一个斜面,又称“斜面接合”              |
| BT_SOLID           | 实画刷。以当前的画刷颜色填充                                 |
| BT_TILED           | 位图画刷。以当前设定的位图进行平铺式填充                     |
| BT_STIPPLED        | 透明点刻画刷。使用当前设定的点刻位图填充,点刻位图中被设置的位将以画刷颜色填充,而未被设置的位将不做绘制(即保留背景) |
| BT_OPAQUE_STIPPLED | 不透明点刻画刷                                               |

| 函数          | 说明                                                         |
| ------------- | ------------------------------------------------------------ |
| LineEx        | 函数将按照当前的画笔属性绘制一条直线段,从(x1,y1)到(x2,y2)    |
| ArcEx         | 函数将按照当前的画笔属性绘制一条圆弧线段,该圆弧的圆心为(x,y),所在圆或椭圆的最小外接矩形宽为width,高为  height;圆弧的起始角度为ang1,以1/64度为单位表示,ang2指的是圆弧终止角度相对起始角度的度数,以1/64度为单位表示;  ang2为正,表示逆时针方向,为负表示顺时针方向。当ang2大于等于360x64时,表示要绘制的不是圆弧而是一个完整的圆弧或者椭圆 |
| FillArcEx     | 函数将按照当前的画刷属性填充一个圆弧扇形。参数意义和ArcEx相同 |
| PolyLinEx     | 函数按照当前的画笔属性绘制多条线段,如果有相连线段,则会根据画笔的属性进行接合处理 |
| PolyArcEx     | 函数按照当前的画笔属性绘制多条圆弧,如果有相连圆弧,则会根据画笔的属性进行接合处理 |
| PolyFillArcEx | 函数填充多个圆弧                                             |

# 13  文本的处理与显示

## 13.1 字体的加载与创建

| 函数                  | 含义                                      | 备注                     |
| --------------------- | ----------------------------------------- | ------------------------ |
| LoadDevFontFromFile   | 动态加载设备字体                          |                          |
| DestroyDynamicDevFont | 卸载动态加载的设备字体                    |                          |
| CreateBMPDevFont      | 创建位图设备字体，不能在MiniGui.cfg中配置 |                          |
| AddGlyphsToBMPFont    | 用来往已创建的位图设备字体里面增加字符    |                          |
| DestroyBMPFont        | 用来销毁位图设备字体                      |                          |
| CreateLogFont         | 创建逻辑字体                              |                          |
| CreateLogFontByName   | 通过名字创建逻辑字体                      |                          |
| CreateLogFontIndirect | 间接的创建逻辑字体                        |                          |
| DestroyLogFont        | 销毁创建的逻辑字体                        | 不能销毁被选中的逻辑字体 |
| GetLogFontInfo        | 获取逻辑字体信息                          |                          |
| GetSystemFont         | 返回指定的系统逻辑字体                    |                          |
| GetCurFont            | 得到当前使用的字体                        |                          |
| SelectFont            | 给窗口设置字体                            |                          |

GetSystemFont函数的参数

- SYSLOGFONT_DEFAULT:系统默认字体,必须是单字节字符集逻辑字体,必须由 RBF 设备字体组成
- SYSLOGFONT_WCHAR_DEF:系统默认多字节字符集字体,通常由RBF设备字体组成,并且多字节字体的宽度是
   SYSLOGFONT_DEFAULT 逻辑字体的两倍
- SYSLOGFONT_FIXED:固定宽度的系统字体
- SYSLOGFONT_CAPTION:用于显示标题栏文本的逻辑字体
- SYSLOGFONT_MENU:用于显示菜单文本的逻辑字体
- SYSLOGFONT_CONTROL:用于控件的默认逻辑字体

## 13.2 文本输出函数

| 函数                     | 含义                                                         | 备注                           |
| ------------------------ | ------------------------------------------------------------ | ------------------------------ |
| GetTextExtentPoint       | 计算在给定的输出宽度内输出多字节文本时,可输出的最大字符个数、每个字符所在的字节位置、每个字符的输出位置,以及实际的输出高度和宽度 |                                |
| GetTabbedTextExtentPoint | 在GetTextExtentPoint函数的基础上,增加了对回车、换行和TAB键等格式化字符的处理 |                                |
| GetFontHeight            | 返回逻辑字体的高度                                           |                                |
| GetMaxFontWidth          | 返回逻辑字体的最大字符宽度                                   |                                |
| GetTextExtent            | 计算文本的输出高度和宽度                                     |                                |
| GetTabbedTextExtent      | 返回格式化字符串的输出高度和宽度                             |                                |
| TextOutLen               | 用来在给定位置输出指定长度的字符串,若长度为-1,则字符串必须是以’\0’ 结尾的 |                                |
| TabbedTextOutLen         | 用来输出格式化字符串                                         |                                |
| TabbedTextOutEx          | 用来输出格式化字符串,但可以指定字符串中每个TAB键的位置       |                                |
| GetLastTextOutPos        | 获取最后的文本位置                                           |                                |
| TextOut                  | TextOutLen输出全部文本                                       |                                |
| TabbedTextOut            | TabbedTextOutLen格式化全部文本                               |                                |
| DrawText                 | 以不同的对齐方式在指定的矩形内部输出文本                     | 尚不支持对UTF-16编码的文本输出 |
| DrawTextEx               | 在DrawText函数的基础上增加了对输入文本首行缩进字符数的指定   |                                |
| DrawTextEx2              | 在前两者的基础上,增加了对首行能显示的字符个数的计算          |                                |
| GetTextCharacterExtra    | 获取当前字符间距值                                           |                                |
| SetTextCharacterExtra    | 设置字符间距值                                               |                                |
| GetTextAboveLineExtra    | 获取行前间隔值                                               |                                |
| SetTextAboveLineExtra    | 设置行前间隔值                                               |                                |
| GetTextBellowLineExtra   | 获取行后间隔值                                               |                                |
| SetTextBellowLineExtra   | 设置行后间隔值                                               |                                |

## 13.3 输出格式标识

| 格式标识符    | 含义                                     | 备注                               |
| ------------- | ---------------------------------------- | ---------------------------------- |
| DT_TOP        | 在垂直方向顶端对齐                       | 只对单行输出有效(DT_SINGLELINE)    |
| DT_VCENTER    | 在垂直方向居中                           | 只对单行输出有效(DT_SINGLELINE)    |
| DT_BOTTOM     | 在垂直方向底端对齐                       | 只对单行输出有效(DT_SINGLELINE)    |
| DT_LEFT       | 水平方向左对齐                           |                                    |
| DT_CENTER     | 水平对中                                 |                                    |
| DT_RIGHT      | 水平方向右对齐                           |                                    |
| DT_WORDBREAK  | 当文本输出超过矩形区时按单词换行输出     |                                    |
| DT_CHARBREAK  | 当文本输出超过矩形区时按字符换行输出     |                                    |
| DT_SINGLELINE | 单行输出                                 | 无此标志时会忽略垂直方向的对齐标志 |
| DT_EXPANDTABS | 扩展TAB字符                              |                                    |
| DT_TABSTOP    | 格式参数的高8位用来指定TAB键宽度         |                                    |
| DT_NOCLIP     | 不作输出剪切，默认将把输出剪切到指定矩形 |                                    |
| DT_CALCRECT   | 不作实际输出,只计算实际的输出矩形大小    |                                    |

## 13.4 字体渲染风格

| 逻辑字体风格名称               | 风格字符          | 逻辑字体风格值       | 含义                                                         |
| ------------------------------ | ----------------- | -------------------- | ------------------------------------------------------------ |
| weight: FONT_WEIGHT_REGULAR    | 第一位“r”         | FS_WEIGHT_REGULAR    | 不作特殊处理                                                 |
| weight: FONT_WEIGHT_BOLD       | 第一位“b”         | FS_WEIGHT_BOLD       | 加粗显示                                                     |
| weight: FONT_WEIGHT_LIGHT      | 第一位“l”         | FS_WEIGHT_LIGHT      | 使用背景色描绘字型的边缘,其他地方透明显示                    |
| weight: FONT_WEIGHT_BOOK       | 第一位“b”         | FS_WEIGHT_BOOK       | 采用低通滤波算法处理矢量字型边缘或者放大后的字型边缘         |
| weight: FONT_WEIGHT_DEMIBOLD   | 第一位“d”         | FS_WEIGHT_DEMIBOLD   | 加粗的同时,采用低通滤波算法处理矢量字型边缘或者放大后的字型边缘 |
| weight: FONT_WEIGHT_SUBPIXEL   | 第一位“s”         | FS_WEIGHT_SUBPIXEL   | 采用sub-pixel技术渲染字型                                    |
| slant: FONT_SLANT_ROMAN        | 第二位“r”         | FONT_SLANT_ROMAN     | 不作特殊处理                                                 |
| slant: FONT_SLANT_ITALIC       | 第二位“i”         | FONT_SLANT_ITALIC    | 斜体显示字型                                                 |
| flip: FONT_OTHER_NIL           | 第三位除H/V/T之外 | N/A                  | 不作任何处理                                                 |
| flip: FONT_FLIP_HORZ           | 第三位“H”         | FS_FLIP_HORZ         | 将字型水平翻转显示                                           |
| flip: FONT_FLIP_VERT           | 第三位“V”         | FS_FLIP_VERT         | 将字型垂直翻转显示                                           |
| other: FONT_OTHER_NIL          | 第四位除 S/N之外  | N/A                  | 不作任何处理                                                 |
| other: FONT_OTHER_AUTOSCALE    | 第四位“S”         | FS_OTHER_AUTOSCALE   | 根据逻辑字体期望的大小自动放大显示设备字体字型,仅适用于点阵字体 |
| other: FONT_OTHER_TTFNOCACHE   | 第四位“N”         | FS_OTHER_TTFNOCACHE  | 在使用 TrueType字体渲染该逻辑时,关闭缓存                     |
| other: FONT_OTHER_LCDPORTRAIT  | 第四位“P”         | FS_OTHER_LCDPORTRAIT | 在使用 TrueType字体渲染该逻辑时, LCD为PORTRAIT模式           |
| underline: FONT_UNDERLINE_NONE | 第五位“n”         | FS_UNDERLINE_NONE    | 无下划线线                                                   |
| underline: FONT_UNDERLINE_LINE | 第五位“u”         | FS_UNDERLINE_LINE    | 添加下划线                                                   |
| struckout: FONT_STRUCKOUT_NONE | 第六位“n”         | FS_STRUCKOUT_NONE    | 无删除线                                                     |
| struckout: FONT_STRUCKOUT_LINE | 第六位“s”         | FS_STRUCKOUT_LINE    | 添加删除线                                                   |

# 14  Mginit专用函数

| 函数                   | 含义                                                         |
| ---------------------- | ------------------------------------------------------------ |
| JoinLayer              | 将自己添加到一个层中                                         |
| GetLayerInfo           | 可以得到层的信息                                             |
| SetTopmostLayer        | 将指定的层设置为最顶层                                       |
| DeleteLayer            | 删除层                                                       |
| ServerStartup          | 创建监听套接字                                               |
| ServerSetTopmostLayer  | 将把指定的层切换到最上面                                     |
| ServerCreateLayer      | 将在系统中创建指定的层                                       |
| ServerDeleteLayer      | 从系统中删除指定的层                                         |
| GetClientByPID         | 根据客户的进程标识号返回客户标识号                           |
| SetTopmostClient       | 通过指定的客户标识号来设置顶层,它将把客户所在的层切换到最上面 |
| SetServerEventHook     | 在mginit中设置底层事件的钩子,在钩子函数返回零给MiniGUI时,MiniGUI将继续事件的处理,并最终将事件发送到当前活动客户;反之将终止事件的处理 |
| Send2Client            | 服务器可利用该消息将指定的消息发送到某个客户                 |
| ServerGetNextZNode     | 用于遍历所有的Z序节点                                        |
| ServerGetZNodeInfo     | 用于获得某个特定Z序节点的信息                                |
| ServerDoZNodeOperation | 用于完成切换主窗口Z序和/或设置某主窗口为当前活动窗口         |

| 事件类型            | 含义                              |
| ------------------- | --------------------------------- |
| LCO_NEW_LAYER       | 系统创建了新的层                  |
| LCO_DEL_LAYER       | 系统删除了一个层                  |
| LCO_JOIN_CLIENT     | 某个层中加入了一个客户            |
| LCO_REMOVE_CLIENT   | 某个客户从所在的层中删除          |
| LCO_TOPMOST_CHANGED | 最上面的层改变了,即发生了层的切换 |
| ZNOP_ALLOCATE       | z-node被创建                      |
| ZNOP_FREE           | z-node被销毁                      |
| ZNOP_MOVE2TOP       | z-node被移为TopMost结点           |
| ZNOP_SHOW           | z-node已被显示                    |
| ZNOP_HIDE           | z-node已被隐藏                    |
| ZNOP_MOVEWIN        | z-node已被移动或其大小已改变      |
| ZNOP_SETACTIVE      | z-node已被设置为活动态            |
| ZNOP_ENABLEWINDOW   | z-node被disabled或enabled         |
| ZNOP_STARTDRAG      | 开始对z-node的拖拽                |
| ZNOP_CANCELDRAG     | 取消对z-node的拖拽                |
| ZNOP_CHANGECAPTION  | z-node的名称已改变                |