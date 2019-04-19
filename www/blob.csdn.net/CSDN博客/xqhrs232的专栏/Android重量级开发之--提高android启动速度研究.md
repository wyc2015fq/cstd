# Android重量级开发之--提高android启动速度研究 - xqhrs232的专栏 - CSDN博客
2012年02月29日 15:06:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：625标签：[android																[service																[archive																[class																[file																[system](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=archive&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.eoeandroid.com/thread-29953-1-1.html](http://www.eoeandroid.com/thread-29953-1-1.html)
Android重量级开发之--提高android启动速度研究
[www.eoeandroid.com](http://www.eoeandroid.com/)首发，作者：[Tigertang2@gmail.com](mailto:Tigertang2@gmail.com)
     大家都知道启动速度慢是智能操作系统的一个通病，Android也不例外，启动速度大概在1分钟左右，虽然日本有一个叫quick boot的一秒启动android的产品，但是毕竟是旁门左道。所以从常规来提高android的启动速度成了大家研究的重点，也是难点。下面将初步研究的一下经验跟大家分享一下。
首先看一下android系统的启动流程：
bootloader
          引导程序
kernel
         内核
init
          init初始化（这个大家都比较熟悉了，不要多说）
- - - loads several daemons and services, including zygote
- see /init.rc and init.*<platform>*.rc
zygote
这个是占用时间最多的，重点修理对象- - - preloads classes
装载了一千多个类，妈呀！！！
- starts package manager
扫描package(下面详细介绍)
service manager
- - - start services
（启动多个服务）
从实际的测试数据来看，有两个地方时最耗时间的，一个是zygote的装载一千多个类和初始化堆栈的过程，用了20秒左右。另一个是扫描
/system/app,
    /system/framework,
    /data/app,
    /data/app-private.
这几个目录下面的package用了大概10秒，所以我们重点能够修理的就是这两个老大的。
一、首先是调试工具的使用，可以测试哪些类和那些过程占用了多少时间，
主要工具为
stopwatch
Message loggers
- - - grabserial
参考[http://elinux.org/Grabserial](http://elinux.org/Grabserial)
- printk times
参考[http://elinux.org/Printk_Times](http://elinux.org/Printk_Times)
- logcat
Android自带
- 
bootchart
参考[http://elinux.org/Bootchart](http://elinux.org/Bootchart)和
[http://elinux.org/Bootchart](http://elinux.org/Bootchart)
strace
    AOSP的一部分（Eclair及以上版本）
使用例子
在init.rc中为了调试zygote
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server改为
service zygote /system/xbin/strace -tt -o/data/boot.strace /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
method tracer*
ftrace*
详细使用可看提供的文档和网页介绍
上面的工具如果不用详细的分析不一定都用到，也可以使用logcat就可以,在代码中加一点计算时间和一些类的调试信息也可以达到很好效果。
二、zygote
装载1千多个类
首先，我们可以添加一点调试信息，以获得具体转载情况。
diff --git a/core/java/com/android/internal/os/ZygoteInit.java b/core/java/com/android/internal/os/ZygoteInit.java
index 404c513..f2b573c 100644
--- a/core/java/com/android/internal/os/ZygoteInit.java
+++ b/core/java/com/android/internal/os/ZygoteInit.java
@@ -259,6 +259,8 @@ public class ZygoteInit {
         } else {
             Log.i(TAG, "Preloading classes...");
             long startTime = SystemClock.uptimeMillis();
+            long lastTime = SystemClock.uptimeMillis();
+            long nextTime = SystemClock.uptimeMillis();
             // Drop root perms while running static initializers.
             setEffectiveGroup(UNPRIVILEGED_GID);
@@ -292,12 +294,24 @@ public class ZygoteInit {
                         if (Config.LOGV) {
                             Log.v(TAG, "Preloading " + line + "...");
                         }
+                        //if (count%5==0) {
+                        //    Log.v(TAG, "Preloading " + line + "...");
+                        //}
+                        Log.v(TAG, "Preloading " + line + "...");
                         Class.forName(line);
+              nextTime = SystemClock.uptimeMillis();
+   if (nextTime-lastTime >50) {
+       Log.i(TAG, "Preloading " + line + "... took " + (nextTime-lastTime) + "ms.");
+   }
+   lastTime = nextTime;
+   
                         if (Debug.getGlobalAllocSize() > PRELOAD_GC_THRESHOLD) {
                             if (Config.LOGV) {
                                 Log.v(TAG,
                                     " GC at " + Debug.getGlobalAllocSize());
                             }
+                            Log.i(TAG,
+                               " GC at " + Debug.getGlobalAllocSize());
                             runtime.gcSoftReferences();
                             runtime.runFinalizationSync();
                             Debug.resetGlobalAllocSize();
上面+代表添加的代码，这样就可以很容易的得到在装载类的过程中具体装载了哪些类，耗费了多久。具体装载的类在文件platform/frameworks/base/ 
     preloaded-classes
内容类似：
android.R$styleable
android.accounts.AccountMonitor
android.accounts.AccountMonitor$AccountUpdater
android.app.Activity
android.app.ActivityGroup
android.app.ActivityManager$MemoryInfo$1
android.app.ActivityManagerNative
android.app.ActivityManagerProxy
android.app.ActivityThread
android.app.ActivityThread$ActivityRecord
android.app.ActivityThread$AppBindData
android.app.ActivityThread$ApplicationThread
android.app.ActivityThread$ContextCleanupInfo
android.app.ActivityThread$GcIdler
android.app.ActivityThread$H
android.app.ActivityThread$Idler
而这个文件是由文件WritePreloadedClassFile.java中的WritePreloadedClassFile类自动生成
*/***
** Writes /frameworks/base/preloaded-classes. Also updates*
** {@link LoadedClass#preloaded} fields and writes over compiled log file.*
**/*
**public****class**WritePreloadedClassFile
    /**
     * Preload any class that take longer to load than MIN_LOAD_TIME_MICROS us.
     */
static final int MIN_LOAD_TIME_MICROS = 1250;//这个代表了装载时间小于1250us即1.25ms的类将不予装载，也许可以改这个参数减少一下类的装载

//这里可以看到什么样的类会被装载
A:启动必须装载的类，比如系统级的类
B：刚才说的装载时间大于1.25ms的类
C：被使用一次以上或被应用装载的类
仔细看看筛选类的具体实现，可以帮助我们认识哪些类比较重要，哪些可以去掉。
筛选规则是
第一  isPreloadable，
    /**Reports if the given class should be preloaded. */
    public static boolean isPreloadable(LoadedClass clazz) {
        return clazz.systemClass && !EXCLUDED_CLASSES.contains(clazz.name);
    }
意思是指除了EXCLUDED_CLASSES包含的类之外的所有系统装载的类。
EXCLUDED_CLASSES包含
    /**
     * Classes which we shouldn't load from the Zygote.
     */
    private static final Set<String> EXCLUDED_CLASSES
            = new HashSet<String>(Arrays.asList(
        // Binders
        "android.app.AlarmManager",
        "android.app.SearchManager",
        "android.os.FileObserver",
        "com.android.server.PackageManagerService$AppDirObserver",
        // Threads
        "android.os.AsyncTask",
        "android.pim.ContactsAsyncHelper",
        "java.lang.ProcessManager"
    ));
目前是跟Binders跟Threads有关的不会被预装载。
第二   clazz.medianTimeMicros() > MIN_LOAD_TIME_MICROS装载时间大于1.25ms。
第三  names.size() > 1 ，既是被processes一次以上的。
上面的都是指的system class,另外还有一些application class需要被装载
规则是fromZygote而且不是服务
proc.fromZygote() && !Policy.isService(proc.name)
fromZygote指的除了com.android.development的zygote类
    public boolean fromZygote() {
        return parent != null && parent.name.equals("zygote")
                && !name.equals("com.android.development");
    }
/除了常驻内存的服务
    /**
     * Long running services. These are restricted in their contribution to the 
     * preloader because their launch time is less critical.
     */
    // TODO: Generate this automatically from package manager.
    private static final Set<String> SERVICES = new HashSet<String>(Arrays.asList(
        "system_server",
        "com.google.process.content",
        "android.process.media",
        "com.android.bluetooth",
        "com.android.calendar",
        "com.android.inputmethod.latin",
        "com.android.phone",
        "com.google.android.apps.maps.FriendService", // pre froyo
        "com.google.android.apps.maps:FriendService", // froyo
        "com.google.android.apps.maps.LocationFriendService",
        "com.google.android.deskclock",
        "com.google.process.gapps",
        "android.tts"
    ));
好了。要转载的就是这些类了。虽然preloaded-classes是在下载源码的时候已经确定了的，也就是对我们来说WritePreloadedClassFile类是没用到的，我们可以做的就是在preloaded-classes文件中，把不预装载的类去掉，试了把所有类去掉，启动确实很快跳过那个地方，但是启动HOME的时候就会很慢了。所以最好的方法就是只去掉那些没怎么用到的，不过要小心处理。至于该去掉哪些，还在摸索，稍后跟大家分享。有兴趣的朋友可以先把preloaded-classes这个文件里面全部清空，启动快了很多，但在启动apk的时候会慢了点。当然了，也可以把android相关的类全部去掉，剩下java的类，试过了也是可以提高速度。
三，系统服务初始化和package
扫描
在启动系统服务的init2()时会启动应用层（Java层）的所有服务。
    public static void main(String[] args) {
        System.loadLibrary("android_servers");
        init1(args); //init1 初始化，完成之后会回调init2()
    }
在init2()中会启动一个线程来启动所有服务
public static final void init2() {
        Log.i(TAG, "Entered the Android system server!");
        Thread thr = new ServerThread();
        thr.setName("android.server.ServerThread");
        thr.start();
    }
class ServerThread extends Thread {
。。。
public void run() {
。。。
关键服务：
  ServiceManager.addService("entropy", new EntropyService());
ServiceManager.addService(Context.POWER_SERVICE, power);
   context = ActivityManagerService.main(factoryTest);
  ServiceManager.addService("telephony.registry", new TelephonyRegistry(context));
    PackageManagerService.main(context,
                    factoryTest != SystemServer.FACTORY_TEST_OFF);//apk扫描的服务
   ServiceManager.addService(Context.ACCOUNT_SERVICE,
                        new AccountManagerService(context));
         ContentService.main(context,
                    factoryTest == SystemServer.FACTORY_TEST_LOW_LEVEL);
       battery = new BatteryService(context);
            ServiceManager.addService("battery", battery);
        hardware = new HardwareService(context);
            ServiceManager.addService("hardware", hardware);
          AlarmManagerService alarm = new AlarmManagerService(context);
            ServiceManager.addService(Context.ALARM_SERVICE, alarm);
ServiceManager.addService(Context.SENSOR_SERVICE, new SensorService(context));
WindowManagerService.main(context, power,
                    factoryTest != SystemServer.FACTORY_TEST_LOW_LEVEL);
            ServiceManager.addService(Context.WINDOW_SERVICE, wm);
上面这些都是关键服务，不建议进行裁剪。
下面的这些不是很关键，可以进行裁剪，当是必须相应的修改framework部分的代码，工作量比较大和复杂。我去掉了20个服务，大概需要相应修改大概20多个文件。
                statusBar = new StatusBarService(context);
                ServiceManager.addService("statusbar", statusBar);
                ServiceManager.addService("clipboard", new ClipboardService(context));
                imm = new InputMethodManagerService(context, statusBar);
                ServiceManager.addService(Context.INPUT_METHOD_SERVICE, imm);
                ServiceManager.addService("netstat", new NetStatService(context));
                connectivity = ConnectivityService.getInstance(context);
                ServiceManager.addService(Context.CONNECTIVITY_SERVICE, connectivity);
                   ServiceManager.addService(Context.ACCESSIBILITY_SERVICE,
                      new AccessibilityManagerService(context));
                notification = new NotificationManagerService(context, statusBar, hardware);
                ServiceManager.addService(Context.NOTIFICATION_SERVICE, notification);
                ServiceManager.addService("mount", new MountService(context));
                ServiceManager.addService(DeviceStorageMonitorService.SERVICE,
                        new DeviceStorageMonitorService(context));
                ServiceManager.addService(Context.LOCATION_SERVICE, new LocationManagerService(context));
                ServiceManager.addService( Context.SEARCH_SERVICE, new SearchManagerService(context) );
            if (INCLUDE_DEMO) {
                Log.i(TAG, "Installing demo data...");
                (new DemoThread(context)).start();
            }
                Intent intent = new Intent().setComponent(new ComponentName(
                        "com.google.android.server.checkin",
                        "com.google.android.server.checkin.CheckinService"));
                    ServiceManager.addService("checkin", new FallbackCheckinService(context));
                wallpaper = new WallpaperManagerService(context);
                ServiceManager.addService(Context.WALLPAPER_SERVICE, wallpaper);
                ServiceManager.addService(Context.AUDIO_SERVICE, new AudioService(context));
                headset = new HeadsetObserver(context);
                dock = new DockObserver(context, power);
                ServiceManager.addService(Context.BACKUP_SERVICE, new BackupManagerService(context));
                ServiceManager.addService(Context.APPWIDGET_SERVICE, appWidget);
package 扫描部分，整个流程为下图所示：
最终的zip文件（apk）读取是在下面这两个函数：
/*
* Open the specified file read-only.  We memory-map the entire thing and
* close the file before returning.
*/
status_t ZipFileRO::open(const char* zipFileName)
{
    int fd = -1;
    off_t length;
    assert(mFileMap == NULL);
LOGD("opening zip '%s'\n", zipFileName);
    /*
     * Open and map the specified file.
     */
    fd = ::open(zipFileName, O_RDONLY);
    if (fd < 0) {
        LOGW("Unable to open zip '%s': %s\n", zipFileName, strerror(errno));
        return NAME_NOT_FOUND;
    }
    length = lseek(fd, 0, SEEK_END);
    if (length < 0) {
        close(fd);
        return UNKNOWN_ERROR;
    }
    mFileMap = new FileMap();
    if (mFileMap == NULL) {
        close(fd);
        return NO_MEMORY;
    }
    if (!mFileMap->create(zipFileName, fd, 0, length, true)) {
        LOGW("Unable to map '%s': %s\n", zipFileName, strerror(errno));
        close(fd);
        return UNKNOWN_ERROR;
    }
    mFd = fd;
    /*
     * Got it mapped, verify it and create data structures for fast access.
     */
    if (!parseZipArchive()) {
        mFileMap->release();
        mFileMap = NULL;
        return UNKNOWN_ERROR;
    }
LOGD("done opening zip\n");
    return OK;
}
/*
* Parse the Zip archive, verifying its contents and initializing internal
* data structures.
*/
bool ZipFileRO::parseZipArchive(void)
{
#define CHECK_OFFSET(_off) {                                                \
        if ((unsigned int) (_off) >= maxOffset) {                           \
            LOGE("ERROR: bad offset %u (max %d): %s\n",                     \
                (unsigned int) (_off), maxOffset, #_off);                   \
            goto bail;                                                      \
        }                                                                   \
    }
    const unsigned char* basePtr = (const unsigned char*)mFileMap->getDataPtr();
    const unsigned char* ptr;
    size_t length = mFileMap->getDataLength();
    bool result = false;
    unsigned int i, numEntries, cdOffset;
    unsigned int val;
    /*
     * The first 4 bytes of the file will either be the local header
     * signature for the first file (kLFHSignature) or, if the archive doesn't
     * have any files in it, the end-of-central-directory signature
     * (kEOCDSignature).
     */
    val = get4LE(basePtr);
    if (val == kEOCDSignature) {
        LOGI("Found Zip archive, but it looks empty\n");
        goto bail;
    } else if (val != kLFHSignature) {
        LOGV("Not a Zip archive (found 0x%08x)\n", val);
        goto bail;
    }
    /*
     * Find the EOCD.  We'll find it immediately unless they have a file
     * comment.
     */
    ptr = basePtr + length - kEOCDLen;
    while (ptr >= basePtr) {
        if (*ptr == (kEOCDSignature & 0xff) && get4LE(ptr) == kEOCDSignature)
            break;
        ptr--;
    }
    if (ptr < basePtr) {
        LOGI("Could not find end-of-central-directory in Zip\n");
        goto bail;
    }
    /*
     * There are two interesting items in the EOCD block: the number of
     * entries in the file, and the file offset of the start of the
     * central directory.
     *
     * (There's actually a count of the #of entries in this file, and for
     * all files which comprise a spanned archive, but for our purposes
     * we're only interested in the current file.  Besides, we expect the
     * two to be equivalent for our stuff.)
     */
    numEntries = get2LE(ptr + kEOCDNumEntries);
    cdOffset = get4LE(ptr + kEOCDFileOffset);
    /* valid offsets are [0,EOCD] */
    unsigned int maxOffset;
    maxOffset = (ptr - basePtr) +1;
    LOGV("+++ numEntries=%d cdOffset=%d\n", numEntries, cdOffset);
    if (numEntries == 0 || cdOffset >= length) {
        LOGW("Invalid entries=%d offset=%d (len=%zd)\n",
            numEntries, cdOffset, length);
        goto bail;
    }
    /*
     * Create hash table.  We have a minimum 75% load factor, possibly as
     * low as 50% after we round off to a power of 2.
     */
    mNumEntries = numEntries;
    mHashTableSize = roundUpPower2(1 + ((numEntries * 4) / 3));
    mHashTable = (HashEntry*) calloc(1, sizeof(HashEntry) * mHashTableSize);
    /*
     * Walk through the central directory, adding entries to the hash
     * table.
     */
    ptr = basePtr + cdOffset;
    for (i = 0; i < numEntries; i++) {
        unsigned int fileNameLen, extraLen, commentLen, localHdrOffset;
        const unsigned char* localHdr;
        unsigned int hash;
        if (get4LE(ptr) != kCDESignature) {
            LOGW("Missed a central dir sig (at %d)\n", i);
            goto bail;
        }
        if (ptr + kCDELen > basePtr + length) {
            LOGW("Ran off the end (at %d)\n", i);
            goto bail;
        }
        localHdrOffset = get4LE(ptr + kCDELocalOffset);
        CHECK_OFFSET(localHdrOffset);
        fileNameLen = get2LE(ptr + kCDENameLen);
        extraLen = get2LE(ptr + kCDEExtraLen);
        commentLen = get2LE(ptr + kCDECommentLen);
        //LOGV("+++ %d: localHdr=%d fnl=%d el=%d cl=%d\n",
        //    i, localHdrOffset, fileNameLen, extraLen, commentLen);
        //LOGV(" '%.*s'\n", fileNameLen, ptr + kCDELen);
        /* add the CDE filename to the hash table */
        hash = computeHash((const char*)ptr + kCDELen, fileNameLen);
        addToHash((const char*)ptr + kCDELen, fileNameLen, hash);
      //  localHdr = basePtr + localHdrOffset;
      //  if (get4LE(localHdr) != kLFHSignature) {
           // LOGW("Bad offset to local header: %d (at %d)\n",
             //   localHdrOffset, i);
          //  goto bail;
     //   }
        ptr += kCDELen + fileNameLen + extraLen + commentLen;
        CHECK_OFFSET(ptr - basePtr);
    }
    result = true;
bail:
    return result;
#undef CHECK_OFFSET
}
     红色部分是修改后的代码，大家可以对比一下。(未完。。。)
参考资料推荐：[http://elinux.org/Improving_Android_Boot_Time](http://elinux.org/Improving_Android_Boot_Time)
[http://jollen.org/wiki/Improving_Android_Boot_Time](http://jollen.org/wiki/Improving_Android_Boot_Time)
再传两个patch
