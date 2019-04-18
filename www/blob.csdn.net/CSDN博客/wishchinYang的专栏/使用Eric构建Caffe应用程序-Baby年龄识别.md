# 使用Eric构建Caffe应用程序-Baby年龄识别 - wishchinYang的专栏 - CSDN博客
2015年04月09日 17:34:02[wishchin](https://me.csdn.net/wishchin)阅读数：905
        训练好的Caffe网络结构，可以固定下来，直接载入程序作为数据库接口使用。本文使用Eric构建运行于Python环境下的图片识别应用程序，因为Eric使用QT作为GUI，且有Python的接口，可直接无缝使用Caffe，并可使用在Linux和Windows环境中。
1.编译好Pycaffe之后，安装Eric4、PyQt4。
       软件中心安装 Eric4、PyQt4；
2.配置Eric：
      根据程序设置向导配置Eric使用的Python版本、其他设置。
**3.建立项目：**
       点击Eric主界面，Project->new ，设置项目名称、位置；
![eric新建项目](http://www.pythoner.com/wp-content/uploads/2013/03/1.png)
**3.1、界面设计**
将左侧的【源代码】选项卡切换至【窗体】选项卡（左数第二个）。
![选择窗体](http://www.pythoner.com/wp-content/uploads/2013/03/2.png)
右键空白区域，选择【新建窗体】。
![新建窗体](http://www.pythoner.com/wp-content/uploads/2013/03/3.png)
在弹出的对话框中选择【对话框】。
![新建对话框](http://www.pythoner.com/wp-content/uploads/2013/03/4.png)
点击Ok之后，填写保存位置，生成一个QtDesigner可以修改的UI文件。
工程左栏右键点击UI文件，点击Open in Qt-Designer弹出设计界面，便可以进行界面设计。
**4、设计界面，建立自定义槽**
对于Button文件，可以在设计界面添加空槽，再进行代码文件内编辑
**![信号和槽](http://www.pythoner.com/wp-content/uploads/2013/03/8.png)**
也可以不在设计界面添加槽，保存UI文件。
回到工程界面，右键点击编译，生成UI_.....Py文件;
再点击生成对话框代码，生成类代码文件。
因为MCV设计模式使设计和逻辑分离，可以设计好界面后，修改任意类代码不影响界面效果。
默认槽的函数形式为：
若按钮名称为btnParse，则类内槽函数为
```python
@pyqtSignature("")
def on_btnParse_clicked(self):
```
程序编译运行后，按动按钮btnParse，触发on_btnParse_clicked(self) 设定的行为。
** 5.设计逻辑**
     我的程序主要使用了类文件里面的成员函数on_RecogOne_clicked(self)；
     以下是参考代码：
```python
import ClassifyFace as cf
.......................................
    @pyqtSignature("")
    def on_RecogOne_clicked(self):
        """
        Slot documentation goes here.
        """
        #self.tvResult.clear()
        fName = self.lblPath.text()
        #print ("fName:%s", fName )#u'Please, load a log file'
        
        # 获取地址框内的文件名
        pattern = self.txtReToMatch.displayText();
        print ("pattern:%s", pattern)#fileName!
        
        path = pattern.toAscii();
        path=path[7:];path=str(path )# 异常必要的强制转化！
        print path;
        
        #载入识别过程
        pathSrc =path
        Out, pathRec=cf.recogImg(path);#识别函数
        Num=-1;idx =0;
        if(Out ==-1):
            print ("Face Detect fail!");
            return;
        else:
            for i in Out:
                if (i ==1):
                    Num=idx;
                idx+=1;
                
        #画出修改图像
        pixmapO=QPixmap()
        pixmapO.load(pathRec);
        itemO=QGraphicsPixmapItem(pixmapO)
        self.scene2.addItem(itemO)
        self.graphicsView_Out.setScene(self.scene2)
        
        if(Num==-1):
            print ("The Face is not a Baby!");
        else:
            print ("The serias %d face is a baby face"%Num);
            #画出脸部图像
            pathf=path+"face"+str(Num)+".jpg";
            pixmapf=QPixmap();
            pixmapf.load(pathf);
            itemf=QGraphicsPixmapItem(pixmapf)
            self.scene3.addItem(itemf)
            self.graphicsView_Face.setScene(self.scene3)
        
        OutStr ="The serias"+ str(Num)+ " X face is a baby face"
        self.label_Outshow.setText(OutStr);
    
        #显示原图像
        pixmap=QPixmap()
        pixmap.load(pathSrc);
        item=QGraphicsPixmapItem(pixmap)
        self.scene.addItem(item)
        self.graphicsView_Src.setScene(self.scene)
```
**6.Debug调试程序**
注意事项：调试时在主程序打开文件窗口按下F2键，否则可能出现异常提示;
                    依次关闭调试使用shift+F10按键，不要按退出键按钮；                    
软件效果：
![](https://img-blog.csdn.net/20150409122743223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
file:///home/wishchin/图片/smplayer_screenshots/屏幕截图 2015-04-09 12:27:33.png
程序运行效果：
![](https://img-blog.csdn.net/20150409173622842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**总结**：
         
 对于简单的图像处理使用CNN问题，比如给出了类似于人脸的结果图片，再进行模式识别。若从0开始，一般可以使用最简单的六层网络，使用Caffe可以仅配置参数就可以构建简单的CNN，一般的六层网络是这样设置的：
          
 InPut——>Conv层——>Pooling层——>Conv层——>
 Pooling层/ ReLU整流层+pooling层——>全链接层——>softMax层——>输出类别概率。
          
 卷积核实现特征提取的各种不变性，同时可以完成梯度计算、主方向确定等功能。当然一般来说使用越大的卷积核越能学习全局的特征，且若要学到较为全局的特征，需要使用至少两个卷积层。
**注意事项**：
        运行过程中：出现cublas无法构建的事，好像是可以忽视的.............
        没有使用cublas成功，貌似使用了哪一个blas库来着.........
