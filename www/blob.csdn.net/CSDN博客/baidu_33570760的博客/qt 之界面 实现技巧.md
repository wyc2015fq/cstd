# qt 之界面 实现技巧 - baidu_33570760的博客 - CSDN博客
2017年05月04日 12:09:18[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2490
 转载自：http://blog.sina.com.cn/s/blog_a6fb6cc90101dech.html
总结一下，在开发Qt的过程中的一些技巧！可遇而不可求。。。
**一、主界面**1、窗口 最小化 最大化 关闭按钮、显示状态自定义    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮
    setWindowFlags(Qt::WindowFlags type)
    Qt::FrameWindowHint:没有边框的窗口
    Qt::WindowStaysOnTopHint:  //总在最上面的窗口
    Qt::CustomizeWindowHint:  //自定义窗口标题栏,以下标志必须与这个标志一起使用才有效,否则窗口将有默认的标题栏
    Qt::WindowTitleHint  //显示窗口标题栏
    Qt::WindowSystemMenuHint  //显示系统菜单
    Qt::WindowMinimizeButtonHint  //显示最小化按钮
    Qt::WindowMaximizeButtonHint  //显示最大化按钮
    Qt::WindowMinMaxButtonsHint  //显示最小化按钮和最大化按钮
    Qt::WindowCloseButtonHint  //显示关闭按钮2、隐藏任务栏显示    setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint）3、关闭按钮实现的功能    hide(); //进行界面隐藏，但是显示托盘，可通过点击托盘实现界面的重新显示
    quit(); //退出系统4、绘制背景图片并且实现圆角效果void paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush;
    brush.setTextureImage(QImage(background_image)); //背景图片
    painter.setBrush(brush);
     painter.setPen(Qt::black);  //边框色
    painter.drawRoundedRect(this->rect(), 5, 5); //圆角5像素
}5、设置应用程序的字体    QFont font("Courier", 10, QFont::Normal, false);
    QApplication::setFont(font);6、设置字体    QFont font = this->font();
    font.setFamily("Courier");  //字体名称
    font.setPixelSize(16);  //字体点大小
    font.setPointSize(18);  //字体像素大小
    font.setBold(true);  //是否加粗
    font.setItalic(true);  //是否斜体
    font.setUnderline(true);  //是否下划线
    this->setFont(font);
字体风格，包括类型、大小、是否加粗、是否斜体等！
也可使用：setStyleSheet("text-align:center; font-size:18px; font-weight:bold; font-style:Courier; color:white; ");
setAlignment(Qt::AlignCenter);  //设置对齐方式7、超链接    QLabel *pLabel = new QLabel(this);
    pLabel->setText(QStringLiteral("超链接"));
    pLabel->setFixedSize(200,100);
    connect(pLabel,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    QDesktopServices::openUrl(QUrl(url));  //openUrl槽里面实现
    pLabel->setOpenExternalLinks(true);
    pLabel->setText("[新浪](http://blog.sina.com.cn/)");8、打开本地文件（夹）    QDesktopServices::openUrl(QUrl::fromLocalFile(local_file));9、显示窗口并激活/提升showNormal();
    raise();
    activateWindow();10、设置滚动条值
    pTextEdit->verticalScrollBar()->setValue(0);  //滚动到最顶层
    pTextEdit->verticalScrollBar()->setValue(pTextEdit->verticalScrollBar()->maximum());  //滚动到最底层11、QToolBar添加拉伸（弹簧）
原以为QToolBar可以通过addStretch()来实现，不想没有此接口，则可通过下来方式实现：
    QWidget *pWidget = new QWidget(this);
    pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); 
    pToolBar->addWidget(pWidget);12、QProgressBar进度条设置文本、样式
