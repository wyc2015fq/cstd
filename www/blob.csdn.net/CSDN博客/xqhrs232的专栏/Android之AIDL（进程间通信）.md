# Android之AIDL（进程间通信） - xqhrs232的专栏 - CSDN博客
2012年01月06日 22:31:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：768标签：[android																[service																[string																[null																[interface																[float](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/huangbiao86/article/details/7096368](http://blog.csdn.net/huangbiao86/article/details/7096368)
前几天学习了Android里进程间的通信。
**使用AIDL设计远程接口(Designing  a Remote  Interface  Using AIDL)**
由于每个应用程序都运行在自己的进程空间，并且可以从应用程序UI运行另一个服务进程，而且经常会在不同的进程间传递对象。在Android平台，一个进程通常不能访问另一个进程的内存空间，所以要想对话，需要将对象分解成操作系统可以理解的基本单元，并且有序的通过进程边界。
通过代码来实现这个数据传输过程是冗长乏味的，Android提供了AIDL工具来处理这项工作。AIDL  (Android  Interface Definition  Language)是一种IDL 语言，用于生成可以在Android设备上两个进程之间进行进程间通信(IPC)的代码。如果在一个进程中（例如Acti vity）要调用另一个进程中（例如Service）对象的操作，就可以使用AIDL生成可序列化的参数。
AIDL  IPC机制是面向接口的，像COM或Corba一样，但是更加轻量级。它是使用代理类在客户端和实现端传递数据。
**使用AIDL实现IPC 服务的步骤是：**
1、首先创建aidl文件，该文件（YourInterface.aidl）定义了客户端可用的方法和数据的接口。它的定义方法与接口是一样的；（会在gen目录下面自动生成对应的YourInterface .java文件）
IRemoteService.aidl
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- package hb.android.aidl;  
- 
- interface IRemoteService {  
- int getPid();  
- 
- void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,  
- double aDouble, String aString);  
- }  
package hb.android.aidl;
interface IRemoteService {
    int getPid();
    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
            double aDouble, String aString);
}IRemoteService2.aidl
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- package hb.android.aidl;  
- 
- interface IRemoteService2{  
- void sysout();  
-     String returnStr();  
- }  
package hb.android.aidl;
interface IRemoteService2{
	void sysout();
	String returnStr();
}自动生成的java文件如图：
![](http://hi.csdn.net/attachment/201112/22/0_1324551691Mbhm.gif)
2、  实现接口-AIDL编译器从AIDL接口文件中利用Java语言创建接口，该接口有一个继承的命名为Stub的内部抽象类（并且实现了一些IPC 调用的附加方法），要做的就是创建一个继承于YourInterface.Stub 的类并且实现在.aidl 文件中声明的方法。（一般是在服务中实现这个接口，在服务中使用IPC）
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- /**
-      * 实现定义的aidl接口。
-      */
- privatefinal IRemoteService.Stub mBinder = new IRemoteService.Stub() {  
- publicint getPid() {  
- return Process.myPid();  
-         }  
- 
- publicvoid basicTypes(int anInt, long aLong, boolean aBoolean,  
- float aFloat, double aDouble, String aString) {  
- // Does nothing 
-             System.out.println("basicTypes");  
-         }  
-     };  
- /**
-      * 实现定义的aidl接口。
-      */
- privatefinal IRemoteService2.Stub mBinder2 = new IRemoteService2.Stub() {  
- @Override
- publicvoid sysout() {  
-             System.out.println("This is IremoteService2.Stub.sysout()");  
-         }  
- 
- @Override
- public String returnStr() throws RemoteException {  
- return"This is IRemoteServicfe2.Stub.returnStr()";  
-         }  
-     };  
/**
	 * 实现定义的aidl接口。
	 */
	private final IRemoteService.Stub mBinder = new IRemoteService.Stub() {
		public int getPid() {
			return Process.myPid();
		}
		public void basicTypes(int anInt, long aLong, boolean aBoolean,
				float aFloat, double aDouble, String aString) {
			// Does nothing
			System.out.println("basicTypes");
		}
	};
	/**
	 * 实现定义的aidl接口。
	 */
	private final IRemoteService2.Stub mBinder2 = new IRemoteService2.Stub() {
		@Override
		public void sysout() {
			System.out.println("This is IremoteService2.Stub.sysout()");
		}
		@Override
		public String returnStr() throws RemoteException {
			return "This is IRemoteServicfe2.Stub.returnStr()";
		}
	};
3、向客户端公开（暴露）接口-如果是编写服务，应该继承Service并且重载Service.onBind(Intent) 以返回实现了接口的对象实例
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- /**
-      * 返回IBinder,在这里可以根据选择的接口进行返回哪一个 如果只有一个接口就不需要判断，直接返回就可以了。
-      */
- @Override
- public IBinder onBind(Intent intent) {  
- if (IRemoteService2.class.getName().equals(intent.getAction())) {  
- return mBinder2;  
-         }  
- if (IRemoteService.class.getName().equals(intent.getAction())) {  
- return mBinder;  
-         }  
- returnnull;  
-     }  
/**
	 * 返回IBinder,在这里可以根据选择的接口进行返回哪一个 如果只有一个接口就不需要判断，直接返回就可以了。
	 */
	@Override
	public IBinder onBind(Intent intent) {
		if (IRemoteService2.class.getName().equals(intent.getAction())) {
			return mBinder2;
		}
		if (IRemoteService.class.getName().equals(intent.getAction())) {
			return mBinder;
		}
		return null;
	}
4、 运行这个程序，那么服务就已经安装到系统了。接下来就是客户端访问服务，利用服务，进行进程间的通信。
**下面再看看如何调用IPC 方法(Calling  an  IPC Method)**
1、   声明.aidl 文件中定义的接口类型的变量和方法。
IRemoteService.aidl
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- package hb.android.aidl;  
- 
- interface IRemoteService {  
- int getPid();  
- 
- void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,  
- double aDouble, String aString);  
- }  
package hb.android.aidl;
interface IRemoteService {
    int getPid();
    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
            double aDouble, String aString);
}IRemoteService2.aidl
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- package hb.android.aidl;  
- 
- interface IRemoteService2 {  
- void sysout();  
-     String returnStr();  
- }  
package hb.android.aidl;
interface IRemoteService2 {
	void sysout();
	String returnStr();
}
2、   实现ServiceConnection
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- /**
-      * ServiceConnection与Service想链接，IRemoteService.Stub.asInterface(service);
-      * 会返回与当前的aidl文件相关的类的实例，这样就可以获取到远程的远程服务实现的IRemoteService接口了。
-      * 注意如果一个ServiceConnection同一时刻只能与Service连接
-      * 。如果需要同时连接一个服务里的多个aidl接口，那么必须定义两个ServiceConnection获取
-      * ，因为一个ServiceConnection连接会返回一个aidl接口的实例。
-      */
- private ServiceConnection mConnection = new ServiceConnection() {  
- // Called when the connection with the service is established
- publicvoid onServiceConnected(ComponentName className, IBinder service) {  
- // Following the example above for an AIDL interface,
- // this gets an instance of the IRemoteInterface, which we can use
- // to call on the service
- try {  
- if (service.getInterfaceDescriptor().equals(  
-                         IRemoteService.class.getName())) {  
-                     mIRemoteService = IRemoteService.Stub.asInterface(service);  
-                     mIRemoteService.basicTypes(0, 0, false, 0, 0, null);  
-                 }  
-             } catch (RemoteException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
- 
- // Called when the connection with the service disconnects unexpectedly
- publicvoid onServiceDisconnected(ComponentName className) {  
-             System.out.println("Service has unexpectedly disconnected");  
-             mIRemoteService = null;  
-         }  
-     };  
- private ServiceConnection mConnection2 = new ServiceConnection() {  
- 
- @Override
- publicvoid onServiceDisconnected(ComponentName name) {  
-             System.out.println("Service2 has unexpectedly disconnected");  
-             mIRemoteService2 = null;  
-         }  
- 
- @Override
- publicvoid onServiceConnected(ComponentName name, IBinder service) {  
- try {  
- if (service.getInterfaceDescriptor().equals(  
-                         IRemoteService2.class.getName())) {  
-                     mIRemoteService2 = IRemoteService2.Stub  
-                             .asInterface(service);  
-                     mIRemoteService2.sysout();  
-                 }  
-             } catch (RemoteException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     };  
/**
	 * ServiceConnection与Service想链接，IRemoteService.Stub.asInterface(service);
	 * 会返回与当前的aidl文件相关的类的实例，这样就可以获取到远程的远程服务实现的IRemoteService接口了。
	 * 注意如果一个ServiceConnection同一时刻只能与Service连接
	 * 。如果需要同时连接一个服务里的多个aidl接口，那么必须定义两个ServiceConnection获取
	 * ，因为一个ServiceConnection连接会返回一个aidl接口的实例。
	 */
	private ServiceConnection mConnection = new ServiceConnection() {
		// Called when the connection with the service is established
		public void onServiceConnected(ComponentName className, IBinder service) {
			// Following the example above for an AIDL interface,
			// this gets an instance of the IRemoteInterface, which we can use
			// to call on the service
			try {
				if (service.getInterfaceDescriptor().equals(
						IRemoteService.class.getName())) {
					mIRemoteService = IRemoteService.Stub.asInterface(service);
					mIRemoteService.basicTypes(0, 0, false, 0, 0, null);
				}
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
		// Called when the connection with the service disconnects unexpectedly
		public void onServiceDisconnected(ComponentName className) {
			System.out.println("Service has unexpectedly disconnected");
			mIRemoteService = null;
		}
	};
	private ServiceConnection mConnection2 = new ServiceConnection() {
		@Override
		public void onServiceDisconnected(ComponentName name) {
			System.out.println("Service2 has unexpectedly disconnected");
			mIRemoteService2 = null;
		}
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			try {
				if (service.getInterfaceDescriptor().equals(
						IRemoteService2.class.getName())) {
					mIRemoteService2 = IRemoteService2.Stub
							.asInterface(service);
					mIRemoteService2.sysout();
				}
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
	};
3、   调用Context.bindService()，传递ServiceConnection的实现
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- intent = new Intent(IRemoteService.class.getName());  
-         System.out.println(IRemoteService.class.getName());  
-         bindService(intent, mConnection, Context.BIND_AUTO_CREATE);  
- 
-         intent2 = new Intent(IRemoteService2.class.getName());  
-         System.out.println(IRemoteService2.class.getName());  
-         bindService(intent2, mConnection2, BIND_AUTO_CREATE);  
intent = new Intent(IRemoteService.class.getName());
		System.out.println(IRemoteService.class.getName());
		bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
		intent2 = new Intent(IRemoteService2.class.getName());
		System.out.println(IRemoteService2.class.getName());
		bindService(intent2, mConnection2, BIND_AUTO_CREATE);
4、   在ServiceConnection.onServiceConnected()方法中会接收到IBinder对象，调用YourInterfaceName.Stub.asInterface((IBinder)service)将返回值转换为YourInterface类型
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- mIRemoteService = IRemoteService.Stub.asInterface(service);  
-                 mIRemoteService.basicTypes(0, 0, false, 0, 0, null);  
	mIRemoteService = IRemoteService.Stub.asInterface(service);
					mIRemoteService.basicTypes(0, 0, false, 0, 0, null);
5、   调用接口中定义的方法。应该总是捕获连接被打断时抛出的DeadObjectException异常，这是远端方法唯一的异常。
6、   调用Context.unbindService()断开连接；
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- protectedvoid onDestroy() {  
- super.onDestroy();  
-         unbindService(mConnection);  
-         unbindService(mConnection2);  
-     };  
protected void onDestroy() {
		super.onDestroy();
		unbindService(mConnection);
		unbindService(mConnection2);
	};
如果想利用进程间的通信来传递对象，那么该对象必须实现Parcelable，下面详细步骤：
1、 使该类实现Parcelabel接口。 
[view plain](http://blog.csdn.net/huangbiao86/article/details/7096368#)[copy
 to clipboard](http://blog.csdn.net/huangbiao86/article/details/7096368#)[print](http://blog.csdn.net/huangbiao86/article/details/7096368#)[?](http://blog.csdn.net/huangbiao86/article/details/7096368#)
- package hb.android.entry;  
- 
- import android.os.Parcel;  
- import android.os.Parcelable;  
- 
- /**
-  * 在进行间通信必须实现Parcelable接口
-  * 
-  * @author Administrator
-  * 
-  */
- publicclass Person implements Parcelable {  
- 
- public String name;  
- public String sex;  
- public String tel;  
- 
- public Person() {  
- super();  
-     }  
- 
- public Person(Parcel in) {  
-         readFromParcel(in);  
-     }  
- 
- publicstaticfinal Parcelable.Creator<Person> CREATOR = new Parcelable.Creator<Person>() {  
- 
- /**
-          * 实现从in中创建出类的实例的功能
-          */
- public Person createFromParcel(Parcel in) {  
- returnnew Person(in);  
-         }  
- 
- /**
-          * 创建一个类型为T，长度为size的数组，仅一句话（return new T[size])即可。估计本方法是供外部类反序列化本类数组使用。
-          */
- public Person[] newArray(int size) {  
- returnnew Person[size];  
-         }  
-     };  
- 
- /**
-      * 从一个外部的Parcel对象读取对象。顺序必须与writeToParcel对应，不然会出错。
-      * 
-      * @param in
-      */
- publicvoid readFromParcel(Parcel in) {  
-         name = in.readString();  
-         sex = in.readString();  
-         tel = in.readString();  
-     }  
- 
- /**
-      * 该方法将类的数据写入外部提供的Parcel中
-      */
- @Override
- publicvoid writeToParcel(Parcel dest, int flags) {  
-         dest.writeString(name);  
-         dest.writeString(sex);  
-         dest.writeString(tel);  
-     }  
- 
- /**
-      * 没搞懂有什么用，反正直接返回0也可以
-      */
- @Override
- publicint describeContents() {  
- return0;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- public String getSex() {  
- return sex;  
-     }  
- 
- publicvoid setSex(String sex) {  
- this.sex = sex;  
-     }  
- 
- public String getTel() {  
- return tel;  
-     }  
- 
- publicvoid setTel(String tel) {  
- this.tel = tel;  
-     }  
- }  
package hb.android.entry;
import android.os.Parcel;
import android.os.Parcelable;
/**
 * 在进行间通信必须实现Parcelable接口
 * 
 * @author Administrator
 * 
 */
public class Person implements Parcelable {
	public String name;
	public String sex;
	public String tel;
	public Person() {
		super();
	}
	public Person(Parcel in) {
		readFromParcel(in);
	}
	public static final Parcelable.Creator<Person> CREATOR = new Parcelable.Creator<Person>() {
		/**
		 * 实现从in中创建出类的实例的功能
		 */
		public Person createFromParcel(Parcel in) {
			return new Person(in);
		}
		/**
		 * 创建一个类型为T，长度为size的数组，仅一句话（return new T[size])即可。估计本方法是供外部类反序列化本类数组使用。
		 */
		public Person[] newArray(int size) {
			return new Person[size];
		}
	};
	/**
	 * 从一个外部的Parcel对象读取对象。顺序必须与writeToParcel对应，不然会出错。
	 * 
	 * @param in
	 */
	public void readFromParcel(Parcel in) {
		name = in.readString();
		sex = in.readString();
		tel = in.readString();
	}
	/**
	 * 该方法将类的数据写入外部提供的Parcel中
	 */
	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeString(name);
		dest.writeString(sex);
		dest.writeString(tel);
	}
	/**
	 * 没搞懂有什么用，反正直接返回0也可以
	 */
	@Override
	public int describeContents() {
		return 0;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
	public String getTel() {
		return tel;
	}
	public void setTel(String tel) {
		this.tel = tel;
	}
}
2、实现public  void  writeToParcel(Parcel  out) 方法，以便可以将对象的当前状态写入包装对象
3、增加名为CREATOR的构造器到类中，并实现Parcelable.Creator接口。
4、  最后，但同样重要的是，创建AIDL文件声明这个可打包的类（见下文），如果使用的是自定义的编译过程，那么不要编译此AIDL文件，它像C语言的头文件一样不需要编译。
AIDL会使用这些方法的成员序列化和反序列化对象。  
两个实例的源码下载：[http://download.csdn.net/download/huangbiao86/3958487](http://download.csdn.net/download/huangbiao86/3958487)
