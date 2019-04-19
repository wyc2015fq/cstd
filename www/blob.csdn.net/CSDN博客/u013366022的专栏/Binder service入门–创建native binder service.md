# Binder service入门–创建native binder service - u013366022的专栏 - CSDN博客
2016年05月03日 13:38:45[slitaz](https://me.csdn.net/u013366022)阅读数：381
本文原创作者:[Cloud Chou.](http://weibo.com/muguachou) 欢迎转载，请注明出处和[本文链接](http://www.cloudchou.com/android/post-332.html)
http://www.cloudchou.com/android/post-332.html
## 1.前言
Binder service入门系列：
- 
Binder service入门—Framework binder service:
[http://www.cloudchou.com/android/post-447.html](http://www.cloudchou.com/android/post-447.html)
- 
Binder service入门—应用层binder service: 
[http://www.cloudchou.com/android/post-458.html](http://www.cloudchou.com/android/post-458.html)
- 
Binder service入门—框架层、应用层调用native binder service:
[http://www.cloudchou.com/android/post-468.html](http://www.cloudchou.com/android/post-468.html)
这些天原计划研究Android启动过程，虽然先前研究过，但是有些机制还是没弄清楚，主要卡在binder service这块，不知道SystemServer调用init1后如何反过来调用init2的，也不知道框架层的那些service和service manager的关系，还有本地service和service manager的关系。网上看了不少资料，感觉很乱，有很多都介绍了binder的详细实现，但大多都只详细介绍各个数据结构的作用，各个数据结构之间的关系都弄不清，也有很多将驱动层和系统层混在一起介绍的，感觉一点都理不清。我觉得学习一样东西，最好自顶向下，并且能有简单的实例入门最好。
接下来我将以实例说明如何编写native的binder service，本实例将向service manager注册一个native的binder service，并用本地程序测试该binder service,整个工程都可以在github上下载：
[https://github.com/cloudchou/NativeBinderTest](https://github.com/cloudchou/NativeBinderTest)
建议读者将源码下载下来在本地看，否则难理解。
## 2.程序构成
编译binder程序需要链接binder动态链接库，应用开发环境下使用ndk编程是不能链接binder动态链接库的，故此需要在源码开发环境下。本实例在vendor目录下建立了子目录shuame，然后把工程目录放在该目录下。
程序由2个部分组成，一个实现binder service的服务端程序，一个测试binder service的client程序，对应的Android.mk如下所示：
|1234567891011121314151617181920212223242526|LOCAL_PATH :=$(**call** my-dir)*#生成binder service的服务端***include**$(CLEAR_VARS)LOCAL_SHARED_LIBRARIES := \    libcutils \    libutils \    libbinder LOCAL_MODULE    := TestServerLOCAL_SRC_FILES := \    TestServer.cpp \    ITestService.cppLOCAL_MODULE_TAGS := optional**include**$(BUILD_EXECUTABLE)*#生成binder service的测试client端***include**$(CLEAR_VARS)LOCAL_SHARED_LIBRARIES := \    libcutils \    libutils \    libbinder LOCAL_MODULE    := TestClientLOCAL_SRC_FILES := \    TestClient.cpp \    ITestService.cppLOCAL_MODULE_TAGS := optional**include**$(BUILD_EXECUTABLE)|
## 3.程序源码构成
Test.h ：包含需要用到的头文件，声明接口，定义操作枚举，声明binder引用类
ITestService.cpp： 接口类方法的实现
TestServer.cpp: 声明并实现binder实体类 ，启动binder服务，并在service manager里注册
TestClient.cpp： 声明并实现binder 引用类， 测试binder服务的client
### 实现binder service的步骤：
### 1) 声明service 接口ITestService(Test.h)
|1234567|```cppclass ITestService : public Iinterface {     public:        */*使用该宏，相当于添加了一些代码，后面会有详细分析*/*             DECLARE_META_INTERFACE(TestService);         virtual void test()=0; //该服务对外提供的操作接口};```|
使用DECLARE_META_INTERFACE(TestService);相当于添加了下述代码：
|123456|```cppstatic const android::String16 descriptor;static android::sp<ITestService> asInterface(          const android::sp<android::IBinder>& obj);virtual const android::String16& getInterfaceDescriptor() const;ITestService ();virtual ~ITestService();```|
也就说添加了字段descriptor，也添加了两个成员函数asInterface，getInterfaceDescriptor, 还添加了构造器和析构器
### 2)为ITestService接口的所有方法声明枚举，一个枚举值对应ITestService接口的一个方法(Test.h)
|123|```cppenum{   TEST = IBinder::FIRST_CALL_TRANSACTION,};```|
TEST相当于ITestService接口里的test方法，后续会为该枚举值调用test方法(Test.h)
### 3)声明binder引用类BpTestService(Test.h)
|12345|```cppclass BpTestService: public BpInterface<ITestService> {    public:    	BpTestService(const sp<IBinder>& impl);    	virtual void test();};```|
因为实现ITestService接口的方法时，必须使用binder 引用类，所以binder引用类需要在Test.h里声明。但是实际上binder引用类的实现将放在TestClient.cpp里，而不是ITestService.cpp里。
### 4)实现ITestService接口的方法(ITestService.cpp)
|1|```cppIMPLEMENT_META_INTERFACE(TestService, "android.TestServer.ITestService");```|
使用IMPLEMENT_META_INTERFACE(TestService, “android.TestServer.ITestService”);相当于添加了下述代码：
|123456789101112131415161718192021222324252627|```cpp*/*初始化用宏DECLARE_META_INTERFACE声明的静态字段descriptor*/*const android::String16 ITestService::descriptor("android.TestServer.ITestService");*/*实现用宏DECLARE_META_INTERFACE声明的方法getInterfaceDescriptor */*const android::String16&                                                  ITestService::getInterfaceDescriptor() const {     return ITestService::descriptor;                   }                                                                   */*实现用宏DECLARE_META_INTERFACE声明的方法asInterface */*android::sp<ITestService> ITestService::asInterface(           const android::sp<android::IBinder>& obj)      {                                                          android::sp<ITestService> intr;                        if (obj != NULL) {                   */*在binder实体类BnInterface里会执行queryLocalInterface*/*                              intr = static_cast<ITestService*>(                         obj->queryLocalInterface(                                      ITestService::descriptor).get());          */*在binder引用类BpInterface里会创建BpInterface的子类对象*/*                    if (intr == NULL) {                                        intr = new BpTestService(obj);                     }                                                  }                                                      return intr;                                       }                                                      */*实现用宏DECLARE_META_INTERFACE声明的构造器和析构器 */*ITestService::ITestService() { }                     ITestService::~ITestService() { }```|
### 5) 实现binder引用类BpTestService(TestClient.cpp)
|12345678910|```cppBpTestService::BpTestService(const sp<IBinder>& impl) :  BpInterface<ITestService>(impl) {}void BpTestService::test() {   printf("in the get Test**\n**");   Parcel data, reply;   data.writeInterfaceToken(ITestService::getInterfaceDescriptor());   remote()->transact(TEST, data, &reply);   printf("send Print %d**\n**", reply.readInt32());}```|
service 的 client端通过service manager拿到的ITestService接口对象其实就是一个binder引用，也就是说其实是一个BpTestService对象，它也会实现ITestService接口的test方法，但是它并没有实现test的功能，而是跨进程调用Binder实体对象BnInterface的test方法。因为涉及到跨进程调用，所以并没有直接调用，而是通过binder驱动转交给服务进程一些参数，然后就阻塞住。服务进程收到这些参数后，知道client端调用test方法，此时服务端再调用test方法，执行完后把执行结果通过binder驱动再返回给client所在进程，此时client的test方法收到结果后再返回。
我们实际编程的时候并没有直接与binder驱动交互，而是通过Android为binder专门设计的一些核心库和框架来交互，这样简化了代码编写复杂度。像BpInterface便是核心库和框架提供的一个类，我们在实现时，只需要调用BpInterface的构造器，并实现ITestService接口的test方法即可，实现test方法时，通过remote()调用transact方法就能把数据提交至service端，servcie的返回结果会放在reply里。
### 6) 实现client端(TestClient.cpp)
|1234567891011|```cppint main() {    */*获取service manager引用*/*sp<IServiceManager> sm = defaultServiceManager();*/*获取test service的binder接口对象*/*sp<IBinder> binder = sm->getService(String16("service.testservice"));*/*转为sp<ITestService>*/*sp<ITestService> cs = interface_cast<ITestService>(binder);*/*通过binder引用调用test方法*/*    cs->test();    return 0;}```|
interface_cast(binder); 实际上调用的方法是用宏DECLARE_META_INTERFACE声明的方法ITestService::asInterface，这里用了内联函数模版。
ProcessState和IPCThreadState是真正直接和binder驱动交互的类，核心库和框架层都是通过和这两个类交互才能和驱动间接交互。它们通过ioctl和binder驱动交互，ProcessState主要负责管理所有的binder引用，IPCThreadState则和binder传输数据。以后如果有时间再详细介绍这两个类。
### 7) 声明并实现binder实体类BnInterface(TestServer.cpp)
|12345678910|```cppclass BnTestService : public BnInterface<ITestService>{ public:    virtual status_t    onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);    virtual void    test() {        printf("Now get test**\n**");    }};```|
BnInterface也是binder核心库和框架中的类，表示binder实体类，在onTransact函数里，会根据客户端提交的操作代码调用不同的函数，而操作代码就是2)里声明的枚举值，在本实例里是TEST,我们收到test后会调用test方法。本实例里将onTransact的实现放在第5步。
### 8) 实现binder实体类的onTransact方法(TestServer.cpp)
|12345678910111213141516171819|```cppIMPLEMENT_META_INTERFACE(TestService, "android.TestServer.ITestService");status_tBnTestService::onTransact(uint_t code, const Parcel& data, Parcel* reply, uint32_t flags) {    switch (code)        {    case TEST:        {            printf("got the client msg**\n**");            CHECK_INTERFACE(ITest, data, reply);            test();            reply->writeInt32(100);            return NO_ERROR;        }        break;    default:        break;        }    return NO_ERROR;}```|
在onTransact里转调test方法即可。
### 9) 实现server端(TestServer.cpp)
|12345678910|```cppint main() {    ProcessState::self();  //初始化单例    */*获取service manager的binder引用*/*    sp<IServiceManager> sm = defaultServiceManager();     */*添加服务 注意字符串必须用String16类型*/*    sm->addService(String16("service.testservice"), new BnTestService());    ProcessState::self()->startThreadPool(); //启动线程池    IPCThreadState::self()->joinThreadPool();//等待线程结束    return 0;}```|
## 4.测试
将TestServer和TestClient推到/data/local/tmp，并修改权限
adb push TestServer /data/local/tmp
adb push TestClient /data/local/tmp
adb shell chmod 755 /data/local/tmp/*
然后开两个cmd窗口测试：
![binder_test](http://www.cloudchou.com/wp-content/uploads/2014/04/binder_test-1024x238.png)
