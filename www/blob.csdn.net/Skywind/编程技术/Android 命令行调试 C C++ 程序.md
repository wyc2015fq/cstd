# Skywind Inside » Android 命令行调试 C/C++  程序
## Android 命令行调试 C/C++  程序
August 25th, 2015[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
传统方式调试 NDK 开发的程序比较麻烦，先要编译成 JNI，又要导出 java接口，还要再写一个 java 工程，改一个地方又要连续改几处，这样效率是很低的。最频繁使用的关键工作路径（编译/调试环节）如果能极致简化，那么可以带来开发效率的成倍提升。其实安卓官方是提供了命令行调试方法的，将你需要调试的 C代码用 NDK直接编译成可执行，然后到设备上执行：
使用 NDK 导出独立工具链，方便以后使用，在 cygwin 下面，将 $NDK 环境变量代表的路径设置好，然后：
> 
cd $NDK      
chmod -R 755 *       
build/tools/make-standalone-toolchain.sh –ndk-dir=$NDK –platform=android-9 –arch=arm –install-dir=/…../path-to-android-9
这样就导出了一套针对 API9 的独立工具链（包含 gcc, ld, ndk必要文件），以后方便使用，比如导出到 d:\android-9下面，那么以后可以跳过 cygwin，直接编译我们的 Hello World：
> 
d:\android-9\bin\arm-linux-androideabi-gcc.exe hello.c –o hello
于是你可以在命令行下直接开发 Android 的非 GUI 应用程序了。
调试也很简单，用 adb push 上传到 /data/local/tmp 下面，并且设置可执行模式为 755：
> 
adb push hello /data/local/tmp/hello      
adb shell chmod 755 /data/local/tmp/hello
运行就是直接：
> 
adb shell /data/local/tmp/hello
不要传到其他目录，比如 /sdcard，这些目录 mount时有 NOEXEC 权限，不能给文件增加可执行权限，而 /data/local/tmp 就是留给大家调试命令行用的，并且不需要 root 权限。
可以编写一些脚本，每次编译好自动上传，配置到你的 Editplus/Vim/Npp 中，一键编译上传，一键运行。比起以前调试下 C代码还需要写一大堆 jni 和 java 的方式，效率高极了。