QProgressBar *pProgressBar = new QProgressBar(this);
    pProgressBar->setFixedSize(150, 10);
    pProgressBar->setToolTip(QStringLiteral("离下一等级还需要1500经验"));
    pProgressBar->setFormat("12345/50000");
    pProgressBar->setMaximum(100);
    pProgressBar->setValue(10);
    QString strQSS = "QProgressBar {color: white;} \
        QProgressBar:horizontal { \
        text-align: center; \
        border: 1px solid rgb(115, 200, 60); \
        background: rgb(145, 210, 75, 150); \
    } \
    QProgressBar::chunk:horizontal { \
        background: rgb(145, 210, 75); \
    }";
    pProgressBar->setStyleSheet(strQSS);**二、全屏显示与还原**1、遮挡任务栏    showFullScreen()
    showMaximized() 这两个都可以！2、不遮挡任务栏void showMaximize()
{
    //若已经最大化
    if(is_max)
    {
        //恢复界面位置，并设置按钮图标为最大化图标，提示“最大化”
        this->setGeometry(location);
        max_button->setIcon(QIcon("maxbtn"));
        max_button->setToolTip(tr("max"));
    } 
    else 
    {
        //设定当前界面的位置，还原时使用
        location = this->geometry();
        //获取桌面位置，设置为最大化，并设置按钮图标为还原图标，提示“还原”
        QDesktopWidget *desk = QApplication::desktop();
        this->setGeometry(desk->availableGeometry());
        max_button->setIcon(QIcon("restorbtn"));
        max_button->setToolTip(tr("restor"));
    }
    is_max = !is_max;
}
 注：窗口既然可以最大化，当然还要进行还原，is_max为一个bool值变量，表示窗口是否最大化，初始值为false。location为桌面的位置，每次最大化开始先记录当前的位置，等待还原时候使用。
**三、QSS样式**1、QComboBox    comboBox->setStyleSheet("QComboBox{border:1px solid gray;}"
    "QComboBox QAbstractItemView::item{height:25px;}"
    "QComboBox::down-arrow{image:url(:/icon/arrowdown);}"
    "QComboBox::drop-down{border:0px;}");
    comboBox->setView(new QListView());(包括设置下拉箭头、下拉选项高度等)、2、QSpinBox    pSpinBox->setStyleSheet("QSpinBox{border:1px solid gray;}"
    "QSpinBox::up-button{image:url(:/icon/arrow_up);}"
    "QSpinBox::down-button{image:url(:/icon/arrow_down);}");3、QMenuQMenu{
    padding:5px;
    background:white;
    border:1px solid gray;
 }
