# QT自定义精美换肤界面 - fanyun的博客 - CSDN博客
2016年09月30日 08:59:08[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：10940标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
     Qt实际库里的自带组件的样式是系统标准的，要得到更好看的界面，光靠库里的样式是远远不够的，样式表的出现使得使用Qt做出更加漂亮的界面成为可能。
1. Qt样式表的动态加载：
代码如下：
```cpp
QFile file(":/qss/stylesheet.qss");  
file.open(QFile::ReadOnly);  
QString styleSheet = QLatin1String(file.readAll());  
qApp->setStyleSheet(styleSheet);  
file.close();
```
        通过以上代码，可将Qt的样式表动态加载到一个.qss的样式表文件中，从而更加方便代码的调试。
2.QPushButton样式
![](https://img-blog.csdn.net/20160920163622376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#DEB887;
	   
}
QPushButton#pushButton{
       background-color:#F08080;
       
}
QPushButton#pushButton_2{
       background-color:#FF8C00;
	   border-radius: 10px; 
}
QPushButton#pushButton_3{
     
       background-color:#4169E1;
}
QPushButton#pushButton_1{
       
       background-image: url(:qss/Skin/black/tools/tab-image.png);
       border-radius: 10px; 
	   border: 2px groove gray;
	   border-style: outset;
}
```
3.QListWidget样式
![](https://img-blog.csdn.net/20160920164312698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#B8B8B8;
	   
}
QListWidget#listWidget{
      background:rgba(210,240,250,255);
	  color:#00CCFF;border:0px solid gray;
	  padding:0px -2px 5px 5px;}"  
                                 
      "QListWidget::item{width:94px;height:35px;border:0px solid gray;padding-left:8px;}
	  "QListWidget::item:!selected{}"  
	  "QListWidget::item:selected:active{background:#FFFFFF;color:#00CCFF;border-width:-1;}"  
	  "QListWidget::item:selected{background:#FFFFFF;color:#00CCFF;}"  
                                
	  }
```
    自选股1和自选股3是自定义状态，自选股2是选中状态。
4.QComboBox样式
![](https://img-blog.csdn.net/20160918164348438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#B8B8B8;
	   
}
QComboBox#comboBox{
      color:#00CCFF;
	  border: 1px solid gray;
      border-radius: 3px;
	  height: 25px; 
	  padding: 1px 2px 1px 2px;  # 针对于组合框中的文本内容
	  subcontrol-origin: padding;
      subcontrol-position: top right;
      width: 20px;
      color:#00CCFF;
	  border:0px solid gray;
	  border:1px solid #d7d7d7; 
	  border-radius: 3px; 
	  padding: 1px 18px 1px 3px;   
	  QComboBox:editable{ background: white; };  
      QComboBox:!editable{ background: #fbfbfb; color:#666666};
      QComboBox::drop-down{ subcontrol-origin: padding;}; 
      subcontrol-position: top right;
      width: 22px; 
	  border-left-width: 1px;
      border-left-color: #c9c9c9;
      border-left-style: solid; /* just a single line */
      border-top-right-radius: 3px; /* same radius as the QComboBox */
      border-bottom-right-radius: 3px; 
	  
	  QComboBox::down-arrow { image: url(:/down.png);} ;
      QComboBox::down-arrow:on { /* shift the arrow when popup is open */ top: 1px; left: 1px;} ;
  	  QComboBox{font-size: 11px} ;
      QComboBox:!selected{color: red};
      QComboBox:selected:active{background-color: orange }; 
	  QComboBox:selected{color: orange}; 
}
```
5.QProgressBar样式
![](https://img-blog.csdn.net/20160919121745584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#B8B8B8;
	   
}
QProgressBar#progressBar{
      border:none;
      background:#696969;
      border-radius:5px;
      text-align:center;
}
QProgressBar::chunk {
      background-color:rgb(58, 154, 255);
      border-radius:4px;
	 
}
```
6.QLineEdit样式
![](https://img-blog.csdn.net/20160919124408074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160919124739782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QLinlineEdit::lineEdit{
      enabled{color:#1964F};
	  disabled{color:orange};
	  border:1px solid gray;
	  border-radius:5px;
	  padding:0px 10px 0px 20px;
	  background:yellow;
	  selection-background-color: darkgary;
	  background-image:url(:qss/Skin/black/tools/edit.png);
}
```
## 7.QScrollBar样式
![](https://img-blog.csdn.net/20160919140741523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QScrollBar#verticalScrollBar:vertical
{
    width:8px;
    background:rgba(0,0,0,0%);
    margin:0px,0px,0px,0px;
    padding-top:16px;   
    padding-bottom:16px;
}
QScrollBar#verticalScrollBar::handle:vertical
{
    width:8px;
    background:rgba(0,0,0,25%);
    border-radius:4px;   
    min-height:20;
}
QScrollBar#verticalScrollBar::handle:vertical:hover
{
    width:8px;
    background:rgba(0,0,0,50%); 
    border-radius:4px;
    min-height:20;
}
QScrollBar#verticalScrollBar::add-line:vertical   
{
    height:16px;width:16px;
    border-image:url(:qss/Skin/black/tools/appbar.menu.hover.png);
    subcontrol-position:bottom;
}
QScrollBar#verticalScrollBar::sub-line:vertical   
{
    height:16px;width:16px;
    border-image:url(:qss/Skin/black/tools/appbar.menu.put.hover.png);
    subcontrol-position:top;
}
QScrollBar#verticalScrollBar::add-line:vertical:hover   
{
    height:16px;width:16px;
    border-image:url(:qss/Skin/black/tools/appbar.menu.down.png);
    subcontrol-position:bottom;
}
QScrollBar#verticalScrollBar::sub-line:vertical:hover 
{
    height:16px;width:16px;
    border-image:url(:qss/Skin/black/tools/appbar.menu.put.down.png);
    subcontrol-position:top;
}
QScrollBar#verticalScrollBar::add-page:vertical,QScrollBar#verticalScrollBar::sub-page:vertical  
{
    border-style:solid;
    background:white;
    border-radius:4px;
}
```
8.QSlider样式
![](https://img-blog.csdn.net/20160919150033360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160919150044266?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#0066CC;
	   
}
QSlider#horizontalSlider::groove:horizontal {  
border: 1px solid #4A708B;  
background: #orange;  
height: 5px;  
border-radius: 1px;  
padding-left:-1px;  
padding-right:-1px;  
}  
  
