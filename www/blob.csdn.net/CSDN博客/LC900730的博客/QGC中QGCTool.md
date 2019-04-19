# QGC中QGCTool - LC900730的博客 - CSDN博客
2017年08月19日 16:42:14[lc900730](https://me.csdn.net/LC900730)阅读数：269
## QGCTool
```
QGCTool::QGCTool(QGCApplication* app,QGCToolbox* toolbox):QObject(toolbox)
, _app(app)
, _toolbox(NULL)
```
void QGCTool::setToolbox(QGCToolbox * toolbox){ 
    _toolbox=toolbox; 
}
在QGCApplication中
```
app->_initCommon(void)
qmlRegisterUncreatableType<Joystick>
...
注册单例模式
qmlRegisterSingletonType<QGroundControlQmlGlobal>
```
```
QGCApplication : public
#ifdef __mobile__
    QGuiApplication     //native Qml based application
#else
    QApplication       //QtWidget based application
#endif
    {
        Q_OBJECT
        public :
            QGCApplication(int &argc,char *argv[],bool unitTesting);
            ~QGCApplication();
    //QGCToolbox * toolbox(void)= {return _toolbox;}
//单例，可以使用qgcApp引用
static QGCApplication* _app;
QGCApplication *qgcApp(void)
    在构造函数中
    _app=this;
```
在main.cc中 
QGCApplication *app=new QGCApplication(argc,argv,runUnitTests);
这个_toolbox是在 
头文件中QGCToolbox *_toolbox; 
_toolbox=new QGCToolbox(this); 
_toolbox->setChildToolboxes();
在QGCApplication.cc中，初始化最后一步是
就是_toolbox=new QGCToolbox(this) 
_toolbox->setChildToolboxes(); 
在main.cc中，new QGCAppliction中，new出来的对象就是这个this，作为QGCToolbox构造参数传入，得到_toolbox。即_toolbox就是new出来的对象的一个属性。
### qgcApp(void)
```
//因此需要qgcApp来得到_app
QGCApplication * qgcApp(void){
    return QGCApplication::_app;
}
```
### QGCToolbox
```
//构造参数是个指针
QGCToolbox::QGCToolbox(QGCApplication *app){
}
```
### QGCApplication构造初始化过程中的this
```
在QGCApplication中
QGCApplication *app=new QGCApplication(argc,argc,runUnitTests);
在QGCApplication构造方法中，最后出现了
    _toolbox=new QGCToolbox(this);
QGCToolbox构造参数需要的QGCApplication *，这里传递的是this？
理解：this其实是个指针，在new QGCApplication的时候，其实生成的是一个匿名对象，这个匿名对象最后通过这个this指针传递出去。
//返回
在类的非静态成员函数中返回对象本身的时候，直接使用return *this(常用于操作符重载和赋值、拷贝等函数)
```
QGCTool是QGCCorePlugin的父类 
class QGCCorePlugin：public QGCTool{}
