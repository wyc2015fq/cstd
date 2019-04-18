# VS下给Qt项目添加图标 - weixin_33985507的博客 - CSDN博客
2018年08月22日 16:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
- 任意准备一张图片，[在线转换为ico图标文件](http://www.bitbug.net/)
- 将.ico文件移动到源码所在目录下（如`C:\Users\downdemo\source\repos\QtGuiApplication1\QtGuiApplication1`）
- 在VS中右键项目，选择添加资源
![5587614-6766cd10ffcc6aa8.png](https://upload-images.jianshu.io/upload_images/5587614-6766cd10ffcc6aa8.png)
- 导入图标文件
![5587614-062cf2b3705f75cc.png](https://upload-images.jianshu.io/upload_images/5587614-062cf2b3705f75cc.png)
- 随后在项目资源列表中可以看到多出了`.ico`和`.rc`文件，编译即可将图标替换为.ico文件
![5587614-75793b1b12d95ab6.png](https://upload-images.jianshu.io/upload_images/5587614-75793b1b12d95ab6.png)
- 若要在窗口左上角显示图标，则需要在`main.cpp`中添加代码
```cpp
#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtGuiApplication1 w;
    QIcon icon;
    icon.addFile(QStringLiteral("icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
```
![5587614-cb9f872d9b13a52d.png](https://upload-images.jianshu.io/upload_images/5587614-cb9f872d9b13a52d.png)
