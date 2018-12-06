@echo off

set path=C:\android-ndk-r18b;%path%
::adb shell "cat /proc/cpuinfo"
::ndk-build clean
::echo #include "cnn4\lmdb\lmdb.inl">lmdb.c
::del lmdb.c
call ndk-build
::call ndk-build
::goto end
adb shell "rm /data/local/tmp/helloworld"
::adb push "E:/OCR_Line/model/densenet-no-blstm/model.json" /data/local/tmp
adb push ./libs/arm64-v8a/helloworld /data/local/tmp
adb shell "chmod 777 /data/local/tmp/helloworld"
adb shell "ls -l /data/local/tmp"
adb shell "/data/local/tmp/helloworld"

:end

