# Mycat JVM appears hung: Timed out waiting for signal from JVM. - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月11日 09:31:36[boonya](https://me.csdn.net/boonya)阅读数：4305








Mycat配置支持的JVM参数是支持1.8_x之前的版本，当JDK升级到1.8_x以后在大数据量操作的时候JVM内存的分配就会存在问题，我们需要做相应的调整以适应这种参数改变，JDK 1.8_x不支持Permgen 。参数需要做的调整为-XX:PermSize=64m -XX:MaxPermSize=128m 变成 -XX:MetaspaceSize=64m -XX:MaxMetaspaceSize=128m 否则在JDK 1.8_x中不生效。


## 错误日志


下面是因为读写内存而导致JVM重启：



```
INFO   | jvm 1    | 2017/09/08 16:50:16 | ======= Backtrace: =========
INFO   | jvm 1    | 2017/09/08 16:50:16 | /lib64/libc.so.6[0x3e62075dee]
INFO   | jvm 1    | 2017/09/08 16:50:16 | /lib64/libc.so.6[0x3e62078ff6]
INFO   | jvm 1    | 2017/09/08 16:50:16 | /home/jdk1.8.0_131/jre/lib/amd64/server/libjvm.so(+0xa9b6de)[0x7fe6fbeaf6de]
INFO   | jvm 1    | 2017/09/08 16:50:16 | [0x7fe6e5017a34]
INFO   | jvm 1    | 2017/09/08 16:50:16 | ======= Memory map: ========
INFO   | jvm 1    | 2017/09/08 16:50:16 | 00400000-00401000 r-xp 00000000 fd:02 917889                             /home/jdk1.8.0_131/bin/java
INFO   | jvm 1    | 2017/09/08 16:50:16 | 00600000-00601000 rw-p 00000000 fd:02 917889                             /home/jdk1.8.0_131/bin/java
INFO   | jvm 1    | 2017/09/08 16:50:16 | 00f45000-00f88000 rw-p 00000000 00:00 0                                  [heap]
INFO   | jvm 1    | 2017/09/08 16:50:16 | 6c0000000-76ab00000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 76ab00000-7c0000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7c0000000-7c02cc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7c02cc000-800000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e61c00000-3e61c20000 r-xp 00000000 fd:00 2752627                        /lib64/ld-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e61e20000-3e61e21000 r--p 00020000 fd:00 2752627                        /lib64/ld-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e61e21000-3e61e22000 rw-p 00021000 fd:00 2752627                        /lib64/ld-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e61e22000-3e61e23000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62000000-3e6218a000 r-xp 00000000 fd:00 2752628                        /lib64/libc-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e6218a000-3e6238a000 ---p 0018a000 fd:00 2752628                        /lib64/libc-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e6238a000-3e6238e000 r--p 0018a000 fd:00 2752628                        /lib64/libc-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e6238e000-3e62390000 rw-p 0018e000 fd:00 2752628                        /lib64/libc-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62390000-3e62394000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62400000-3e62417000 r-xp 00000000 fd:00 2752629                        /lib64/libpthread-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62417000-3e62617000 ---p 00017000 fd:00 2752629                        /lib64/libpthread-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62617000-3e62618000 r--p 00017000 fd:00 2752629                        /lib64/libpthread-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62618000-3e62619000 rw-p 00018000 fd:00 2752629                        /lib64/libpthread-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62619000-3e6261d000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62800000-3e62802000 r-xp 00000000 fd:00 2752636                        /lib64/libdl-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62802000-3e62a02000 ---p 00002000 fd:00 2752636                        /lib64/libdl-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62a02000-3e62a03000 r--p 00002000 fd:00 2752636                        /lib64/libdl-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62a03000-3e62a04000 rw-p 00003000 fd:00 2752636                        /lib64/libdl-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62c00000-3e62c83000 r-xp 00000000 fd:00 2752633                        /lib64/libm-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62c83000-3e62e82000 ---p 00083000 fd:00 2752633                        /lib64/libm-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62e82000-3e62e83000 r--p 00082000 fd:00 2752633                        /lib64/libm-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e62e83000-3e62e84000 rw-p 00083000 fd:00 2752633                        /lib64/libm-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e63000000-3e63007000 r-xp 00000000 fd:00 2752630                        /lib64/librt-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e63007000-3e63206000 ---p 00007000 fd:00 2752630                        /lib64/librt-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e63206000-3e63207000 r--p 00006000 fd:00 2752630                        /lib64/librt-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e63207000-3e63208000 rw-p 00007000 fd:00 2752630                        /lib64/librt-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e66c00000-3e66c16000 r-xp 00000000 fd:00 2752664                        /lib64/libgcc_s-4.4.7-20120601.so.1
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e66c16000-3e66e15000 ---p 00016000 fd:00 2752664                        /lib64/libgcc_s-4.4.7-20120601.so.1
INFO   | jvm 1    | 2017/09/08 16:50:16 | 3e66e15000-3e66e16000 rw-p 00015000 fd:00 2752664                        /lib64/libgcc_s-4.4.7-20120601.so.1
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe64c000000-7fe64c24a000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe64c24a000-7fe650000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6528e9000-7fe6528ec000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6528ec000-7fe6529ea000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6529ea000-7fe6529ed000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6529ed000-7fe652aeb000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652aeb000-7fe652aee000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652aee000-7fe652bec000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652bec000-7fe652bef000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652bef000-7fe652ced000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652ced000-7fe652cf0000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652cf0000-7fe652dee000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652dee000-7fe652df1000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652df1000-7fe652eef000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652eef000-7fe652ef2000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652ef2000-7fe652ff0000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652ff0000-7fe652ff3000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe652ff3000-7fe6530f1000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6530f1000-7fe6530f4000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6530f4000-7fe6531f2000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6531f2000-7fe6531f5000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6531f5000-7fe6532f3000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6532f3000-7fe6532f6000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6532f6000-7fe6533f4000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6533f4000-7fe6533f7000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6533f7000-7fe6534f5000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6534f5000-7fe6534f8000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6534f8000-7fe6535f6000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6535f6000-7fe6535f9000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6535f9000-7fe6536f7000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6536f7000-7fe6536fa000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6536fa000-7fe6537f8000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6537f8000-7fe6537fb000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6537fb000-7fe6538f9000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6538f9000-7fe6538fc000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6538fc000-7fe6539fa000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6539fa000-7fe6539fd000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6539fd000-7fe653afb000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653afb000-7fe653afe000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653afe000-7fe653bfc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653bfc000-7fe653bff000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653bff000-7fe653cfd000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653cfd000-7fe653d00000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653d00000-7fe653dfe000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653dfe000-7fe653e01000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653e01000-7fe653eff000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653eff000-7fe653f02000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe653f02000-7fe654000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe654000000-7fe654261000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe654261000-7fe658000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe658000000-7fe65828f000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe65828f000-7fe65c000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe65c000000-7fe65c25e000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe65c25e000-7fe660000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe660000000-7fe660223000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe660223000-7fe664000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe664000000-7fe66420a000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe66420a000-7fe668000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe668000000-7fe66820f000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe66820f000-7fe66c000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe66c000000-7fe66c5bc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe66c5bc000-7fe670000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe670000000-7fe670206000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe670206000-7fe674000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe674000000-7fe674295000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe674295000-7fe678000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe678000000-7fe678021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe678021000-7fe67c000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe67c000000-7fe67c090000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe67c090000-7fe680000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe680000000-7fe680036000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe680036000-7fe684000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe684000000-7fe684021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe684021000-7fe688000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe688000000-7fe688021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe688021000-7fe68c000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe68c000000-7fe68c021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe68c021000-7fe690000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe690000000-7fe690021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe690021000-7fe694000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe694000000-7fe694021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe694021000-7fe698000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe698000000-7fe698021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe698021000-7fe69c000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe69c000000-7fe69def1000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe69def1000-7fe6a0000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a0000000-7fe6a0a26000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a0a26000-7fe6a4000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a4000000-7fe6a5e54000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a5e54000-7fe6a8000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8056000-7fe6a8216000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8216000-7fe6a8256000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8256000-7fe6a8456000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8456000-7fe6a8656000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8656000-7fe6a8659000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8659000-7fe6a8757000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8757000-7fe6a875a000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a875a000-7fe6a8858000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8858000-7fe6a885b000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a885b000-7fe6a8959000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8959000-7fe6a895c000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a895c000-7fe6a8a5a000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8a5a000-7fe6a8a5d000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8a5d000-7fe6a8b5b000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8b5b000-7fe6a8b5e000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8b5e000-7fe6a8c5c000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8c5c000-7fe6a8c5f000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8c5f000-7fe6a8d5d000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8d5d000-7fe6a8d60000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8d60000-7fe6a8e5e000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8e5e000-7fe6a8e61000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8e61000-7fe6a8f5f000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8f5f000-7fe6a8f62000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a8f62000-7fe6a9060000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9060000-7fe6a9063000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9063000-7fe6a9161000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9161000-7fe6a9164000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9164000-7fe6a9262000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9262000-7fe6a9265000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9265000-7fe6a9363000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9363000-7fe6a9366000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9366000-7fe6a9464000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9464000-7fe6a9467000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6a9467000-7fe6aa16b000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6aa16b000-7fe6b0000000 r--p 00000000 fd:00 2107771                    /usr/lib/locale/locale-archive
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b0000000-7fe6b0021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b0021000-7fe6b4000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b4000000-7fe6b4021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b4021000-7fe6b8000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b8000000-7fe6b80ea000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6b80ea000-7fe6bc000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6bc000000-7fe6bc021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6bc021000-7fe6c0000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6c0000000-7fe6c8000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6c8000000-7fe6c8021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6c8021000-7fe6cc000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6cc0e0000-7fe6cc0e3000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6cc0e3000-7fe6d0000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d0000000-7fe6d0021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d0021000-7fe6d4000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d40e0000-7fe6d40e3000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d40e3000-7fe6d8000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d8000000-7fe6d8021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6d8021000-7fe6dc000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dc0ab000-7fe6dc0ae000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dc0ae000-7fe6dd9b8000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dd9b8000-7fe6dd9bb000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dd9bb000-7fe6ddcb9000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6ddcb9000-7fe6ddcbc000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6ddcbc000-7fe6ddfba000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6ddfba000-7fe6ddfbd000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6ddfbd000-7fe6de0bb000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de0bb000-7fe6de0be000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de0be000-7fe6de1bc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de1bc000-7fe6de1bf000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de1bf000-7fe6de2bd000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de2bd000-7fe6de2c0000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de2c0000-7fe6de3be000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de3be000-7fe6de3c1000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de3c1000-7fe6de4bf000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de4bf000-7fe6de4da000 r--s 001d3000 fd:02 917544                     /home/jdk1.8.0_131/jre/lib/ext/nashorn.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de4da000-7fe6de6da000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de6da000-7fe6de8da000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de8da000-7fe6de8dd000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de8dd000-7fe6de9db000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de9db000-7fe6de9de000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6de9de000-7fe6deadc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deadc000-7fe6deade000 r-xp 00000000 fd:02 5242911                    /home/mycat/lib/libwrapper-linux-x86-64.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deade000-7fe6debde000 ---p 00002000 fd:02 5242911                    /home/mycat/lib/libwrapper-linux-x86-64.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6debde000-7fe6debdf000 rw-p 00002000 fd:02 5242911                    /home/mycat/lib/libwrapper-linux-x86-64.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6debdf000-7fe6debe0000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6debe0000-7fe6dece0000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dece0000-7fe6dece3000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dece3000-7fe6dede1000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dede1000-7fe6dede4000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dede4000-7fe6deee2000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deee2000-7fe6deee5000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deee5000-7fe6defe3000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6defe3000-7fe6defe8000 r--s 0009a000 fd:02 917844                     /home/jdk1.8.0_131/jre/lib/jsse.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6defe8000-7fe6defed000 r--s 00034000 fd:02 5242940                    /home/mycat/lib/leveldb-0.7.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6defed000-7fe6defef000 r--s 00008000 fd:02 5242939                    /home/mycat/lib/log4j-1.2-api-2.5.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6defef000-7fe6deff3000 r--s 0001f000 fd:02 5242938                    /home/mycat/lib/kryo-2.10.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deff3000-7fe6deff8000 r--s 0005a000 fd:02 5242937                    /home/mycat/lib/fastjson-1.2.12.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6deff8000-7fe6df000000 r--s 0005f000 fd:02 5242936                    /home/mycat/lib/mongo-java-driver-2.11.4.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df000000-7fe6df002000 r--s 0000a000 fd:02 5242935                    /home/mycat/lib/asm-4.0.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df002000-7fe6df00b000 r--s 0006f000 fd:02 5242934                    /home/mycat/lib/log4j-1.2.17.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df00b000-7fe6df00d000 r--s 00001000 fd:02 5242933                    /home/mycat/lib/leveldb-api-0.7.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df00d000-7fe6df039000 r--s 00208000 fd:02 5242932                    /home/mycat/lib/guava-19.0.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df039000-7fe6df05b000 r--s 001e6000 fd:02 5242931                    /home/mycat/lib/druid-1.0.26.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df05b000-7fe6df076000 r--s 0019e000 fd:02 5242968                    /home/mycat/lib/Mycat-server-1.6-RELEASE.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df076000-7fe6df07c000 r--s 0002b000 fd:02 5242929                    /home/mycat/lib/curator-framework-2.11.0.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df07c000-7fe6df084000 r--s 0006e000 fd:02 5242928                    /home/mycat/lib/sequoiadb-driver-1.12.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df084000-7fe6df09c000 r--s 00110000 fd:02 5242927                    /home/mycat/lib/netty-3.7.0.Final.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df09c000-7fe6df0a2000 r--s 00060000 fd:02 5242926                    /home/mycat/lib/mapdb-1.0.7.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df0a2000-7fe6df0ae000 r--s 000b6000 fd:02 5242922                    /home/mycat/lib/zookeeper-3.4.6.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df0ae000-7fe6df0c9000 r--s 0012e000 fd:02 5242921                    /home/mycat/lib/ehcache-core-2.6.11.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df0c9000-7fe6df0da000 r-xp 00000000 fd:02 917607                     /home/jdk1.8.0_131/jre/lib/amd64/libnio.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df0da000-7fe6df2d9000 ---p 00011000 fd:02 917607                     /home/jdk1.8.0_131/jre/lib/amd64/libnio.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df2d9000-7fe6df2da000 rw-p 00010000 fd:02 917607                     /home/jdk1.8.0_131/jre/lib/amd64/libnio.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df2da000-7fe6df5da000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df5da000-7fe6df5f0000 r-xp 00000000 fd:02 917621                     /home/jdk1.8.0_131/jre/lib/amd64/libnet.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df5f0000-7fe6df7f0000 ---p 00016000 fd:02 917621                     /home/jdk1.8.0_131/jre/lib/amd64/libnet.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df7f0000-7fe6df7f1000 rw-p 00016000 fd:02 917621                     /home/jdk1.8.0_131/jre/lib/amd64/libnet.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df7f1000-7fe6df7fa000 r-xp 00000000 fd:02 917598                     /home/jdk1.8.0_131/jre/lib/amd64/libmanagement.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df7fa000-7fe6df9f9000 ---p 00009000 fd:02 917598                     /home/jdk1.8.0_131/jre/lib/amd64/libmanagement.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df9f9000-7fe6df9fa000 rw-p 00008000 fd:02 917598                     /home/jdk1.8.0_131/jre/lib/amd64/libmanagement.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df9fa000-7fe6df9fd000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6df9fd000-7fe6dfafb000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfafb000-7fe6dfafe000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfafe000-7fe6dfbfc000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfbfc000-7fe6dfbff000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfbff000-7fe6dfcfd000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfcfd000-7fe6dfd00000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfd00000-7fe6dfdfe000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfdfe000-7fe6dfe01000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfe01000-7fe6dfeff000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dfeff000-7fe6dff02000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6dff02000-7fe6e0000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e0000000-7fe6e0021000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e0021000-7fe6e4000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e4001000-7fe6e4002000 r--s 00005000 fd:02 5242925                    /home/mycat/lib/log4j-slf4j-impl-2.5.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e4002000-7fe6e4004000 r--s 00009000 fd:02 5242924                    /home/mycat/lib/hamcrest-core-1.3.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e4004000-7fe6e400b000 r--s 0003e000 fd:02 5242923                    /home/mycat/lib/curator-recipes-2.11.0.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e400b000-7fe6e4023000 r--s 000f7000 fd:02 5242917                    /home/mycat/lib/log4j-core-2.5.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e4023000-7fe6e4026000 r--s 00011000 fd:02 5242916                    /home/mycat/lib/disruptor-3.3.4.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e4026000-7fe6e402d000 r--s 00049000 fd:02 5242915                    /home/mycat/lib/univocity-parsers-2.2.1.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e402d000-7fe6e403e000 r--s 00089000 fd:02 5242907                    /home/mycat/lib/joda-time-2.9.3.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e403e000-7fe6e404c000 r--s 0007f000 fd:02 5242906                    /home/mycat/lib/commons-collections-3.2.1.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e404c000-7fe6e404f000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e404f000-7fe6e414d000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e414d000-7fe6e414e000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e414e000-7fe6e5000000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e5000000-7fe6e62c0000 rwxp 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6e62c0000-7fe6f4000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f4000000-7fe6f46b3000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f46b3000-7fe6f8000000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f8001000-7fe6f8006000 r--s 00048000 fd:02 5242913                    /home/mycat/lib/dom4j-1.6.1.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f8006000-7fe6f800e000 r--s 00066000 fd:02 5242905                    /home/mycat/lib/velocity-1.7.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f800e000-7fe6f825f000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f825f000-7fe6f8438000 r--s 03d40000 fd:02 917643                     /home/jdk1.8.0_131/jre/lib/rt.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f8438000-7fe6f9581000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9581000-7fe6f9582000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9582000-7fe6f9682000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9682000-7fe6f9683000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9683000-7fe6f9783000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9783000-7fe6f9784000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9784000-7fe6f9884000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9884000-7fe6f9885000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9885000-7fe6f9edb000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6f9edb000-7fe6fa430000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fa430000-7fe6fa6db000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fa6db000-7fe6fa727000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fa727000-7fe6faa9c000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faa9c000-7fe6faab6000 r-xp 00000000 fd:02 917613                     /home/jdk1.8.0_131/jre/lib/amd64/libzip.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faab6000-7fe6facb6000 ---p 0001a000 fd:02 917613                     /home/jdk1.8.0_131/jre/lib/amd64/libzip.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6facb6000-7fe6facb7000 rw-p 0001a000 fd:02 917613                     /home/jdk1.8.0_131/jre/lib/amd64/libzip.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6facb7000-7fe6facc4000 r-xp 00000000 fd:00 2752541                    /lib64/libnss_files-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6facc4000-7fe6faec3000 ---p 0000d000 fd:00 2752541                    /lib64/libnss_files-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faec3000-7fe6faec4000 r--p 0000c000 fd:00 2752541                    /lib64/libnss_files-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faec4000-7fe6faec5000 rw-p 0000d000 fd:00 2752541                    /lib64/libnss_files-2.12.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faec5000-7fe6faec6000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faec6000-7fe6faec7000 r--s 00003000 fd:02 5242914                    /home/mycat/lib/reflectasm-1.03.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faec7000-7fe6faeca000 r--s 0001d000 fd:02 5242910                    /home/mycat/lib/mysql-binlog-connector-java-0.4.1.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faeca000-7fe6faecc000 r--s 00014000 fd:02 5242909                    /home/mycat/lib/jline-0.9.94.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faecc000-7fe6faecf000 r--s 00006000 fd:02 5242904                    /home/mycat/lib/jsr305-2.0.3.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faecf000-7fe6faed1000 r--s 00005000 fd:02 5242903                    /home/mycat/lib/slf4j-api-1.6.1.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faed1000-7fe6faed2000 r--s 0000c000 fd:02 5242902                    /home/mycat/lib/hamcrest-library-1.3.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faed2000-7fe6faed4000 r--s 00007000 fd:02 5242900                    /home/mycat/lib/objenesis-1.2.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faed4000-7fe6faed7000 r--s 00021000 fd:02 5242899                    /home/mycat/lib/log4j-api-2.5.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faed7000-7fe6faed9000 r--s 00010000 fd:02 5242897                    /home/mycat/lib/curator-client-2.11.0.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faed9000-7fe6faf04000 r-xp 00000000 fd:02 917605                     /home/jdk1.8.0_131/jre/lib/amd64/libjava.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6faf04000-7fe6fb103000 ---p 0002b000 fd:02 917605                     /home/jdk1.8.0_131/jre/lib/amd64/libjava.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb103000-7fe6fb105000 rw-p 0002a000 fd:02 917605                     /home/jdk1.8.0_131/jre/lib/amd64/libjava.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb105000-7fe6fb112000 r-xp 00000000 fd:02 917618                     /home/jdk1.8.0_131/jre/lib/amd64/libverify.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb112000-7fe6fb311000 ---p 0000d000 fd:02 917618                     /home/jdk1.8.0_131/jre/lib/amd64/libverify.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb311000-7fe6fb313000 rw-p 0000c000 fd:02 917618                     /home/jdk1.8.0_131/jre/lib/amd64/libverify.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb313000-7fe6fb316000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb316000-7fe6fb414000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fb414000-7fe6fc0e3000 r-xp 00000000 fd:02 917612                     /home/jdk1.8.0_131/jre/lib/amd64/server/libjvm.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc0e3000-7fe6fc2e3000 ---p 00ccf000 fd:02 917612                     /home/jdk1.8.0_131/jre/lib/amd64/server/libjvm.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc2e3000-7fe6fc3bc000 rw-p 00ccf000 fd:02 917612                     /home/jdk1.8.0_131/jre/lib/amd64/server/libjvm.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc3bc000-7fe6fc40a000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc40a000-7fe6fc41f000 r-xp 00000000 fd:02 918018                     /home/jdk1.8.0_131/lib/amd64/jli/libjli.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc41f000-7fe6fc61f000 ---p 00015000 fd:02 918018                     /home/jdk1.8.0_131/lib/amd64/jli/libjli.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc61f000-7fe6fc620000 rw-p 00015000 fd:02 918018                     /home/jdk1.8.0_131/lib/amd64/jli/libjli.so
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc620000-7fe6fc621000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc621000-7fe6fc623000 r--s 00000000 fd:02 5242896                    /home/mycat/lib/minlog-1.2.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc623000-7fe6fc628000 r--s 00041000 fd:02 5242895                    /home/mycat/lib/commons-lang-2.6.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc628000-7fe6fc62b000 r--s 00012000 fd:02 5242912                    /home/mycat/lib/wrapper.jar
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc62b000-7fe6fc633000 rw-s 00000000 fd:00 786463                     /tmp/hsperfdata_root/7660
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc633000-7fe6fc634000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc634000-7fe6fc635000 ---p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fe6fc635000-7fe6fc636000 rw-p 00000000 00:00 0 
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fff5c077000-7fff5c08d000 rw-p 00000000 00:00 0                          [stack]
INFO   | jvm 1    | 2017/09/08 16:50:16 | 7fff5c1ff000-7fff5c200000 r-xp 00000000 00:00 0                          [vdso]
INFO   | jvm 1    | 2017/09/08 16:50:16 | ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
ERROR  | wrapper  | 2017/09/08 16:50:31 | JVM exited unexpectedly.
STATUS | wrapper  | 2017/09/08 16:50:31 | JVM exited in response to signal UNKNOWN (6).
STATUS | wrapper  | 2017/09/08 16:50:35 | Launching a JVM...
INFO   | jvm 2    | 2017/09/08 16:50:36 | Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=64M; support was removed in 8.0
INFO   | jvm 2    | 2017/09/08 16:50:36 | Wrapper (Version 3.2.3) http://wrapper.tanukisoftware.org
INFO   | jvm 2    | 2017/09/08 16:50:36 |   Copyright 1999-2006 Tanuki Software, Inc.  All Rights Reserved.
INFO   | jvm 2    | 2017/09/08 16:50:36 |
```
下面是因为大批量数据查询而导致程序挂起的JVM重启：

