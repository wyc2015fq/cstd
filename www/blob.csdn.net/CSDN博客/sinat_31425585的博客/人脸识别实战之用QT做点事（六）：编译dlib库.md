# 人脸识别实战之用QT做点事（六）：编译dlib库 - sinat_31425585的博客 - CSDN博客
2018年07月03日 21:58:00[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：806
个人分类：[QT+opencv](https://blog.csdn.net/sinat_31425585/article/category/7742815)
    Dlib是非常优秀的机器学习库，可以无需编译直接用visual studio进行调用，但是需要在release模式下才能成功。对于QT creator MignGM版本编译dlib，现有的教程十分少，在连续几天的失败之后，参考博客：
https://blog.csdn.net/qq_18854309/article/details/78094225
    终于在今天，编译成功，现在总结一下：
**1、下载并安装相关软件包：    **
    cmake我使用的版本为cmake-3.8.0-rc1-win32-x86，32位的cmake、QT minGM版本为5.9.6， dlib版本为19.7。
    下载地址分别为：
    https://cmake.org/files/v3.8/
    http://dlib.net/files/
   注意，安装完QT后，将以下两个路径添加到系统环境变量path中去：
    D:\softWare\Qt\5.9.6\mingw53_32\bin
    D:\softWare\Qt\Tools\mingw530_32\bin
注：环境变量配置完成后，最好重启以下电脑，让环境变量生效。
     然后，打开cmake的gui界面，将dlib路径和编译路径配置好，我这里设置的编译路径是新建一个dlib文件夹，然后在里面新建一个build子文件夹，然后将build文件夹设置为二进制文件的存放路径：
![](https://img-blog.csdn.net/2018070321313640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180703213241196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
cmake gui界面：
![](https://img-blog.csdn.net/20180703213447602?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
** 2、点击configure，设置生成器及编译器**
    生成器设置为MinGW Makefiles
![](https://img-blog.csdn.net/20180703214109465?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择Specify native compilers，设置编译器：
这里gcc和g++位于路径：D:\softWare\Qt\Tools\mingw530_32\bin
![](https://img-blog.csdn.net/2018070321441262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击Finish。
**3、设置编译选项（这一步很重要）**
**![](https://img-blog.csdn.net/20180703214725390?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**
    取消GIF格式支持，因为这个需要另外安装库，不然会报错，INSTALL_PREFIX对应就是最终编译完成，得到库的保存路径。另外两个矩阵运算库，如果已经提前安装了，可以勾选。
    然后，configure -> Generate
**  4、编译**
    这一步就很简单了，只需要进入刚才编译的二进制文件保存路径，启动cmd
![](https://img-blog.csdn.net/20180703215316494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    依次运行：
    mingw32-make    
    mingw32-make install
    这样就可以完成编译。
**5、测试**
 这个就直接贴代码了：
    配置文件：
```cpp
LIBS+=D:\library\dlib\lib/libdlib.a
INCLUDEPATH+=D:\library\dlib\include
```
    cpp文件：
```cpp
void MainWindow::on_pushButton_clicked()
{
    QString imgName = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    if(imgName.isNull())return;
    QTextCodec*code=QTextCodec::codecForName("gb2312");
    std::string name=code->fromUnicode(imgName).data();
    //--------------------------------------------------
    QImage*labImg=new QImage;
    if(!(labImg->load(name.data())))
    {
        delete labImg;
        return;
    }
    QPixmap pixmap=QPixmap::fromImage(*labImg);
    QPainter painter(&pixmap);
    painter.setPen(Qt::red);
    //--------------------------------------------------
    frontal_face_detector detector=get_frontal_face_detector();
    dlib::array2d<unsigned char>img;
    load_image(img,name.data());
    dlib::pyramid_up(img);
    std::vector<dlib::rectangle>dets=detector(img);
    dlib::rectangle myRect;
    int faceCount=dets.size();
    ui->textBrowser->append(QString::number(faceCount));
    for(int32_t i=0; i < faceCount; i++)
    {
        myRect=dets.at(i);
        ui->textBrowser->append("x:"+QString::number(myRect.left()/2));
        ui->textBrowser->append("y:"+QString::number(myRect.top()/2));
        ui->textBrowser->append("width:"+QString::number(myRect.width()/2));
        ui->textBrowser->append("height:"+QString::number(myRect.height()/2));
        painter.drawRect(myRect.left()/2,myRect.top()/2,myRect.width()/2,myRect.height()/2);
    }
    ui->label->setPixmap(pixmap);
}
```
    效果如下：
![](https://img-blog.csdn.net/20180703215743176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
