# QGC瓦片地图 - LC900730的博客 - CSDN博客
2017年08月17日 20:49:15[lc900730](https://me.csdn.net/LC900730)阅读数：879
## 内置的地图
QGCLocationPlugin文件夹中 
头文件中：
```
//QGCMapEngine.h
class QGCTileSet(){
public:
    QGCTileSet(){
        clear();
    }
    QGCTileSet &operator+=(QGCTileSet &other){
        tileX0+=other.tileX0;
        ...
        return *this;
    }
    void clear(){
        tileX0=0;
        ...
    }
    int tileX0;
    int tileX1;
    int tileY0;
    int tileY1;
    quint64 tileCount;
    quint64 tileSize;
}
class QGCMapEngine:public QObject{
    Q_OBJECT
public:
    QGCMapEngine();
    ~QGCMapEngine();
    void init();
    void addTask(QGCMapTask *task);
    ...
extern QGCMapEngine* getQGCMapEngine();
extern void destroyMapEngine();
}
```
### Q_PROPERTY
是一个宏，Q_PROPERTY()是一个宏，用来在一个类中声明一个属性property，由于该宏是qt特有的，需要用moc进行编译，故必须继承于QObject类。 
Q_PROPERTY(Qstring name READ name NOTIFY nameChanged) 
QString： 类型 
name：属性名 
READ：读属性 
name:   通过name()函数可以读取这个属性 
NOTIFY：定义一个信号，当这个属性值发生变化后面函数会自动触发
### QGCMapTileSet.h
### QGCMapEngine.cpp
```
QGCMapengine::QGCMapEngine():_urlFactory(new UrlFactory())
#ifdef WE_ARE_KOSHER
    ...
    _userAgent("QGRoundControl(Macintosh;Interxxx")
#else
    #if defined Q_OS_MAC
        ,_userAgent("Mozilla/5.0(Macintosh;xxxxx)"
    ...
#endif
    ,_maxDiskCache(0)
```
