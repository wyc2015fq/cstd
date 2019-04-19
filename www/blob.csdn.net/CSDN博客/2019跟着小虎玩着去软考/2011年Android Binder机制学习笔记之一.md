# 2011年Android Binder机制学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月29日 18:51:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2070
**2011****年****Android Binder****机制学习笔记之一**
# 一．Bn是什么缩写？
Binder Native
# 二．Bp是什么缩写？
Binder Proxy
# 三．Binder两个方面?
从应用程序的角度看Binder一共有两个方面：
■Native 本地：例如BnABC，这是一个需要被继承和实现的类。
■Proxy 代理：例如BpABC，这是一个在接口框架中被实现，但是在接口中没有体现的类。
客户端：例如客户端得到一个接口ABC，在调用的时候实际上被调用的是BpABC
# 四．Binder的驱动程序在哪个目录？
kernel/include/linux/binder.h
kernel/drivers/android/binder.c
# 五．Binder驱动是一个什么设备？
是一个miscdevice，主设备号为10；
# 六．Binder设备节点，或者说文件路径是什么？
/dev/binder
# 七．Binder驱动程序在proc文件系统建立的信息，包含什么信息？
proc目录：调用Binder各个进程的内容
state文件：使用函数binder_read_proc_state
stats文件：使用函数binder_read_proc_stats
transactions文件：使用函数binder_read_proc_transactions
transaction_log文件：使用函数binder_read_proc_transaction_log，其参数为binder_transaction_log (类型为struct binder_transaction_log)
failed_transaction_log文件：使用函数binder_read_proc_transaction_log 其参数为
binder_transaction_log_failed (类型为struct binder_transaction_log)
# 八．Binder在proc文件夹位置：
/proc/binder
# 九．BR与BC的含义
BR_XXX等宏为BinderDriverReturnProtocol，表示Binder驱动返回协议。
BC_XXX等宏为BinderDriverCommandProtocol，表示Binder驱动命令协议。
# 十．binder_thread数据结构是怎么样的？
struct binder_thread {
struct binder_proc *proc;
struct rb_node rb_node;
int pid;
int looper;
struct binder_transaction *transaction_stack;
struct list_head todo;
uint32_t return_error;
uint32_t return_error2;
wait_queue_head_t wait;
struct binder_stats stats;
};
# 十一．binder_write_read是怎么样的？
struct binder_write_read {
signed long write_size; 
signed long write_consumed; 
unsigned long write_buffer;
signed long read_size; 
signed long read_consumed; 
unsigned long read_buffer;
};
# 十二．binder_thread 的各个成员信息是从哪里获得的？
rb_node
# 十三．Servicemanager的作用
servicemanager是一个守护进程，用该进程和/dev/binder进行通讯。
# 十四．servicemanager可执行程序的路径：
/system/bin/servicemanager
# 十五．BnInterface和BpInterface是两个重要的模版，在哪个文件中定义的？
IInterface.h文件中
# 十六．BnInterface和BpInterface是如何定义的？
BnInterface模版的定义如下所示：
template class BnInterface : public INTERFACE, public BBinder
{
public:
virtual spqueryLocalInterface(const String16& _descriptor);
virtual String16getInterfaceDescriptor() const;
protected:
virtual IBinder*onAsBinder();
};
BpInterface模版的定义如下所示：
template class BpInterface : public INTERFACE, public BpRefBase
{
public:
BpInterface(const sp& remote);
protected:
virtual IBinder*onAsBinder();
};
# 十七．IsericeManager相关的两个文件是什么？
ISericeManager.h
ISericeManager.cpp
# 十八．ISericeManager是系统最先被启动的服务
**十九．IserviceManager知识要点：**
1．ISericeManager本地功能并没有使现，
2．它实际上由ServiceManager守护进程执行，
3．用户程序通过调用BpServiceManager来获得其他的服务
# 二十．如何得到默认的或者缺省的ISericeManager
在ISericeManager.h中定义了一个接口，用于得到默认的ISericeManager：
sp_defaultServiceManager();此时得到的ISericeManager实际上是一个全局的ISericeManager。
