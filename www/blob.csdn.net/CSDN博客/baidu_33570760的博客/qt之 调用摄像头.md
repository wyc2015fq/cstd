# qt之 调用摄像头 - baidu_33570760的博客 - CSDN博客
2017年05月04日 12:04:07[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1537

转载自：http://blog.sina.com.cn/s/blog_a6fb6cc90101g2mg.html
Qt如何调用系统摄像设备进行显示、截图、录制？
    QCamera：系统摄像设备（摄像头）
   QCameraViewfinder：摄像取景器部件
   QCameraImageCapture：截图部件
capture按钮：用于截图（截图后在右上角显示）
save按钮：用于保存截取后的图片（此处测试路径为：F:\a.jpg）
exit按钮：退出界面
效果如下：
![](https://img-blog.csdn.net/20170515150523073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
#include "test_capture.h"
Dialog::Dialog(QWidget *parent) :
QDialog(parent)
{
this->resize(600, 400);
camera = new QCamera();
view_finder = new QCameraViewfinder();
camera_image_capture = new QCameraImageCapture(camera);
capture_button = new QPushButton();
save_button = new QPushButton();
exit_button = new QPushButton(); 
display_label = new QLabel();
QHBoxLayout *main_layout = new QHBoxLayout();
QVBoxLayout *v_layout = new QVBoxLayout();
display_label->setFixedSize(150, 150);
display_label->setScaledContents(true);
v_layout->addWidget(display_label);
v_layout->addStretch();
v_layout->addWidget(capture_button);
v_layout->addWidget(save_button);
v_layout->addWidget(exit_button);
main_layout->addWidget(view_finder);
main_layout->addLayout(v_layout);
connect(capture_button, &QPushButton::clicked, this,&Dialog::captureImage);
connect(save_button, &QPushButton::clicked, this,&Dialog::saveImage);
connect(exit_button, &QPushButton::clicked, this,&Dialog::close);
connect(camera_image_capture,&QCameraImageCapture::imageCaptured, this,&Dialog::displayImage);
camera_image_capture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
camera->setCaptureMode(QCamera::CaptureStillImage);
camera->setViewfinder(view_finder);
camera->start(); //启动摄像头
this->setLayout(main_layout);
this->translateLanguage();
}
Dialog::~Dialog()
{
}
void Dialog::translateLanguage()
{
this->setWindowTitle("testCapture");
capture_button->setText(tr("capture"));
save_button->setText(tr("save"));
exit_button->setText(tr("exit"));
}
void Dialog::displayImage(int id, QImage image)
{
display_label->setPixmap(QPixmap::fromImage(image)); 
}
void Dialog::captureImage()
{
//截图
camera_image_capture->capture();  
}
void Dialog::saveImage()
{
const QPixmap *pixmap = display_label->pixmap();
if(pixmap)
{
pixmap->save("F:\\a.jpg");
}
}
**注：**
   技术在于交流、沟通，转载请注明出处并保持作品的完整性。
