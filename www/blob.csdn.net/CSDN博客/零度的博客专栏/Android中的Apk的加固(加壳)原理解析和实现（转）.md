# Android中的Apk的加固(加壳)原理解析和实现（转） - 零度的博客专栏 - CSDN博客
2016年05月23日 17:19:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：2285
### 一、前言
今天又到周末了，憋了好久又要出博客了，今天来介绍一下Android中的如何对Apk进行加固的原理。现阶段。我们知道Android中的反编译工作越来越让人操作熟练，我们辛苦的开发出一个apk,结果被人反编译了，那心情真心不舒服。虽然我们混淆，做到native层，但是这都是治标不治本。反编译的技术在更新，那么保护Apk的技术就不能停止。现在网上有很多Apk加固的第三方平台，最有名的应当属于：爱加密和梆梆加固了。其实加固有些人认为很高深的技术，其实不然，说的简单点就是对源Apk进行加密，然后在套上一层壳即可，当然这里还有一些细节需要处理，这就是本文需要介绍的内容了。
### **二、原理解析**
下面就来看一下Android中加壳的原理：
我们在加固的过程中需要三个对象：
**1、需要加密的Apk(源Apk)**
**2、壳程序Apk(负责解密Apk工作)**
**3、加密工具(将源Apk进行加密和壳Dex合并成新的Dex)**
主要步骤：
**我们拿到需要加密的Apk和自己的壳程序Apk，然后用加密算法对源Apk进行加密在将壳Apk进行合并得到新的Dex文件，最后替换壳程序中的dex文件即可，得到新的Apk,那么这个新的Apk我们也叫作脱壳程序Apk.他已经不是一个完整意义上的Apk程序了，他的主要工作是：负责解密源Apk.然后加载Apk,让其正常运行起来。**
在这个过程中我们可能需要了解的一个知识是：**如何将源Apk和壳Apk进行合并成新的Dex**
这里就需要了解Dex文件的格式了。下面就来简单介绍一下Dex文件的格式
具体Dex文件格式的详细介绍可以查看这个文件：[http://download.csdn.net/detail/jiangwei0910410003/9102599](http://download.csdn.net/detail/jiangwei0910410003/9102599)
主要来看一下Dex文件的头部信息，其实Dex文件和Class文件的格式分析原理都是一样的，他们都是有固定的格式，我们知道现在反编译的一些工具:
**1、jd-gui：可以查看jar中的类，其实他就是解析class文件，只要了解class文件的格式就可以**
**2、dex2jar：将dex文件转化成jar，原理也是一样的，只要知道Dex文件的格式，能够解析出dex文件中的类信息就可以了**
当然我们在分析这个文件的时候，最重要的还是头部信息，应该他是一个文件的开始部分，也是索引部分，内部信息很重要。
我们今天只要关注上面红色标记的三个部分：
**1) checksum **
**文件校验码 ，使用alder32 算法校验文件除去 maigc ，checksum 外余下的所有文件区域 ，用于检查文件错误 。**
**2) signature **
**使用 SHA-1 算法 hash 除去 magic ,checksum 和 signature 外余下的所有文件区域 ，用于唯一识别本文件 。**
**3) file_size**
**Dex 文件的大小 。**
为什么说我们只需要关注这三个字段呢？
因为我们需要将一个文件(加密之后的源Apk)写入到Dex中，那么我们肯定需要修改文件校验码(checksum).因为他是检查文件是否有错误。那么signature也是一样，也是唯一识别文件的算法。还有就是需要修改dex文件的大小。
**不过这里还需要一个操作，就是标注一下我们加密的Apk的大小，因为我们在脱壳的时候，需要知道Apk的大小，才能正确的得到Apk。那么这个值放到哪呢？这个值直接放到文件的末尾就可以了。**
所以总结一下我们需要做：修改Dex的三个文件头，将源Apk的大小追加到壳dex的末尾就可以了。
我们修改之后得到新的Dex文件样式如下：
那么我们知道原理了，下面就是代码实现了。所以这里有三个工程：
**1、源程序项目(需要加密的Apk)**
**2、脱壳项目(解密源Apk和加载Apk)**
**3、对源Apk进行加密和脱壳项目的Dex的合并**
### **三、项目案例**
下面先来看一下源程序
#### 1、需要加密的源程序Apk项目：**ForceApkObj**
需要一个Application类，这个到后面说为什么需要：
**MyApplication.java**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- package com.example.forceapkobj;  
- 
- import android.app.Application;  
- import android.util.Log;  
- 
- public class MyApplication extends Application{  
- 
- @Override
- public void onCreate() {  
- super.onCreate();  
-         Log.i("demo", "source apk onCreate:"+this);  
-     }  
- 
- }  
就是打印一下onCreate方法。
**MainActivity.java**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- package com.example.forceapkobj;  
- 
- import android.app.Activity;  
- import android.content.Intent;  
- import android.os.Bundle;  
- import android.util.Log;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- import android.widget.TextView;  
- 
- public class MainActivity extends Activity {  
- 
- @Override
- protected void onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
- 
-         TextView content = new TextView(this);  
-         content.setText("I am Source Apk");  
-         content.setOnClickListener(new OnClickListener(){  
- @Override
- public void onClick(View arg0) {  
-                 Intent intent = new Intent(MainActivity.this, SubActivity.class); 
-                 startActivity(intent);  
-             }});  
-         setContentView(content);  
- 
-         Log.i("demo", "app:"+getApplicationContext());  
- 
-     }  
- 
- }  
也是打印一下内容。
#### 2、加壳程序项目：**DexShellTools**
加壳程序其实就是一个Java工程，因为我们从上面的分析可以看到，他的工作就是加密源Apk，然后将其写入到脱壳Dex文件中，修改文件头，得到一个新的Dex文件即可。
看一下代码：
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- package com.example.reforceapk;  
- 
- import java.io.ByteArrayOutputStream;  
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import java.security.MessageDigest;  
- import java.security.NoSuchAlgorithmException;  
- import java.util.zip.Adler32;  
- 
- 
- public class mymain {  
- /**
-      * @param args
-      */
- public static void main(String[] args) {  
- // TODO Auto-generated method stub
- try {  
-             File payloadSrcFile = new File("force/ForceApkObj.apk");   //需要加壳的程序
-             System.out.println("apk size:"+payloadSrcFile.length());  
-             File unShellDexFile = new File("force/ForceApkObj.dex");    //解客dex
- byte[] payloadArray = encrpt(readFileBytes(payloadSrcFile));//以二进制形式读出apk，并进行加密处理//对源Apk进行加密操作
- byte[] unShellDexArray = readFileBytes(unShellDexFile);//以二进制形式读出dex
- int payloadLen = payloadArray.length;  
- int unShellDexLen = unShellDexArray.length;  
- int totalLen = payloadLen + unShellDexLen +4;//多出4字节是存放长度的。
- byte[] newdex = new byte[totalLen]; // 申请了新的长度
- //添加解壳代码
-             System.arraycopy(unShellDexArray, 0, newdex, 0, unShellDexLen);//先拷贝dex内容
- //添加加密后的解壳数据
-             System.arraycopy(payloadArray, 0, newdex, unShellDexLen, payloadLen);//再在dex内容后面拷贝apk的内容
- //添加解壳数据长度
-             System.arraycopy(intToByte(payloadLen), 0, newdex, totalLen-4, 4);//最后4为长度
- //修改DEX file size文件头
-             fixFileSizeHeader(newdex);  
- //修改DEX SHA1 文件头
-             fixSHA1Header(newdex);  
- //修改DEX CheckSum文件头
-             fixCheckSumHeader(newdex);  
- 
-             String str = "force/classes.dex";  
-             File file = new File(str);  
- if (!file.exists()) {  
-                 file.createNewFile();  
-             }  
- 
-             FileOutputStream localFileOutputStream = new FileOutputStream(str); 
-             localFileOutputStream.write(newdex);  
-             localFileOutputStream.flush();  
-             localFileOutputStream.close();  
- 
- 
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- //直接返回数据，读者可以添加自己加密方法
- private static byte[] encrpt(byte[] srcdata){  
- for(int i = 0;i<srcdata.length;i++){  
-             srcdata[i] = (byte)(0xFF ^ srcdata[i]);  
-         }  
- return srcdata;  
-     }  
- 
- /**
-      * 修改dex头，CheckSum 校验码
-      * @param dexBytes
-      */
- private static void fixCheckSumHeader(byte[] dexBytes) {  
-         Adler32 adler = new Adler32();  
-         adler.update(dexBytes, 12, dexBytes.length - 12);//从12到文件末尾计算校验码
- long value = adler.getValue();  
- int va = (int) value;  
- byte[] newcs = intToByte(va);  
- //高位在前，低位在前掉个个
- byte[] recs = new byte[4];  
- for (int i = 0; i < 4; i++) {  
-             recs[i] = newcs[newcs.length - 1 - i];  
-             System.out.println(Integer.toHexString(newcs[i]));  
-         }  
-         System.arraycopy(recs, 0, dexBytes, 8, 4);//效验码赋值（8-11）
-         System.out.println(Long.toHexString(value));  
-         System.out.println();  
-     }  
- 
- 
- /**
-      * int 转byte[]
-      * @param number
-      * @return
-      */
- public static byte[] intToByte(int number) {  
- byte[] b = new byte[4];  
- for (int i = 3; i >= 0; i--) {  
-             b[i] = (byte) (number % 256);  
-             number >>= 8;  
-         }  
- return b;  
-     }  
- 
- /**
-      * 修改dex头 sha1值
-      * @param dexBytes
-      * @throws NoSuchAlgorithmException
-      */
- private static void fixSHA1Header(byte[] dexBytes)  
- throws NoSuchAlgorithmException {  
-         MessageDigest md = MessageDigest.getInstance("SHA-1");  
-         md.update(dexBytes, 32, dexBytes.length - 32);//从32为到结束计算sha--1
- byte[] newdt = md.digest();  
-         System.arraycopy(newdt, 0, dexBytes, 12, 20);//修改sha-1值（12-31）
- //输出sha-1值，可有可无
-         String hexstr = "";  
- for (int i = 0; i < newdt.length; i++) {  
-             hexstr += Integer.toString((newdt[i] & 0xff) + 0x100, 16)  
-                     .substring(1);  
-         }  
-         System.out.println(hexstr);  
-     }  
- 
- /**
-      * 修改dex头 file_size值
-      * @param dexBytes
-      */
- private static void fixFileSizeHeader(byte[] dexBytes) {  
- //新文件长度
- byte[] newfs = intToByte(dexBytes.length);  
-         System.out.println(Integer.toHexString(dexBytes.length));  
- byte[] refs = new byte[4];  
- //高位在前，低位在前掉个个
- for (int i = 0; i < 4; i++) {  
-             refs[i] = newfs[newfs.length - 1 - i];  
-             System.out.println(Integer.toHexString(newfs[i]));  
-         }  
-         System.arraycopy(refs, 0, dexBytes, 32, 4);//修改（32-35）
-     }  
- 
- 
- /**
-      * 以二进制读出文件内容
-      * @param file
-      * @return
-      * @throws IOException
-      */
- private static byte[] readFileBytes(File file) throws IOException {  
- byte[] arrayOfByte = new byte[1024];  
-         ByteArrayOutputStream localByteArrayOutputStream = new ByteArrayOutputStream(); 
-         FileInputStream fis = new FileInputStream(file);  
- while (true) {  
- int i = fis.read(arrayOfByte);  
- if (i != -1) {  
-                 localByteArrayOutputStream.write(arrayOfByte, 0, i);  
-             } else {  
- return localByteArrayOutputStream.toByteArray();  
-             }  
-         }  
-     }  
- }  
下面来分析一下：   
红色部分其实就是最核心的工作：
**1>、加密源程序Apk文件**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- byte[] payloadArray = encrpt(readFileBytes(payloadSrcFile));//以二进制形式读出apk，并进行加密处理//对源Apk进行加密操作
加密算法很简单：
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //直接返回数据，读者可以添加自己加密方法
- private static byte[] encrpt(byte[] srcdata){  
- for(int i = 0;i<srcdata.length;i++){  
-         srcdata[i] = (byte)(0xFF ^ srcdata[i]);  
-     }  
- return srcdata;  
- }  
对每个字节进行异或一下即可。
**(说明：这里是为了简单，所以就用了很简单的加密算法了，其实为了增加破解难度，我们应该使用更高效的加密算法，同事最好将加密操作放到native层去做)**
**2>、合并文件：将加密之后的Apk和原脱壳Dex进行合并**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- int payloadLen = payloadArray.length;  
- int unShellDexLen = unShellDexArray.length;  
- int totalLen = payloadLen + unShellDexLen +4;//多出4字节是存放长度的。
- byte[] newdex = new byte[totalLen]; // 申请了新的长度
- //添加解壳代码
- System.arraycopy(unShellDexArray, 0, newdex, 0, unShellDexLen);//先拷贝dex内容
- //添加加密后的解壳数据
- System.arraycopy(payloadArray, 0, newdex, unShellDexLen, payloadLen);//再在dex内容后面拷贝apk的内容
**3>、在文件的末尾追加源程序Apk的长度**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //添加解壳数据长度
- System.arraycopy(intToByte(payloadLen), 0, newdex, totalLen-4, 4);//最后4为长度
**4>、修改新Dex文件的文件头信息：file_size; sha1; check_sum**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //修改DEX file size文件头
- fixFileSizeHeader(newdex);  
- //修改DEX SHA1 文件头
- fixSHA1Header(newdex);  
- //修改DEX CheckSum文件头
- fixCheckSumHeader(newdex);  
具体修改可以参照之前说的文件头格式，修改指定位置的字节值即可。
这里我们还需要两个输入文件：
**1>、源Apk文件：ForceApkObj.apk**
**2>、脱壳程序的Dex文件：ForceApkObj.dex**
那么第一个文件我们都知道，就是上面的源程序编译之后的Apk文件，那么第二个文件我们怎么得到呢？这个就是我们要讲到的第三个项目：脱壳程序项目，他是一个Android项目，我们在编译之后，能够得到他的classes.dex文件，然后修改一下名称就可。
#### 3、脱壳项目：**ReforceApk**
在讲解这个项目之前，我们先来了解一下这个脱壳项目的工作：
1>、通过反射置换android.app.ActivityThread 中的mClassLoader为加载解密出APK的DexClassLoader，该DexClassLoader一方面加载了源程序、另一方面以原mClassLoader为父节点，这就保证了即加载了源程序又没有放弃原先加载的资源与系统代码。
关于这部分内容，不了解的同学可以看一下ActivityThread.java的源码:
或者直接看一下这篇文章：
[http://blog.csdn.net/jiangwei0910410003/article/details/48104455](http://blog.csdn.net/jiangwei0910410003/article/details/48104455)
如何得到系统加载Apk的类加载器，然后我们怎么将加载进来的Apk运行起来等问题都在这篇文章中说到了。
2>、找到源程序的Application，通过反射建立并运行。
这里需要注意的是，我们现在是加载一个完整的Apk,让他运行起来，那么我们知道一个Apk运行的时候都是有一个Application对象的，这个也是一个程序运行之后的全局类。所以我们必须找到解密之后的源Apk的Application类，运行的他的onCreate方法，这样源Apk才开始他的运行生命周期。这里我们如何得到源Apk的Application的类呢？这个我们后面会说道。使用meta标签进行设置。
下面来看一下整体的流程图：
所以我们看到这里还需要一个核心的技术就是动态加载。关于动态加载技术，不了解的同学可以看这篇文章：
[http://blog.csdn.net/jiangwei0910410003/article/details/48104581](http://blog.csdn.net/jiangwei0910410003/article/details/48104581)
下面来看一下代码：
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- package com.example.reforceapk;  
- 
- import java.io.BufferedInputStream;  
- import java.io.ByteArrayInputStream;  
- import java.io.ByteArrayOutputStream;  
- import java.io.DataInputStream;  
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import java.lang.ref.WeakReference;  
- import java.lang.reflect.Method;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.Iterator;  
- import java.util.zip.ZipEntry;  
- import java.util.zip.ZipInputStream;  
- 
- import android.app.Application;  
- import android.app.Instrumentation;  
- import android.content.Context;  
- import android.content.pm.ApplicationInfo;  
- import android.content.pm.PackageManager;  
- import android.content.pm.PackageManager.NameNotFoundException;  
- import android.content.res.AssetManager;  
- import android.content.res.Resources;  
- import android.content.res.Resources.Theme;  
- import android.os.Bundle;  
- import android.util.ArrayMap;  
- import android.util.Log;  
- import dalvik.system.DexClassLoader;  
- 
- public class ProxyApplication extends Application{  
- private static final String appkey = "APPLICATION_CLASS_NAME";  
- private String apkFileName;  
- private String odexPath;  
- private String libPath;  
- 
- //这是context 赋值
- @Override
- protected void attachBaseContext(Context base) {  
- super.attachBaseContext(base);  
- try {  
- //创建两个文件夹payload_odex，payload_lib 私有的，可写的文件目录
-             File odex = this.getDir("payload_odex", MODE_PRIVATE);  
-             File libs = this.getDir("payload_lib", MODE_PRIVATE);  
-             odexPath = odex.getAbsolutePath();  
-             libPath = libs.getAbsolutePath();  
-             apkFileName = odex.getAbsolutePath() + "/payload.apk";  
-             File dexFile = new File(apkFileName);  
-             Log.i("demo", "apk size:"+dexFile.length());  
- if (!dexFile.exists())  
-             {  
-                 dexFile.createNewFile();  //在payload_odex文件夹内，创建payload.apk
- // 读取程序classes.dex文件
- byte[] dexdata = this.readDexFileFromApk();  
- 
- // 分离出解壳后的apk文件已用于动态加载
- this.splitPayLoadFromDex(dexdata);  
-             }  
- // 配置动态加载环境
-             Object currentActivityThread = RefInvoke.invokeStaticMethod(  
- "android.app.ActivityThread", "currentActivityThread",  
- new Class[] {}, new Object[] {});//获取主线程对象 http://blog.csdn.net/myarrow/article/details/14223493
-             String packageName = this.getPackageName();//当前apk的包名
- //下面两句不是太理解
-             ArrayMap mPackages = (ArrayMap) RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mPackages");  
-             WeakReference wr = (WeakReference) mPackages.get(packageName);  
- //创建被加壳apk的DexClassLoader对象  加载apk内的类和本地代码（c/c++代码）
-             DexClassLoader dLoader = new DexClassLoader(apkFileName, odexPath, 
-                     libPath, (ClassLoader) RefInvoke.getFieldOjbect(  
- "android.app.LoadedApk", wr.get(), "mClassLoader"));  
- //base.getClassLoader(); 是不是就等同于 (ClassLoader) RefInvoke.getFieldOjbect()? 有空验证下//?
- //把当前进程的DexClassLoader 设置成了被加壳apk的DexClassLoader  ----有点c++中进程环境的意思~~
-             RefInvoke.setFieldOjbect("android.app.LoadedApk", "mClassLoader", 
-                     wr.get(), dLoader);  
- 
-             Log.i("demo","classloader:"+dLoader);  
- 
- try{  
-                 Object actObj = dLoader.loadClass("com.example.forceapkobj.MainActivity"); 
-                 Log.i("demo", "actObj:"+actObj);  
-             }catch(Exception e){  
-                 Log.i("demo", "activity:"+Log.getStackTraceString(e));  
-             }  
- 
- 
-         } catch (Exception e) {  
-             Log.i("demo", "error:"+Log.getStackTraceString(e));  
-             e.printStackTrace();  
-         }  
-     }  
- 
- @Override
- public void onCreate() {  
-         {  
- //loadResources(apkFileName);
- 
-             Log.i("demo", "onCreate");  
- // 如果源应用配置有Appliction对象，则替换为源应用Applicaiton，以便不影响源程序逻辑。
-             String appClassName = null;  
- try {  
-                 ApplicationInfo ai = this.getPackageManager()  
-                         .getApplicationInfo(this.getPackageName(),  
-                                 PackageManager.GET_META_DATA);  
-                 Bundle bundle = ai.metaData;  
- if (bundle != null && bundle.containsKey("APPLICATION_CLASS_NAME")) {  
-                     appClassName = bundle.getString("APPLICATION_CLASS_NAME");//className 是配置在xml文件中的。
-                 } else {  
-                     Log.i("demo", "have no application class name");  
- return;  
-                 }  
-             } catch (NameNotFoundException e) {  
-                 Log.i("demo", "error:"+Log.getStackTraceString(e));  
-                 e.printStackTrace();  
-             }  
- //有值的话调用该Applicaiton
-             Object currentActivityThread = RefInvoke.invokeStaticMethod(  
- "android.app.ActivityThread", "currentActivityThread",  
- new Class[] {}, new Object[] {});  
-             Object mBoundApplication = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mBoundApplication");  
-             Object loadedApkInfo = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread$AppBindData",  
-                     mBoundApplication, "info");  
- //把当前进程的mApplication 设置成了null
-             RefInvoke.setFieldOjbect("android.app.LoadedApk", "mApplication", 
-                     loadedApkInfo, null);  
-             Object oldApplication = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mInitialApplication");  
- //http://www.codeceo.com/article/android-context.html
-             ArrayList<Application> mAllApplications = (ArrayList<Application>) RefInvoke 
-                     .getFieldOjbect("android.app.ActivityThread",  
-                             currentActivityThread, "mAllApplications");  
-             mAllApplications.remove(oldApplication);//删除oldApplication
- 
-             ApplicationInfo appinfo_In_LoadedApk = (ApplicationInfo) RefInvoke 
-                     .getFieldOjbect("android.app.LoadedApk", loadedApkInfo,  
- "mApplicationInfo");  
-             ApplicationInfo appinfo_In_AppBindData = (ApplicationInfo) RefInvoke 
-                     .getFieldOjbect("android.app.ActivityThread$AppBindData", 
-                             mBoundApplication, "appInfo");  
-             appinfo_In_LoadedApk.className = appClassName;  
-             appinfo_In_AppBindData.className = appClassName;  
-             Application app = (Application) RefInvoke.invokeMethod(  
- "android.app.LoadedApk", "makeApplication", loadedApkInfo,  
- new Class[] { boolean.class, Instrumentation.class },  
- new Object[] { false, null });//执行 makeApplication（false,null）
-             RefInvoke.setFieldOjbect("android.app.ActivityThread",  
- "mInitialApplication", currentActivityThread, app);  
- 
- 
-             ArrayMap mProviderMap = (ArrayMap) RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mProviderMap");  
-             Iterator it = mProviderMap.values().iterator();  
- while (it.hasNext()) {  
-                 Object providerClientRecord = it.next();  
-                 Object localProvider = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread$ProviderClientRecord",  
-                         providerClientRecord, "mLocalProvider");  
-                 RefInvoke.setFieldOjbect("android.content.ContentProvider",  
- "mContext", localProvider, app);  
-             }  
- 
-             Log.i("demo", "app:"+app);  
- 
-             app.onCreate();  
-         }  
-     }  
- 
- /**
-      * 释放被加壳的apk文件，so文件
-      * @param data
-      * @throws IOException
-      */
- private void splitPayLoadFromDex(byte[] apkdata) throws IOException {  
- int ablen = apkdata.length;  
- //取被加壳apk的长度   这里的长度取值，对应加壳时长度的赋值都可以做些简化
- byte[] dexlen = new byte[4];  
-         System.arraycopy(apkdata, ablen - 4, dexlen, 0, 4);  
-         ByteArrayInputStream bais = new ByteArrayInputStream(dexlen);  
-         DataInputStream in = new DataInputStream(bais);  
- int readInt = in.readInt();  
-         System.out.println(Integer.toHexString(readInt));  
- byte[] newdex = new byte[readInt];  
- //把被加壳apk内容拷贝到newdex中
-         System.arraycopy(apkdata, ablen - 4 - readInt, newdex, 0, readInt);  
- //这里应该加上对于apk的解密操作，若加壳是加密处理的话
- //?
- 
- //对源程序Apk进行解密
-         newdex = decrypt(newdex);  
- 
- //写入apk文件   
-         File file = new File(apkFileName);  
- try {  
-             FileOutputStream localFileOutputStream = new FileOutputStream(file); 
-             localFileOutputStream.write(newdex);  
-             localFileOutputStream.close();  
-         } catch (IOException localIOException) {  
- throw new RuntimeException(localIOException);  
-         }  
- 
- //分析被加壳的apk文件
-         ZipInputStream localZipInputStream = new ZipInputStream(  
- new BufferedInputStream(new FileInputStream(file)));  
- while (true) {  
-             ZipEntry localZipEntry = localZipInputStream.getNextEntry();//不了解这个是否也遍历子目录，看样子应该是遍历的
- if (localZipEntry == null) {  
-                 localZipInputStream.close();  
- break;  
-             }  
- //取出被加壳apk用到的so文件，放到 libPath中（data/data/包名/payload_lib)
-             String name = localZipEntry.getName();  
- if (name.startsWith("lib/") && name.endsWith(".so")) {  
-                 File storeFile = new File(libPath + "/"
-                         + name.substring(name.lastIndexOf('/')));  
-                 storeFile.createNewFile();  
-                 FileOutputStream fos = new FileOutputStream(storeFile);  
- byte[] arrayOfByte = new byte[1024];  
- while (true) {  
- int i = localZipInputStream.read(arrayOfByte);  
- if (i == -1)  
- break;  
-                     fos.write(arrayOfByte, 0, i);  
-                 }  
-                 fos.flush();  
-                 fos.close();  
-             }  
-             localZipInputStream.closeEntry();  
-         }  
-         localZipInputStream.close();  
- 
- 
-     }  
- 
- /**
-      * 从apk包里面获取dex文件内容（byte）
-      * @return
-      * @throws IOException
-      */
- private byte[] readDexFileFromApk() throws IOException {  
-         ByteArrayOutputStream dexByteArrayOutputStream = new ByteArrayOutputStream(); 
-         ZipInputStream localZipInputStream = new ZipInputStream(  
- new BufferedInputStream(new FileInputStream(  
- this.getApplicationInfo().sourceDir)));  
- while (true) {  
-             ZipEntry localZipEntry = localZipInputStream.getNextEntry();  
- if (localZipEntry == null) {  
-                 localZipInputStream.close();  
- break;  
-             }  
- if (localZipEntry.getName().equals("classes.dex")) {  
- byte[] arrayOfByte = new byte[1024];  
- while (true) {  
- int i = localZipInputStream.read(arrayOfByte);  
- if (i == -1)  
- break;  
-                     dexByteArrayOutputStream.write(arrayOfByte, 0, i);  
-                 }  
-             }  
-             localZipInputStream.closeEntry();  
-         }  
-         localZipInputStream.close();  
- return dexByteArrayOutputStream.toByteArray();  
-     }  
- 
- 
- // //直接返回数据，读者可以添加自己解密方法
- private byte[] decrypt(byte[] srcdata) {  
- for(int i=0;i<srcdata.length;i++){  
-             srcdata[i] = (byte)(0xFF ^ srcdata[i]);  
-         }  
- return srcdata;  
-     }  
- 
- 
- //以下是加载资源
- protected AssetManager mAssetManager;//资源管理器  
- protected Resources mResources;//资源  
- protected Theme mTheme;//主题  
- 
- protected void loadResources(String dexPath) {    
- try {    
-             AssetManager assetManager = AssetManager.class.newInstance();    
-             Method addAssetPath = assetManager.getClass().getMethod("addAssetPath", String.class);   
-             addAssetPath.invoke(assetManager, dexPath);    
-             mAssetManager = assetManager;    
-         } catch (Exception e) {    
-             Log.i("inject", "loadResource error:"+Log.getStackTraceString(e)); 
-             e.printStackTrace();    
-         }    
-         Resources superRes = super.getResources();    
-         superRes.getDisplayMetrics();    
-         superRes.getConfiguration();    
-         mResources = new Resources(mAssetManager, superRes.getDisplayMetrics(),superRes.getConfiguration());   
-         mTheme = mResources.newTheme();    
-         mTheme.setTo(super.getTheme());  
-     }    
- 
- @Override
- public AssetManager getAssets() {    
- return mAssetManager == null ? super.getAssets() : mAssetManager;    
-     }    
- 
- @Override
- public Resources getResources() {    
- return mResources == null ? super.getResources() : mResources;    
-     }    
- 
- @Override
- public Theme getTheme() {    
- return mTheme == null ? super.getTheme() : mTheme;    
-     }   
- 
- }  
首先我们来看一下具体步骤的代码实现：
**1>、得到脱壳Apk中的dex文件，然后从这个文件中得到源程序Apk.进行解密，然后加载**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //这是context 赋值
- @Override
- protected void attachBaseContext(Context base) {  
- super.attachBaseContext(base);  
- try {  
- //创建两个文件夹payload_odex，payload_lib 私有的，可写的文件目录
-         File odex = this.getDir("payload_odex", MODE_PRIVATE);  
-         File libs = this.getDir("payload_lib", MODE_PRIVATE);  
-         odexPath = odex.getAbsolutePath();  
-         libPath = libs.getAbsolutePath();  
-         apkFileName = odex.getAbsolutePath() + "/payload.apk";  
-         File dexFile = new File(apkFileName);  
-         Log.i("demo", "apk size:"+dexFile.length());  
- if (!dexFile.exists())  
-         {  
-             dexFile.createNewFile();  //在payload_odex文件夹内，创建payload.apk
- // 读取程序classes.dex文件
- byte[] dexdata = this.readDexFileFromApk();  
- 
- // 分离出解壳后的apk文件已用于动态加载
- this.splitPayLoadFromDex(dexdata);  
-         }  
- // 配置动态加载环境
-         Object currentActivityThread = RefInvoke.invokeStaticMethod(  
- "android.app.ActivityThread", "currentActivityThread",  
- new Class[] {}, new Object[] {});//获取主线程对象 http://blog.csdn.net/myarrow/article/details/14223493
-         String packageName = this.getPackageName();//当前apk的包名
- //下面两句不是太理解
-         ArrayMap mPackages = (ArrayMap) RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mPackages");  
-         WeakReference wr = (WeakReference) mPackages.get(packageName);  
- //创建被加壳apk的DexClassLoader对象  加载apk内的类和本地代码（c/c++代码）
-         DexClassLoader dLoader = new DexClassLoader(apkFileName, odexPath,  
-                 libPath, (ClassLoader) RefInvoke.getFieldOjbect(  
- "android.app.LoadedApk", wr.get(), "mClassLoader"));  
- //base.getClassLoader(); 是不是就等同于 (ClassLoader) RefInvoke.getFieldOjbect()? 有空验证下//?
- //把当前进程的DexClassLoader 设置成了被加壳apk的DexClassLoader  ----有点c++中进程环境的意思~~
-         RefInvoke.setFieldOjbect("android.app.LoadedApk", "mClassLoader",  
-                 wr.get(), dLoader);  
- 
-         Log.i("demo","classloader:"+dLoader);  
- 
- try{  
-             Object actObj = dLoader.loadClass("com.example.forceapkobj.MainActivity"); 
-             Log.i("demo", "actObj:"+actObj);  
-         }catch(Exception e){  
-             Log.i("demo", "activity:"+Log.getStackTraceString(e));  
-         }  
- 
- 
-     } catch (Exception e) {  
-         Log.i("demo", "error:"+Log.getStackTraceString(e));  
-         e.printStackTrace();  
-     }  
- }  
这里需要注意的一个问题，就是我们需要找到一个时机，就是在脱壳程序还没有运行起来的时候，来加载源程序的Apk，执行他的onCreate方法，那么这个时机不能太晚，不然的话，就是运行脱壳程序，而不是源程序了。查看源码我们知道。Application中有一个方法：**attachBaseContext**这个方法，他在Application的onCreate方法执行前就会执行了，那么我们的工作就需要在这里进行
**1)、从脱壳程序Apk中找到源程序Apk,并且进行解密操作**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //创建两个文件夹payload_odex，payload_lib 私有的，可写的文件目录
- File odex = this.getDir("payload_odex", MODE_PRIVATE);  
- File libs = this.getDir("payload_lib", MODE_PRIVATE);  
- odexPath = odex.getAbsolutePath();  
- libPath = libs.getAbsolutePath();  
- apkFileName = odex.getAbsolutePath() + "/payload.apk";  
- File dexFile = new File(apkFileName);  
- Log.i("demo", "apk size:"+dexFile.length());  
- if (!dexFile.exists())  
- {  
-     dexFile.createNewFile();  //在payload_odex文件夹内，创建payload.apk
- // 读取程序classes.dex文件
- byte[] dexdata = this.readDexFileFromApk();  
- 
- // 分离出解壳后的apk文件已用于动态加载
- this.splitPayLoadFromDex(dexdata);  
- }  
这个脱壳解密操作一定要和我们之前的加壳以及加密操作对应，不然就会出现Dex加载错误问题
A) 从Apk中获取到Dex文件
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- /**
- * 从apk包里面获取dex文件内容（byte）
- * @return
- * @throws IOException
- */
- private byte[] readDexFileFromApk() throws IOException {  
-     ByteArrayOutputStream dexByteArrayOutputStream = new ByteArrayOutputStream(); 
-     ZipInputStream localZipInputStream = new ZipInputStream(  
- new BufferedInputStream(new FileInputStream(  
- this.getApplicationInfo().sourceDir)));  
- while (true) {  
-         ZipEntry localZipEntry = localZipInputStream.getNextEntry();  
- if (localZipEntry == null) {  
-             localZipInputStream.close();  
- break;  
-         }  
- if (localZipEntry.getName().equals("classes.dex")) {  
- byte[] arrayOfByte = new byte[1024];  
- while (true) {  
- int i = localZipInputStream.read(arrayOfByte);  
- if (i == -1)  
- break;  
-                 dexByteArrayOutputStream.write(arrayOfByte, 0, i);  
-             }  
-         }  
-         localZipInputStream.closeEntry();  
-     }  
-     localZipInputStream.close();  
- return dexByteArrayOutputStream.toByteArray();  
- }  
其实就是解压Apk文件，直接得到dex文件即可
B) 从脱壳Dex中得到源Apk文件
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- /**
- * 释放被加壳的apk文件，so文件
- * @param data
- * @throws IOException
- */
- private void splitPayLoadFromDex(byte[] apkdata) throws IOException {  
- int ablen = apkdata.length;  
- //取被加壳apk的长度   这里的长度取值，对应加壳时长度的赋值都可以做些简化
- byte[] dexlen = new byte[4];  
-     System.arraycopy(apkdata, ablen - 4, dexlen, 0, 4);  
-     ByteArrayInputStream bais = new ByteArrayInputStream(dexlen);  
-     DataInputStream in = new DataInputStream(bais);  
- int readInt = in.readInt();  
-     System.out.println(Integer.toHexString(readInt));  
- byte[] newdex = new byte[readInt];  
- //把被加壳apk内容拷贝到newdex中
-     System.arraycopy(apkdata, ablen - 4 - readInt, newdex, 0, readInt);  
- //这里应该加上对于apk的解密操作，若加壳是加密处理的话
- //?
- 
- //对源程序Apk进行解密
-     newdex = decrypt(newdex);  
- 
- //写入apk文件   
-     File file = new File(apkFileName);  
- try {  
-         FileOutputStream localFileOutputStream = new FileOutputStream(file); 
-         localFileOutputStream.write(newdex);  
-         localFileOutputStream.close();  
-     } catch (IOException localIOException) {  
- throw new RuntimeException(localIOException);  
-     }  
- 
- //分析被加壳的apk文件
-     ZipInputStream localZipInputStream = new ZipInputStream(  
- new BufferedInputStream(new FileInputStream(file)));  
- while (true) {  
-         ZipEntry localZipEntry = localZipInputStream.getNextEntry();//不了解这个是否也遍历子目录，看样子应该是遍历的
- if (localZipEntry == null) {  
-             localZipInputStream.close();  
- break;  
-         }  
- //取出被加壳apk用到的so文件，放到 libPath中（data/data/包名/payload_lib)
-         String name = localZipEntry.getName();  
- if (name.startsWith("lib/") && name.endsWith(".so")) {  
-             File storeFile = new File(libPath + "/"
-                     + name.substring(name.lastIndexOf('/')));  
-             storeFile.createNewFile();  
-             FileOutputStream fos = new FileOutputStream(storeFile);  
- byte[] arrayOfByte = new byte[1024];  
- while (true) {  
- int i = localZipInputStream.read(arrayOfByte);  
- if (i == -1)  
- break;  
-                 fos.write(arrayOfByte, 0, i);  
-             }  
-             fos.flush();  
-             fos.close();  
-         }  
-         localZipInputStream.closeEntry();  
-     }  
-     localZipInputStream.close();  
- 
- 
- }  
C) 解密源程序Apk
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- ////直接返回数据，读者可以添加自己解密方法
- private byte[] decrypt(byte[] srcdata) {  
- for(int i=0;i<srcdata.length;i++){  
-         srcdata[i] = (byte)(0xFF ^ srcdata[i]);  
-     }  
- return srcdata;  
- }  
这个解密算法和加密算法是一致的
**2>、加载解密之后的源程序Apk**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- //配置动态加载环境
- Object currentActivityThread = RefInvoke.invokeStaticMethod(  
- "android.app.ActivityThread", "currentActivityThread",  
- new Class[] {}, new Object[] {});//获取主线程对象 http://blog.csdn.net/myarrow/article/details/14223493
- String packageName = this.getPackageName();//当前apk的包名
- //下面两句不是太理解
- ArrayMap mPackages = (ArrayMap) RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mPackages");  
- WeakReference wr = (WeakReference) mPackages.get(packageName);  
- //创建被加壳apk的DexClassLoader对象  加载apk内的类和本地代码（c/c++代码）
- DexClassLoader dLoader = new DexClassLoader(apkFileName, odexPath,  
-         libPath, (ClassLoader) RefInvoke.getFieldOjbect(  
- "android.app.LoadedApk", wr.get(), "mClassLoader"));  
- //base.getClassLoader(); 是不是就等同于 (ClassLoader) RefInvoke.getFieldOjbect()? 有空验证下//?
- //把当前进程的DexClassLoader 设置成了被加壳apk的DexClassLoader  ----有点c++中进程环境的意思~~
- RefInvoke.setFieldOjbect("android.app.LoadedApk", "mClassLoader",  
-         wr.get(), dLoader);  
- 
- Log.i("demo","classloader:"+dLoader);  
- 
- try{  
-     Object actObj = dLoader.loadClass("com.example.forceapkobj.MainActivity"); 
-     Log.i("demo", "actObj:"+actObj);  
- }catch(Exception e){  
-     Log.i("demo", "activity:"+Log.getStackTraceString(e));  
- }  
**2)、找到源程序的Application程序，让其运行**
**[java]**[view plain](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/48415225#)
- @Override
- public void onCreate() {  
-     {  
- //loadResources(apkFileName);
- 
-         Log.i("demo", "onCreate");  
- // 如果源应用配置有Appliction对象，则替换为源应用Applicaiton，以便不影响源程序逻辑。
-         String appClassName = null;  
- try {  
-             ApplicationInfo ai = this.getPackageManager()  
-                     .getApplicationInfo(this.getPackageName(),  
-                             PackageManager.GET_META_DATA);  
-             Bundle bundle = ai.metaData;  
- if (bundle != null && bundle.containsKey("APPLICATION_CLASS_NAME")) {  
-                 appClassName = bundle.getString("APPLICATION_CLASS_NAME");//className 是配置在xml文件中的。
-             } else {  
-                 Log.i("demo", "have no application class name");  
- return;  
-             }  
-         } catch (NameNotFoundException e) {  
-             Log.i("demo", "error:"+Log.getStackTraceString(e));  
-             e.printStackTrace();  
-         }  
- //有值的话调用该Applicaiton
-         Object currentActivityThread = RefInvoke.invokeStaticMethod(  
- "android.app.ActivityThread", "currentActivityThread",  
- new Class[] {}, new Object[] {});  
-         Object mBoundApplication = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mBoundApplication");  
-         Object loadedApkInfo = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread$AppBindData",  
-                 mBoundApplication, "info");  
- //把当前进程的mApplication 设置成了null
-         RefInvoke.setFieldOjbect("android.app.LoadedApk", "mApplication",  
-                 loadedApkInfo, null);  
-         Object oldApplication = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mInitialApplication");  
- //http://www.codeceo.com/article/android-context.html
-         ArrayList<Application> mAllApplications = (ArrayList<Application>) RefInvoke 
-                 .getFieldOjbect("android.app.ActivityThread",  
-                         currentActivityThread, "mAllApplications");  
-         mAllApplications.remove(oldApplication);//删除oldApplication
- 
-         ApplicationInfo appinfo_In_LoadedApk = (ApplicationInfo) RefInvoke  
-                 .getFieldOjbect("android.app.LoadedApk", loadedApkInfo,  
- "mApplicationInfo");  
-         ApplicationInfo appinfo_In_AppBindData = (ApplicationInfo) RefInvoke 
-                 .getFieldOjbect("android.app.ActivityThread$AppBindData",  
-                         mBoundApplication, "appInfo");  
-         appinfo_In_LoadedApk.className = appClassName;  
-         appinfo_In_AppBindData.className = appClassName;  
-         Application app = (Application) RefInvoke.invokeMethod(  
- "android.app.LoadedApk", "makeApplication", loadedApkInfo,  
- new Class[] { boolean.class, Instrumentation.class },  
- new Object[] { false, null });//执行 makeApplication（false,null）
-         RefInvoke.setFieldOjbect("android.app.ActivityThread",  
- "mInitialApplication", currentActivityThread, app);  
- 
- 
-         ArrayMap mProviderMap = (ArrayMap) RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread", currentActivityThread,  
- "mProviderMap");  
-         Iterator it = mProviderMap.values().iterator();  
- while (it.hasNext()) {  
-             Object providerClientRecord = it.next();  
-             Object localProvider = RefInvoke.getFieldOjbect(  
- "android.app.ActivityThread$ProviderClientRecord",  
-                     providerClientRecord, "mLocalProvider");  
-             RefInvoke.setFieldOjbect("android.content.ContentProvider",  
- "mContext", localProvider, app);  
-         }  
- 
-         Log.i("demo", "app:"+app);  
- 
-         app.onCreate();  
-     }  
- }  
直接在脱壳的Application中的onCreate方法中进行就可以了。这里我们还可以看到是通过AndroidManifest.xml中的meta标签获取源程序Apk中的Application对象的。
下面来看一下AndoridManifest.xml文件中的内容：
在这里我们定义了源程序Apk的Application类名。
**项目下载：[http://download.csdn.net/detail/jiangwei0910410003/9102741](http://download.csdn.net/detail/jiangwei0910410003/9102741)**
### 四、运行程序
那么到这里我们就介绍完了，这三个项目的内容，下面就来看看如何运行吧：
运行步骤：
**第一步：得到源程序Apk文件和脱壳程序的Dex文件**
运行源程序和脱壳程序项目，之后得到这两个文件(记得将classes.dex文件改名ForceApkObj.dex)，然后使用加壳程序进行加壳：
这里的ForceApkObj.apk文件和ForceApkObj.dex文件是输入文件，输出的是classes.dex文件。
**第二步：替换脱壳程序中的classes.dex文件**
我们在第一步中得到加壳之后的classes.dex文件之后，并且我们在第一步运行脱壳项目的时候得到一个ReforceApk.apk文件，这时候我们使用解压缩软件进行替换：
**第三步：我们在第二步的时候得到替换之后的ReforceApk.apk文件，这个文件因为被修改了，所以我们需要从新对他签名，不然运行也是报错的。**
工具下载：[http://download.csdn.net/detail/jiangwei0910410003/9102767](http://download.csdn.net/detail/jiangwei0910410003/9102767)
下载之后的工具需要用ReforeceApk.apk文件替换ReforceApk_des.apk文件，然后运行run.bat就可以得到签名之后的文件了。
run.bat文件的命令如下：
**cd C:\Users\i\Desktop\forceapks     jarsigner -verbose -keystore forceapk -storepass 123456 -keypass 123456 -sigfile CERT -digestalg SHA1 -sigalg MD5withRSA -signedjar ReforceApk_des.apk ReforceApk.apk jiangwei     del ReforceApk.apk**
这里最主要的命令就是中间的一条签名的命令,关于命令的参数说明如下：
**jarsigner -verbose -keystore 签名文件 -storepass 密码  -keypass alias的密码 -sigfile CERT -digestalg SHA1 -sigalg MD5withRSA  签名后的文件 签名前的apk alias名称    eg:      jarsigner -verbose -keystore forceapk -storepass 123456 -keypass 123456 -sigfile CERT -digestalg SHA1 -sigalg MD5withRSA -signedjar ReforceApk_des.apk ReforceApk_src.apk jiangwei     签名文件的密码：123456      alais的密码：123456**
所以这里我们在得到ReforceApk.apk文件的时候，需要签名，关于Eclipse中如何签名一个Apk的话，这里就不多说了，自己google一下吧：
那么通过上面的三个步骤之后我们得到一个签名之后的最终文件：**ReforceApk_des.apk**
我们安装这个Apk，然后运行，效果如下：
看到运行结果的那一瞬间，我们是多么的开心，多么的有成就感，但是这个过程中遇到的问题，是可想而知的。
我们这个时候再去反编译一下源程序Apk(这个文件是我们脱壳出来的payload.apk，看ReforeceApk中的代码，就知道他的位置了)
发现dex文件格式是不正确的。说明我们的加固是成功的。
### 五、遇到的问题
1、研究的过程中遇到签名不正确的地方，开始的时候，直接替换dex文件之后，就直接运行了Apk,但是总是提示签名不正确。
2、运行的过程中说找不到源程序中的Activity，这个问题其实我在动态加载的那篇文章中说道了，我们需要在脱壳程序中的AndroidManifest.xml中什么一下源程序中的Activiity：
### 六、技术要点
**1、对Dex文件格式的了解**
**2、动态加载技术的深入掌握**
**3、Application的执行流程的了解**
**4、如何从Apk中得到Dex文件**
**5、如何从新签名一个Apk程序**
### 七、综合概述
我们通过上面的过程可以看到，关于Apk加固的工作还是挺复杂的，涉及到的东西也挺多的，下面就在来总结一下吧：
**1、加壳程序**
任务：对源程序Apk进行加密，合并脱壳程序的Dex文件 ，然后输入一个加壳之后的Dex文件
语言：任何语言都可以，不限于Java语言
技术点：对Dex文件格式的解析
**2、脱壳程序**
任务：获取源程序Apk,进行解密，然后动态加载进来，运行程序
语言：Android项目(Java)
技术点：如何从Apk中获取Dex文件，动态加载Apk，使用反射运行Application
### 八、总结
Android中的Apk反编译可能是每个开发都会经历的事，但是在反编译的过程中，对于源程序的开发者来说那是不公平的，那么Apk加固也是应运而生，但是即使是这样，我们也还是做不到那么的安全，现在网上也是有很多文章在解析梆梆加固的原理了。而且有人破解成功了，那么加固还不是怎么安全。最后一句话：逆向和加固是一个永不停息的战争。