QSlider#horizontalSlider::sub-page:horizontal {  
background: qlineargradient(x1:0, y1:0, x2:0, y2:1,   
    stop:0 #B1B1B1, stop:1 #c4c4c4);  
background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  
    stop: 0 #5DCCFF, stop: 1 #1874CD);  
border: 1px solid #4A708B;  
height: 10px;  
border-radius: 2px;  
}  
  
QSlider#horizontalSlider::add-page:horizontal {  
background: #575757;  
border: 0px solid #777;  
height: 10px;  
border-radius: 2px;  
}  
  
QSlider#horizontalSlider::handle:horizontal   
{  
    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,   
    stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  
  
    width: 11px;  
    margin-top: -3px;  
    margin-bottom: -3px;  
    border-radius: 5px;  
}  
  
QSlider#horizontalSlider::handle:horizontal:hover {  
    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,   
    stop:0.778409 rgba(255, 255, 255, 255));  
  
    width: 11px;  
    margin-top: -3px;  
    margin-bottom: -3px;  
    border-radius: 5px;  
}  
  
QSlider#horizontalSlider::sub-page:horizontal:disabled {  
background: #00009C;  
border-color: #999;  
}  
  
QSlider#horizontalSlider::add-page:horizontal:disabled {  
background: #eee;  
border-color: #999;  
}  
  
QSlider#horizontalSlider::handle:horizontal:disabled {  
background: #eee;  
border: 1px solid #aaa;  
border-radius: 4px;  
}
```
9.QTabWidget样式
美化前：
![](https://img-blog.csdn.net/20160920165454751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
美化后：
![](https://img-blog.csdn.net/20160920165144391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
QWidget#centralWidget{
       border-style:solid;
	   background: transparent;
	   background-color:#2F4F4F;
	   
}
QtableWidget#tableWidget:pane{
       background:  transparent; 
       background-color:#FFE4B5;
}
QTabBar::tab {
    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
    border: 0.5px solid #C4C4C3;
    border-bottom-color: #C2C7CB;
    border-top-left-radius: 1px;
    border-top-right-radius: 1px;
    min-width: 8ex;
    padding: 2px;
	color:white;
}
 
QTabBar::tab:selected, QTabBar::tab:hover {
    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
}
 QTabBar::tab:!selected {
    border-color: black;
	border-style:solid;
	background: transparent;
    border-bottom-color: red;
	background-image: url(:qss/Skin/black/tools/tab-image-pressed.png); 
}
 QTabBar::tab:selected {
    border-style:solid;
    border-color: black;
	background: transparent;
    border-bottom-color: red;
	background-image: url(:qss/Skin/black/tools/tab-image.png); 
}
```
如需要增加特殊背景，用如下方式：
代码如下：
```cpp
QTabWidget#tab>QTabBar::tab:first 
{ 
border-image: url(:/Default_N.png); 
} 
QTabWidget#tab>QTabBar::tab:first:hover 
{ 
border-image: url(:/Default_P.png); 
} 
QTabWidget#tab>QTabBar::tab:first:pressed 
{ 
border-image: url(:/Default_P.png); 
} 
QTabWidget#tab>QTabBar::tab:last 
{ 
border-image: url(:/Apply_N.png); 
} 
QTabWidget#tab>QTabBar::tab:last:hover 
{ 
border-image: url(:/Apply_P.png); 
} 
QTabWidget#tab>QTabBar::tab:last:pressed 
{ 
border-image: url(:/Apply_P.png); 
}
```
10.其它使用细节总结：
    自定义的前台背景与后台背景的颜色：
