@echo off

set path=C:\android-ndk-r18b;%path%
::adb shell "cat /proc/cpuinfo"
::ndk-build clean
call ndk-build
::call ndk-build
::goto end
adb push ./libs/arm64-v8a/helloworld /data/local/tmp
adb shell "chmod 777 /data/local/tmp/helloworld"
::adb shell "rm /data/local/tmp/helloworld"
adb shell "/data/local/tmp/helloworld"

:end

