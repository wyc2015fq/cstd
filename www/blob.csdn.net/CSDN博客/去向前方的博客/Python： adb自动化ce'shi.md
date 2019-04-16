# Python： adb自动化ce'shi - 去向前方的博客 - CSDN博客





2018年07月16日 10:47:17[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：150








在python中使用adb命令，可以导入os模块,手机开启开发者模式，然后连接上电脑。 

注意使用的python版本是2.7.

## 1 简单的adb命令。

如：os.system(‘adb version’)

## 2 稍微复杂的adb命令。

如：os.system(‘adb shell “dumpsys activity | grep “mFocusedActivity”“’)

## 3 希望将adb读出的内容保存。

如：out = os.popen(‘adb shell “dumpsys activity | grep “mFocusedActivity”“’).read()

总结一下：os.system不支持读取， os.popen支持读取。

下面有一个转来的脚本，学习一下：

```python
import os
    import re


    os.system('adb version')
    os.system('adb devices') #os.system是不支持读取操作的
    out = os.popen('adb shell "dumpsys activity | grep "mFocusedActivity""').read() #os.popen支持读取操作
    print(out)

    #下面的代码是获取当前窗口的component参数
    def getFocusedPackageAndActivity():

            pattern = re.compile(r"[a-zA-Z0-9\.]+/[a-zA-Z0-9\.]+") #这里使用了正则表达式，对输出的内容做了限制，只会显示类似"com.mediatek.factorymode/com.mediatek.factorymode.FactoryMode"的字符串
            out = os.popen("adb shell dumpsys window windows | findstr \/ | findstr name=").read() #window下使用findstr
            list = pattern.findall(out)
            component = list[0] #输出列表中的第一条字符串

            return component
    print(getFocusedPackageAndActivity())
```

python交互

使用subprocess 

使设备具有部分Linux命令 

python脚本：下载busybox，并且放在与脚本相同的目录下
```python
# coding:utf-8
# 这个脚本用于使环境拥有busybox的环境
import subprocess

import os
print "开始push文件"
os.system(" adb push busybox /system/xbin/")
print "开始root"
os.system("adb root")
print "开始remote"
os.system("adb remount")

#这里面的命令全部都会在Linux的环境下执行，最后需要退出。
cmds = [
    "chmod  755 /system/xbin/busybox",
    "cd /system/xbin/",
    "busybox --install .",
    "awk",
    "ps |grep com.txznet.music|awk '{print $2}' | head -n 1",
    "exit",#这是是非常关键的，退出
]
print "开始安装"
pipe = subprocess.Popen("adb shell", stdin=subprocess.PIPE, stdout=subprocess.PIPE)
code = pipe.communicate("\n".join(cmds) + "\n");
print code
print "安装结束"

杀掉相应的进程

# coding:utf-8
# 这个脚本是用来杀掉music的进程的
import os
import string
import subprocess
def kill(result):
    print result.strip()
    code = os.system(" adb shell kill " + result.strip())
    print code

processname = os.system('''adb shell ps|grep com.txznet.music|awk '{print$2","}''''')


# print processname
# result="["+str(processname)+"]"
# print result
# for processID in result:
#     print processID.strip()
#     code=os.system(" adb shell kill "+processID.strip())
#     print code

pipe = subprocess.Popen("adb shell ps|grep com.txznet.music|awk '{print$2}'", stdin=subprocess.PIPE,
                        stdout=subprocess.PIPE)

# result=pipe.communicate("ps|grep com.txznet.music|awk '{print$2","}'")
# print result
result = pipe.stdout.readline()
result1 = pipe.stdout.readline()
result2 = pipe.stdout.readline()
print result
kill(result)
print result1
kill(result1)
print result2
kill(result2)
```




