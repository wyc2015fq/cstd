# Python与Qt UI的图形界面编程（三） - lyx的专栏 - CSDN博客





2016年02月03日 22:47:14[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3597









Qt提供了很多关于获取窗体位置及显示区域大小的函数，本实例利用一个简单的对话框显示窗体的各种位置信息，包括窗体的所在点位置，长，宽信息等。本实例的目的是分析各个有关位置信息的函数之间的区别，如x(),y(),pos(),rect(),size(),geometry()等，以及在不同的情况下应使用哪个函数来获取位置信息。实现的效果如下图：

![](http://www.linuxidc.com/upload/2012_06/120624072157791.gif)


在实例中，分别调用了x(),y(),frameGeometry(),pos(),geometry(),width(),height(),rect(),size()几个函数，这几个函数均是QWidget提供的。当改变对话框的大小，或移动对话框时，调用各个函数所获得的信息显示也相应地发生变化，从变化中可得知各函数之间的区别。


由于本实例的目的是为了分析各函数之间的区别并获得结论，而程序本身的实现较为简单，因些只简单介绍实现过程。实现代码如下：

- from PyQt4.QtCore import *  
- from PyQt4.QtGui import *  
- import sys  
- 
- class Geometry(QDialog):  
- 
- def __init__(self,parent=None):  
-         super(Geometry,self).__init__(parent)  
- 
- self.setWindowTitle("Geometry")  
- 
-         Label1=QLabel("x0:")  
-         Label2=QLabel("y0:")  
-         Label3=QLabel("frameGeometry():")  
-         Label4=QLabel("pos():")  
-         Label5=QLabel("geometry():")  
-         Label6=QLabel("width():")  
-         Label7=QLabel("height():")  
-         Label8=QLabel("rect():")  
-         Label9=QLabel("size():")  
- 
- self.xLabel=QLabel()  
- self.yLabel=QLabel()  
- self.frameGeoLabel=QLabel()  
- self.posLabel=QLabel()  
- self.geoLabel=QLabel()  
- self.widthLabel=QLabel()  
- self.heightLabel=QLabel()  
- self.rectLabel=QLabel()  
- self.sizeLabel=QLabel()  
- 
-         layout=QGridLayout()  
-         layout.addWidget(Label1,0,0)  
-         layout.addWidget(self.xLabel,0,1)  
-         layout.addWidget(Label2,1,0)  
-         layout.addWidget(self.yLabel,1,1)  
-         layout.addWidget(Label3,2,0)  
-         layout.addWidget(self.frameGeoLabel,2,1)  
-         layout.addWidget(Label4,3,0)  
-         layout.addWidget(self.posLabel,3,1)  
-         layout.addWidget(Label5,4,0)  
-         layout.addWidget(self.geoLabel,4,1)  
-         layout.addWidget(Label6,5,0)  
-         layout.addWidget(self.widthLabel,5,1)  
-         layout.addWidget(Label7,6,0)  
-         layout.addWidget(self.heightLabel,6,1)  
-         layout.addWidget(Label8,7,0)  
-         layout.addWidget(self.rectLabel,7,1)  
-         layout.addWidget(Label9,8,0)  
-         layout.addWidget(self.sizeLabel,8,1)  
- 
- self.setLayout(layout)  
- 
- self.updateLabel()  
- 
- def moveEvent(self,event):  
- self.updateLabel()  
- 
- def resizeEvent(self,event):  
- self.updateLabel()  
- 
- def updateLabel(self):  
-         temp=QString()  
- 
- self.xLabel.setText(temp.setNum(self.x()))  
- self.yLabel.setText(temp.setNum(self.y()))  
- self.frameGeoLabel.setText(temp.setNum(self.frameGeometry().x())+","+  
-                                    temp.setNum(self.frameGeometry().y())+","+  
-                                    temp.setNum(self.frameGeometry().width())+","+  
-                                    temp.setNum(self.frameGeometry().height()))  
- 
- self.posLabel.setText(temp.setNum(self.pos().x())+","+  
-                               temp.setNum(self.pos().y()))  
- self.geoLabel.setText(temp.setNum(self.geometry().x())+","+  
-                               temp.setNum(self.geometry().y())+","+  
-                               temp.setNum(self.geometry().width())+","+  
-                               temp.setNum(self.geometry().height()))  
- self.widthLabel.setText(temp.setNum(self.width()))  
- self.heightLabel.setText(temp.setNum(self.height()))  
- self.rectLabel.setText(temp.setNum(self.rect().x())+","+  
-                                temp.setNum(self.rect().y())+","+  
-                                temp.setNum(self.rect().width())+","+  
-                                temp.setNum(self.rect().height()))  
- self.sizeLabel.setText(temp.setNum(self.size().width())+","+  
-                                temp.setNum(self.size().height()))  
- 
- 
- app=QApplication(sys.argv)  
- form=Geometry()  
- form.show()  
- app.exec_()  





程序初始化时调用updateLabel()函数，以获得各位置函数的信息并显示。


updateLabel()函数负责调用各个位置函数获得结果并显示。


重定义QWidget的moveEvent()和resizeEvent()函数，分别响应对话框的移动事件和调整大小事件，使得窗体在被移动或窗体大小发生改变时，能同步更新各函数结果的显示。


通过这个例子可以获得如下图所示的结论。

![](http://www.linuxidc.com/upload/2012_06/120624072157792.gif)


x(),y()和pos()函数都是获得整个窗体左上角的坐标位置。而frameGeometry()与geometry()相对应，frameGeometry()是获得整个窗体的左上顶点和长，宽值，而geometry()函数获得的是窗体内中央区域的左上顶点坐标以及长，宽值。直接调用width()和height()函数获得的是中央区域的长和宽的值。


还有两个函数rect()和size()，调用它们获得的结果也都是对于窗体的中央区域而言的，size()获得的是窗体中央区域的长，宽值，rect()与geometry()一样返回一个QRect对象。其中，两个函数获得的长，宽值是一样的，都是窗体中央区域的长，宽值，只是左上顶点坐标值不一样，geometry()获得的左上顶点坐标是相对于父窗体而言的坐标，而rect()获得的左上顶点坐标始终为(0,0)。


因此，在实际应用中需根据情况使用正确的位置信息函数以获得准确的位置尺寸信息，尤其是在编写对位置精度要求较高的程序时，如地图浏览程序，更应注意函数的选择，避免产生不必要的误差。


在编写程序时，初始化窗体时最好不要使用setGeometry()函数，而用resize()和move()函数代替，因为使用setGeometry()会导致窗体show()之后在错误的位置上停留很短暂的一段时间，带来闪烁现象。



