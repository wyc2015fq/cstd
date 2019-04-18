# Object::connect: No such slot （QT槽丢失问题） - wishchinYang的专栏 - CSDN博客
2015年12月04日 15:05:46[wishchin](https://me.csdn.net/wishchin)阅读数：5547
1、看看你的类声明中有没有Q_OBJECT，并继承public QMainWindow{
    例如：        
```cpp
class CPlot: public QMainWindow{
	Q_OBJECT
```
2、你声明的函数要加声明:
      private slots:
         void xxxx();
 并且要把上面一段函数添加到构造函数的下面，被构造函数直接访问：
```cpp
public:
    CPlot(QWidget *parent = 0);
    private slots:
        void openFile();
        void segmentObj();
        void capture();
        void drawRays();
```
3、检查槽函数名拼写是否有误，以及对应的signal和slot的参数是否一致
       如果还不行的话,清理项目,删掉原有的moc_xxx.cpp,重新执行qmake.
或者自己修改一下：
      例如：添加槽：
```cpp
static const char qt_meta_stringdata_OpenGLViewer[] = {
	"MainViewer\0\0openFile()\0segmentObj()\0"
	"capture()\0"
};
```
```cpp
void CPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
	if (_c == QMetaObject::InvokeMetaMethod) {
		Q_ASSERT(staticMetaObject.cast(_o));
		CPlot *_t = static_cast<CPlot *>(_o);
		switch (_id) {
		case 0: _t->openFile(); break;
		case 1: _t->segmentObj(); break;
		case 2: _t->capture(); break;
		default: ;
		}
	}
	Q_UNUSED(_a);
}
```
