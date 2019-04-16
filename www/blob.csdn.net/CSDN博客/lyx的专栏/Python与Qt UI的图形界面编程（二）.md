# Python与Qt UI的图形界面编程（二） - lyx的专栏 - CSDN博客





2016年02月03日 22:44:21[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2663









和大多数操作系统一样，Windows及Linux都提供了一系列的标准对话框，如文件选择，字体选择，颜色选择等，这些标准对话框为应用程序提供了一致的观感。Qt对这些标准对话框都定义了相关的类，这些类让使用者能够很方便地使用标准对话框进行文件，颜色以及字体的选择。标准对话框在软件设计过程中是经常需要使用的。


Qt提供的标准对话框除了本实例提到的，还有QErrorMessage，QInputDialog，QMessageBox，QPrintDialog，QProcessDialog等，这些标准对话框的使用在本书的后续部分将会陆续介绍。


本实例主要演示上面几种标准对话框的使用，如下图所示

![](http://www.linuxidc.com/upload/2012_06/120624072053382.jpg)


在上图中，单击“文件对话框”按钮，会弹出文件选择对话框，选中的文件名将显示在右连，单击“颜色对话框”按钮，会弹出颜色选择对话框，选中的颜色将显示在右边，单击“字体对话框”按钮，会弹出字体选择对话框，选中的字体将更新右边显示的字符串。


实现代码如下：

- # -*- coding: utf-8 -*- 
- **from** PyQt4.QtGui **import** *  
- **from** PyQt4.QtCore **import** *  
- **import** sys  
- 
- QTextCodec.setCodecForTr(QTextCodec.codecForName("utf8"))  
- 
- **class** StandardDialog(QDialog):  
- 
- **def** __init__(self,parent=None):  
-         super(StandardDialog,self).__init__(parent)  
- 
- self.setWindowTitle("Standard Dialog")  
- 
-         filePushButton=QPushButton(self.tr("文件对话框"))  
-         colorPushButton=QPushButton(self.tr("颜色对话框"))  
-         fontPushButton=QPushButton(self.tr("字体对话框"))  
- 
- self.fileLineEdit=QLineEdit()  
- self.colorFrame=QFrame()  
- self.colorFrame.setFrameShape(QFrame.Box)  
- self.colorFrame.setAutoFillBackground(True)  
- self.fontLineEdit=QLineEdit("Hello World!")  
- 
-         layout=QGridLayout()  
-         layout.addWidget(filePushButton,0,0)  
-         layout.addWidget(self.fileLineEdit,0,1)  
-         layout.addWidget(colorPushButton,1,0)  
-         layout.addWidget(self.colorFrame,1,1)  
-         layout.addWidget(fontPushButton,2,0)  
-         layout.addWidget(self.fontLineEdit,2,1)  
- 
- self.setLayout(layout)  
- 
- self.connect(filePushButton,SIGNAL("clicked()"),self.openFile)  
- self.connect(colorPushButton,SIGNAL("clicked()"),self.openColor)  
- self.connect(fontPushButton,SIGNAL("clicked()"),self.openFont)  
- 
- **def** openFile(self):  
- 
-         s=QFileDialog.getOpenFileName(self,"Open file dialog","/","Python files(*.py)")  
- self.fileLineEdit.setText(str(s))  
- 
- **def** openColor(self):  
- 
-         c=QColorDialog.getColor(Qt.blue)  
- **if** c.isValid():  
- self.colorFrame.setPalette(QPalette(c))  
- 
- **def** openFont(self):  
- 
-         f,ok=QFontDialog.getFont()  
- **if** ok:  
- self.fontLineEdit.setFont(f)  
- 
- app=QApplication(sys.argv)  
- form=StandardDialog()  
- form.show()  
- app.exec_()  





第6行设定tr方法使用utf8编码来解析文字。


第13行设置程序的标题。


第15到17行创建各个按钮控件。


第19行创建一个QLineEdit类实例fileLineEdit，用来显示选择的文件名。


第20行创建一个QFrame类实例colorFrame，当用户选择不同的颜色时，colorFrame会根据用户选择的颜色更新其背景。


第23行创建一个QLineEdit类实例fontLineEdit，当用户选择不同的字体时，fontLineEdit会根据用户选择的字体更新其内容。


第25到33行将各个控件进行布局。


第35到37行将各个按钮的clicked信号相应的槽进行连接。


slotFile()方法是文件对话框按钮的clicked信号的槽，其中getOpenFileName()是QFileDialog类的一个静态方法，返回用户选择的文件名，如果用户选择取消，则返回一个空串。函数形式如下：


QString getOpenFileName (QWidget parent = None, QString caption = QString(), QString directory = QString(), QString filter = QString(), Options options = 0)

QString getOpenFileName (QWidget parent = None, QString caption = QString(), QString directory = QString(), QString filter = QString(), QString selectedFilter = None, Options options = 0)


调用getOpenFileName()函数将创建一个模态的文件对话框，如下图所示。directory参数指定了默认的目录，如果directory参数带有文件名，则该文件将是默认选中的文件，filter参数对文件类型进行过滤，只有与过滤器匹配的文件类型才显示，filter可以同时指定多种过滤方式供用户选择，多种过滤器之间用";;"隔开，用户选择的过滤器通过参数selectedFilter 返回。

![](http://www.linuxidc.com/upload/2012_06/120624072053383.jpg)


QFileDialog类还提供了类似的其他静态函数，如下表，通过这些函数，用户可以方便地定制自己的文件对话框。


|静态函数|说明|
|----|----|
|getOpenFileName|获得用户选择的文件名|
|getSaveFileName|获得用户保存的文件名|
|getExistingDirectory|获得用户选择的已存在的目录名|
|getOpenFileNames|获得用户选择的文件名列表|
slotColor()函数是颜色对话框按钮clicked信号的槽。其中getColor()是QColorDialog的一个静态函数，返回用户选择的颜色值，函数形式如下：



QColor QColorDialog.getColor (QColor initial = Qt.white, QWidget parent = None)


QColor QColorDialog.getColor (QColor, QWidget, QString, ColorDialogOptions options = 0)


调用getColor()函数将创建一个模态的颜色对话框，如下图所示。initial参数指定了默认的颜色，默认为白色，通过isValid()可以判断用户选择的颜色是否有效，若用户选择取消，isValid()将返回false。

![](http://www.linuxidc.com/upload/2012_06/120624072053381.jpg)


slotFont()函数是字体对话框按钮clicked信号的槽。其中getFont()是QFontDialog的一个静态函数，返回用户选择的字体，函数形式如下：


(QFont, bool) getFont (QFont, QWidget, QString, FontDialogOptions)

(QFont, bool) getFont (QFont, QWidget, QString)

(QFont, bool) getFont (QFont, QWidget parent = None)

(QFont, bool) getFont (QWidget parent = None)


调用getFont()函数将创建一个模态的字体对话框，如下图所示。用户选择OK，函数返回(用户选择的字体,True),否则返回(默认字体,False)

![](http://www.linuxidc.com/upload/2012_06/120624072053381.jpg)