如果需要样一个文本编辑器的背景变为黄色， 下面是代码行：
    qApp->setStyleSheet("QLineEdit { background-color: yellow }");
针对一个对话框的内容中使用 QLineEdit 以及 QLineEdit 的子类的背景都变成黄色， 下面是代码：
         myDialog->setStyleSheet("QLineEdit { background-color: yellow }");
如果只需要制定一个 QLineEdit 的内容， 将使用 QObject::setObjectName() 下面是一个实例：
        myDialog->setStyleSheet("QLineEdit#nameEdit { background-color: yellow }");
同时也可以针对每一个指定的部件做直接的类型设置， 下面是一个实例：
 nameEdit->setStyleSheet("background-color: yellow");
为了做一个鲜明的对比， 将要为文本设置合适的颜色。
 nameEdit->setStyleSheet("color: blue; background-color: yellow");
当然最好的办法还有针对选择的文本来进行设置， 下面设置了一个选择文本的类型属性：
 nameEdit->setStyleSheet("color: blue;"
                         "background-color: yellow;"
                         "selection-color: yellow;"
                         "selection-background-color: blue;");
在有一些情况下， 不需要用户参与， 而有软件设计人员来自己制定样式， 即使这些是有违审美角度。 下面就从应用程序开发角度来设计样式。
*[mandatoryField="true"] { background-color: yellow }
上面的意思是一些强制的区域是需要用 Qt 的属性管理来强制设置成为黄色的背景。
这样一些强制的部件，将需要通过函数来设置当前的属性已经被强制设置， 下面是实现的代码：
 QLineEdit *nameEdit = new QLineEdit(this);
 nameEdit->setProperty("mandatoryField", true);
 QLineEdit *emailEdit = new QLineEdit(this);
 emailEdit->setProperty("mandatoryField", true);
 QSpinBox *ageSpinBox = new QSpinBox(this);
 ageSpinBox->setProperty("mandatoryField", true);
QPushButton：
下面我们将通过一个按钮的部件来设置属性样式：
首先来设置一下样式：
     QPushButton#evilButton { background-color: red }
说明设置的当前的按钮为红色。 作为一个 flat 平滑的按钮时没有边界的。 下面是来改进一下对边界的设置。
    QPushButton#evilButton {
     background-color: red;
     border-style: outset;
     border-width: 2px;
     border-color: beige;
 }
在这里设置了一个边界的类型与边界的宽度。 这样看上去就好多了，文档中无法展现图片， 有兴趣可以去 Qt的变成环境当中去尝试。即使这样设计， 按钮看上去也是显得混乱，与主部件没有办法分开。 首先是在边界设置出一个空间出来， 并且强制的制定最小宽度，与环绕的弧度， 并且提供一个按钮的字体设置， 似的按钮看上去比较好看。
 QPushButton#evilButton {
     background-color: red;
     border-style: outset;
     border-width: 2px;
     border-radius: 10px;
     border-color: beige;
     font: bold 14px;
     min-width: 10em;
     padding: 6px;
 }
如此这样当我们点击按钮的时候按钮也不会发生什么样的深刻变化。 所以就需要指定一个合适的背景颜色与不一样的边界类型。
 QPushButton#evilButton {
     background-color: red;
     border-style: outset;
     border-width: 2px;
     border-radius: 10px;
     border-color: beige;
     font: bold 14px;
     min-width: 10em;
     padding: 6px;
 }
 QPushButton#evilButton:pressed {
     background-color: rgb(224, 0, 0);
     border-style: inset;
 }
