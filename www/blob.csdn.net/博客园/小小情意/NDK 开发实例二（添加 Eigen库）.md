
# NDK 开发实例二（添加 Eigen库） - 小小情意 - 博客园






# [NDK 开发实例二（添加 Eigen库）](https://www.cnblogs.com/xiaoxiaoqingyi/p/6558873.html)
上一篇，我已经阐述了如何创建一个简单的NDK实例：[NDK 开发实例一（Android.mk环境配置下）](http://www.cnblogs.com/xiaoxiaoqingyi/p/6524165.html)
在上一篇的基础上，我们来添加Eigen库，然后做一个简单实例。
Eigen是一个高层次的C ++库，有效支持线性代数，矩阵和矢量运算，数值分析及其相关的算法。下面我们介绍一下
如何添加Eigen库。
1、首先在Eigen官网（[http://eigen.tuxfamily.org](http://eigen.tuxfamily.org)）下载最新的zip包，解压，获取Eigen源码库：
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170316123236073-1945062898.png)
2、把Eigen文件夹的源码添加到 项目jni目录下。因为Eigen 是C++库，所以注意C++文件的后缀名为 .cpp。
还有一些JNI的方法与C语言也存在差异。
Eigen库用例如下：
\#include <jni.h>\#include<string>\#include<Eigen/Dense>\#include<iostream>usingnamespaceEigen;extern"C"jstring
 Java_com_magicing_eigenndk_NDKUtils_invokeCmethod(
        JNIEnv*env,
        jobject/*this*/) {
    MatrixXd m(2,2);
    m(0,0) =3;
    m(1,0) =2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);//std::cout << "Here is the matrix m:\n" << m << std::endl;VectorXd v(2);
    v(0) =4;
    v(1) = v(0) -1;//std::cout << "Here is the vector v:\n" << v << std::endl;std::stringhello ="Hello Eigen v(1)=";charout[1024];
     sprintf(out,"%s%f",hello.c_str(),v(1));returnenv->NewStringUTF(out);
  }
// 这是Java public native String invokeCmethod(); 对于的C++ 方法。
3、在Android.mk 的配置如下：
LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := NDKUtils
LOCAL_SRC_FILES := com_magicing_eigenndk_NDKUtils.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Eigen
include $(BUILD_SHARED_LIBRARY)
4、在Application.mk 的配置如下：
APP_PLATFORM := android-23APP_ABI :=armeabi
APP_STL := stlport_static
5、ndk-build后,编译出相应的.so库，运行，输出相应处理的 矩阵 信息。
小技巧：
在 app/build.grade 配置如下后，就不用每次都去 ndk-build,编译 .so库了
import org.apache.tools.ant.taskdefs.condition.Os
apply plugin:'com.android.application'android {
    compileSdkVersion25buildToolsVersion'25.0.0'defaultConfig {
        applicationId"com.magicing.eigenndk"minSdkVersion15targetSdkVersion25versionCode1versionName"1.0"testInstrumentationRunner"android.support.test.runner.AndroidJUnitRunner"}
    buildTypes {
        release {
            minifyEnabledfalseproguardFiles getDefaultProguardFile('proguard-android.txt'),'proguard-rules.pro'}
    }
    sourceSets{
        main{
            jni.srcDirs= []//disable automatic ndk-build call, which ignore our Android.mkjniLibs.srcDir'src/main/jni/libs'}
    }
    task ndkBuild(type: Exec) {
        File workingDir=file('src/main/jni')
        println workingDir.absolutePath
        commandLine getNdkBuildCmd(),'NDK_PROJECT_PATH='+workingDir.absolutePath,'APP_BUILD_SCRIPT='+workingDir.absolutePath+'/Android.mk','NDK_APPLICATION_MK='+workingDir.absolutePath+'/Application.mk'}
    tasks.withType(JavaCompile) {
        compileTask->compileTask.dependsOn ndkBuild
    }
}//获取NDK目录路径def getNdkDir() {if(System.env.ANDROID_NDK_ROOT !=null)returnSystem.env.ANDROID_NDK_ROOT
    Properties properties=newProperties()
    properties.load(project.rootProject.file('local.properties').newDataInputStream())
    def ndkdir= properties.getProperty('ndk.dir',null)if(ndkdir ==null)thrownewGradleException("NDK location not found. Define location with ndk.dir in the local.properties file or with an ANDROID_NDK_ROOT environment variable.")returnndkdir
}//根据不同系统获取ndk-build脚本def getNdkBuildCmd() {
    def ndkbuild= getNdkDir() +"/ndk-build"if(Os.isFamily(Os.FAMILY_WINDOWS))
        ndkbuild+=".cmd"returnndkbuild
}
dependencies {
    compile fileTree(dir:'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group:'com.android.support', module:'support-annotations'})
    compile'com.android.support:appcompat-v7:25.1.1'testCompile'junit:junit:4.12'}
[源代码用例下载链接](http://files.cnblogs.com/files/xiaoxiaoqingyi/EigenNDK%28%E5%BC%80%E5%8F%91%E7%AC%94%E8%AE%B0%E4%BA%8C%29.rar)







