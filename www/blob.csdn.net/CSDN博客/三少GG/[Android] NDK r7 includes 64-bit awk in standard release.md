# [Android] NDK r7 includes 64-bit awk in standard release - 三少GG - CSDN博客
2011年12月31日 21:39:43[三少GG](https://me.csdn.net/scut1135)阅读数：1372标签：[android																[ubuntu																[google																[comments																[file																[go](https://so.csdn.net/so/search/s.do?q=go&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=comments&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
## [[Android] NDK r7 includes 64-bit awk in standard release](http://blog.pona.tw/2011/11/18/android-ndk-r7-includes-64-bit-awk-in-standard-release/)
Posted by [Deren](http://blog.pona.tw/author/admin/)on 2011年11月18日 [Leave a comment](http://blog.pona.tw/2011/11/18/android-ndk-r7-includes-64-bit-awk-in-standard-release/#respond) (1)[Go
 to comments](http://blog.pona.tw/2011/11/18/android-ndk-r7-includes-64-bit-awk-in-standard-release/#comments)
小弟我第一次抓到 Google 的東西有問題，真是非常的感動  （？
其實這也不見得是錯誤，說不定是 Google 故意要把 64-bits 東西當做主流  ＠＠
但我的機器就是只有32而已，你硬給我64是怎樣 ＝ ＝
這問題簡單的說，就是 NDK r7 裡面的 awk 執行檔是 64 bits 所使用的，所以在一般的 32 bits 機器上會完全無法使用，必須要把它換掉才行，大概的錯誤訊息如下…….
```
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/samples/hello-jni/jni
```
```bash
$ ndk-build
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin/awk
```
```bash
: 1:ELF: not found
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin/awk
```
```bash
: 4: Syntax error: word unexpected (expecting
```
```bash
")"
```
```bash
)
```
```bash
Android NDK: Host
```
```bash
'awk'
```
```bash
tool is outdated. Please define HOST_AWK to point to Gawk or Nawk !
```
```bash
/opt/android-ndk-r7/build/core/init
```
```bash
.mk:258: *** Android NDK: Aborting.    .  Stop.
```
```
到這個 awk 所在的檔案夾裡面去，我們檢查一下是不是檔案有問題……
```
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin
```
```bash
$
```
```bash
file
```
```bash
*
```
```bash
awk
```
```bash
:     ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs),
```
```bash
for
```
```bash
GNU
```
```bash
/Linux
```
```bash
2.6.15, stripped
```
```bash
make
```
```bash
:    ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs),
```
```bash
for
```
```bash
GNU
```
```bash
/Linux
```
```bash
2.6.8, stripped
```
```bash
sed
```
```bash
:     ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs),
```
```bash
for
```
```bash
GNU
```
```bash
/Linux
```
```bash
2.6.8, stripped
```
```
看吧看吧  ＝ ＝ ，這個 awk 居然是 64 位元的版本………我們來給他修正一下！
```
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin
```
```bash
$
```
```bash
m**v**
```
**```bashawk``````bashawk``````bash-x64```**
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin
```
```bash
$
```
```bash
echo
```
```bash
`
```
```bash
type
```
```bash
awk
```
```bash
`
```
```bash
awk
```
```bash
is
```
```bash
/usr/bin/awk
```
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/prebuilt/linux-x86/bin
```
```bash
$
```
**```bashln``````bash-s``````bash/usr/bin/awk``````bashawk```**
```
這樣應該就沒問題了，讓我們回到原來的地方編譯程式……….
```
```bash
deren@ubuntu1104:
```
```bash
/opt/android-ndk-r7/samples/hello-jni/jni
```
```bash
$ ndk-build
```
```bash
Gdbserver      : [arm-linux-androideabi-4.4.3] libs
```
```bash
/armeabi/gdbserver
```
```bash
Gdbsetup       : libs
```
```bash
/armeabi/gdb
```
```bash
.setup
```
```bash
Compile thumb  : hello-jni <= hello-jni.c
```
```bash
SharedLibrary  : libhello-jni.so
```
```bash
Install        : libhello-jni.so => libs
```
```bash
/armeabi/libhello-jni
```
```bash
.so
```
```
大功告成，收工！
