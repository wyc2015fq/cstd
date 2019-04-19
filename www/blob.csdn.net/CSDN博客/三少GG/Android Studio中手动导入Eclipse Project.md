# Android Studio中手动导入Eclipse Project - 三少GG - CSDN博客
2014年09月07日 19:24:41[三少GG](https://me.csdn.net/scut1135)阅读数：851
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
Android Studio中手动导入Eclipse Project
RT，这应该是很多朋友刚从Eclipse转到[Android](http://www.2cto.com/kf/yidong/Android/) Studio后遇到最大的一个问题，首先我们需要重新认识AS里面的目录结构，在我前一篇帖子里面也有提到（Android Studio中的Project相当于Eclipse中的Workspace，Module则相当于Eclipse中的Project）。
所以我们手动导入Project，其实就是导入AS里面的Module。主要有以下几个步骤：
1.复制build.gradle到需要导入的项目中
2.复制你需要导入的项目至AS Project根目录文件夹下（即存在gradlew, gradlew.bat, .gradle的那个文件夹）
3.修改AS Project中的settings.gradle，添加include，告诉AS我们的Project里面需要包含这个Module（例如include ':SlidingMenuLibrary'）
4.Rebuild Project，会为项目自动生成.iml文件（iml文件是AS识别项目的配置文件，跟Eclipse里面的.project文件作用类似）
下面贴出build.gradle主要内容：
[?](http://www.2cto.com/kf/201408/329821.html#)
```
```java
apply
 plugin:
```
```java
'com.android.application'
```
```java
dependencies
 {
```
```java
```
```java
compile
 fileTree(dir:
```
```java
'libs'
```
```java
,
 include:
```
```java
'*.jar'
```
```java
)
```
```java
}
```
```java
android
 {
```
```java
```
```java
compileSdkVersion
```
```java
17
```
```java
```
```java
buildToolsVersion
```
```java
"20.0.0"
```
```java
```
```java
sourceSets
 {
```
```java
```
```java
main
 {
```
```java
```
```java
manifest.srcFile
```
```java
'AndroidManifest.xml'
```
```java
```
```java
java.srcDirs
 = [
```
```java
'src'
```
```java
]
```
```java
```
```java
resources.srcDirs
 = [
```
```java
'src'
```
```java
]
```
```java
```
```java
aidl.srcDirs
 = [
```
```java
'src'
```
```java
]
```
```java
```
```java
renderscript.srcDirs
 = [
```
```java
'src'
```
```java
]
```
```java
```
```java
res.srcDirs
 = [
```
```java
'res'
```
```java
]
```
```java
```
```java
assets.srcDirs
 = [
```
```java
'assets'
```
```java
]
```
```java
```
```java
}
```
```java
```
```java
//
 Move the tests to tests/java, tests/res, etc...
```
```java
```
```java
instrumentTest.setRoot(
```
```java
'tests'
```
```java
)
```
```java
```
```java
//
 Move the build types to build-types/<type>
```
```java
```
```java
//
 For instance, build-types/debug/java, build-types/debug/AndroidManifest.xml, ...
```
```java
```
```java
//
 This moves them out of them default location under src/<type>/... which would
```
```java
```
```java
//
 conflict with src/ being used by the main source set.
```
```java
```
```java
//
 Adding new build types or product flavors should be accompanied
```
```java
```
```java
//
 by a similar customization.
```
```java
```
```java
debug.setRoot(
```
```java
'build-types/debug'
```
```java
)
```
```java
```
```java
release.setRoot(
```
```java
'build-types/release'
```
```java
)
```
```java
```
```java
}
```
```java
}</type></type>
```
```
其实上面这个文件就是用Eclipse导出的，大家可以放心的直接复制使用。还有需要注意以下几点：
1.上面提供的是App的gradle文件，如果是Library项目，则需要修改apply plugin: 'com.android.application' 为 apply plugin: 'com.android.library'即可
2. compileSdkVersion 和 buildToolsVersion，需要根据本地的SDK版本具体修改，打开SDK Manager看下就行了
3. sourceSets main里面指定了源代码的目录位置，因为AS默认的代码结构与Eclipse的是不一样的
4. dependencies里面指定了依赖库，compile fileTree(dir: 'libs', include: ['*.jar'])编译libs目录下所有的.jar库。如果依赖某些库项目，则可以添加：compile project(':Cordova')
结束语：其实，最新版本的AS已经支持直接导入Module了，但有一个问题，它在导入的时候，会Copy一份你的项目（相当于重新生成一份），然后导入后目录结构就变成了AS的目录结构，如果想保持Eclipse目录结构，还是使用上面的方法吧，嘿嘿。
