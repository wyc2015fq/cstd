# Qt学习笔记--窗口部件（Widget） - xqhrs232的专栏 - CSDN博客
2016年10月13日 22:57:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1189
原文地址::[http://blog.csdn.net/dipperkun/article/details/6266149](http://blog.csdn.net/dipperkun/article/details/6266149)
相关文章
1、QT学习笔记widget,MainWindow和Dialog的选择使用----[http://wenku.baidu.com/link?url=M8e9r__yaM65TKKsk0Mo4t_CJDwvdAj6I4ep28fnBVx_c67n6a46bTanEQGOA8G0epjrguvZ49tcX_oVbPuM-S0Q5ShOHuWEkZRMNoq0E_C](http://wenku.baidu.com/link?url=M8e9r__yaM65TKKsk0Mo4t_CJDwvdAj6I4ep28fnBVx_c67n6a46bTanEQGOA8G0epjrguvZ49tcX_oVbPuM-S0Q5ShOHuWEkZRMNoq0E_C)
2、[QT QTableWidget 用法总结](http://blog.csdn.net/polokang/article/details/6696982)----[http://blog.csdn.net/polokang/article/details/6696982](http://blog.csdn.net/polokang/article/details/6696982)
3、
# [Qt学习笔记--编程技巧总结](http://blog.csdn.net/dipperkun/article/details/6266103)----[http://blog.csdn.net/dipperkun/article/details/6266103](http://blog.csdn.net/dipperkun/article/details/6266103)
1. Widget是GUI编程的基本组件
2. 每个Widget可以放置在一个UI内或者作为一个独立的窗口
3. 每种类型的组件都是Widget的子类，Widget继承QObject
4. 每个Widget在构造时可以指定它的父对象，这样可以保证它不用时自动析构
5. 存在父子关系的Widgets，每个子Widget都显示在父Widget内；当父Widget析构时，
所有的子Widget自动析构
6. 每个程序一般都包含一个main.cpp，里面有main函数
**[c-sharp]**[view
 plain](http://blog.csdn.net/dipperkun/article/details/6266149#)[copy](http://blog.csdn.net/dipperkun/article/details/6266149#)
[print](http://blog.csdn.net/dipperkun/article/details/6266149#)[?](http://blog.csdn.net/dipperkun/article/details/6266149#)
- #include <QtGui>
- // 其他的自定义头文件
- // ...
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- // 创建Widget，显示它
- // ...
- return app.exec();  
- }  
7. 如果一个Widget构造时没有父对象，那么它被视为一个窗口（window），or顶级Widget
8. 隐藏或者关闭一个window时，并不会自动销毁（destroy）它，在程序退出时才销毁它
9. 使用QLayout进行布局，布局可以嵌套
**[cpp]**[view
 plain](http://blog.csdn.net/dipperkun/article/details/6266149#)[copy](http://blog.csdn.net/dipperkun/article/details/6266149#)
[print](http://blog.csdn.net/dipperkun/article/details/6266149#)[?](http://blog.csdn.net/dipperkun/article/details/6266149#)
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
-     QWidget window;   // 顶级Widget
- // 手动布局
- // 指明父对象，调用translate翻译，context为childwidget
-     QPushButton *button = new QPushButton(  
-             QApplication::translate("childwidget", "Press me"), &window);  
-     button->move(100, 100);  
-     button->show();  
- 
-     QLabel *label = new QLabel(QApplication::translate("windowlayout", "Name:"));  
-     QLineEdit *lineEdit = new QLineEdit();  
- 
- // 自动布局，父子关系暂不明朗
-     QHBoxLayout *layout = new QHBoxLayout();  
-     layout->addWidget(label);  
-     layout->addWidget(lineEdit);  
-     window.setLayout(layout); // 确定父子关系
-     window.setWindowTitle(  
-             QApplication::translate("windowlayout", "Window layout"));  
-     window.show();  
- return app.exec();  
- }  
10. 自定义Widget，实现paintEvent函数，它在以下情况下自动调用：
- 第一次显示时
- 被遮挡后，又暴露出来
- 调用update函数
**[cpp]**[view
 plain](http://blog.csdn.net/dipperkun/article/details/6266149#)[copy](http://blog.csdn.net/dipperkun/article/details/6266149#)
[print](http://blog.csdn.net/dipperkun/article/details/6266149#)[?](http://blog.csdn.net/dipperkun/article/details/6266149#)
- class AnalogClock : public QWidget  
- {  
-     Q_OBJECT  
- 
- public:  
-     AnalogClock(QWidget *parent = 0);  
- 
- protected:  
- void paintEvent(QPaintEvent *event);  
- };  
- AnalogClock::AnalogClock(QWidget *parent)  
-     : QWidget(parent)  
- {  
-     ...  
-     setWindowTitle(tr("..."));  
-     resize(200, 200);  
- }  
- void AnalogClock::paintEvent(QPaintEvent *event)  
- {  
-     Q_UNUSED(event);  
-     QPainter painter(this); // 绘图工具
-     painter.setRenderHint(QPainter::Antialiasing); // 反锯齿
- // 坐标转换，移到源点到Widget的中心
-     painter.translate(width() / 2, height() / 2);  
-     painter.scale(side / 200.0, side / 200.0);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)

