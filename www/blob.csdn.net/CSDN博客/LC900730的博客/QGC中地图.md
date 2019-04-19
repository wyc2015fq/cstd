# QGC中地图 - LC900730的博客 - CSDN博客
2017年08月18日 18:24:26[lc900730](https://me.csdn.net/LC900730)阅读数：967
### QGCLocationPlugin
定义了QGCLocationPlugin.pri文件，这个文件包含QtLocationPlugin文件夹里面的头文件和源文件
在main.cc文件中，include “QGCMapEngine.h”,这个头文件与Qt项目中总体头文件不在一个目录中，但是之所以能直接包含，就是因为在.pro文件中include QGCLocationPlugin.pri。
### main.cc中getQGCMapEngine()
在main.cc中使用了这个getQGCMapEngine()->init(),不是对象或者类调用，经过追代码发现是在QGC。
```
extern QGCMapEngine * getQGCMapEngine();
extern void destroyMapEngine();
```
1️⃣当extern 与C一起使用,如extern “C” void func(int a,int b);则告诉编译器在编译func这个函数名字的时候； 
2️⃣当extern 不与C一起使用的时候，如在头文件extern int g_int,它的作用就是声明函数或者全局变量，其声明的函数和变量可以在本模块或者其他模块中使用。是声明而不是定义。 
QGCMapEngine.cpp中定义了 
kMapTypes[]={} 
kMapboxTypes[]={} 
kEsriTypes[]={}
#### getQGCMapEngine()
```
static QGCMapEngine *kMapEngine=null;
    QGCMapEngine *getQGCMapEngine(){
        if(!kMapEngine)
            kMapEngine=new QGCMapEngine();
            return kMapEngine;
    }
```
#### destroyMapEngine()
```
void destroyMapEngine()(){
        if(kMapEngine)
            kMapEngine=null;
    }
```
### 地图的表示
```
struct stQGeoTileCacheQGCMapTypes{
        const char * name;
        UrlFactory::Maptype type;
    }
```
## QGCMapEngine
```
//构造方法中给_urlFactory赋值
    QGCMapEngine::QGCMapEngine():_urlFactory(new UrlFactory())
    _userAgent()
    ,_maxDiskCache(0)
    ,_prunning(false)
    ,_cacheWasReset(false)
    ,_isInternetActive(false)
    //注册
    qRegisterMetaType<QGCMapTask::TaskType>();
    qRegisterMetaType<QGCTile>();
    qRegisterMetaType<QList<QGCTile *>>();
    //信号与槽的连接
    connect(&_worker,&QGCCacheWorker::updateTotals,this,&QGCMapEngine::_updateTotals);
    connect(&_worker,&QGCCacheWorker::internetStatus,this,&QGCMapEngine::_internetStatus);
```
### QGCMapEngine析构方法
```
QGCMapEngine::~QGCMapEngine(){
        _worker.quit();
        _worker.wait();
        if(_urlFactory)
            delete _urlFactory;     
    }
```
### 加载地图的init()
```
void QGCMapEngine::init(){
        _wipeOldCache();
    ...
    //设置地图缓存目录
    ...
    QGCMapTask *task=new QGCMapTask(QGCMapTask::taskInit);
    _worker.enqueueTask(task);
    }
```
### 从地图名称中获取类型
```
UrlFactroy::MapType QGCMapEngine::getTypeFromName(const QString &name){
    size_t i;
    for(i=0;i<NUM_MAPS;i++){
        if(name.compare(kMapTypes[i].name,Qt::CaseInsensitive)==0)
            return kMapTypes[i].type;
    }
    ....
    如果是kMapboxType类型，那么遍历后返回
        return kMapboxTypes[i].type;
    ...
    如果是kEsriTypes类型，那么遍历后返回
    kEsriTypes[i].type
}
//如果都不是，那么返回UrlFactory::Invalid
```
### 获得地图名称列表getMapNameList
```
QStringList QGCMapEngine::getMapNameList(){
    QStringList mapList;
    for(size_t i=0;i<NUM_MAPS;i++){
        mapList<<kMapTypes[i].name;
    }
    if(!qgcApp()->toolox()->settingManager()->appSettings()->mapboxToken()->rawValue().toString().isEmpty()){
        for(size_t i=0;i<NUM_MAPBOXMAPS;i++){
            mapList<<kMapboxTypes[i].name;
        }
    }
    if(!qgcApp()->toolox()->settingManager()->appSettings()->esriToken()->rawValue().toString().isEmpty()){
        for(size_t i=0;i<NUM_ESRIMAPS;i++){
            mapList<<kEsriTypes[i].name;
        }
    }
    return mapList;
```
### QGCMapEngine中的_worker
private: 
    QGCCacheWorker _worker;
定义在QGCTileCacheWorker.cpp中
### QGCCacheWorker::enqueueTask
```
class QGCCacheWorker:public QThread{
    Q_OBJECT
public:
    QGCCacheWorker();
    ~QGCCacheWorker();
    void quit();
    bool enqueueTask(QGCMaoptask *task);
    void setDatabaseFile(const QString &path);
}
void QGCCacheWorker::run(){
if(!_valid&&!_failed){
    _init();
}
if(_valid){
    _db=new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE",kSession));
    _db->setDatabaseName(_databasePath);
    _db->setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
    _valid=_db->open();
}
while(true){
    QGCMapTask* task;
    if(_taskQueue.count()){
        _mutex.lock();
        task=_taskQueue.dequeue();
        _mutex.unlock();
        switch(task->type()){
            case QGCMapTask::taskInit
                break;
            case QGCMapTask::taskCacheTile
                _saveTile(task);
                break;
        ...
    }
    task->deleteLater();
    //根据task数量来确定时间间隔
    size_t count =_taskQueue.count();
    if(count>100){
            //define LONG_TIMEOUT  5
        _updateTimeout=LONG_TIMEOUT;
    }else if(count<25){
            //define LONG_TIMEOUT  2
        _updateTimeout=SHORT_TIMEOUT;
    }
    }   
}
}
}
```
//测试网络
```
QGCCacheWorker::_testInternet(){
QTcpSocket socket;
socket.connectToHost("www.github.com",80);
if(socket.waitForConnected(2500)){
    qCDebug(QGCTileCacheLog)<<"Yes Internet Access";
    emit internetStatus(true);
    return;
    }
    qWarning()<<"No Internet Access";
    emit internetStatus(false);
}
```
### class QGCMapEngineData
定义了class QGCTile
```
class QGCTile{
    public QGCTile():
        :_x(0)
        ,_y(0)
        ,_z(0)
        ,_set(UINT64_MAX)
        ,_type(UrlFactory::Invalid){
    }
enum TyleState{
    StatePending=0,
    StateDownloading,
    StateError,
    StateComplete
};
}
```
//定义了QGCCacheTile
```
class QGCCacheTile:public QObject{
        Q_OBJECT
    }
```
//定义了QGCMapTask类
```
class QGCMapTask:public QObject{
        Q_OBJECT
    public:
            enum TaskType{
                taskInit,
                taskTestInternet,
                ...
                ...
            };
    //构造函数，需要初始化一个值
    QGCMapTask(TaskType type):_type(type){}
    virtual ~QGCMapTask(){}
    }
//虚函数，C++中多态的体现
virtual TaskType type(){
    return _type;
}
```
### QGCMapTileSet.h
```
class QGCCachedTileSet:public QObject{
    Q_OBJECT
public:
    QGCCachedTileSet(const QString &name);
    ~QGCCachedTileSet();
}
```
### 地图中的UrlFactory
```
class UrlFactory :public QObject{
    Q_OBJECT
public:
    enum MapType{
        Invalid =-1;
        GoogleMap =1;
        ...
    };
    UrlFactory();
    ~UrlFactory();
}
#ifdef QGC_NO_GOOGLE_MAPS
    bool _googleVersionRetrived;
    QNetworkReply* _googleReply;
    ...
    QString _versionGoogleMap;
    ...
#endif
//BingMaps
QString _versionBingMaps;
```
