# 在Qt中使用ActiveX控件 - gauss的专栏 - CSDN博客
2013年01月03日 18:14:15[gauss](https://me.csdn.net/mathlmx)阅读数：434
                
文为原创作品http://blog.csdn.net/tingsking18/article/details/5403038
Qt的windows商业版本提供了ActiveQt这个framework，使用这个组件我们可以在Qt中使用ActiveX控件，并且也开发基于Qt的ActiveX控件。ActiveQt包含了两个组件QAxContainer和QAxServer。
l         QAxContainer允许我们使用COM对象，并且可以将将ActiveX控件嵌入到Qt程序中去。
l         QAxServer可以将我们写的Qt控件导出为COM对象或者是ActiveX控件。
第一个例子我们来演示一下在Qt中使用shockwaveFlash这个ActiveX控件。
#include <QApplication>
#include <QtGui>
#include <QAxWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QAxWidget *flash = new QAxWidget(0,0);
    flash->resize(500,80);
    flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
    flash->dynamicCall("LoadMovie(long,string)",0,"c:/1.swf");
    flash->show();
    return a.exec();
}
QAxContainer不包含在QtCore里面，所以要使用QAxContainer的话还必须要在.pro文件中添加 CONFIG+=qaxcontainer
下面就来解释一下上面的代码：
1.       首先我们创建了一个QAxWidget对象。QAxWidget对象是用来封装ShockwaveFlash这个ActiveX控件。QAxContainer是有三个类组成的。分别是：QAxObject封装了COM对象、QAxWidget封装了ActiveX控件、QAxBase是QAxObject和QAxWidget的父类，它实现了封装COM的核心函数。
这三个类的关系如下：
![](http://hi.csdn.net/attachment/201003/22/1207120_1269219037thl8.jpg)
2.       然后通过setControl设置ShockwaveFlash这个ActiveX控件的class_id，class ID可以通过一些其它的编程工具获得(vc/Delphi/Microsoft ActiveX Control Pad)。这时就会调用CoCreateInstance创建ActiveX控件的实例。这时这个ActiveX控件的所有的属性、方法、事件将通过QAxWidget转换为Qt的properties、signals和slots。
通过调用QAxWidget的property()方法和SetProperty方法可以获取和设置ActiveX控件的属性。
既然可以调用ActiveX控件的方法属性，当然也就需要有COM中的数据类型和Qt中的使用的数据类型的转换了。下面就是COM中数据类型和Qt中的数据类型对应的表格：
![Qt中数据类型与COM中的数据类型对照关系](http://hi.csdn.net/attachment/201003/22/1207120_1269219037QN1C.jpg)
3.       通过dynamicCall方法来调用ActiveX控件的方法。
1)dynamicCall()最多传递八个QVariant类型的参数，并且返回类型也是QVariant。
2)如果我们需要传递IDispatch *或者是IUnknown *，我们可以将这些组件封装成QAxObject，然后通过调用asVariant()方法来将这个COM对象转换为QVariant。同样如果我们返回一个IDispatch*或者IUnknown*的对象，就需要通过querySubObject方法将这个对象转换为QAxObject。
        QAxObject *session = outlook.querySubObject("Session");3)如果不想这样做还可以QAxWidget还为我们提供了QueryInterface方法IWebBrowser2 *webBrowser = 0; activeX->queryInterface(IID_IWebBrowser2, (void *)&webBrowser); if (webBrowser) {     webBrowser->Navigate2(pvarURL);     webBrowser->Release(); }       4)dynamicCall方法还可以设置COM控件的属性：        activeX->dynamicCall("Value", 5);        QString text = activeX->dynamicCall("Text").toString();
5)通过dynamicCall调用COM对象的方法的时候需要提供完成的函数签名。
activeX->dynamicCall("Navigate(const QString&)", "qt.nokia.com");
