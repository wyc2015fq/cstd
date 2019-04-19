# Android GPS架构分析之一 - xqhrs232的专栏 - CSDN博客
2011年12月01日 22:46:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1057
原文地址::[http://www.51x86.com/chongzi865458/blog/11-10/232232_50018.html#articletop](http://www.51x86.com/chongzi865458/blog/11-10/232232_50018.html#articletop)
首先来看看GPS模块的代码结构：
**Framework：**
1.frameworks/base/location/java/android/location
这里主要是用来被App调用的，API包是android.location。
2.frameworks/base/location/java/com/android/internal/location
这个目录是Framework对Location服务的内部实现。
3.framework\services\java\com\android\server
这个目录只有一个文件
|-- LocationManagerService.java
是Location服务对内部实现的一种封装。
**JNI：**
frameworks/base/core/jni/android_location_GpsLocationProvider.cpp
JNI层只有一个文件，起到承上启下的作用。上层承接Framework，下层调用HAL层具体硬件抽象实现。
**HAL：**Hardware Abstract Layer 硬件抽象层
hardware\libhardware_legacy\gps
hardware\libhardware_legacy\include\hardware_legacy\gps.h
HAL层相当于一个linux应用程序接口，通过open，close等操作，操作硬件设备。Android的源代码只实现了模拟器的gps接口，具体在文件gps_qemu.c中。在2.2版本中提供了对QCOM公司的gps的实现，在以下目录：
\hardware\qcom
下面介绍几个重要的数据结构：
1. GpsInterface接口是gps模块中最重要的数据结构，它是底层驱动实现的接口，如果要porting到自己的板子上，就需要实现这些接口。该接口的定义在gps.h中，模拟器实现在gps_qemu.c中。
```
/** Represents the standard GPS interface. */
typedef struct {
    /**
     * Opens the interface and provides the callback routines
     * to the implemenation of this interface.
     */
    int (*init)( GpsCallbacks* callbacks );
    /** Starts navigating. */
    int (*start)( void );
    /** Stops navigating. */
    int (*stop)( void );
    /** Closes the interface. */
    void (*cleanup)( void );
    /** Injects the current time. */
    int (*inject_time)(GpsUtcTime time, int64_t timeReference,
                         int uncertainty);
    /** Injects current location from another location provider
     * (typically cell ID).
     * latitude and longitude are measured in degrees
     * expected accuracy is measured in meters
     */
    int (*inject_location)(double latitude, double longitude, float accuracy);
    /**
     * Specifies that the next call to start will not use the
     * information defined in the flags. GPS_DELETE_ALL is passed for
     * a cold start.
     */
    void (*delete_aiding_data)(GpsAidingData
 flags);
    /**
     * fix_frequency represents the time between fixes in seconds.
     * Set fix_frequency to zero for a single-shot fix.
     */
    int (*set_position_mode)(GpsPositionMode
 mode, int fix_frequency);
    /** Get a pointer to extension information. */
    const void* (*get_extension)(const char* name);
} GpsInterface;
```
## 2. GpsCallbacks回调函数
这个是回调函数结构体，定义也在gps.h中。它们的实现是在android_location_GpsLocationProvider.cpp中，google已经实现了，我们不需要做任何动作。
```
/** GPS callback structure. */
typedef struct {
        gps_location_callback location_cb;
        gps_status_callback status_cb;
        gps_sv_status_callback sv_status_cb;
        gps_nmea_callback nmea_cb;
} GpsCallbacks;
/** Callback with location information. */
typedef void (* gps_location_callback)(GpsLocation* location);
/** Callback with status information. */
typedef void (* gps_status_callback)(GpsStatus* status);
/** Callback with SV status information. */
typedef void (* gps_sv_status_callback)(GpsSvStatus* sv_info);
/** Callback for reporting NMEA sentences. */
typedef void (* gps_nmea_callback)(GpsUtcTime
 timestamp, const char* nmea, int length);
```
## 3. GpsLocation
## 表示Locatin数据信息，底层驱动获得Location的raw信息，通常是nmea码，然后通过解析就得到了location信息。
```
/** Represents a location. */
typedef struct {
    /** Contains GpsLocationFlags bits. */
    uint16_t flags;
    /** Represents latitude in degrees. */
    double latitude;
    /** Represents longitude in degrees. */
    double longitude;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double altitude;
    /** Represents speed in meters per second. */
    float speed;
    /** Represents heading in degrees. */
    float bearing;
    /** Represents expected accuracy in meters. */
    float accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime timestamp;
} GpsLocation;
```
