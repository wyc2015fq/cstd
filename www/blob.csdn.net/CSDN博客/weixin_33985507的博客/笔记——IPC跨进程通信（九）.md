# 笔记——IPC跨进程通信（九） - weixin_33985507的博客 - CSDN博客
2018年12月10日 16:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
——》个人平时笔记，看到的同学欢迎指正错误，文中多处摘录于各大博主精华、书籍
##### 1、安卓它是一种基于Linux内核的移动操作系统。Linux上可以通过命名管道、共享内存、信号量等来进行进程间通信。而安卓有自己的一套跨进程通信的实现方式，如下：
> 
<1.使用Binder
<2.Bundle绑定数据后通过intent启动另一进程的组件传递数据
<3.使用文件共享，多个进程操作同一个文件实现共享数据，缺点是容易丢失数据
<4.使用Messenger（信使，底层实现是AIDL）
<5.使用AIDL（Android Interface Defined Language）
<6.使用ContentProvider（底层实现是Binder）
<7.使用Socket通信
<8.管道、SystemV
IPC方式除了以上7、8点以外其实底层都是通过Binder来实现的，比如说Messager，Bundle，ContentProvider，Bundle，只是它们的封装方式不一样而已。
进程是具有一定独立功能的程序关于某个数据集合上的一次运行活动,进程是系统进行资源分配和调度的一个独立单位。
线程是进程的一个实体,是CPU调度和分派的基本单位,它是比进程更小的能独立运行的基本单位。
（1）线程是进程的一部分
（2）CPU调度的是线程
（3）系统为进程分配资源，不对线程分配资源
另外“:”开头的进程属于当前应用的私有进程，其他应用的组件不可以和它跑在同一个进程中，而进程名不以“:”开头的进程属于全局进程，其他应用通过ShareUID方式可以和它跑在同一个进程中。
> 
性能方面:
在移动设备上（性能受限制的设备，比如要省电），广泛地使用跨进程通信对通信机制的性能有严格的要求，Binder相对出传统的Socket方式，更加高效。Binder数据拷贝只需要一次，而管道、消息队列、Socket都需要2次，共享内存方式一次内存拷贝都不需要，但实现方式又比较复杂。
安全方面:
传统的进程通信方式（管道、SystemV、Socket等）对于通信双方的身份并没有做出严格的验证，比如Socket通信ip地址是客户端手动填入，很容易进行伪造，而Binder机制从协议本身就支持对通信双方做身份校检，因而大大提升了安全性。
###### 运行在不同进程中的组件，属于不同的虚拟机和Application，每开启一个进程就会多实例化一个Application和虚拟机，每个进程都分配一个独立的虚拟机，不同的虚拟机在内存分配上有不同的地址空间。
##### 2、Binder的使用与详解
# `AIDL文件的本质是系统为我们提供了一种快速实现Binder的工具，仅此而已。`
- 
《Android开发艺术探索》一书中定义：
1.直观来说，**Binder是Android中的一个类，它继承了IBinder接口**。
2.**从IPC角度来说**，Binder是Android中的一种跨进程通信方式，Binder还可以理解为一种虚拟的物理设备，它的设备驱动是/dev/binder，该通信方式在Linux中没有；
3.**从Android Framework角度来说**，Binder是ServiceManager连接各种Manager（ActivityManager、WindowManager，等等）和相应ManagerService的桥梁；
4.**从Android应用层来说**，Binder是客户端和服务端进行通信的媒介，当bindService的时候，服务端会返回一个**包含了服务端业务调用的Binder对象**，通过这个Binder对象，客户端就可以获取服务端提供的服务或者数据，这里的服务包括普通服务和基于AIDL的服务。
- 
Binder主要用在Service中，包括AIDL和Messenger，而Messenger底层其实是AIDL，AIDL又是用Binder。
- 
当客户端发起远程请求时，由于当前线程会被挂起直至服务端进程返回数据，所以如果一个远程方法是很耗时的，那么不能在UI线程中发起此远程请求；其次，由于服务端的Binder方法运行在Binder的线程池中，所以Binder方法不管是否耗时都应该采用同步的方式去实现，因为它已经运行在一个线程中了。
![5106443-e38cff4ff1455925.png](https://upload-images.jianshu.io/upload_images/5106443-e38cff4ff1455925.png)
aidl_binder.png
- AIDL文件实现的Binder.java类解析
```
/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: F:\\AndroidDemo\\MyView\\app\\src\\main\\aidl\\com\\fivefloor\\bo\\myview\\IBookManager.aidl
 */
package com.fivefloor.bo.myview;
// Declare any non-default types here with import statements
import android.os.IBinder;
public interface IBookManager extends android.os.IInterface {
    /**
     * Local-side IPC implementation stub class.
     */
    public static abstract class Stub extends android.os.Binder implements com.fivefloor.bo.myview.IBookManager {
        //Binder的唯一标识，一般用当前Binder的类名表示
        private static final java.lang.String DESCRIPTOR = "com.fivefloor.bo.myview.IBookManager";
        // 客户Proxy的transact，和服务Stub的onTransact使用二个标识来识别对方：
        // 一个是DESCRIPTOR，这个是标识Binder的Token，也就是说是标识服务端和客户端；
        // 方法的标识Code就是TRANSACTION_XXXXXX，是用来标识调用的是哪个方法。
        /**
         * Construct the stub at attach it to the interface.
         */
        public Stub() {
            this.attachInterface(this, DESCRIPTOR);
        }
        /**
         * 运行与服务端Server
         * <p>
         * Cast an IBinder object into an com.fivefloor.bo.myview.IBookManager interface,
         * generating a proxy if needed.
         * 将服务端的Binder对象转换成客户端所需的AIDL接口类型的对象
         */
        public static com.fivefloor.bo.myview.IBookManager asInterface(android.os.IBinder obj) {
            if ((obj == null)) {
                return null;
            }
            android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
            if (((iin != null) && (iin instanceof com.fivefloor.bo.myview.IBookManager))) {
                //如果客户端和服务端位于同一进程，那么此方法返回的就是服务端的Stub对象本身
                //仅是在同一个进程中，就直接使用Binder就好了，没有必要创建AIDL文件
                return ((com.fivefloor.bo.myview.IBookManager) iin);
            }
            //返回系统封装后的Stub.proxy对象
            return new com.fivefloor.bo.myview.IBookManager.Stub.Proxy(obj);
        }
        @Override
        public android.os.IBinder asBinder() {
            return this;
        }
        /**
         * 运行于服务端Server
         * <p>
         * 这个方法运行在服务端中的Binder线程池中，
         * 当客户端发起跨进程请求时，远程请求会通过系统底层封
         * 装后交由此方法来处理（处理客户端的请求）
         *
         * @param code  目标方法code
         * @param data  输入型Parcel对象
         * @param reply 输出型Parcel对象
         * @param flags
         * @return
         * @throws android.os.RemoteException
         */
        @Override
        public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException {
            java.lang.String descriptor = DESCRIPTOR;
            //服务端通过code可以确定客户端所请求的目标方法是什么
            switch (code) {
                case INTERFACE_TRANSACTION: {
                    reply.writeString(descriptor);
                    return true;
                }
                case TRANSACTION_getBookList: {
                    //从data中取出目标方法所需的参数（如果目标方法有参数的话），然后执行目标方法
                    data.enforceInterface(descriptor);
                    java.util.List<com.fivefloor.bo.myview.bean.Book> _result = this.getBookList();
                    //当目标方法执行完毕后，就向reply中写入返回值（如果目标方法有返回值的话）
                    reply.writeNoException();
                    reply.writeTypedList(_result);
                    return true;
                }
                case TRANSACTION_addBook: {
                    data.enforceInterface(descriptor);
                    com.fivefloor.bo.myview.bean.Book _arg0;
                    if ((0 != data.readInt())) {
                        _arg0 = com.fivefloor.bo.myview.bean.Book.CREATOR.createFromParcel(data);
                    } else {
                        _arg0 = null;
                    }
                    //从data中取出目标方法所需的参数（如果目标方法有参数的话），然后执行目标方法
                    this.addBook(_arg0);
                    reply.writeNoException();
                    return true;
                }
                default: {
                    return super.onTransact(code, data, reply, flags);
                }
            }
        }
        /**
         * 客户端Client
         * Stub的内部代理类Proxy
         */
        private static class Proxy implements com.fivefloor.bo.myview.IBookManager {
            private android.os.IBinder mRemote;
            Proxy(android.os.IBinder remote) {
                mRemote = remote;
            }
            @Override
            public android.os.IBinder asBinder() {
                return mRemote;
            }
            public java.lang.String getInterfaceDescriptor() {
                return DESCRIPTOR;
            }
            /**
             * 这个方法运行在客户端Client
             * Demonstrates some basic types that you can use as parameters
             * and return values in AIDL.
             *///    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
//            double aDouble, String aString);
            @Override
            public java.util.List<com.fivefloor.bo.myview.bean.Book> getBookList() throws android.os.RemoteException {
                android.os.Parcel _data = android.os.Parcel.obtain();//创建输入型Parcel对象_data
                android.os.Parcel _reply = android.os.Parcel.obtain();//创建输出型Parcel对象_reply
                java.util.List<com.fivefloor.bo.myview.bean.Book> _result;//创建List返回值对象_result
                try {
                    _data.writeInterfaceToken(DESCRIPTOR);//参数信息写入_data中（如果有参数的话）
                    mRemote.transact(Stub.TRANSACTION_getBookList, _data, _reply, 0);//transact（）通过系统底层调用上方的onTransact(）方法
                    _reply.readException();
                    _result = _reply.createTypedArrayList(com.fivefloor.bo.myview.bean.Book.CREATOR);//_reply中取出RPC过程的返回结果
                } finally {
                    _reply.recycle();//回收，避免内存泄漏
                    _data.recycle();//回收，避免内存泄漏
                }
                //返回_reply中的数据
                return _result;
            }
            /**
             * 这个方法运行在客户端Client
             * 它的执行过程和getBookList是一样的
             *
             * @param book
             * @throws android.os.RemoteException
             */
            @Override
            public void addBook(com.fivefloor.bo.myview.bean.Book book) throws android.os.RemoteException {
                android.os.Parcel _data = android.os.Parcel.obtain();//创建输入型Parcel对象_data
                android.os.Parcel _reply = android.os.Parcel.obtain();//创建输出型Parcel对象_reply
                try {
                    _data.writeInterfaceToken(DESCRIPTOR);//参数信息写入_data中（如果有参数的话）
                    if ((book != null)) {
                        _data.writeInt(1);//信息写入_data中
                        book.writeToParcel(_data, 0);//信息写入book中
                    } else {
                        _data.writeInt(0);
                    }
                    mRemote.transact(Stub.TRANSACTION_addBook, _data, _reply, 0);//transact（）通过系统底层调用上方的onTransact(）方法
                    _reply.readException();
                } finally {
                    _reply.recycle();//回收，避免内存泄漏
                    _data.recycle();//回收，避免内存泄漏
                }
            }
        }
        static final int TRANSACTION_getBookList = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
        static final int TRANSACTION_addBook = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    }
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     *///    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
//            double aDouble, String aString);
    public java.util.List<com.fivefloor.bo.myview.bean.Book> getBookList() throws android.os.RemoteException;
    public void addBook(com.fivefloor.bo.myview.bean.Book book) throws android.os.RemoteException;
}
```
##### 3、Bundle方式：由于Bundle实现了Parcelable接口，Bundle就相当于一个数据包装集合，所以它可以方便地在不同的进程间传输。当我们在一个进程中启动了另一个进程的Activity、Service和Receiver，我们就可以在Bundle中附加我们需要传输给远程进程的信息并通过Intent发送出去。传输的数据必须能够被序列化，比如基本类型、实现了Parcellable接口的对象、实现了Serializable接口的对象以及一些Android支持的特殊对象，具体内容可以看Bundle这个类，就可以看到所有它支持的类型。Bundle不支持的类型我们无法通过它在进程间传递数据。
Parcelable的底层使用了Parcel机制。传递实际上是使用了binder机制，binder机制会将Parcel序列化的数据写入到一个共享内存中，读取时也是binder从共享内存中读出字节流，然后Parcel反序列化后使用。这就是Intent或Bundle能够在activity或者跨进程通信的原理。
##### 4、文件共享方式：
- Serializable是Java中的序列化接口，其使用起来简单但是开销很大，序列化和反序列化过程需要大量I/O操作。而Parcelable是Android中的序列化方式，因此更适合用在Android平台上，虽然它使用起来稍微麻烦点，但是它的效率很高。 需要注意静态成员变量属于类不属于对象，所以不会参与序列化过程；其次用transient关键字标记的成员变量不参与序列化过程。
- 系统已经为我们提供了许多实现了Parcelable接口的类，它们都是可以直接序列化的，比如Intent、Bundle、Bitmap等，同时List和Map也可以序列化，前提是它们里面的每个元素都是可序列化的。
- 文件共享方式适合在对数据同步要求不高的进程之间进行通信，并且要妥善处理并发读/写的问题。
- SharedPreferences是个特例，由于系统对它的读/写有一定的缓存策略，即在内存中会有一份SharedPreferences文件的缓存，因此在多进程模式下，系统对它的读/写就变得不可靠，当面对**高并发的读/写访问**，Sharedpreferences有很大几率会丢失数据。
##### 5、Messenger方式：
Messenger和Message都实现了Parcelable接口，因此可以跨进程传输。Message中所支持的数据类型就是Messenger所支持的传输类型。
Messenger来传输Message，Message中能使用的载体只有what、arg1、arg2、Bundle以及replyTo。自定义的Parcelable对象是无法通过object字段来传输的，只有系统提供的实现了Parcelable接口的对象才可以。所幸Bundle支持大量的数据类型。
