# android第三方应用apk安装和运行控制解决方案 - xqhrs232的专栏 - CSDN博客
2010年12月07日 17:31:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1310标签：[android																[filter																[exception																[application																[string																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=filter&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::
[http://blog.csdn.net/w591570973/archive/2010/03/04/5345831.aspx](http://blog.csdn.net/w591570973/archive/2010/03/04/5345831.aspx)
 最近在做android 的项目 ,其实是边学边做.网上查了下资料 不多，但许多办法可以相互综合 需求: 要求让用户选择喜欢的第三方应用 下载 并通过自己的UI运行
关键问题: 
(1) 可交互下载, 用户可取消,显示进度.................这个问题好解决 ,有很多方式,如果不会可以联系我
 (2) 安装apk 
(3) 运行安装过的应用 ...... 怎样定位应用的Activity并激活 ( 如果是自己的activity 就好办了 ) 
解决方案 
(1) 安装apk 直接贴出代码 就明白
 Intent intent = new Intent(Intent.ACTION_VIEW);
 intent.setDataAndType(Uri.fromFile(new File("/sdcard/1.apk")), "app lication/vnd.android.package-archive");
startActivity(intent); 其中/sdcard/1.apk 就是要安装的apk，上面代码实际上是请求android的管理安装的activity 如果要强制安装过程，目前我还没有找到解决方法
(2)运行应用 
首先android有一个PackageManager，这玩意功能 很强大，功能就和它的意思一样 假设：如果我们知道一个第三方Application的包的名称和Activity的名称，是否可以启动它的，答案当让市YES
启动代码:
 PackageManager pm; 
//初始化pm, 比如在activity里可以PackageManager pm = getPackageManager(); 
PackageInfo pi = pm.getPackageInfo("包的名称", PackageManager.GET_ACTIVITIES); 
//PackageInfo 包含丰富的包的信息，这个'包的名称'是什么，在AndroidManifest.xml中有明确定义 
// 比如 package="xxx.yyy.Portal.UI" ActivityInfo ai=pi.activities[0]; 
// ActivityInfo 同样道理 他是 Activity的信息 
//这里指向第一个包中的Activity, 大多数都是第一个Activity为启动Activity if(ai==null) throw new Exception(pkg+"不包含任何Activity");
String sName=ai.name; 
//这里就得到Activity的类名了 启动它:
 Intent intent = new Intent();
 intent.setComponent(new ComponentName(pkg,sName)); 
parent.startActivity(intent); 
(3)获得第三方应用的包名 只要有包名，就可以启动Activity了，但是不知道名字怎么办? 如果应用没有安装自然启动不了，只要安装了就有办法。 
方法1 : 
MyBroadcastReceiver myReceiver = new MyBroadcastReceiver(); 
IntentFilter filter = new IntentFilter(Intent.ACTION_PACKAGE_INSTALL); filter.addAction(Intent.ACTION_PACKAGE_ADDED);
 filter.addDataScheme("package");
 registerReceiver(myReceiver, filter); 
监视安装，在MyBroadcastReceiver的onReceive函数里 
public void onReceive(Context arg0, Intent intent) { String sPackageName=intent.getDataString(); //这个就是包名 } 也可以使用 AndroidManifest.xml配置来监视
方法2 ： 枚举所有安装的应用，根据你自己的条件筛选 使用PackageManager的 public abstract List getInstalledPackages (int flags) 可以获得安装应用的列表
本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/w591570973/archive/2010/03/04/5345831.aspx
