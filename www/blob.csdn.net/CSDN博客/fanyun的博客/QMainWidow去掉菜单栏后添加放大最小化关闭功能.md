# QMainWidow去掉菜单栏后添加放大最小化关闭功能 - fanyun的博客 - CSDN博客
2018年05月27日 21:56:54[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：250
Qt去掉菜单栏：
```cpp
/* 标题栏样式 */    
this->setWindowFlags(Qt::FramelessWindowHint |    
                     Qt::WindowSystemMenuHint |    
                     Qt::WindowMinMaxButtonsHint);
```
去掉以后，发现窗口无法移动，无法放大缩小关闭，那么如何解决以上问题呢？
设置窗口最大化、最小化、原状态也可以使用如下方法：
```cpp
void showMaximized();  
void showMinimized();  
void showNormal();
```
重写窗口鼠标事件：
```cpp
void mousePressEvent(QMouseEvent *t);
void mouseMoveEvent(QMouseEvent *t);
void mouseReleaseEvent(QMouseEvent *t);
```
