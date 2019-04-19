# Android调试的必杀技——反汇编 - xqhrs232的专栏 - CSDN博客
2011年04月21日 16:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1014标签：[android																[汇编																[c																[crash																[system																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=crash&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.unix-center.net/~Simon_fu/?p=527](http://my.unix-center.net/~Simon_fu/?p=527)
在移植Android过程中会遇到很多Crash的情况，尤其是启动Android过程中。一般这些问题都可以通过看代码能解决，当然也有一些比较“妖娆”的问题，非常难找到头绪，在logcat日志也只会打印一些崩溃的堆栈，这些信息很难帮助我们定位问题。根据个人一个实例来介绍一下在Android移植过程中反汇编的用法。
     首先先看一下我遇到的一个logcat关于Crash的打印信息：
I/DEBUG   ( 1417): *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
I/DEBUG   ( 1417): Build fingerprint: 'generic/sdk/generic/:Eclair/ECLAIR/eng.simon.20100607.133011:eng/test-keys'
I/DEBUG   ( 1417): pid: 1434, tid: 1460  >>> system_server <<<
I/DEBUG   ( 1417): signal 11 (SIGSEGV), fault addr 00000000
I/DEBUG   ( 1417):  zr 00000000  at 00000000  v0 00007265  v1 00193228
I/DEBUG   ( 1417):  a0 00000001  a1 0000000a  a2 00193228  a3 001f2ccf
I/DEBUG   ( 1417):  t0 00193228  t1 80000008  t2 8007e8dc  t3 fffffff8
I/DEBUG   ( 1417):  t4 00030465  t5 00000000  t6 00200073  t7 00000000
I/DEBUG   ( 1417):  s0 2fa0786c  s1 30564f00  s2 7ef5d990  s3 2fa0786c
I/DEBUG   ( 1417):  s4 30465000  s5 00100000  s6 7b10a8b4  s7 00000001
I/DEBUG   ( 1417):  t8 00000000  t9 7ef50d44  k0 00000000  k1 00000000
I/DEBUG   ( 1417):  gp 7ef6fd60  sp 30564e90  s8 2fa07858  ra 7ef53470
I/DEBUG   ( 1417):  hi 0000000f  lo 04444444 bva 00007265 epc 7ef534a4
I/DEBUG   ( 1417):          #00  pc 7ef534a4  /system/lib/libc.so
I/DEBUG   ( 1417):          #01  ra 7ef53470  /system/lib/libc.so
I/DEBUG   ( 1417):
I/DEBUG   ( 1417): code around pc:
I/DEBUG   ( 1417): 7ef53494 afa7002c afa40030 1040000b afa50028
I/DEBUG   ( 1417): 7ef534a4 8c4a0000 8c440008 8c590004 8fa2001c
I/DEBUG   ( 1417): 7ef534b4 0320f809 ac4a0034 8fa9001c 8d220034
I/DEBUG   ( 1417):
I/DEBUG   ( 1417): code around lr:
I/DEBUG   ( 1417): 7ef53460 afbc0010 8f9987c4 0320f809 00808021
I/DEBUG   ( 1417): 7ef53470 afa2001c 8fa8001c 8fa6001c 8fa3001c
I/DEBUG   ( 1417): 7ef53480 8d07000c 8cc40010 8c650008 8c420034
I/DEBUG   ( 1417):
I/DEBUG   ( 1417): stack:
I/DEBUG   ( 1417):     30564e50  00000000
I/DEBUG   ( 1417):     30564e54  00000000
I/DEBUG   ( 1417):     30564e58  00000000
I/DEBUG   ( 1417):     30564e5c  00000000
I/DEBUG   ( 1417):     30564e60  00009004
I/DEBUG   ( 1417):     30564e64  00000000
I/DEBUG   ( 1417):     30564e68  00000000
I/DEBUG   ( 1417):     30564e6c  00000000
I/DEBUG   ( 1417):     30564e70  00000009
I/DEBUG   ( 1417):     30564e74  00000000
I/DEBUG   ( 1417):     30564e78  00000000
I/DEBUG   ( 1417):     30564e7c  7ef12af4  /system/lib/libc.so
I/DEBUG   ( 1417):     30564e80  7ef6fd60
I/DEBUG   ( 1417):     30564e84  00000000
I/DEBUG   ( 1417):     30564e88  00000000
I/DEBUG   ( 1417):     30564e8c  7ef53470  /system/lib/libc.so
I/DEBUG   ( 1417):     30564e90  7ef6fd60
I/DEBUG   ( 1417):     30564e94  00000000
I/DEBUG   ( 1417):     30564e98  30564eb4
I/DEBUG   ( 1417):     30564e9c  00000000
I/DEBUG   ( 1417):     30564ea0  7ef6fd60
I/DEBUG   ( 1417):     30564ea4  7b10a8e8  /system/lib/libsqlite.so
I/DEBUG   ( 1417):     30564ea8  7b10a8b4  /system/lib/libsqlite.so
I/DEBUG   ( 1417):     30564eac  00193228  [heap]
I/DEBUG   ( 1417):     30564eb0  2fa07870
I/DEBUG   ( 1417):     30564eb4  00000000
I/DEBUG   ( 1417):     30564eb8  0000000a
I/DEBUG   ( 1417):     30564ebc  001f2ccf  [heap]
I/DEBUG   ( 1417):     30564ec0  00000001
I/DEBUG   ( 1417):     30564ec4  7ef537fc  /system/lib/libc.so
I/DEBUG   ( 1417):     30564ec8  00193228  [heap]
I/DEBUG   ( 1417):     30564ecc  30564f00
I/DEBUG   ( 1417):     30564ed0  7ef5d990  /system/lib/libc.so
I/DEBUG   ( 1417):     30564ed4  2fa0786c
I/DEBUG   ( 1417):     30564ed8  30465000
I/DEBUG   ( 1417):     30564edc  00100000  [heap]
I/DEBUG   ( 1417):     30564ee0  7b10a8b4  /system/lib/libsqlite.so
I/DEBUG   ( 1417):     30564ee4  00000001
I/DEBUG   ( 1417):     30564ee8  2fa07858
I/DEBUG   ( 1417):     30564eec  7ef530ac  /system/lib/libc.so
I/DEBUG   ( 1417):     30564ef0  7b10a8b4  /system/lib/libsqlite.so
I/DEBUG   ( 1417):     30564ef4  2fa0786c
I/DEBUG   ( 1417):     30564ef8  00000000
I/DEBUG   ( 1417):     30564efc  00000000
I/DEBUG   ( 1417):     30564f00  30564f00
I/DEBUG   ( 1417):     30564f04  00193228  [heap]
I/DEBUG   ( 1417):     30564f08  00000009
I/DEBUG   ( 1417):     30564f0c  00000000
I/DEBUG   ( 1417):     30564f10  00000000
I/DEBUG   ( 1417):     30564f14  00000000
I/DEBUG   ( 1417):     30564f18  00000000
I/DEBUG   ( 1417):     30564f1c  00000000
I/DEBUG   ( 1417):     30564f20  00000000
I/DEBUG   ( 1417):     30564f24  00000000
I/DEBUG   ( 1417):     30564f28  00000000
I/DEBUG   ( 1417):     30564f2c  00000000
I/DEBUG   ( 1417):     30564f30  00000000
I/DEBUG   ( 1417):     30564f34  00000000
I/DEBUG   ( 1417):     30564f38  00000000
I/DEBUG   ( 1417):     30564f3c  00000000
I/DEBUG   ( 1417):     30564f40  00000000
I/DEBUG   ( 1417):     30564f44  00000000
I/DEBUG   ( 1417):     30564f48  00000000
I/DEBUG   ( 1417):     30564f4c  00000000
I/DEBUG   ( 1417):     30564f50  00000000
I/DEBUG   ( 1417):     30564f54  00000000
I/DEBUG   ( 1417):     30564f58  00000000
I/DEBUG   ( 1417):     30564f5c  00000000
I/DEBUG   ( 1417):     30564f60  00000000
I/DEBUG   ( 1417):     30564f64  00000000
I/DEBUG   ( 1417):     30564f68  00000000
I/DEBUG   ( 1417):     30564f6c  00000000
I/DEBUG   ( 1417):     30564f70  00000000
I/DEBUG   ( 1417):     30564f74  00000000
I/DEBUG   ( 1417):     30564f78  00000000
I/DEBUG   ( 1417):     30564f7c  00000000
I/DEBUG   ( 1417):     30564f80  00000000
I/DEBUG   ( 1417):     30564f84  00000000
I/DEBUG   ( 1417):     30564f88  00000000
I/DEBUG   ( 1417):     30564f8c  00000000
I/DEBUG   ( 1417):     30564f90  00000000
I/DEBUG   ( 1417):     30564f94  00000000
I/DEBUG   ( 1417):     30564f98  00000000
I/DEBUG   ( 1417):     30564f9c  00000000
I/DEBUG   ( 1417):     30564fa0  00000000
I/DEBUG   ( 1417):     30564fa4  00000000
I/DEBUG   ( 1417):     30564fa8  00000000
I/DEBUG   ( 1417):     30564fac  00000000
I/DEBUG   ( 1417):     30564fb0  00000000
I/DEBUG   ( 1417):     30564fb4  00000000
I/DEBUG   ( 1417):     30564fb8  00000000
I/DEBUG   ( 1417):     30564fbc  00000000
I/DEBUG   ( 1417):     30564fc0  00000000
I/DEBUG   ( 1417):     30564fc4  00000000
I/DEBUG   ( 1417):     30564fc8  00000000
I/DEBUG   ( 1417):     30564fcc  00000000
I/DEBUG   ( 1417):     30564fd0  00000000
I/DEBUG   ( 1417):     30564fd4  00000000
I/DEBUG   ( 1417):     30564fd8  00000000
I/DEBUG   ( 1417):     30564fdc  00000000
I/DEBUG   ( 1417):     30564fe0  00000000
I/DEBUG   ( 1417):     30564fe4  00000000
I/DEBUG   ( 1417):     30564fe8  00000000
I/DEBUG   ( 1417):     30564fec  00000000
I/DEBUG   ( 1417):     30564ff0  00000000
I/DEBUG   ( 1417):     30564ff4  00000000
I/DEBUG   ( 1417):     30564ff8  00000000
I/DEBUG   ( 1417):     30564ffc  00000000
I/DEBUG   ( 1417):     30565000  00000000
I/DEBUG   ( 1417):     30565004  00000000
I/DEBUG   ( 1417):     30565008  00000000
I/DEBUG   ( 1417):     3056500c  00000000
I/DEBUG   ( 1417):     30565010  00000000
I/DEBUG   ( 1417):     30565014  00000000
I/DEBUG   ( 1417):     30565018  00000000
I/DEBUG   ( 1417):     3056501c  00000000
I/DEBUG   ( 1417):     30565020  00000000
I/DEBUG   ( 1417):     30565024  00000000
I/DEBUG   ( 1417):     30565028  00000000
I/DEBUG   ( 1417):     3056502c  00000000
I/DEBUG   ( 1417):     30565030  00000000
I/DEBUG   ( 1417):     30565034  00000000
I/DEBUG   ( 1417):     30565038  00000000
I/DEBUG   ( 1417):     3056503c  00000000
I/DEBUG   ( 1417):     30565040  00000000
W/SyncManager( 1434): Updating fornew accounts...
     通过这个log信息我们可以看到libc.so崩溃了，再研究堆栈发现是libsqilte.so引起的，那么具体是哪一个函数崩溃了呢？这里面没有信息。另外内核加载动态库是动态加载的，就算我们反汇编出来libc.so和libsqlite.so，符号表也没有办法和log中地址对应上，除非我们能知道内核加载libc.so和libsqlite.so的基地址，这样我们就可以通过偏移找到相应的函数了。很幸运，Android确实规定了系统中的大部分库的内核加载地址。文件的位置在build/core下，有对应平台的map文件，比如：Arm平台文件名叫做prelink-linux-arm.map，Mips平台叫做prelink-linux-mips.map。我是在Mips平台出的问题，所以应该用prelink-linux-mips.map文件来定位。文件内容如下：
# 0x7F100000 - 0x7FFF0000 Thread 0 stack
# 0x7F000000 - 0x7F0FFFFF Linker
# 0x70000000 - 0x7EFFFFFF Prelinked System Libraries
# 0x60000000 - 0x6FFFFFFF Prelinked App Libraries
# 0x50000000 - 0x5FFFFFFF Non-prelinked Libraries
# 0x40000000 - 0x4FFFFFFF mmap'd stuff
# 0x10000000 - 0x3FFFFFFF Thread Stacks
# 0x00080000 - 0x0FFFFFFF .text / .data / heap
# core system libraries
libdl.so                0x7EFF0000
libc.so                 0x7EF00000
libstdc++.so            0x7EEF0000
libm.so                 0x7EE90000
liblog.so               0x7EE80000
libcutils.so            0x7EE00000
libthread_db.so         0x7ED80000
libz.so                 0x7ED00000
libevent.so             0x7EC80000
libssl.so               0x7EC00000
libcrypto.so            0x7EA00000
libffi.so	        0x7E980000
libsysutils.so          0x7E900000
# bluetooth
liba2dp.so              0x7E780000
audio.so                0x7E700000
input.so                0x7E680000
libhcid.so              0x7E600000
libbluedroid.so         0x7E580000
libbluetooth.so         0x7E500000
libdbus.so              0x7E400000
# extended system libraries
libril.so               0x7E300000
libreference-ril.so     0x7E000000
libwpa_client.so        0x7DC00000
libnetutils.so          0x7DB00000
# core dalvik runtime support
libandroid_servers.so   0x7D900000
#libicudata.so           0x7D700000
libicuuc.so             0x7D500000
libicui18n.so           0x7D380000
libandroid_runtime.so   0x7D2a0000
libnativehelper.so      0x7D200000
libdvm-MIPS.so          0x7D180000
libdvm.so               0x7D000000
# graphics
libpixelflinger.so      0x7CF00000
libsurfaceflinger.so    0x7CD00000
libagl.so               0x7CC00000
libGLESv1_CM.so         0x7CB00000
libGLESv2.so            0x7CA00000
libOpenVG_CM.so         0x7C900000
libOpenVGU_CM.so        0x7C800000
libEGL.so               0x7C700000
libexif.so              0x7C500000
libui.so                0x7C400000
libsgl.so               0x7C000000
# audio
libspeech.so            0x7BA00000
libaudio.so             0x7B900000
libsonivox.so           0x7B800000
libsoundpool.so         0x7B700000
libvorbisidec.so        0x7B600000
libmedia_jni.so         0x7B500000
libmediaplayerservice.so 0x7B480000
libmedia.so             0x7B400000
libFFTEm.so             0x7B300000
libaudioflinger.so      0x7B200000
# assorted system libraries
libsqlite.so            0x7B100000
libexpat.so             0x7B000000
libwebcore.so           0x7A000000
libutils.so             0x79D00000
libcameraservice.so     0x79C80000
libhardware.so          0x79C70000
libhardware_legacy.so   0x79C00000
libapp_process.so       0x79B00000
libsystem_server.so     0x79A00000
libime.so               0x79800000
libgps.so               0x79700000
libcamera.so            0x79680000
libqcamera.so           0x79400000
# pv libraries
libpvasf.so                    0x79200000
libpvasfreg.so                 0x79100000
libomx_sharedlibrary.so        0x790e0000
libopencore_download.so        0x79000000
libopencore_downloadreg.so     0x78f00000
libopencore_net_support.so     0x78e00000
libopencore_rtsp.so            0x78d00000
libopencore_rtspreg.so         0x78c00000
libopencore_author.so          0x78a00000
libomx_aacdec_sharedlibrary.so 0x789c0000
libomx_amrdec_sharedlibrary.so 0x78990000
libomx_amrenc_sharedlibrary.so 0x78970000
libomx_avcdec_sharedlibrary.so 0x78958000
libomx_m4vdec_sharedlibrary.so 0x78930000
libomx_m4venc_sharedlibrary.so 0x788f0000
libomx_mp3dec_sharedlibrary.so 0x788d0000
libopencore_mp4local.so        0x78800000
libopencore_mp4localreg.so     0x78700000
libopencore_player.so          0x78400000
# opencore hardware support
libmm-adspsvc.so              0x783c0000
libOmxCore.so                 0x783a0000
libOmxMpeg4Dec.so             0x78370000
libOmxH264Dec.so              0x78340000
libOmxVidEnc.so               0x78310000
libopencorehw.so              0x78300000
libopencore_common.so         0x78180000
#libqcomm_omx.so               0xA5A00000
# libraries for specific apps or temporary libraries
libcam_ipl.so           0x6F000000
libwbxml.so             0x6E800000
libwbxml_jni.so         0x6E400000
libxml2wbxml.so         0x6E000000
libaes.so               0x6DC00000
libdrm1.so              0x6D800000
libdrm1_jni.so          0x6D400000
libwapcore.so           0x6D000000
libstreetview.so        0x6CC00000
libwapbrowsertest.so    0x6C800000
libminiglobe.so         0x6C400000
libearth.so             0x6C000000
libembunit.so           0x6BC00000
libneon.so              0x6B800000
libjni_example.so       0x6B400000
libjni_load_test.so     0x6B000000
libjni_lib_test.so      0x6AC00000
librunperf.so           0x6A800000
libctest.so             0x6A700000
libUAPI_jni.so          0x6A500000
librpc.so               0x6A400000
libtrace_test.so        0x6A300000
libsrec_jni.so          0x6A200000
libcerttool_jni.so      0x6A100000
libacc.so               0x6A000000
libbinder.so            0x69F00000
libskia.so              0x69000000
libGLES_android.so      0x68800000
libRS.so                0x68000000
libaudiopolicygeneric.so 0x67c00000
librs_jni.so            0x67800000
# Sigma Designs libraries
libcore.so                      0x61400000
libdisplay.so                   0x61000000
libdrm.so                       0x60c00000
libhw.so                        0x60800000
libplayback.so                  0x60000000
     从这个map文件我们可以查询到每个lib库的加载基地址。比如libc.so将会被内核加载到0x7EF00000，libsqlite.so加载到0x7B100000。我们可以对照一下Crash的log信息也对应的上，说明这个文件在Android加载过程中起了作用。
     下一步我们需要反汇编libc.so和libsqlite.so。一般交叉编译器都提供了反汇编的工具，我的mips平台提供了mips-linux-gnu-objdump命令来进行反汇编。
mips-linux-gnu-objdump -dS libc.so > libc.dump
mips-linux-gnu-objdump -dS libsqlite.so > libsqlite.dump
     这样就可以得到libc和libsqlite的符号表。然后通过符号表，Android加载动态库的基地址，log信息就可以定位到那个函数出问题了，如果你对对应平台汇编语言熟悉的话可以阅读汇编代码找出问题。本文就不具体讲怎样利用这个三个文件信息了。有了这个三个文件，稍一研究就可以明白怎样分析了。
     一般情况下，Crash都不是Android源码的问题，最有可能的是内核有些模块没有编译进去。本例中就是和Mutex相关的模块没有编译进内核引起的问题。
     以上是个人摸索出来的方法，如果你有更好的方法或者我的方法有错误，请你不吝指教。