指定 QPushButton 菜单指示器的子控制
子控提供了访问子子元素的功能， 例如通常的时候一个按钮将会管理一个菜单，
 QPushButton#evilButton::menu-indicator {
     image: url(myindicator.png);
 }
同时如果美化一个按钮的话， 那么将可以通过定位符来确定美化按钮的路径， 通常可以是一个图片。
 QPushButton::menu-indicator {
     image: url(myindicator.png);
     subcontrol-position: right center;
     subcontrol-origin: padding;
     left: -2px;
 }
经过以上的设置那么 QPushButton 将会在方格的中心显示一个 myindicator.png 的图片。
定制按钮：
QPushButton {
         border: 2px solid #8f8f91;
         border-radius: 6px;
         background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                           stop: 0 #f6f7fa, stop: 1 #dadbde);
         min-width: 80px;
     }
     QPushButton:pressed {
         background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                           stop: 0 #dadbde, stop: 1 #f6f7fa);
     }
     QPushButton:flat {
         border: none; /*  没有边框的按钮  */
     }
     QPushButton:default {
         border-color: navy; /* 使得按钮显示变得更加突出  */
     }
        QPushButton:open { /* when the button has its menu open */
         background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                           stop: 0 #dadbde, stop: 1 #f6f7fa);
     }
     QPushButton::menu-indicator {
         image: url(menu_indicator.png);
         subcontrol-origin: padding;
         subcontrol-position: bottom right;
     }
     QPushButton::menu-indicator:pressed, QPushButton::menu-indicator:open {
         position: relative;
         top: 2px; left: 2px; /* shift the arrow by 2 px */
     }
QSlider：
下面的横向的slider 
    QSlider::groove:horizontal {
         border: 1px solid #999999;
         height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */
         background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);
         margin: 2px 0;
     }
     QSlider::handle:horizontal {
         background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);
         border: 1px solid #5c5c5c;
         width: 18px;
         margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
         border-radius: 3px;
     }
       QSlider::groove:vertical {
         background: red;
         position: absolute; /* absolutely position 4px from the left and right of the widget. setting margins on the widget should work too... */
         left: 4px; right: 4px;
     }
     QSlider::handle:vertical {
         height: 10px;
         background: green;
         margin: 0 -4px; /* expand outside the groove */
     }
     QSlider::add-page:vertical {
         background: white;
     }
     QSlider::sub-page:vertical {
         background: pink;
     }
QSizeGrip：
一般讲通过一个图片进行设置 ：
     QSizeGrip {
         image: url(:/images/sizegrip.png);
         width: 16px;
         height: 16px;
     }
定制QSplitter：
QSplitter::handle {
         image: url(images/splitter.png);
     }
     QSplitter::handle:horizontal {
         height: 2px;
     }
     QSplitter::handle:vertical {
         width: 2px;
     }
QStatusBar：
将提供一个状态栏的边框与项目的定制：
    QStatusBar {
         background: brown;
     }
     QStatusBar::item {
         border: 1px solid red;
         border-radius: 3px;
     }
QTabWidget与QTabBar：
     QTabWidget::pane { /* The tab widget frame */
         border-top: 2px solid #C2C7CB;
     }
 QTabWidget::tab-bar { /* move to the right by 5px */
         left: 5px;      }
     /* Style the tab using the tab sub-control. Note that
         it reads QTabBar _not_ QTabWidget */
 QTabBar::tab {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                     stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
         border: 2px solid #C4C4C3;
         border-bottom-color: #C2C7CB;  /* same as the pane color */
         border-top-left-radius: 4px;
         border-top-right-radius: 4px;
         min-width: 8ex;
         padding: 2px;
     }
 QTabBar::tab:selected, QTabBar::tab:hover {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                     stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
     }
QTabBar::tab:selected {
         border-color: #9B9B9B;
         border-bottom-color: #C2C7CB;  /* same as pane color */
     }
QTabBar::tab:!selected {
         margin-top: 2px;  /* make non-selected tabs look smaller */
     }
QTabWidget::pane {  /* The tab widget frame */
         border-top: 2px solid #C2C7CB;
     }
QTabWidget::tab-bar {
         left: 5px;  /* move to the right by 5px */
     }
/* Style the tab using the tab sub-control. Note that
         it reads QTabBar _not_ QTabWidget */
 QTabBar::tab {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                     stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
         border: 2px solid #C4C4C3;
         border-bottom-color: #C2C7CB;  /* same as the pane color */
         border-top-left-radius: 4px;
         border-top-right-radius: 4px;
         min-width: 8ex;
         padding: 2px;
     }