QMenu::item{
    padding:0px 40px 0px 30px;
    height:25px;
}
QMenu::item:selected:enabled{
    background:lightgray;
    color:white;
}
QMenu::item:selected:!enabled{
    background:transparent;
}
QMenu::separator{
    height:1px;
    background:lightgray;
    margin:5px 0px 5px 0px;
}4、QSliderQSlider::groove:horizontal{
    border:0px;
    height:4px;
}  
QSlider::sub-page:horizontal{
    background:white;
}  
QSlider::add-page:horizontal{
    background:lightgray;
} 
QSlider::handle:horizontal{
    background:white;
    width:10px;
    border-radius:5px;
    margin:-3px 0px -3px 0px;
}5、QCheckBoxQCheckBox{
    spacing: 2px; 
}
QCheckBox::indicator {
    width: 20px;
    height: 20px;
}
QCheckBox::indicator:unchecked {
    image: url(:/login/checkbox);
}
QCheckBox::indicator:unchecked:hover {
    image: url(:/login/checkbox_hover);
}
QCheckBox::indicator:unchecked:pressed {
    image: url(:/login/checkbox_press);
}
QCheckBox::indicator:checked {
    image: url(:/login/checkbox_selected);
}
QCheckBox::indicator:checked:hover {
    image: url(:/login/checkbox_selected_hover);
}
QCheckBox::indicator:checked:pressed {
    image: url(:/login/checkbox_selected_press);
}6、QScrollBar水平滚动条 
QScrollBar:horizontal { 
    max-height: 9px;
    background: transparent;
    padding-left: 9px;
    padding-right: 9px;
}
QScrollBar::handle:horizontal {
    height: 9px;
    min-width: 20px;
    background: rgb(190, 190, 190);
}
QScrollBar::handle:horizontal:hover {
    background: rgb(170, 170, 170);
}
QScrollBar::sub-line:horizontal {
    height: 9px;
    width: 9px;
    background: rgb(220, 220, 220);
    border-image: url(:/CalcSetting/ArrowLeft);
    subcontrol-position: left;
}
QScrollBar::add-line:horizontal {
    height: 9px;
    width: 9px;
    background: rgb(220, 220, 220);
    border-image: url(:/CalcSetting/ArrowRight);
    subcontrol-position: right;
}
QScrollBar::sub-line:horizontal:hover {
    background: rgb(190, 190, 190);
    border-image: url(:/CalcSetting/ArrowLeftHover);
}
QScrollBar::add-line:horizontal:hover {
    background: rgb(190, 190, 190);
    border-image: url(:/CalcSetting/ArrowRightHover);
}
QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal {
    background: rgb(220, 220, 220);
}
垂直滚动条 
QScrollBar:vertical {
    max-width: 9px;
    background: transparent;
    padding-top: 9px;
    padding-bottom: 9px;
}
QScrollBar::handle:vertical {
    width: 9px;
    min-height: 20px;
    background: rgb(190, 190, 190);
} 
QScrollBar::handle:vertical:hover {
    background: rgb(170, 170, 170);
} 
QScrollBar::sub-line:vertical {
    height: 9px;
    width: 9px;
    background: rgb(220, 220, 220);
    border-image: url(:/CalcSetting/ArrowTop);
    subcontrol-position: top;
} 
QScrollBar::add-line:vertical {
    height: 9px;
    width: 9px;
    background: rgb(220, 220, 220);
    border-image: url(:/CalcSetting/ArrowBottom);
    subcontrol-position: bottom;
} 
QScrollBar::sub-line:vertical:hover {
    background: rgb(190, 190, 190);
    border-image: url(:/CalcSetting/ArrowTopHover);
}
QScrollBar::add-line:vertical:hover {
    background: rgb(190, 190, 190);
    border-image: url(:/CalcSetting/ArrowBottomHover);
}
QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical {
    background: rgb(220, 220, 220);
}7、QStatusBarQStatusBar::item{
    border: 0px;
}
//不存在分隔线8、QComboBox
QComboBox {
    border-image: url(:/OverdraftControl/comboBox);
}
QComboBox:hover {
    border-image: url(:/OverdraftControl/comboBoxHover);
}
QComboBox:pressed {
    border-image: url(:/OverdraftControl/comboBoxPressed);
}
QComboBox::down-arrow {
    image: none;
}
QComboBox::drop-down {
    width: 15px;
    border: none;
}
QComboBox QAbstractItemView {
    border: 1px solid lightgray;
    outline: 0px;  //去掉item虚线
}
QComboBox QAbstractItemView::item {
    color: black;
    height: 22px;
    border: 1px solid transparent;
}
QComboBox QAbstractItemView::item:selected {
    border: 1px solid rgb(170, 190, 230);
    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(230, 240, 250), stop: 0.5 rgb(220, 235, 255), stop: 1.0 rgb(210, 230, 255));
}一般也可以使用QPalette
    QPalette palette;  
    palette.setBrush(QPalette::WindowText, QBrush(Qt::white));  
    pCheckBox->setPalette(palette);  
    pCheckBox->setStyleSheet("color:white");调色板类QPalette提供了颜色角色（color roles）的概念，指当前界面中颜色的职责，通过枚举变量QPalette::ColorRole来定义，比较常用的角色有：
    QPalette::Window  通常指窗口部件背景色
    QPalette::WindowText  通常指窗口部件的前景色
    QPalette::Base  指文本的背景色（QTextEdit、QLineEdit等）
    QPalette::Text  与QPalette::Base一块使用，指文本输入窗口部件前景色
    QPalette::Button   指按钮窗口部件的背景色
    QPalette::ButtonText  指按钮窗口部件的前景色    以上是遇到的常用但有资料较少的组件的样式，注重细节才能做出好的产品！
**注：**
技术在于交流、沟通，转载请注明出处并保持作品的完整性
