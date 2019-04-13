
# adb shell logcat 输出对应APK的日志 - 嵌入式Linux - CSDN博客

2014年12月22日 12:42:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4425



```python
#!/bin/bash
packageName=$1
if [ -z "$packageName" ];then
echo "please input the packageName"
packageName=com.android.settings
else
echo "input ok"
fi
pid= adb shell ps | grep $packageName | awk '{print $2}'
echo $pid
adb shell logcat -c && adb shell logcat | grep --color=auto '$pid'
```

1、可以输出echo $pid的值
2、可是最后一行写在脚本里面运行的时候就是不成功，要是单独打印出来就可以。还不知道什么原因。

然后单独写成下面的又是可以的。


```python
#!/bin/bash
echo "www"
adb shell logcat -c && adb shell logcat | grep --color=auto $1
```

```python
weiqifa@ubuntu:~/logcat$ ./test2.sh 1850
www
W/System.err( 1850): Removed 2131231252
W/System.err( 1850): Removed 2131231261
D/ActivityManager( 1850): monkey mode:false
D/ActivityManager( 1850): monkey running:false
I/WifiEnabler( 1850): action == android.net.wifi.WIFI_STATE_CHANGED
I/WifiEnabler( 1850): state ==1
I/WifiEnabler( 1850): action == android.net.wifi.supplicant.STATE_CHANGE
I/WifiEnabler( 1850): action == android.net.wifi.STATE_CHANGE
D/dalvikvm( 1850): GC_CONCURRENT freed 111K, 4% free 8680K/9031K, paused 11ms+4ms, total 37ms
W/System.err( 1850): Removed 2131231252
```

所以现在我就只能先运行adb shell ps ,然后再运行我的那个shell. 才可以正常打印出的我的那个log.
具体是什么原因还是不知道，要是有谁知道的请告诉我，我还怀疑是不是shell版本的原因。

睡觉！



