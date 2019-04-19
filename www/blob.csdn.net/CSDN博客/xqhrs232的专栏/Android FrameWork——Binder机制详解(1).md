# Android FrameWork——Binder机制详解(1) - xqhrs232的专栏 - CSDN博客
2012年03月01日 16:06:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：511标签：[android																[transactions																[java																[null																[blog																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=blog&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=transactions&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/stonecao/article/details/6657438](http://blog.csdn.net/stonecao/article/details/6657438)
1.前面我曾经发表过一篇blog介绍了aidl实现机制(aidl实现机制浅析)，不过那只是停留在java表层，并遗留了一个问题,那就是BinderProxy.transact 该方法本地化实现，同时我指出了它的具体代码位置：
\frameworks\base\core\jni\android_util_Binder.cpp->static jboolean android_os_BinderProxy_transact(JNIEnv* env, jobject obj,jint code, jobject dataObj,jobject replyObj, jint flags)
要进一步深入进去，其实涉及了android ipc机制，也就是android独有进程通信机制的Binder，本文主要就是想深入说明Binder机制，
首先说明，这个有点难度，只是对我来说，呵呵，我参加完台湾教授，高焕堂先生开设的android高阶培训课程（android软硬件结合开发培训），并反复看了课件android底层Android框架底层结构知多少？以及网上其它同仁的blog后才有所体会，不过，在些这blog前我还是有不少不解之处，我和大家一样，带着疑问去写这个blog,边写边学习，有问题我会遗留，同时可能有不少不正确的地方，希望路过的高手解答一下，小弟不胜感激。
2.好了，结束开场白，开始Binder机制的探究，就从android_util_Binder.cpp->static jboolean android_os_BinderProxy_transact函数开始，当我们在java层使用java进行进程通信时BinderProxy.transact是本地方法，它jni实现方法是android_os_BinderProxy_transact：
static jboolean android_os_BinderProxy_transact(JNIEnv* env, jobject obj,
                                                jint code, jobject dataObj,
                                                jobject replyObj, jint flags)
{
   //把java对象转换成c对象
   ...
  //把java层的obj(BinderProxy)对象转换成C层的BpBinder对象
   IBinder* target = (IBinder*)
        env->GetIntField(obj, gBinderProxyOffsets.mObject);
   status_t err = target->transact(code, *data, reply, flags);//BpBinder->transact
    ...
}
我只写出了关键代码，具体的你可以认真阅读相关代码，其实该函数主要做的工作就是把java对象转成c++对象，然后调用BpBinder->transact方法。
3.BinderProxy，BpBinder等这些对象你看了可能有些晕呼呼，这些是什么呢，在进一步深入研究前先画一个类静态结构图描述一下这些类之间的相关关系：
![](http://hi.csdn.net/attachment/201108/3/0_1312361831pmAm.gif)
                                                    (图1)
--1.在java层，有一个IBinder接口，这个在读这个blog前你应该已经很熟悉了，它有两个实现类，一个是BinderProxy,是用作客户端的，一个是Binder用作服务端的存根类
--2.类似在C++层，也有一个IBinder接口，同样它也有两个子类，BpBinder用作客户端代理类，BBinber用作服务端存根类
--3.从上图可以很清晰的看出我在段落2中写的BinderProxy.transact调用轨迹，BinderProxy.transact()-->android_util_Binder.cpp(android_os_BiinderProxy_transact())-->BpBinder.transact()
4.接下来我们进入BpBinder.transact()研究一下：
//code:指定调用哪个函数的code常量
//data:java Parcel Object to c++ Parcel Object,不过这里作为引用传递参数防止修改data指针(C++大学学过，工作中一直从事java开发，若有误，请提出)
//reply:返回数据Parcel对象指针
//flags:0：normal调用，同步方式，1：异步调用，发送数据结束后不等待调用结果返回
status_t BpBinder::transact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    // Once a binder has died, it will never come back to life.
    if (mAlive) {
status_t status = IPCThreadState::self()->transact(
            mHandle, code, data, reply, flags);
        if (status == DEAD_OBJECT) mAlive = 0;
        return status;
    }
    return DEAD_OBJECT;
}
////////////////////////////////////////////////////////////////////////////
/*BpBinder::transact-->*/
//handle:==BpBinder.mHandle,它在BpBinder构建的时候被赋值，它代表BpBinder所连接的BBinder所在的地址
//其它参数见上文说明
status_t IPCThreadState::transact(int32_t handle,
                                  uint32_t code, const Parcel& data,
                                  Parcel* reply, uint32_t flags)
{
    status_t err = data.errorCheck();//校验传入参数
    flags |= TF_ACCEPT_FDS;
    IF_LOG_TRANSACTIONS() {
        TextOutput::Bundle _b(alog);
        alog << "BC_TRANSACTION thr " << (void*)pthread_self() << " / hand "
            << handle << " / code " << TypeCode(code) << ": "
            << indent << data << dedent << endl;
    }
    if (err == NO_ERROR) {
        LOG_ONEWAY(">>>> SEND from pid %d uid %d %s", getpid(), getuid(),
            (flags & TF_ONE_WAY) == 0 ? "READ REPLY" : "ONE WAY");
        err = writeTransactionData(BC_TRANSACTION, flags, handle, code, data, NULL);//传输数据
    }
    if (err != NO_ERROR) {
        if (reply) reply->setError(err);//校验或者传输数据出错，设置返回数据错误信息
        return (mLastError = err);
    }
    if ((flags & TF_ONE_WAY) == 0) {
        if (reply) {
            err = waitForResponse(reply);//等待返回数据
        } else {
            Parcel fakeReply;
            err = waitForResponse(&fakeReply);
        }
        IF_LOG_TRANSACTIONS() {
            TextOutput::Bundle _b(alog);
            alog << "BR_REPLY thr " << (void*)pthread_self() << " / hand "
                << handle << ": ";
            if (reply) alog << indent << *reply << dedent << endl;
            else alog << "(none requested)" << endl;
        }
    } else {
        err = waitForResponse(NULL, NULL);
    }
    return err;
}
我再对代码进行一下梳理，BpBinder.transact()方法很简单，它调用了IPCThreadState::self()->transact(mHandle, code, data, reply, flags);就结束了，并没有做什么事情，不过增加了一个参数，mHandle，它在BpBinder被构建的时候创建的，而具体mHandle值是怎么得来的，涉及到serviceManager框架，我这里先保留这个问题，在后面我弄清楚serviceManager框架后再对该问题进行说明。
然后再看下IPCThreadState::transact的实现，我再代码中作出了简单的注释，详细的代码逻辑我也没有搞太清楚，阅读C++代码，我能力还是有待提高，我关注两个关键函数调用，一个是传输数据，另一个是等待数据返回，我在函数中已经把它标注成为红色了。
--cmd==BC_TRANSACTION
--binderFlags==处理后的flags, flags |= TF_ACCEPT_FDS;
--handle BpBinder句柄
--data 传输数据引用
--statusBuffer==null
status_t IPCThreadState::writeTransactionData(int32_t cmd, uint32_t binderFlags,
    int32_t handle, uint32_t code, const Parcel& data, status_t* statusBuffer)
{
    binder_transaction_data tr;//构造传输数据包
    tr.target.handle = handle;//设置目标句柄
    tr.code = code;//设置目标函数code
    tr.flags = binderFlags;//设置调用方式，是否等待返回
    const status_t err = data.errorCheck();//传输参数数据校验
    if (err == NO_ERROR) {
        tr.data_size = data.ipcDataSize();//设置消息体长度
        tr.data.ptr.buffer = data.ipcData();//设置消息体
        tr.offsets_size = data.ipcObjectsCount()*sizeof(size_t);//??
        tr.data.ptr.offsets = data.ipcObjects();
    } else if (statusBuffer) {
        tr.flags |= TF_STATUS_CODE;
        *statusBuffer = err;
        tr.data_size = sizeof(status_t);
        tr.data.ptr.buffer = statusBuffer;
        tr.offsets_size = 0;
        tr.data.ptr.offsets = NULL;
    } else {
        return (mLastError = err);
    }
    mOut.writeInt32(cmd);//写入命令
    mOut.write(&tr, sizeof(tr));//写入数据
    return NO_ERROR;
}
这里已经可以看到了IPC底层通信的一些轨迹了，不过遗憾我还没有弄懂mOut是怎么被赋值，如何与Binder驱动建立联系的，若路过的高手知道的话，希望能够不吝赐教
--waitForResponse
//reply返回数据的Parcel指针
//acquireResult:??
status_t IPCThreadState::waitForResponse(Parcel *reply, status_t *acquireResult)
{
    int32_t cmd;//返回消息命令
    int32_t err;
    while (1) {//消息接收循环
        if ((err=talkWithDriver()) < NO_ERROR) break;//跟Binder Driver通信，看是否有要接收数据
        err = mIn.errorCheck();
        if (err < NO_ERROR) break;
        if (mIn.dataAvail() == 0) continue;        
        cmd = mIn.readInt32();//获取消息命令        
       ...
        //BR开头的cmd都是返回数据，否则就应该是当该进程作为服务端的时候接收来自客户端的命令
        switch (cmd) {
        case BR_TRANSACTION_COMPLETE:
           ...
        case BR_DEAD_REPLY:
            ...        
           ...      
        case BR_REPLY://猜测这表示有返回数据的命令
            {
                binder_transaction_data tr;
                err = mIn.read(&tr, sizeof(tr));
                LOG_ASSERT(err == NO_ERROR, "Not enough command data for brREPLY");
                if (err != NO_ERROR) goto finish;
                if (reply) {
                    if ((tr.flags & TF_STATUS_CODE) == 0) {
                        reply->ipcSetDataReference(//设置返回数据
                            reinterpret_cast<const uint8_t*>(tr.data.ptr.buffer),
                            tr.data_size,
                            reinterpret_cast<const size_t*>(tr.data.ptr.offsets),
                            tr.offsets_size/sizeof(size_t),
                            freeBuffer, this);
                    } else {
                        err = *static_cast<const status_t*>(tr.data.ptr.buffer);
                        freeBuffer(NULL,
                            reinterpret_cast<const uint8_t*>(tr.data.ptr.buffer),
                            tr.data_size,
                            reinterpret_cast<const size_t*>(tr.data.ptr.offsets),
                            tr.offsets_size/sizeof(size_t), this);
                    }
                } else {
                    freeBuffer(NULL,
                        reinterpret_cast<const uint8_t*>(tr.data.ptr.buffer),
                        tr.data_size,
                        reinterpret_cast<const size_t*>(tr.data.ptr.offsets),
                        tr.offsets_size/sizeof(size_t), this);
                    continue;
                }
            }
            goto finish;
        default://不是返回数据，作为服务端接收来自客户端的命令？？
            err = executeCommand(cmd);
            if (err != NO_ERROR) goto finish;
            break;
        }
    }
finish:
    if (err != NO_ERROR) {
        if (acquireResult) *acquireResult = err;
        if (reply) reply->setError(err);
        mLastError = err;
    }
    return err;
}
为了保证文章篇幅，我可能剪裁了一些不是很相关代码，我并不打算细致了解waitForResponse，我只是要知道它是如何接收并设置返回数据的，mIn与mOut一样，我同样不知道它是怎么构建的，我暂且把它理解成Binder驱动设备的输入输出流，这样理解应该是没有问题的，待有进一步的学习了解，我再作说明
5.IPCThreadState* IPCThreadState::self()
为什么我这里要讨论 IPCThreadState::self函数呢，如果你是带着思考来看我的blog的话你可能已经发现了IPCThreadState::transact函数是有点问题的，什么问题呢？看我在代码中标注为红色的传输数据和等待返回数据的两个函数调用，这样的同步调用显然是需要同步锁的，不然的话多线程执行的话就会有问题的，可能我等待回来的数据是另一个binder调用返回的结果，而一个客户端显然是容许多个线程调用多个binder的，而 IPCThreadState::self()似乎跟java中的单例对象很像，这样的话所有线程都调用transact而又不加同步锁的话是有问题的，我后面仔细看了好几回代码，里外都没有加锁的代码，最后发现问题在IPCThreadState::self()：
IPCThreadState* IPCThreadState::self()
{
    if (gHaveTLS) {//gHaveTLS:猜测表示IPCThreadState是否已经存在
restart:
        const pthread_key_t k = gTLS;
        IPCThreadState* st = (IPCThreadState*)pthread_getspecific(k);//获取当前线程绑定的IPCThreadState
        if (st) return st;
        return new IPCThreadState;
    }
    if (gShutdown) return NULL;
    pthread_mutex_lock(&gTLSMutex);
    if (!gHaveTLS) {//IPCThreadState 没有绑定到当前线程
        if (pthread_key_create(&gTLS, threadDestructor) != 0) {//创建并一个IPCThreadState到当前线程，并把引用地址回填gTLS
            pthread_mutex_unlock(&gTLSMutex);//绑定失败，释放锁，并返回null
            return NULL;
        }
        gHaveTLS = true;//绑定成功，修改标志变量
    }
    pthread_mutex_unlock(&gTLSMutex);//释放锁
    goto restart;
}
我不知道我上面的注释是否正确，涉及到系统函数调用，我理解不一定正确，如你是C开发者，请帮我指正，谢谢！
从我的注释，你应该已经理解了，self函数返回的是线程绑定对象，而非是单例对象，这个跟数据库事务线程绑定应该是类似的，看完这个我就恍然大悟，原来IPCThreadState是线程持有的，也就是每个线程拥有一个对象，这样的话它
调用writeTransactionData写完数据后调用waitForResponse等待返回数据是没有问题的，就像我原来写ServerSocket的Socket连接池一样，每个Socket连接new一个线程进行通信一样，没有问题的。