```
ERROR  | wrapper  | 2017/09/08 16:57:15 | JVM appears hung: Timed out waiting for signal from JVM.
ERROR  | wrapper  | 2017/09/08 16:57:16 | JVM did not exit on request, terminated
INFO   | wrapper  | 2017/09/08 16:57:16 | JVM exited on its own while waiting to kill the application.
STATUS | wrapper  | 2017/09/08 16:57:16 | JVM exited in response to signal SIGKILL (9).
STATUS | wrapper  | 2017/09/08 16:57:20 | Launching a JVM...
INFO   | jvm 3    | 2017/09/08 16:57:20 | Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=64M; support was removed in 8.0
INFO   | jvm 3    | 2017/09/08 16:57:21 | Wrapper (Version 3.2.3) http://wrapper.tanukisoftware.org
INFO   | jvm 3    | 2017/09/08 16:57:21 |   Copyright 1999-2006 Tanuki Software, Inc.  All Rights Reserved.
INFO   | jvm 3    | 2017/09/08 16:57:21 |
```




## 解决方法


分别修改源码中的startup_nowrap.sh 和startup_nowrap.bat 支持1.8_x前后的JVM 调整。

**修改mycat\src\main\assembly\bin\startup_nowrap.bat：**




```
REM check JAVA_HOME & java
set "JAVA_CMD=%JAVA_HOME%/bin/java"
if "%JAVA_HOME%" == "" goto noJavaHome
if exist "%JAVA_HOME%\bin\java.exe" goto mainEntry
:noJavaHome
echo ---------------------------------------------------
echo WARN: JAVA_HOME environment variable is not set. 
echo ---------------------------------------------------
set "JAVA_CMD=java"
:mainEntry
REM set HOME_DIR
set "CURR_DIR=%cd%"
cd ..
set "MYCAT_HOME=%cd%"
cd %CURR_DIR%
#set JAVA_OPTS for JDK1.8_x PermGen is unsupported,handle and keep it for before versions.
"%JAVA_CMD%" -server -Xms1G -Xmx2G -XX:PermSize=64M  -XX:MaxPermSize=128M   -XX:MetaspaceSize=64M -XX:MaxMetaspaceSize=128M -XX:+AggressiveOpts -XX:MaxDirectMemorySize=1G -DMYCAT_HOME=%MYCAT_HOME% -cp "..\conf;..\lib\*" io.mycat.MycatStartup
```