QTabBar::tab:selected, QTabBar::tab:hover {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                     stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
     }
QTabBar::tab:selected {
         border-color: #9B9B9B;
         border-bottom-color: #C2C7CB;  /* same as pane color */
     }
QTabBar::tab:!selected {
         margin-top: 2px;  /* make non-selected tabs look smaller */
     }
/* make use of negative margins for overlapping tabs */
 QTabBar::tab:selected {
         /* expand/overlap to the left and right by 4px */
         margin-left: -4px;
         margin-right: -4px;
     }
 QTabBar::tab:first:selected {
         margin-left: 0;  /* the first selected tab has nothing to overlap with on the left */
     }
QTabBar::tab:last:selected {
         margin-right: 0;  /* the last selected tab has nothing to overlap with on the right */
     }
QTabBar::tab:only-one {
         margin: 0;  /* 如果只有一个页面, 我们不能重叠边缘  */
     }
QTabWidget::pane { /* The tab widget frame */
         border-top: 2px solid #C2C7CB;
         position: absolute;
         top: -0.5em;
     }
QTabWidget::tab-bar {
         alignment: center;
     }
/* Style the tab using the tab sub-control. Note that
         it reads QTabBar _not_ QTabWidget */
 QTabBar::tab {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                     stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
         border: 2px solid #C4C4C3;
         border-bottom-color: #C2C7CB; /* same as the pane color */
         border-top-left-radius: 4px;
         border-top-right-radius: 4px;
         min-width: 8ex;
         padding: 2px;
     }
QTabBar::tab:selected, QTabBar::tab:hover {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                     stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
     }
QTabBar::tab:selected {
         border-color: #9B9B9B;
         border-bottom-color: #C2C7CB; /* same as pane color */
     }
定制 QTableWidget：
  下面将设置 QTableWidget  的粉色选中区域， 与白色背景。
 QTableWidget {
         selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,
                                     stop: 0 #FF92BB, stop: 1 white);
     }
QTableWidget QTableCornerButton::section {
         background: red;
         border: 2px outset red;
     }
QToolBox：
下面是对工具条的一些选项进行定制
          QToolBar {
         background: red;
         spacing: 3px; /* spacing between items in the tool bar */
     }
     QToolBar::handle {
         image: url(handle.png);  // 可以设置工具条的背景图片
     } 
 定制 QToolBox：
将使用到  tab  的子控部分
            QToolBox::tab {
         background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                     stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                     stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
         border-radius: 5px;
         color: darkgray;
     }
     QToolBox::tab:selected { /* italicize selected tabs */
         font: italic;
         color: white;
     }
定制QToolButton：
     QToolButton { /* all types of tool button */
         border: 2px solid #8f8f91;
         border-radius: 6px;
         background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                           stop: 0 #f6f7fa, stop: 1 #dadbde);
     }
     QToolButton[popupMode="1"] { /* only for MenuButtonPopup */
         padding-right: 20px; /* make way for the popup button */
     }
     QToolButton:pressed {
         background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                           stop: 0 #dadbde, stop: 1 #f6f7fa);
     }
     /* the subcontrols below are used only in the MenuButtonPopup mode */
     QToolButton::menu-button {
         border: 2px solid gray;
         border-top-right-radius: 6px;
         border-bottom-right-radius: 6px;
         /* 16px width + 4px for border = 20px allocated above */
         width: 16px;
     }
     QToolButton::menu-arrow {
         image: url(downarrow.png);
     }
     QToolButton::menu-arrow:open {
         top: 1px; left: 1px; /* shift it a bit */
     }
QTreeView：
QTreeView::branch {
             background: palette(base);
     }
     QTreeView::branch:has-siblings:!adjoins-item {
             background: cyan;
     }
     QTreeView::branch:has-siblings:adjoins-item {
             background: red;
     }
     QTreeView::branch:!has-children:!has-siblings:adjoins-item {
             background: blue;
     }
     QTreeView::branch:closed:has-children:has-siblings {
             background: pink;
     }
     QTreeView::branch:has-children:!has-siblings:closed {
             background: gray;
     }
     QTreeView::branch:open:has-children:has-siblings {
             background: magenta;
     }
     QTreeView::branch:open:has-children:!has-siblings {
             background: green;
     }
QSS、CSS样式表属性参考表：
[http://www.w3school.com.cn/css/css_background.asp](http://www.w3school.com.cn/css/css_background.asp)
