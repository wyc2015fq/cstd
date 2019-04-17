# Ubuntu15.10的64位版本启动eclipse+adt时的若干问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月01日 09:24:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2027
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)









1、问题一：在Ubuntu15.10的64位版本上下载adt bundle，启动elcipse后提示connot run program:/sdk/platform_tools/adb:error=2，没有那个文件或目录； 

     经分析，可能是adb为32位文件故而无法在64位环境下启动；

     解决：sudo apt-get install lib32z1    安装32位运行库




2、问题二：在Ubuntu15.10的64位版本上下载adt bundle，
- aapt: error while loading shared libraries: libstdc++.so.6: cannot open shared object file: No such file or directory



    解决：sudo apt-get installlib32stdc++6
   安装32位运行库




3、问题三：创建AVD时提示No CPU/ABI system image
 available for this target

    解决：在Android
 SDK Manager 里下载对应版本的 ARM EABI v7a System Image

4、问题四：eclipse安装android sdk后工具栏没有图标

    解决：选择'Window>Customize Perspective...>Commands'，并在'Available
 command groups'中勾选'android'相关的几个选项




5、问题五：SDK Manager镜像地址

`mirrors.neusoft.edu.cn` 端口：80



