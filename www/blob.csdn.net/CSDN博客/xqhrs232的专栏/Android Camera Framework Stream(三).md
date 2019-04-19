# Android Camera Framework Stream(三) - xqhrs232的专栏 - CSDN博客
2011年06月17日 13:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：841标签：[stream																[android																[callback																[ext																[interface																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=ext&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146336.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146336.aspx)
转自：[http://blog.163.com/shawpin@126/blog/static/11666375220109231035974/](http://blog.163.com/shawpin@126/blog/static/11666375220109231035974/)
7． 那么现在的关键就是Client类了·进一步跟进：
CameraService::Client::Client(const sp<CameraService>& cameraService,
const sp<ICameraClient>& cameraClient, pid_t clientPid)
{
…..
mCameraService = cameraService;
mCameraClient = cameraClient;
mClientPid = clientPid;
mHardware = openCameraHardware();
}
将cameraService和cameraClient的实例分别赋值给了Client的类成员变量。
另外openCameraHardware()是值得注意的地方，也就是连接上层应用和底层驱动的关键，通过调用openCameraHardware()得到了一个CameraHardwareInterface实例对象，并赋值给自己的类成员：`
sp<CameraHardwareInterface> mHardware;
对hardware的操作就是通过该对象完成的，所以说真正意义上的功能实现其实就是在这里，即client类的函数接口调用。
对于hardware的东东咱们暂时不去关注吧。
那么我们再次仔细研究下Client类的继承关系(这些继承关系很容易混乱，涉及到较多的多态类型转换)，这个其实往往都很关键：
Client继承于BnCamera，而BnCamera则继承于ICamera，也就是说Client继承了ICamera,实现了ICamera中的函数。
进而发现，原来绕一个大圈，把最开始的图简化下：
8． 除此之外还有两个步骤或许需要去研究下：
先从单一函数去跟进，看具体一些callback的实现流程：
// callback from camera service
void Camera::notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2)
{
sp<CameraListener> listener;
{
Mutex::Autolock _l(mLock);
listener = mListener;
}
if (listener != NULL) {
listener->notify(msgType, ext1, ext2);
}
}
这是Camera类中一个callback函数实现，但其本质在哪？先看camera类的继承关系：
通过以上的继承关系，继续跟进其父类ICameraClient:
class ICameraClient: public IInterface
{
public:
DECLARE_META_INTERFACE(CameraClient);
virtual void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2) = 0;
virtual void dataCallback(int32_t msgType, const sp<IMemory>& data) = 0;
virtual void dataCallbackTimestamp(nsecs_t timestamp, int32_t msgType, const sp<IMemory>& data) = 0;
};
其中notifyCallback()又是纯虚函数,则同样说明实现在其子类BpCameraClient中：
// generic callback from camera service to app
void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2)
{
LOGV("notifyCallback");
Parcel data, reply;
data.writeInterfaceToken(ICameraClient::getInterfaceDescriptor());
data.writeInt32(msgType);
data.writeInt32(ext1);
data.writeInt32(ext2);
remote()->transact(NOTIFY_CALLBACK,data, &reply, IBinder::FLAG_ONEWAY);
}
然后通过Binder通讯调用到BnCameraClient中实现：
status_t BnCameraClient::onTransact(
uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
switch(code) {
case NOTIFY_CALLBACK: {
LOGV("NOTIFY_CALLBACK");
CHECK_INTERFACE(ICameraClient, data, reply);
int32_t msgType = data.readInt32();
int32_t ext1 = data.readInt32();
int32_t ext2 = data.readInt32();
notifyCallback(msgType, ext1, ext2);
return NO_ERROR;
} break;
….
}
进而调用到了Camera.cpp中的函数实现了，但或许你有疑问，这些callback是涉及到一些驱动的callback，哪怎么跟驱动联系起来那？
结合之前对hardware接口调用的类Client，进一步可以发现callback的处理同样是在Client类实例化的时候：
CameraService::Client::Client(const sp<CameraService>& cameraService,
const sp<ICameraClient>& cameraClient, pid_t clientPid)
{
…..
mHardware->setCallbacks(notifyCallback,
dataCallback,
dataCallbackTimestamp,
mCameraService.get());
…..
}
调用了mHardware将callback传入，但此处的notifyCallback并不是camera.cpp中的函数，而是client类的notifyCallback函数。
再继续看client类中的notifyCallback函数实现：
void CameraService::Client::notifyCallback(int32_t msgType, int32_t ext1,int32_t ext2, void* user)
{
…..
default:
sp<ICameraClient> c = client->mCameraClient;
if (c != NULL) {
c->notifyCallback(msgType, ext1, ext2);
}
break;
…..
}
通过得到ICameraClient实例进而调用到了具体的对象Camera的notifyCallback()函数。这个地方估计会遇见跟ICameraService函数调用一样的问题，ICameraClient函数调用所需要的函数实例在哪？
记得上述ICameraService讲到的connect()函数嘛？其中有一个参数不能被忽略掉的，就是ICameraClient，但它在真正传入的时候却是一个ICameraClient子类camera的实例对象。
CameraService:
sp<ICamera> CameraService::connect(const sp<ICameraClient>& cameraClient)
{
…..
// create a new Client object
client = new Client(this, cameraClient, callingPid);
…..
}
Client:
CameraService::Client::Client(const sp<CameraService>& cameraService,
const sp<ICameraClient>& cameraClient, pid_t clientPid)
{
….
mCameraService = cameraService;
mCameraClient = cameraClient;
….
}
这样就清楚了，其实Client在调用设置callback的调用最终还是调用到了camera.cpp中的callback函数，进而将具体内容通过callback反馈给上层应用做出相应的处理。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146336.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146336.aspx)

