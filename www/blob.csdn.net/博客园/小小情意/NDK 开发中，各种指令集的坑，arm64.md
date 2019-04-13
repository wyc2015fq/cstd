
# NDK 开发中，各种指令集的坑，arm64 - 小小情意 - 博客园






# [NDK 开发中，各种指令集的坑，arm64](https://www.cnblogs.com/xiaoxiaoqingyi/p/7093502.html)
最近在NDK开发中遇到了一个奇怪的问题，希望记录下，可以帮到大家：
我编译了一些 .so 动态库，只编译了armeabi-v7a、armeabi 指令集，其它指令集编译不了，具体原因还没查出。因为同时也调用了第三方的.so 动态库，而第三方提供7个指令集，分别为：arm64-v8a、armeabi、armeabi-v7a、mips、mips64、x86、x86_64。 所以，我只能把刚编译的动态库填充到 armeabi、armeabi-v7a 两个指令集目录。
在JNI 调用的时候，有些手机调用成功，有些手机不行，成功的有：
（vivo Y51A） Android 系统为：5.1 ，支持指令集：armeabi-v7a、armeabi
（oppo r9s） Android 系统为：5.1 ，支持指令集：arm64-v8a、armeabi-v7a、armeabi
（魅族 note2） Android 系统为：5.1 ，支持指令集：arm64-v8a、armeabi-v7a、armeabi
不成功的手机：
（华为 荣耀6） Android 系统为：7.0 ，支持指令集：arm64-v8a、armeabi-v7a、armeabi
（华为 mate9） Android 系统为：7.0 ，支持指令集：arm64-v8a、armeabi-v7a、armeabi
你可以通过 以下代码获取你手机支持的指令集：
String[] abis =newString[]{}; abis = Build.SUPPORTED_ABIS;

一般64位芯片的手机都会兼容 32位的库，只是运行时性能慢一点。如华为 mate9 的芯片就是 64位的，它首先会 查询 字符数组 abis 支持指令集 的第一个元素：arm64-v8a，如果这个目录没有相应的 库，就会到第二个元素目录搜索， 以此类推。
我不确定是因为华为手机的问题， 还是 Android 7.0 系统的问题：当你在 jniLibs 目录下建立了 arm64-v8a，程序就会直接在 arm64-v8a 目录下查找，如果查找不到，就会直接报错，而不会去其它 指令集目录查找，错误提示为：
nativeLibraryDirectories=[/data/app/com.lukouapp-1/lib/arm64, /vendor/lib64, /system
而上面列举成功的三款手机就会主动搜索 其它指令集目录下的库。这令我很疑惑，那我就尝试把
除 armeabi-v7a、armeabi 目录外的其它目录都删了，然后运行。 还是不行，各种尝试，都没有成功，后面我在 stackoverflow 找到一个解决方案，链接如下：
[https://stackoverflow.com/questions/30474073/unsatisfiedlinkerror-nativelibrarydirectories-vendor-lib64-system-lib64](https://stackoverflow.com/questions/30474073/unsatisfiedlinkerror-nativelibrarydirectories-vendor-lib64-system-lib64)
按照stackoverflow 的解决方案是：
找到apk 安装文件，然后用rar 之类的解压工具打开：
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170629111449883-233344759.png)
lib 目录，就是编译后的静态或者动态库目录，然后再进去看看：
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170629111555258-185517909.png)

果然， lib 目录下 除 armeabi-v7a、armeabi 目录外，还有其它目录，我在 JniLibs目录下 删除了其它目录，根本没有起到作用，我觉得原因可能是之前 build 时候缓存了 这些目录，所以不管我怎么在 jniLibs 目录下删除其它指令集目录都没用。 你可以尝试在 Android Studio 的菜单 Build =》 clean Project，然后再运行项目。
其次也可以 在 build.gradle 文件下 添加如下配置：
android {
    splits {
        abi {
            enabletruereset()
            include'armeabi-v7a', 'armeabi'}
    }
}
该配置是 让再构建build的时候只 加入armeabi-v7a', 'armeabi' 两种指令集。最后，大功告成，以上手机都运行通过。目前大多数第三方库 都已经提供 arm64-v8a 指令集， 所以最佳方案还是尽量去编译 arm64-v8a 指令集。
tips：华为手机在系统方面走得比较前，更新迭代快，对权限以及其它方面都比较多特殊的限制，对我们开发者来说是一种尝试，也是一种挑战，希望开发者不要忽视了它，毕竟它在大陆市场用户率不低。