**修改mycat\src\main\assembly\bin\startup_nowrap.sh：**




```
#!/bin/sh

#check JAVA_HOME & java
noJavaHome=false
if [ -z "$JAVA_HOME" ] ; then
    noJavaHome=true
fi
if [ ! -e "$JAVA_HOME/bin/java" ] ; then
    noJavaHome=true
fi
if $noJavaHome ; then
    echo
    echo "Error: JAVA_HOME environment variable is not set."
    echo
    exit 1
fi
#==============================================================================
#set JAVA_OPTS for JDK1.8_x PermGen is unsupported,handle and keep it for before versions.
JAVA_OPTS="-server -Xms2G -Xmx2G -XX:PermSize=64M  -XX:MaxPermSize=128M   -XX:MetaspaceSize=64M -XX:MaxMetaspaceSize=128M  -XX:+AggressiveOpts -XX:MaxDirectMemorySize=2G"
#JAVA_OPTS="-server -Xms4G -Xmx4G -XX:MaxPermSize=64M  -XX:+AggressiveOpts -XX:MaxDirectMemorySize=6G"
#performance Options
#JAVA_OPTS="$JAVA_OPTS -Xss256k"
#JAVA_OPTS="$JAVA_OPTS -XX:+AggressiveOpts"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseBiasedLocking"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseFastAccessorMethods"
#JAVA_OPTS="$JAVA_OPTS -XX:+DisableExplicitGC"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseParNewGC"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseConcMarkSweepGC"
#JAVA_OPTS="$JAVA_OPTS -XX:+CMSParallelRemarkEnabled"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseCMSCompactAtFullCollection"
#JAVA_OPTS="$JAVA_OPTS -XX:+UseCMSInitiatingOccupancyOnly"
#JAVA_OPTS="$JAVA_OPTS -XX:CMSInitiatingOccupancyFraction=75"
#JAVA_OPTS="$JAVA_OPTS -XX:CMSInitiatingOccupancyFraction=75"
#GC Log Options
#JAVA_OPTS="$JAVA_OPTS -XX:+PrintGCApplicationStoppedTime"
#JAVA_OPTS="$JAVA_OPTS -XX:+PrintGCTimeStamps"
#JAVA_OPTS="$JAVA_OPTS -XX:+PrintGCDetails"
#debug Options
#JAVA_OPTS="$JAVA_OPTS -Xdebug -Xrunjdwp:transport=dt_socket,address=8065,server=y,suspend=n"
#==============================================================================

#set HOME
CURR_DIR=`pwd`
cd `dirname "$0"`/..
MYCAT_HOME=`pwd`
cd $CURR_DIR
if [ -z "$MYCAT_HOME" ] ; then
    echo
    echo "Error: MYCAT_HOME environment variable is not defined correctly."
    echo
    exit 1
fi
#==============================================================================

#set CLASSPATH
MYCAT_CLASSPATH="$MYCAT_HOME/conf:$MYCAT_HOME/lib/classes"
for i in "$MYCAT_HOME"/lib/*.jar
do
    MYCAT_CLASSPATH="$MYCAT_CLASSPATH:$i"
done
#==============================================================================

#startup Server
RUN_CMD="\"$JAVA_HOME/bin/java\""
RUN_CMD="$RUN_CMD -DMYCAT_HOME=\"$MYCAT_HOME\""
RUN_CMD="$RUN_CMD -classpath \"$MYCAT_CLASSPATH\""
RUN_CMD="$RUN_CMD $JAVA_OPTS"
RUN_CMD="$RUN_CMD io.mycat.MycatStartup  $@"
RUN_CMD="$RUN_CMD >> \"$MYCAT_HOME/logs/console.log\" 2>&1 &"
echo $RUN_CMD
eval $RUN_CMD
#==============================================================================
```





## 参考文章

[http://blog.csdn.net/jiangguilong2000/article/details/47975457](http://blog.csdn.net/jiangguilong2000/article/details/47975457)

[http://www.mycat.io/document/Mycat_V1.6.0.pdf (JVM性能调优)](http://www.mycat.io/document/Mycat_V1.6.0.pdf)


[http://ifeve.com/java-permgen-removed/](http://ifeve.com/java-permgen-removed/)




