# 禁用android studio自身的ndk编译disable automatic ndk-build call - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月08日 09:58:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1841
1，让studio不自动编译jni文件，而是我们手动通过ndk-build编译
打开工程下面的app文件夹，
找到build.gradle
添加如下：
 defaultConfig {
        applicationId "com.elesos.jnidemux"
        minSdkVersion 15
        targetSdkVersion 21
        versionCode 1
        versionName "1.0"
         ndk {
           moduleName "codec"    //libcodec.so
         }
    }
    sourceSets.main {
        jniLibs.srcDir 'src/main/libs'         //set .so files location to libs
        jni.srcDirs = []                       //disable automatic ndk-build call
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
