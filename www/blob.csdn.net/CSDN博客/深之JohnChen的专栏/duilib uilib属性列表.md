# duilib uilib属性列表 - 深之JohnChen的专栏 - CSDN博客

2019年03月20日 00:00:35[byxdaz](https://me.csdn.net/byxdaz)阅读数：62


duilib属性：

` `- 
`<?xml version="1.0" encoding="UTF-8"?>`

- 
`<?xml-stylesheet type="text/xsl" href="属性列表.xslt"?>`

- 
`<!-- 可能有错漏，欢迎补充。wangchyz(wangchyz@gmail.com) -->`

- 
`<Controls>`

- 
`<Window parent="">`

- 
`<Attribute name="size" default="0,0" type="SIZE" comment="窗口的初始化大小,如(800,600)"/>`

- 
`<Attribute name="sizebox" default="0,0,0,0" type="RECT" comment="窗口可拖动改变窗口大小的边距,如(4,4,6,6)"/>`

- 
`<Attribute name="caption" default="0,0,0,0" type="RECT" comment="窗口可拖动的标题栏大小的边距,最后一个参数是指离上边框的距离,如(0,0,0,28)"/>`

- 
`<Attribute name="roundcorner" default="0,0" type="SIZE" comment="窗口圆角大小,如(4,4)"/>`

- 
`<Attribute name="mininfo" default="0,0" type="SIZE" comment="窗口最小大小,如(320,240)"/>`

- 
`<Attribute name="maxinfo" default="0,0" type="SIZE" comment="窗口最大大小,如(1600,1200)"/>`

- 
`<Attribute name="alpha" default="255" type="BYTE" comment="窗口的alpha值(0-255),如(100)"/>`

- 
`<Attribute name="bktrans" default="false" type="BOOL" comment="窗口是否使用静态透明背景,如(false)"/>`

- 
`<Attribute name="disabledfontcolor" default="0xFFA7A6AA" type="DWORD" comment="默认的disabled字体颜色,如(0xFFA7A6AA)"/>`

- 
`<Attribute name="defaultfontcolor" default="0xFF000000" type="DWORD" comment="默认的字体颜色,如(0xFF000000)"/>`

- 
`<Attribute name="linkfontcolor" default="0xFF0000FF" type="DWORD" comment="默认的link字体颜色,如(0xFF0000FF)"/>`

- 
`<Attribute name="linkhoverfontcolor" default="0xFFD3215F" type="DWORD" comment="默认的linkhoverfont字体颜色,如(0xFFD3215F)"/>`

- 
`<Attribute name="selectedcolor" default="0xFFBAE4FF" type="DWORD" comment="默认的selected字体颜色,如(0xFFBAE4FF)"/>`

- 
`<Attribute name="showshadow" default="false" type="BOOL" comment="是否启用窗体阴影"/>`

- 
`<Attribute name="shadowimage" default="" type="STRING" comment="阴影图片，使用此属性后自动屏蔽算法阴影(不支持source等属性设置)"/>`

- 
`<Attribute name="shadowcorner" default="0,0,0,0" type="RECT" comment="图片阴影的九宫格描述"/>`

- 
`<Attribute name="shadowsize" default="0" type="BYTE" comment="算法阴影的宽度(-20到20)"/>`

- 
`<Attribute name="shadowsharpness" default="255" type="BYTE" comment="算法阴影的锐度"/>`

- 
`<Attribute name="shadowdarkness" default="255" type="BYTE" comment="算法阴影的深度(相当于透明度)"/>`

- 
`<Attribute name="shadowpositon" default="0,0" type="SIZE" comment="算法阴影的偏移量"/>`

- 
`<Attribute name="shadowcolor" default="0x000000" type="DWORD" comment="算法阴影的颜色，RGB格式，不支持透明度，使用shadowdarkness设置透明度"/>`

- 
`<Attribute name="gdiplustext" default="false" type="BOOL" comment="是否用gdi+渲染文字（抗锯齿效果更好，但是效率低）"/>`

- 
`</Window>`

- 
`<ChildLayout parent="Container" >`

- 
`<Attribute name="xmlfile" default="" type="STRING" comment="子窗体XML布局文件"/>`

- 
`</ChildLayout>`

- 
`<Control parent="" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="nativebkcolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="0" type="INT | RECT" comment="可以设置INT或RECT类型的值。当值为ING时则左、上、右、[下都](https://www.baidu.com/s?wd=%E4%B8%8B%E9%83%BD&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)用该值作为宽。值为RECT类型时则分别设置左、上、右、下的边框"/>`

- 
`<Attribute name="leftbordersize" default="0" type="INT" comment="左边边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="topbordersize" default="0" type="INT" comment="顶部边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="rightbordersize" default="0" type="INT" comment="右边边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="bottombordersize" default="0" type="INT" comment="底部边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="borderstyle" default="0" type="INT" comment="边框样式的设置,数值范围0-5"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="keyboard" default="true" type="BOOL" comment="非CButtonUI类忽略该值，为false时不支持TAB_STOP,且该对象不处理键盘信息"/>`

- 
`<Attribute name="virtualwnd" default="" type="string" comment="设置控件的虚拟窗体的名字"/>`

- 
`</Control>`

- 
`<Container parent="Control" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="relativepos" default="0,0,0,0" type="RECT" comment="设置相对移动，float为真时，分别表示横纵向位移值，横纵向缩放值，移动单位建议50或100"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`</Container>`

- 
`<VerticalLayout parent="Container" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="sepheight" default="0" type="INT" comment="分隔符高度,正负表示分隔符在顶部还是底部,如(4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`</VerticalLayout>`

- 
`<HorizontalLayout parent="Container" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="sepwidth" default="0" type="INT" comment="分隔符宽,正负表示分隔符在左边还是右边,如(-4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`</HorizontalLayout>`

- 
`<TileLayout parent="Container" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="columns" default="1" type="INT" comment="列数,如(4)"/>`

- 
`<Attribute name="itemsize" default="0,0" type="SIZE" comment="子项固定大小，如(128,128)"/>`

- 
`</TileLayout>`

- 
`<TabLayout parent="Container" notifies="setfocus killfocus timer menu tabselect windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="DWORD" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="selectedid" default="0" type="INT" comment="默认选中的页面id,如(0)"/>`

- 
`</TabLayout>`

- 
`<ActiveX parent="Control" notifies="setfocus killfocus timer menu showactivex windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="clsid" default="" type="STRING" comment="activex的clsid,如({8856F961-340A-11D0-A96B-00C04FD705A2})"/>`

- 
`<Attribute name="modulename" default="" type="STRING" comment="activex从指定位置加载,如(flash/flash.ocx)"/>`

- 
`<Attribute name="delaycreate" default="true" type="BOOL" comment="是否需要延迟创建activex,如(false)"/>`

- 
`</ActiveX>`

- 
`<WebBrowser parent="ActiveX" >`

- 
`<Attribute name="homepage" default="" type="STRING" comment="默认首页" />`

- 
`<Attribute name="autonavi" default="false" type="BOOL" comment="是否打开默认首页" />`

- 
`</WebBrowser>`

- 
`<Combo parent="Container" notifies="setfocus killfocus timer menu dropdown itemselect windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮的状态图片"/>`

- 
`<Attribute name="pushedimage" default="" type="STRING" comment="鼠标按下的状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点时的状态图片"/>`

- 
`<Attribute name="disabledimage" default="" type="STRING" comment="禁用的状态图片"/>`

- 
`<Attribute name="dropboxsize" default="0,150" type="STRING" comment="弹出框大小设置"/>`

- 
`<Attribute name="itemfont" default="-1" type="INT" comment="item的字体id,如(0)"/>`

- 
`<Attribute name="itemalign" default="center" type="STRING" comment="item对齐方式,取值left、right、center，如(center)"/>`

- 
`<Attribute name="itemendellipsis" default="false" type="BOOL" comment="item句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="itemtextpadding" default="0,0,0,0" type="RECT" comment="item文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="itemtextcolor" default="0xFF000000" type="DWORD" comment="item字体颜色"/>`

- 
`<Attribute name="itembkcolor" default="0x00000000" type="DWORD" comment="item背景颜色"/>`

- 
`<Attribute name="itembkimage" default="" type="STRING" comment="item背景图片"/>`

- 
`<Attribute name="itemaltbk" default="false" type="BOOL" comment="item是否使用隔行交替背景"/>`

- 
`<Attribute name="itemselectedtextcolor" default="0xFF000000" type="DWORD" comment="item被选中时的字体颜色"/>`

- 
`<Attribute name="itemselectedbkcolor" default="0xFFC1E3FF" type="DWORD" comment="item被选中时的背景颜色"/>`

- 
`<Attribute name="itemselectedimage" default="" type="STRING" comment="item被选中时的背景图片"/>`

- 
`<Attribute name="itemhottextcolor" default="0xFF000000" type="DWORD" comment="item鼠标悬浮时的字体颜色"/>`

- 
`<Attribute name="itemhotbkcolor" default="0xFFE9F5FF" type="DWORD" comment="item鼠标悬浮时的背景颜色"/>`

- 
`<Attribute name="itemhotimage" default="" type="STRING" comment="item鼠标悬浮时的背景图片"/>`

- 
`<Attribute name="itemdisabledtextcolor" default="0xFFCCCCCC" type="DWORD" comment="item禁用时的字体颜色"/>`

- 
`<Attribute name="itemdisabledbkcolor" default="0xFFFFFFFF" type="DWORD" comment="item禁用时的背景颜色"/>`

- 
`<Attribute name="itemdisabledimage" default="" type="STRING" comment="item禁用时的背景图片"/>`

- 
`<Attribute name="itemlinecolor" default="0x00000000" type="DWORD" comment="item行分割线颜色"/>`

- 
`<Attribute name="itemshowhtml" default="false" type="BOOL" comment="item是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="multiexpanding" default="false" type="BOOL" comment="是否支持多个item同时打开,如(false)"/>`

- 
`</Combo>`

- 
`<Label parent="Control" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、center、top、bottom，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="enabledeffect" default="false" type="BOOL" comment="是否使用字体特效，使用下面文字特效属性必须该属性设置为true方有效"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字横向对齐方式,取值left、center、right"/>`

- 
`<Attribute name="valign" default="center" type="STRING" comment="文字纵向对齐方式,取值top、center、bottom"/>`

- 
`<Attribute name="rhaa" default="0" type="STRING" comment="字体质量0-5"/>`

- 
`<Attribute name="enabledstroke" default="false" type="BOOL" comment="是否使用描边效果"/>`

- 
`<Attribute name="strokecolor" default="0x00000000" type="STRING" comment="字体描边的颜色"/>`

- 
`<Attribute name="transstroke" default="255" type="STRING" comment="字体描边的颜色透明度 0-255"/>`

- 
`<Attribute name="enabledshadow" default="false" type="BOOL" comment="是否使用阴影效果"/>`

- 
`<Attribute name="gradientangle" default="0" type="INT" comment="渐变色渲染角度"/>`

- 
`<Attribute name="gradientlength" default="0" type="INT" comment="渐变距离"/>`

- 
`<Attribute name="textcolor1" default="0x00000000" type="STRING" comment="字体渐变色"/>`

- 
`<Attribute name="textshadowcolora" default="0xff000000" type="STRING" comment="字体阴影渐变色"/>`

- 
`<Attribute name="textshadowcolorb" default="0xff000000" type="STRING" comment="字体阴影渐变色"/>`

- 
`<Attribute name="transshadow" default="100" type="INT" comment="阴影透明度"/>`

- 
`<Attribute name="transshadow1" default="100" type="INT" comment="阴影透明度"/>`

- 
`<Attribute name="transtext" default="100" type="INT" comment="字体色彩透明度"/>`

- 
`<Attribute name="transtext1" default="100" type="INT" comment="字体色彩透明度"/>`

- 
`<Attribute name="autocalcwidth" default="false" type="INT" comment="是否自动计算宽度"/> `

- 
`</Label>`

- 
`<GifAnim parent="Control" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="Gif动画图片路径(不支持source等属性设置)"/>`

- 
`<Attribute name="autoplay" default="true" type="BOOL" comment="是否自动播放GIF动画"/>`

- 
`<Attribute name="autosize" default="false" type="BOOL" comment="是否根据图片自动设置控件大小(开启后width和height属性失效)"/>`

- 
`</GifAnim>`

- 
`<Menu parent="CListUI" notifies="setfocus killfocus timer menu itemselect windowinit(root)">`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="菜单的高度等于所有菜单项的高度之和，加上inset属性的top和bottom"/>`

- 
`<!-- Menu标签的属性要通过Default来定义，为了让下级菜单也能拥有同样的外观，其他属性设置见List -->`

- 
`</Menu>`

- 
`<MenuElement parent="CListContainerElementUI" notifies="click valuechanged WM_MENUCLICK">`

- 
`<Attribute name="icon" default="" type="STRING" comment="菜单项的图标图片"/>`

- 
`<Attribute name="iconsize" default="0,0" type="SIZE" comment="图片的大小，最大为26x26"/>`

- 
`<Attribute name="checkitem" default="false" type="BOOL" comment="是否有复选功能"/>`

- 
`<Attribute name="ischeck" default="false" type="BOOL" comment="是否被选中（前提是开启了复选功能，复选功能属性应该写在本属性的前面）"/>`

- 
`<Attribute name="linetype" default="false" type="BOOL" comment="是否是分割线（开启后将不会显示图标）"/>`

- 
`<Attribute name="linepadding" default="29,0,7,0" type="RECT" comment="分割线的外边据"/>`

- 
`<Attribute name="linecolor" default="0xFFBCBFC4" type="DWORD" comment="分割线的颜色"/>`

- 
`<Attribute name="expland" default="false" type="BOOL" comment="是否显示下级菜单的小三角图片（需要通过Default标签设置ExplandIcon属性图片的路径）"/>`

- 
`<Attribute name="height" default="30" type="INT" comment="菜单项高度（分割线默认高度是6）"/>`

- 
`</MenuElement>`

- 
`<Button parent="Label" notifies="setfocus killfocus timer menu click windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮的状态图片"/>`

- 
`<Attribute name="pushedimage" default="" type="STRING" comment="鼠标按下的状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点时的状态图片"/>`

- 
`<Attribute name="disabledimage" default="" type="STRING" comment="禁用的状态图片"/>`

- 
`<Attribute name="hottextcolor" default="0x00000000" type="DWORD" comment="鼠标悬浮字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="pushedtextcolor" default="0x00000000" type="DWORD" comment="鼠标按下字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="focusedtextcolor" default="0x00000000" type="DWORD" comment="获得焦点字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="autocalcwidth" default="false" type="INT" comment="是否自动计算宽度"/> `

- 
`<Attribute name="bindtablayoutname" default="" type="STRING" comment="绑定TabLayout控件"/>`

- 
`<Attribute name="bindtabindex" default="" type="INT" comment="点击后将触发绑定TabLayout控件的对应索引号"/>`

- 
`<Attribute name="multiline" default="false" type="INT" comment="是否支持多行文字"/>`

- 
`</Button>`

- 
`<Option parent="Button" notifies="setfocus killfocus timer menu click selectchanged windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮的状态图片"/>`

- 
`<Attribute name="pushedimage" default="" type="STRING" comment="鼠标按下的状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点时的状态图片"/>`

- 
`<Attribute name="disabledimage" default="" type="STRING" comment="禁用的状态图片"/>`

- 
`<Attribute name="selectedimage" default="" type="STRING" comment="选中的状态图片"/>`

- 
`<Attribute name="selectedhotimage" default="" type="STRING" comment="选中鼠标悬浮的的状态图片"/>`

- 
`<Attribute name="selectedpushedimage" default="" type="STRING" comment="选中鼠标按下的的状态图片"/>`

- 
`<Attribute name="foreimage" default="" type="STRING" comment="前景图片"/>`

- 
`<Attribute name="slectedforeimage" default="" type="STRING" comment="选中的前景图片"/>`

- 
`<Attribute name="hottextcolor" default="0x00000000" type="DWORD" comment="鼠标悬浮字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="pushedtextcolor" default="0x00000000" type="DWORD" comment="鼠标按下字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="focusedtextcolor" default="0x00000000" type="DWORD" comment="获得焦点字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="selectedtextcolor" default="0x00000000" type="DWORD" comment="选中状态字体颜色，0表示不使用此颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="group" default="" type="STRING" comment="所属组的名称，可不设"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中"/>`

- 
`<Attribute name="bindtablayoutname" default="" type="STRING" comment="绑定TabLayout控件"/>`

- 
`<Attribute name="bindtabindex" default="" type="INT" comment="点击后将触发绑定TabLayout控件的对应索引号"/>`

- 
`</Option>`

- 
`<Text parent="Label" notifies="setfocus killfocus timer menu link windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="2,0,2,0" type="RECT" comment="文字显示的边距,如(2,0,2,0)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="autocalcwidth" default="false" type="INT" comment="是否自动计算宽度"/> `

- 
`</Text>`

- 
`<Progress parent="Label" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="certer" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="2,0,2,0" type="RECT" comment="文字显示的边距,如(2,0,2,0)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="foreimage" default="" type="STRING" comment="前景图片"/>`

- 
`<Attribute name="hor" default="true" type="BOOL" comment="水平或垂直,如(true)"/>`

- 
`<Attribute name="min" default="0" type="INT" comment="进度最小值，如(0)"/>`

- 
`<Attribute name="max" default="100" type="INT" comment="进度最大值，如(100)"/>`

- 
`<Attribute name="value" default="0" type="INT" comment="进度值，如(50)"/>`

- 
`<Attribute name="isstretchfore" default="TRUE" type="BOOL" comment="指定前景图片是否缩放显示"/>`

- 
`<Attribute name="autocalcwidth" default="false" type="INT" comment="是否自动计算宽度"/> `

- 
`</Progress>`

- 
`<Slider parent="Progress" notifies="setfocus killfocus timer menu valuechanged movevaluechanged windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="center" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="2,0,2,0" type="RECT" comment="文字显示的边距,如(2,0,2,0)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="foreimage" default="" type="STRING" comment="前景图片"/>`

- 
`<Attribute name="hor" default="true" type="BOOL" comment="水平或垂直,如(true)"/>`

- 
`<Attribute name="min" default="0" type="INT" comment="进度最小值，如(0)"/>`

- 
`<Attribute name="max" default="100" type="INT" comment="进度最大值，如(100)"/>`

- 
`<Attribute name="value" default="0" type="INT" comment="进度值，如(50)"/>`

- 
`<Attribute name="thumbimage" default="" type="STRING" comment="拖动滑块普通状态图片"/>`

- 
`<Attribute name="thumbhotimage" default="" type="STRING" comment="拖动滑块鼠标悬浮状态图片"/>`

- 
`<Attribute name="thumbpushedimage" default="" type="STRING" comment="拖动滑块鼠标按下状态图片"/>`

- 
`<Attribute name="thumbsize" default="10,10" type="SIZE" comment="拖动滑块大小,如(10,10)"/>`

- 
`<Attribute name="step" default="1" type="INT" comment="进度步长，如(1)"/>`

- 
`<Attribute name="sendmove" default="false" type="BOOL" comment="是否发送movevaluechanged消息"/>`

- 
`</Slider>`

- 
`<Edit parent="Label" notifies="setfocus killfocus timer menu return textchanged windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、center、top、button，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不完是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="disabledtextcolor" default="0x00000000" type="DWORD" comment="disabled字体颜色，0表示使用默认disabled字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="readonly" default="false" type="BOOL" comment="是否只读,如(false)"/>`

- 
`<Attribute name="password" default="false" type="BOOL" comment="是否显示密码字符,如(false)"/>`

- 
`<Attribute name="maxchar" default="255" type="INT" comment="输入字符最大长度，如(100)"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点状态图片"/>`

- 
`<Attribute name="disabledimage" default="" type="STRING" comment="禁用状态图片"/>`

- 
`<Attribute name="nativebkcolor" default="0x00000000" type="DWORD" comment="windows原生edit控件的背景颜色,如(0xFFFFFFFF)"/>`

- 
`<Attribute name="nativetextcolor" default="0x00000000" type="DWORD" comment="windows原生edit控件的文字颜色,如(0xFFFFFFFF)"/>`

- 
`<Attribute name="passwordchar" default="" type="STRING" comment="设置密码字符"/>`

- 
`<Attribute name="tipvalue" default="" type="STRING" comment="文本框内提示文字，当文本框text为空时显示并失去焦点时显示"/>`

- 
`<Attribute name="tipvaluecolor" default="0xFFBAC0C5" type="DWORD" comment="文本框内提示文字的颜色"/>`

- 
`</Edit>`

- 
`<ScrollBar parent="Control" notifies="timer menu scrolled windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="button1normalimage" default="" type="STRING" comment="左或上按钮普通状态图片"/>`

- 
`<Attribute name="button1hotimage" default="" type="STRING" comment="左或上按钮鼠标悬浮状态图片"/>`

- 
`<Attribute name="button1pushedimage" default="" type="STRING" comment="左或上按钮鼠标按下状态图片"/>`

- 
`<Attribute name="button1disabledimage" default="" type="STRING" comment="左或上按钮禁用状态图片"/>`

- 
`<Attribute name="button2normalimage" default="" type="STRING" comment="右或下按钮普通状态图片"/>`

- 
`<Attribute name="button2hotimage" default="" type="STRING" comment="右或下按钮鼠标悬浮状态图片"/>`

- 
`<Attribute name="button2pushedimage" default="" type="STRING" comment="右或下按钮鼠标按下状态图片"/>`

- 
`<Attribute name="button2disabledimage" default="" type="STRING" comment="右或下按钮禁用状态图片"/>`

- 
`<Attribute name="thumbnormalimage" default="" type="STRING" comment="滑块普通状态图片"/>`

- 
`<Attribute name="thumbhotimage" default="" type="STRING" comment="滑块鼠标悬浮状态图片"/>`

- 
`<Attribute name="thumbpushedimage" default="" type="STRING" comment="滑块鼠标按下状态图片"/>`

- 
`<Attribute name="thumbdisabledimage" default="" type="STRING" comment="滑块禁用状态图片"/>`

- 
`<Attribute name="railnormalimage" default="" type="STRING" comment="滑块中间标识普通状态图片"/>`

- 
`<Attribute name="railhotimage" default="" type="STRING" comment="滑块中间标识鼠标悬浮状态图片"/>`

- 
`<Attribute name="railpushedimage" default="" type="STRING" comment="滑块中间标识鼠标按下状态图片"/>`

- 
`<Attribute name="raildisabledimage" default="" type="STRING" comment="滑块中间标识禁用状态图片"/>`

- 
`<Attribute name="bknormalimage" default="" type="STRING" comment="背景普通状态图片"/>`

- 
`<Attribute name="bkhotimage" default="" type="STRING" comment="背景鼠标悬浮状态图片"/>`

- 
`<Attribute name="bkpushedimage" default="" type="STRING" comment="背景鼠标按下状态图片"/>`

- 
`<Attribute name="bkdisabledimage" default="" type="STRING" comment="背景禁用状态图片"/>`

- 
`<Attribute name="hor" default="true" type="BOOL" comment="水平或垂直,如(true)"/>`

- 
`<Attribute name="linesize" default="8" type="INT" comment="滚动一行的大小，如(8)"/>`

- 
`<Attribute name="range" default="100" type="INT" comment="滚动范围，如(100)"/>`

- 
`<Attribute name="value" default="0" type="INT" comment="滚动位置，如(0)"/>`

- 
`<Attribute name="showbutton1" default="true" type="BOOL" comment="是否显示左或上按钮,如(true)"/>`

- 
`<Attribute name="showbutton2" default="true" type="BOOL" comment="是否显示右或下按钮,如(true)"/>`

- 
`</ScrollBar>`

- 
`<List parent="VerticalLayout" notifies="setfocus killfocus timer menu itemselect windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="sepheight" default="0" type="INT" comment="分隔符高度,正负表示分隔符在顶部还是底部,如(4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`<Attribute name="header" default="true" type="BOOL" comment="是否显示表头,如(true),隐藏则设置为hidden"/>`

- 
`<Attribute name="headerbkimage" default="" type="STRING" comment="表头背景图片"/>`

- 
`<Attribute name="scrollselect" default="false" type="BOOL" comment="是否随滚动改变选中项,如(false)"/>`

- 
`<Attribute name="itemfont" default="-1" type="INT" comment="item的字体id,如(0)"/>`

- 
`<Attribute name="itemalign" default="center" type="STRING" comment="item对齐方式,取值left、right、center，如(center)"/>`

- 
`<Attribute name="itemendellipsis" default="false" type="BOOL" comment="item句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="itemtextpadding" default="0,0,0,0" type="RECT" comment="item文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="itemtextcolor" default="0xFF000000" type="DWORD" comment="item字体颜色"/>`

- 
`<Attribute name="itembkcolor" default="0x00000000" type="DWORD" comment="item背景颜色"/>`

- 
`<Attribute name="itembkimage" default="" type="STRING" comment="item背景图片"/>`

- 
`<Attribute name="itemaltbk" default="false" type="BOOL" comment="item是否使用隔行交替背景"/>`

- 
`<Attribute name="itemselectedtextcolor" default="0xFF000000" type="DWORD" comment="item被选中时的字体颜色"/>`

- 
`<Attribute name="itemselectedbkcolor" default="0xFFC1E3FF" type="DWORD" comment="item被选中时的背景颜色"/>`

- 
`<Attribute name="itemselectedimage" default="" type="STRING" comment="item被选中时的背景图片"/>`

- 
`<Attribute name="itemhottextcolor" default="0xFF000000" type="DWORD" comment="item鼠标悬浮时的字体颜色"/>`

- 
`<Attribute name="itemhotbkcolor" default="0xFFE9F5FF" type="DWORD" comment="item鼠标悬浮时的背景颜色"/>`

- 
`<Attribute name="itemhotimage" default="" type="STRING" comment="item鼠标悬浮时的背景图片"/>`

- 
`<Attribute name="itemdisabledtextcolor" default="0xFFCCCCCC" type="DWORD" comment="item禁用时的字体颜色"/>`

- 
`<Attribute name="itemdisabledbkcolor" default="0xFFFFFFFF" type="DWORD" comment="item禁用时的背景颜色"/>`

- 
`<Attribute name="itemdisabledimage" default="" type="STRING" comment="item禁用时的背景图片"/>`

- 
`<Attribute name="itemlinecolor" default="0x00000000" type="DWORD" comment="item行分割线颜色"/>`

- 
`<Attribute name="itemshowhtml" default="false" type="BOOL" comment="item是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="multiexpanding" default="false" type="BOOL" comment="是否支持多个item同时打开,如(false)"/>`

- 
`</List>`

- 
`<ListHeader parent="HorizontalLayout" notifies="setfocus killfocus timer windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="sepwidth" default="0" type="INT" comment="分隔符宽,正负表示分隔符在左边还是右边,如(-4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`<Attribute name="scaleheader" default="false" type="BOOL" comment="每个表头的宽度是否按照百分比来设置"/>`

- 
`</ListHeader>`

- 
`<ListHeaderItem parent="Control" notifies="setfocus killfocus timer headerclick windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="16" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="dragable" default="true" type="BOOL" comment="是否可拖动改变大小,如(true)"/>`

- 
`<Attribute name="sepwidth" default="4" type="INT" comment="分隔符宽,如(4)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、cente，如(center)"/>`

- 
`<Attribute name="endellipsis" default="false" type="BOOL" comment="句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="字体颜色，0表示使用默认字体颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="textpadding" default="2,0,2,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="showhtml" default="false" type="BOOL" comment="是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮的状态图片"/>`

- 
`<Attribute name="pushedimage" default="" type="STRING" comment="鼠标按下的状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点时的状态图片"/>`

- 
`<Attribute name="sepimage" default="" type="STRING" comment="拖动条图片"/>`

- 
`<Attribute name="scale" default="0" type="INT" comment="设置子表头所占总表头的百分比宽度，如40（代表占40%的宽度）"/>`

- 
`</ListHeaderItem>`

- 
`<ListLabelElement parent="Control" notifies="setfocus killfocus timer itemactivate itemclick windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)"/>`

- 
`</ListLabelElement>`

- 
`<ListTextElement parent="ListLabelElement" notifies="setfocus killfocus timer itemactivate itemclick link windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)"/>`

- 
`</ListTextElement>`

- 
`<ListContainerElement parent="Container" notifies="setfocus killfocus timer itemactivate itemclick itemexpanded itemcollapsed windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)"/>`

- 
`</ListContainerElement>`

- 
`<RichEdit parent="Container" notifies="setfocus killfocus timer menu return windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="0" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bordervisible" default="false" type="BOOL" comment="是否显示边框"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="autovscroll" default="false" type="BOOL" comment="是否随输入竖向滚动,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="autohscroll" default="false" type="BOOL" comment="是否随输入横向滚动,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="wanttab" default="true" type="BOOL" comment="是否接受tab按键消息,如(true)"/>`

- 
`<Attribute name="wantreturn" default="true" type="BOOL" comment="是否接受return按键消息,如(true)"/>`

- 
`<Attribute name="wantctrlreturn" default="true" type="BOOL" comment="是否接受ctrl+return按键消息,如(true)"/>`

- 
`<Attribute name="transparent" default="true" type="BOOL" comment="是否背景透明,如(true)"/>`

- 
`<Attribute name="rich" default="true" type="BOOL" comment="是否使用富格式,如(true)"/>`

- 
`<Attribute name="multiline" default="true" type="BOOL" comment="是否使用多行,如(true)"/>`

- 
`<Attribute name="readonly" default="false" type="BOOL" comment="是否只读,如(false)"/>`

- 
`<Attribute name="password" default="false" type="BOOL" comment="是否显示密码符,如(true)"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字对齐方式,取值left、right、cente，如(center)"/>`

- 
`<Attribute name="font" default="-1" type="INT" comment="字体id,如(0)"/>`

- 
`<Attribute name="textcolor" default="0xFF000000" type="DWORD" comment="字体颜色，如(0xFFFF0000)"/>`

- 
`<Attribute name="maxchar" default="-1" type="INT" comment="输入字符最大长度，如(100),默认为无限制"/>`

- 
`<Attribute name="normalimage" default="" type="STRING" comment="普通状态图片"/>`

- 
`<Attribute name="hotimage" default="" type="STRING" comment="鼠标悬浮状态图片"/>`

- 
`<Attribute name="focusedimage" default="" type="STRING" comment="获得焦点状态图片"/>`

- 
`<Attribute name="disabledimage" default="" type="STRING" comment="禁用状态图片"/>`

- 
`<Attribute name="textpadding" default="0,0,0,0" type="RECT" comment="文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="tipvalue" default="" type="STRING" comment="文本框内提示文字，当文本框text为空时显示并失去焦点时显示"/>`

- 
`<Attribute name="tipvaluecolor" default="0xFFBAC0C5" type="DWORD" comment="文本框内提示文字的颜色"/>`

- 
`</RichEdit>`

- 
`<TreeView parent="List" notifies="selectchanged setfocus killfocus timer menu itemselect windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="vscrollbarstyle" default="" type="STRING" comment="设置本容器的纵向滚动条的样式"/>`

- 
`<Attribute name="hscrollbarstyle" default="" type="STRING" comment="设置本容器的横向滚动条的样式"/>`

- 
`<Attribute name="sepheight" default="0" type="INT" comment="分隔符高度,正负表示分隔符在顶部还是底部,如(4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`<Attribute name="header" default="true" type="BOOL" comment="是否显示表头,如(true)"/>`

- 
`<Attribute name="headerbkimage" default="" type="STRING" comment="表头背景图片"/>`

- 
`<Attribute name="scrollselect" default="false" type="BOOL" comment="是否随滚动改变选中项,如(false)"/>`

- 
`<Attribute name="itemfont" default="-1" type="INT" comment="item的字体id,如(0)"/>`

- 
`<Attribute name="itemalign" default="center" type="STRING" comment="item对齐方式,取值left、right、center，如(center)"/>`

- 
`<Attribute name="itemendellipsis" default="false" type="BOOL" comment="item句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="itemtextpadding" default="0,0,0,0" type="RECT" comment="item文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="itemtextcolor" default="0xFF000000" type="DWORD" comment="item字体颜色"/>`

- 
`<Attribute name="itembkcolor" default="0x00000000" type="DWORD" comment="item背景颜色"/>`

- 
`<Attribute name="itembkimage" default="" type="STRING" comment="item背景图片"/>`

- 
`<Attribute name="itemaltbk" default="false" type="BOOL" comment="item是否使用隔行交替背景"/>`

- 
`<Attribute name="itemselectedtextcolor" default="0xFF000000" type="DWORD" comment="item被选中时的字体颜色"/>`

- 
`<Attribute name="itemselectedbkcolor" default="0xFFC1E3FF" type="DWORD" comment="item被选中时的背景颜色"/>`

- 
`<Attribute name="itemselectedimage" default="" type="STRING" comment="item被选中时的背景图片"/>`

- 
`<Attribute name="itemhottextcolor" default="0xFF000000" type="DWORD" comment="item鼠标悬浮时的字体颜色"/>`

- 
`<Attribute name="itemhotbkcolor" default="0xFFE9F5FF" type="DWORD" comment="item鼠标悬浮时的背景颜色"/>`

- 
`<Attribute name="itemhotimage" default="" type="STRING" comment="item鼠标悬浮时的背景图片"/>`

- 
`<Attribute name="itemdisabledtextcolor" default="0xFFCCCCCC" type="DWORD" comment="item禁用时的字体颜色"/>`

- 
`<Attribute name="itemdisabledbkcolor" default="0xFFFFFFFF" type="DWORD" comment="item禁用时的背景颜色"/>`

- 
`<Attribute name="itemdisabledimage" default="" type="STRING" comment="item禁用时的背景图片"/>`

- 
`<Attribute name="itemlinecolor" default="0x00000000" type="DWORD" comment="item行分割线颜色"/>`

- 
`<Attribute name="itemshowhtml" default="false" type="BOOL" comment="item是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="multiexpanding" default="false" type="BOOL" comment="是否支持多个item同时打开,如(false)"/>`

- 
`<!--TreeView 私有属性-->`

- 
`<Attribute name="multipleitem" default="true" type="BOOL" comment="是否允许item多选"/>`

- 
`<Attribute name="itemcheckimgsize" default="0,0" type="SIZE" comment="Item的复选框图片大小,如(2,2)"/>`

- 
`<Attribute name="itemiconimgsize" default="0,0" type="SIZE" comment="Item的图标大小,如(2,2)"/>`

- 
- 
`<Attribute name="visiblefolderbtn" default="true" type="BOOL" comment="是否显示展开与收缩按钮对象"/>`

- 
`<Attribute name="visiblecheckbtn" default="false" type="BOOL" comment="是否显示复选框对象"/>`

- 
`<Attribute name="itemminwidth" default="0" type="UINT" comment="设置Item的最小宽度，当hscrollbar为真且itemminwidth大于TreeView宽度时才会显示横向滚动条"/>`

- 
`<Attribute name="itemtextcolor" default="0x00000000" type="DWORD" comment="item文本颜色"/>`

- 
`<Attribute name="itemhottextcolor" default="0x00000000" type="DWORD" comment="鼠标进入item时文本颜色"/>`

- 
`<Attribute name="selitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时文本颜色"/>`

- 
`<Attribute name="selitemhottextcolor" default="0x00000000" type="DWORD" comment="item被选中时且鼠标进入时的文本颜色"/>`

- 
`</TreeView>`

- 
`<TreeNode parent="ListContainerElement" notifies="setfocus killfocus timer itemactivate itemclick itemexpanded itemcollapsed windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true),同时禁用或者启用子控件"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)"/>`

- 
`<!--TreeNode 私有属性-->`

- 
`<Attribute name="horizattr" default="" type="STRING" comment="item虚线、复选框、展开与收缩、文本按钮等对象容器属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="dotlineattr" default="" type="STRING" comment="item虚线对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="folderattr" default="" type="STRING" comment="item展开与收缩按钮对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="checkboxattr" default="" type="STRING" comment="item复选框对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="itemattr" default="" type="STRING" comment="item按钮对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="item文本颜色"/>`

- 
`<Attribute name="texthotcolor" default="0x00000000" type="DWORD" comment="鼠标进入item时文本颜色"/>`

- 
`<Attribute name="selitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时文本颜色"/>`

- 
`<Attribute name="selhotitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时且鼠标进入时的文本颜色"/>`

- 
`<Attribute name="defaultexpand" default="false" type="BOOL" comment="设置初始状态是否展开"/>`

- 
`</TreeNode>`

- 
`</Controls>`


UiLib扩展后的属性：

` `- 
`<?xml version="1.0" encoding="UTF-8"?>`

- 
`<?xml-stylesheet type="text/xsl" href="属性列表.xslt"?>`

- 
`<!-- 以下仅列出对DuiLib扩展后支持的新特性。可能有错漏，欢迎补充。(UiLib@qq.com) -->`

- 
`<Controls>`

- 
`<Window parent="">`

- 
`<Attribute name="trayiconid" default="" type="UINT" comment="图标资源ID，将显示在托盘"/>`

- 
`<Attribute name="traytiptext" default="" type="STRING" comment="托盘图标在鼠标闲停时提示的文本，备注：有效长度小于64字节"/>`

- 
`</Window>`

- 
`<Include parent="Container" >`

- 
`<Attribute name="xmlfile" default="" type="STRING" comment="外部XML布局文件"/>`

- 
`<Attribute name="count" default="1" type="INT" comment="重复创建xmlfile布局的个数"/>`

- 
`</Include> `

- 
`<!--动作组定义标识-->`

- 
`<ActionScript parent="" />`

- 
`<!--控件属性动作组定义标识-->`

- 
`<AGroup parent="ActionScript" >`

- 
`<Attribute name="name" default="" type="STRING" comment="动作样式组名称"/>`

- 
`<Attribute name="msgtype" default="" type="STRING" comment="动作样式组的触发机制类型 notify 或 event"/>`

- 
`<Attribute name="msgvalue" default="" type="notify=STRING | event=INT" comment="对应触发机制类型 notify 或 event 的值notify则为字符串，event则为[整型](https://www.baidu.com/s?wd=%E6%95%B4%E5%9E%8B&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)"/>`

- 
`<Attribute name="defaultinterval" default="" type="INT" comment="缺省触发周期"/>`

- 
`<Attribute name="defaulttimer" default="" type="INT" comment="缺省总周期之和"/>`

- 
`<Attribute name="defaultautostart" default="" type="BOOL" comment="缺省是否自动触发"/>`

- 
`<Attribute name="defaultloop" default="" type="BOOL" comment="缺省周期之和到后是否续集循环触发"/>`

- 
`<Attribute name="defaultreverse" default="" type="BOOL" comment="缺省周期之和到后是否倒序触发"/>`

- 
`</AGroup>`

- 
`<!--控件属性动作定义-->`

- 
`<Property parent="AGroup" >`

- 
`<Attribute name="name" default="" type="STRING" comment="指定需要动画处理的控件属性名称"/>`

- 
`<Attribute name="type" default="" type="STRING" comment="指定过度的属性值类型，支持（int,size,rect,color,image.source,image.corner,image.desc,image.fade,image.mask"/>`

- 
`<Attribute name="startvalue" default="" type="STRING" comment="值的起始值"/>`

- 
`<Attribute name="endvalue" default="" type="STRING" comment="值的终点值"/>`

- 
`<Attribute name="interval" default="" type="" comment="若定义了该值且根AGroup定义的缺省值defaultinterval不同，则独立运作。"/>`

- 
`<Attribute name="timer" default="" type="" comment="若定义了该值且根AGroup定义的缺省值defaulttimer不同，则独立运作。"/>`

- 
`<Attribute name="autostart" default="" type="" comment="若定义了该值且根AGroup定义的缺省值defaultautostart不同，则独立运作。"/>`

- 
`<Attribute name="loop" default="" type="" comment="若定义了该值且根AGroup定义的缺省值defaultloop不同，则独立运作。"/>`

- 
`<Attribute name="reverse" default="" type="" comment="若定义了该值且根AGroup定义的缺省值defaultreverse不同，则独立运作。"/>`

- 
`</Property>`

- 
`<!--控件属性样式组定义标识-->`

- 
`<Styles parent="">`

- 
`<Attribute name="name" default="" type="STRING" comment="样式模版名称"/>`

- 
`<Attribute name="default" default="" type="BOOL" comment="是否作为当前默认模版"/>`

- 
`<Attribute name="inherit" default="" type="STRING" comment="从指定的模版中继承样式"/>`

- 
`<Attribute name="stylefile" default="" type="STRING" comment="从XML模版中继承"/>`

- 
`</Styles>`

- 
`<Style parent="">`

- 
`<Attribute name="stylename" default="" type="STRING" comment="样式名称"/>`

- 
`<!--`

- 
`除stylename属性外可将任意控件属性作为模版属性。`

- 
`若stylename等于XML的节点元控件元素名，则将作为控件的默认样式：如<Style stylename="Button" bkcolor="#ffffffff" />则所有[Button控件](https://www.baidu.com/s?wd=Button%E6%8E%A7%E4%BB%B6&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的默认背景为#ffffffff`

- 
`包括控件的name属性，也可定义到样式中。`

- 
`按控件的具体拥有的属性进行组合。`

- 
`相同属性名，则为最后个生效。`

- 
`-->`

- 
`<Attribute 控件属性名称="控件属性值" default="" type="" comment="定义的控件属性组，控件自身属性将覆盖模版样式属性"/>`

- 
`</Style>`

- 
- 
`<EffectsStyles parent="">`

- 
`<Attribute name="stylefile" default="" type="STRING" comment="外部XML文件加载样式定义"/>`

- 
`</EffectsStyles>`

- 
- 
`<EffectsStyle parent="">`

- 
`<Attribute name="name" default="" type="STRING" comment="定义动画特效的样式名称"/>`

- 
`<Attribute name="value" default="" type="STRING" comment="定义动画特效的参数"/>`

- 
`</EffectsStyle>`

- 
- 
`<Control parent="" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<Attribute name="leftbordersize" default="0" type="INT" comment="左边边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="topbordersize" default="0" type="INT" comment="顶部边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="rightbordersize" default="0" type="INT" comment="右边边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="bottombordersize" default="0" type="INT" comment="底部边框大小，如(1)，设置该值大于0，则将忽略bordersize属性的设置"/>`

- 
`<Attribute name="borderstyle" default="0" type="INT" comment="边框样式的设置,数值范围0-5"/>`

- 
`<Attribute name="animeffects" default="false" type="BOOL" comment="是否开启动画特效,如(true)"/>`

- 
`<!--EffectsStyle value 属性说明-->`

- 
`<!--zoom 整型：缩放设置 0为不缩放 小于0缩小 大于0放大-->`

- 
`<!--fillingbk 字符串：动画渲染时的背景设置，设置为none或#FFFFFFFF时为保持原背景，否则填充成设置的背景色-->`

- 
`<!--offsetx 整型：动画X坐标偏移量-->`

- 
`<!--offsety 整型：动画Y坐标偏移量-->`

- 
`<!--alpha 整型：动画透明度 -255至255 -->`

- 
`<!--rotation 浮点：动画旋转角度 -->`

- 
`<!--needtimer 整型：动画需要时长，单位毫秒建议大于150毫秒 -->`

- 
`<!--anim 使用简易特效模版，模版特效种类 【zoom+放大】【zoom-缩小】【left2right左到右】【right2left右到左】【top2bottom上到下】【bottom2top下到上】-->`

- 
`<!--offset 整型：简易特效坐标偏移量 -->`

- 
`<!--备注 easyeffects 属性设置会覆盖 adveffects属性的设置 -->`

- 
`<Attribute name="easyeffects" default="" type="STRING" comment="简易的自定义调用的动画特效。(action='' zoom='5' fillingbk='none' offsetx='0' offsety='0' alpha='255' rotation='0.0' needtimer='350' )"/>`

- 
`<Attribute name="adveffects" default="" type="STRING" comment="自定义调用的动画特效。如(zoom='5' fillingbk='none' offsetx='0' offsety='0' alpha='255' rotation='0.0' needtimer='350' )"/>`

- 
`<Attribute name="effectstyle" default="" type="STRING" comment="自定义调用的特效样式名称模。"/>`

- 
`<Attribute name="mouseineffects" default="" type="STRING" comment="鼠标进入控件时的动画特效"/>`

- 
`<Attribute name="mouseouteffects" default="" type="STRING" comment="鼠标离开控件时的动画特效"/>`

- 
`<Attribute name="mouseclickeffects" default="" type="STRING" comment="鼠标点击控件时的动画特效(仅Button控件支持该属性)"/>`

- 
`<Attribute name="mouseinstyle" default="" type="STRING" comment="鼠标进入控件时所调用的的动画特样式"/>`

- 
`<Attribute name="mouseoutstyle" default="" type="STRING" comment="鼠标离开控件时所调用的的动画特样式"/>`

- 
`<Attribute name="mouseclickstyle" default="" type="STRING" comment="鼠标点击控件时所调用的的动画特样式(仅Button控件支持该属性)"/>`

- 
`</Control>`

- 
- 
`<Label parent="Control" notifies="setfocus killfocus timer menu windowinit(root)">`

- 
`<!--UiLib 扩展更新的属性-->`

- 
`<Attribute name="enabledeffect" default="false" type="BOOL" comment="是否使用字体特效，使用下面文字特效属性必须该属性设置为true方有效"/>`

- 
`<Attribute name="align" default="left" type="STRING" comment="文字横向对齐方式,取值left、center、right"/>`

- 
`<Attribute name="valign" default="center" type="STRING" comment="文字纵向对齐方式,取值top、center、bottom"/>`

- 
`<Attribute name="rhaa" default="0" type="STRING" comment="字体质量0-5"/>`

- 
- 
`<Attribute name="enabledstroke" default="false" type="BOOL" comment="是否使用描边效果"/>`

- 
`<Attribute name="strokecolor" default="0x00000000" type="STRING" comment="字体描边的颜色"/>`

- 
`<Attribute name="transstroke" default="255" type="STRING" comment="字体描边的颜色透明度 0-255"/>`

- 
- 
`<Attribute name="enabledshadow" default="false" type="BOOL" comment="是否使用阴影效果"/>`

- 
`<Attribute name="gradientangle" default="0" type="INT" comment="渐变色渲染角度"/>`

- 
`<Attribute name="gradientlength" default="0" type="INT" comment="渐变距离"/>`

- 
`<Attribute name="textcolor1" default="0x00000000" type="STRING" comment="字体渐变色"/>`

- 
`<Attribute name="textshadowcolora" default="0xff000000" type="STRING" comment="字体阴影渐变色"/>`

- 
`<Attribute name="textshadowcolorb" default="0xff000000" type="STRING" comment="字体阴影渐变色"/>`

- 
`<Attribute name="transshadow" default="100" type="INT" comment="阴影透明度"/>`

- 
`<Attribute name="transshadow1" default="100" type="INT" comment="阴影透明度"/>`

- 
`<Attribute name="transtext" default="100" type="INT" comment="字体色彩透明度"/>`

- 
`<Attribute name="transtext1" default="100" type="INT" comment="字体色彩透明度"/>`

- 
`</Label>`

- 
- 
`<!--UiLib 扩展事件 OnSelcetDay 日期选择完成时触发该事件-->`

- 
`<Button parent="Label" notifies="OnSelcetDay setfocus killfocus timer menu click windowinit(root)">`

- 
`<!--UiLib 扩展属性-->`

- 
`<Attribute name="hotbkcolor" default="" type="STRING" comment="鼠标悬浮在按钮上时的按钮背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="foreimage" default="" type="STRING" comment="最前端图片"/>`

- 
`<Attribute name="hotforeimage" default="" type="STRING" comment="获得焦点时的最前端状态图片"/>`

- 
`<Attribute name="bindtablayoutname" default="" type="STRING" comment="绑定TabLayout控件"/>`

- 
`<Attribute name="bindtabindex" default="" type="INT" comment="点击后将触发绑定TabLayout控件的对应索引号"/>`

- 
`</Button>`

- 
- 
`<Option parent="Button" notifies="setfocus killfocus timer menu click selectchanged windowinit(root)">`

- 
`<!--UiLib 扩展属性-->`

- 
`<Attribute name="hotbkcolor" default="" type="STRING" comment="鼠标悬浮在按钮上时的背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="selectedbkcolor" default="" type="STRING" comment="选中时的背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="selectedhotimage" default="" type="STRING" comment="选中时的鼠标悬浮背景图片"/>`

- 
`<Attribute name="foreimage" default="" type="STRING" comment="最前端图片，通常用在Icon的贴图"/>`

- 
`<Attribute name="hotforeimage" default="" type="STRING" comment="获得焦点时的最前端状态图片"/>`

- 
- 
`<Attribute name="bindtablayoutname" default="" type="STRING" comment="绑定TabLayout控件"/>`

- 
`<Attribute name="bindtabindex" default="" type="INT" comment="点击后将触发绑定TabLayout控件的对应索引号"/>`

- 
`</Option>`

- 
- 
`<!--UiLib 扩展的UI元素对象-->`

- 
`<CheckBox parent="Option" notifies="setfocus killfocus timer menu click selectchanged windowinit(root)" />`

- 
- 
`<!--UiLib 扩展的UI元素对象-->`

- 
`<RadioBox parent="Option" notifies="setfocus killfocus timer menu click selectchanged windowinit(root)" />`

- 
- 
`<!--Edit UiLib 扩展事件 OnEditTimer OnEditRegex -->`

- 
`<Edit parent="Label" notifies="setfocus killfocus timer menu return textchanged editTimer editRegex windowinit(root)">`

- 
`<!--UiLib 扩展属性-->`

- 
`<Attribute name="disabledbkcolor" default="0xFFF0F0F0" type="STRING" comment="禁用状态背景色"/>`

- 
`<Attribute name="tipvalue" default="" type="STRING" comment="文本框内提示文字，当文本框text为空时显示并失去焦点时显示"/>`

- 
`<Attribute name="tipvaluecolor" default="0xFFBAC0C5" type="DWORD" comment="文本框内提示文字的颜色"/>`

- 
`<Attribute name="enabletimer" default="false" type="DWORD" comment="当焦点在文本框时是否开启定时器，开启后会定时收到OnEditTimer事件，一般用在自动完成功能上，达到定时取值的效果"/>`

- 
`<Attribute name="timerdelay" default="1000" type="DWORD" comment="enabletimer为true时才属性才有效，设置OnEditTimer事件的触发间隔"/>`

- 
`<Attribute name="regularcheck" default="" type="STRING" comment="正则规则，在Edit失去焦点时进行正则匹配，匹配时并触发OnEditRegex事件，通过该事件wParam取得匹配成功(IDOK)或失败(IDNO)的状态"/>`

- 
`<Attribute name="regulartip" default="" type="STRING" comment="匹配失败时的提示信息，为空时不提示"/>`

- 
`</Edit>`

- 
- 
`<List parent="VerticalLayout" notifies="headerwidthchanged setfocus killfocus timer menu itemselect windowinit(root)">`

- 
`<!--UiLib 扩展属性-->`

- 
`<Attribute name="multipleitem" default="true" type="BOOL" comment="是否允许item多选"/>`

- 
`<Attribute name="itemcheckimgsize" default="0,0" type="SIZE" comment="Item的复选框图片大小,如(2,2)"/>`

- 
`<Attribute name="itemiconimgsize" default="0,0" type="SIZE" comment="Item的图标大小,如(2,2)"/>`

- 
`</List>`

- 
- 
`<!--ListImageTextElement UiLib 扩展元素 -->`

- 
`<ListImageTextElement parent="ListLabelElement" notifies="setfocus killfocus timer itemactivate itemclick link windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)" />`

- 
`</ListImageTextElement>`

- 
- 
`<RichEdit parent="Container" notifies="setfocus killfocus timer menu return windowinit(root)">`

- 
`<!--UiLib 扩展属性-->`

- 
`<Attribute name="disabledbkcolor" default="0xFFF0F0F0" type="STRING" comment="禁用状态背景色"/>`

- 
`</RichEdit>`

- 
- 
`<!--UiLib 扩展的UI元素对象-->`

- 
`<TreeView parent="List" notifies="selectchanged setfocus killfocus timer menu itemselect windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="sepheight" default="0" type="INT" comment="分隔符高度,正负表示分隔符在顶部还是底部,如(4)"/>`

- 
`<Attribute name="sepimm" default="false" type="BOOL" comment="拖动分隔符是否立即改变大小,如(false)"/>`

- 
`<Attribute name="header" default="true" type="BOOL" comment="是否显示表头,如(true)"/>`

- 
`<Attribute name="headerbkimage" default="" type="STRING" comment="表头背景图片"/>`

- 
`<Attribute name="scrollselect" default="false" type="BOOL" comment="是否随滚动改变选中项,如(false)"/>`

- 
`<Attribute name="itemfont" default="-1" type="INT" comment="item的字体id,如(0)"/>`

- 
`<Attribute name="itemalign" default="center" type="STRING" comment="item对齐方式,取值left、right、center，如(center)"/>`

- 
`<Attribute name="itemendellipsis" default="false" type="BOOL" comment="item句末显示不全是否使用...代替,如(true)"/>`

- 
`<Attribute name="itemtextpadding" default="0,0,0,0" type="RECT" comment="item文字显示的边距,如(2,2,2,2)"/>`

- 
`<Attribute name="itemtextcolor" default="0xFF000000" type="DWORD" comment="item字体颜色"/>`

- 
`<Attribute name="itembkcolor" default="0x00000000" type="DWORD" comment="item背景颜色"/>`

- 
`<Attribute name="itembkimage" default="" type="STRING" comment="item背景图片"/>`

- 
`<Attribute name="itemaltbk" default="false" type="BOOL" comment="item是否使用隔行交替背景"/>`

- 
`<Attribute name="itemselectedtextcolor" default="0xFF000000" type="DWORD" comment="item被选中时的字体颜色"/>`

- 
`<Attribute name="itemselectedbkcolor" default="0xFFC1E3FF" type="DWORD" comment="item被选中时的背景颜色"/>`

- 
`<Attribute name="itemselectedimage" default="" type="STRING" comment="item被选中时的背景图片"/>`

- 
`<Attribute name="itemhottextcolor" default="0xFF000000" type="DWORD" comment="item鼠标悬浮时的字体颜色"/>`

- 
`<Attribute name="itemhotbkcolor" default="0xFFE9F5FF" type="DWORD" comment="item鼠标悬浮时的背景颜色"/>`

- 
`<Attribute name="itemhotimage" default="" type="STRING" comment="item鼠标悬浮时的背景图片"/>`

- 
`<Attribute name="itemdisabledtextcolor" default="0xFFCCCCCC" type="DWORD" comment="item禁用时的字体颜色"/>`

- 
`<Attribute name="itemdisabledbkcolor" default="0xFFFFFFFF" type="DWORD" comment="item禁用时的背景颜色"/>`

- 
`<Attribute name="itemdisabledimage" default="" type="STRING" comment="item禁用时的背景图片"/>`

- 
`<Attribute name="itemlinecolor" default="0x00000000" type="DWORD" comment="item行分割线颜色"/>`

- 
`<Attribute name="itemshowhtml" default="false" type="BOOL" comment="item是否使用类html富文本绘制,如(false)"/>`

- 
`<Attribute name="multiexpanding" default="false" type="BOOL" comment="是否支持多个item同时打开,如(false)"/>`

- 
- 
`<!--TreeView 独有的属性-->`

- 
`<Attribute name="multipleitem" default="true" type="BOOL" comment="是否允许item多选"/>`

- 
`<Attribute name="itemcheckimgsize" default="0,0" type="SIZE" comment="Item的复选框图片大小,如(2,2)"/>`

- 
`<Attribute name="itemiconimgsize" default="0,0" type="SIZE" comment="Item的图标大小,如(2,2)"/>`

- 
- 
`<Attribute name="visiblefolderbtn" default="true" type="BOOL" comment="是否显示展开与收缩按钮对象"/>`

- 
`<Attribute name="visiblecheckbtn" default="false" type="BOOL" comment="是否显示复选框对象"/>`

- 
`<Attribute name="itemminwidth" default="0" type="UINT" comment="设置Item的最小宽度，当hscrollbar为真且itemminwidth大于TreeView宽度时才会显示横向滚动条"/>`

- 
`<Attribute name="itemtextcolor" default="0x00000000" type="DWORD" comment="item文本颜色"/>`

- 
`<Attribute name="itemhottextcolor" default="0x00000000" type="DWORD" comment="鼠标进入item时文本颜色"/>`

- 
`<Attribute name="selitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时文本颜色"/>`

- 
`<Attribute name="selitemhottextcolor" default="0x00000000" type="DWORD" comment="item被选中时且鼠标进入时的文本颜色"/>`

- 
`</TreeView>`

- 
- 
`<!--UiLib 扩展的UI元素对象-->`

- 
`<TreeNode parent="ListContainerElement" notifies="setfocus killfocus timer itemactivate itemclick itemexpanded itemcollapsed windowinit(root)">`

- 
`<Attribute name="name" default="" type="STRING" comment="控件名字，同一窗口内必须唯一，如(testbtn)"/>`

- 
`<Attribute name="pos" default="0,0,0,0" type="RECT" comment="位置，如果为float控件则指定位置和大小，否则只指定大小,如(0,0,100,100)"/>`

- 
`<Attribute name="padding" default="0,0,0,0" type="RECT" comment="外边距,如(2,2,2,2)"/>`

- 
`<Attribute name="bkcolor" default="0x00000000" type="DWORD" comment="背景颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="bkcolor2" default="0x00000000" type="DWORD" comment="背景渐变色2,和bkcolor配合使用,如(0xFFFFFF00)"/>`

- 
`<Attribute name="bkcolor3" default="0x00000000" type="DWORD" comment="背景渐变色3,和bkcolor、bkcolor2配合使用,如(0xFFFF00FF)"/>`

- 
`<Attribute name="bordercolor" default="0x00000000" type="DWORD" comment="边框颜色,如(0xFF000000)"/>`

- 
`<Attribute name="focusbordercolor" default="0x00000000" type="DWORD" comment="获得焦点时边框的颜色,如(0xFFFF0000)"/>`

- 
`<Attribute name="colorhsl" default="false" type="BOOL" comment="本控件的颜色是否随窗口的hsl变化而变化,如(false)"/>`

- 
`<Attribute name="bordersize" default="1" type="INT" comment="边框大小，如(1)"/>`

- 
`<Attribute name="borderround" default="0,0" type="SIZE" comment="边框圆角半径,如(2,2)"/>`

- 
`<Attribute name="bkimage" default="" type="STRING" comment="背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"/>`

- 
`<Attribute name="width" default="0" type="INT" comment="控件预设的宽度，如(100)"/>`

- 
`<Attribute name="height" default="0" type="INT" comment="控件预设的高度，如(30)"/>`

- 
`<Attribute name="minwidth" default="0" type="INT" comment="控件的最小宽度，如(100)"/>`

- 
`<Attribute name="minheight" default="0" type="INT" comment="控件的最小高度，如(30)"/>`

- 
`<Attribute name="maxwidth" default="9999" type="INT" comment="控件的最大宽度，如(100)"/>`

- 
`<Attribute name="maxheight" default="9999" type="INT" comment="控件的最大高度，如(30)"/>`

- 
`<Attribute name="text" default="" type="STRING" comment="显示文本,如(测试文本)"/>`

- 
`<Attribute name="tooltip" default="" type="STRING" comment="鼠标悬浮提示,如(请在这里输入你的密码)"/>`

- 
`<Attribute name="userdata" default="" type="STRING" comment="自定义标识"/>`

- 
`<Attribute name="enabled" default="true" type="BOOL" comment="是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="mouse" default="true" type="BOOL" comment="本控件是否可以响应鼠标操作,如(true)"/>`

- 
`<Attribute name="mousechild" default="true" type="BOOL" comment="本控件的子控件是否可以响应用户操作,如(true)"/>`

- 
`<Attribute name="visible" default="true" type="BOOL" comment="是否可见,如(true)"/>`

- 
`<Attribute name="float" default="false" type="BOOL" comment="是否使用绝对定位,如(true)"/>`

- 
`<Attribute name="shortcut" default="" type="CHAR" comment="对应的快捷键,如(P)"/>`

- 
`<Attribute name="menu" default="false" type="BOOL" comment="是否需要右键菜单,如(true)"/>`

- 
`<Attribute name="inset" default="0,0,0,0" type="RECT" comment="容器的内边距,如(2,2,2,2)"/>`

- 
`<Attribute name="vscrollbar" default="false" type="BOOL" comment="是否使用竖向滚动条,如(true)"/>`

- 
`<Attribute name="hscrollbar" default="false" type="BOOL" comment="是否使用横向滚动条,如(true)"/>`

- 
`<Attribute name="childpadding" default="0" type="INT" comment="子控件之间的额外距离,如(4)"/>`

- 
`<Attribute name="selected" default="false" type="BOOL" comment="是否选中,如(true)"/>`

- 
- 
`<!--TreeNode 独有的属性-->`

- 
`<Attribute name="horizattr" default="" type="STRING" comment="item虚线、复选框、展开与收缩、文本按钮等对象容器属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="dotlineattr" default="" type="STRING" comment="item虚线对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="folderattr" default="" type="STRING" comment="item展开与收缩按钮对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="checkboxattr" default="" type="STRING" comment="item复选框对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="itemattr" default="" type="STRING" comment="item按钮对象属性设置，格式参考Default元素的属性设置"/>`

- 
`<Attribute name="textcolor" default="0x00000000" type="DWORD" comment="item文本颜色"/>`

- 
`<Attribute name="texthotcolor" default="0x00000000" type="DWORD" comment="鼠标进入item时文本颜色"/>`

- 
`<Attribute name="selitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时文本颜色"/>`

- 
`<Attribute name="selhotitemtextcolor" default="0x00000000" type="DWORD" comment="item被选中时且鼠标进入时的文本颜色"/>`

- 
`</TreeNode>`

- 
`</Controls>`



