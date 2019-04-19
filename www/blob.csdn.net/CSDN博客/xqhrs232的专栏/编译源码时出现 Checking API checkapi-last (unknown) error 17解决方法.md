# 编译源码时出现 Checking API: checkapi-last (unknown): error 17解决方法 - xqhrs232的专栏 - CSDN博客
2012年04月06日 16:12:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4797
原文地址::http://www.eoeandroid.com/thread-84145-1-1.html
相关网帖
1.关于android中的check api----http://blog.sina.com.cn/s/blog_55b1b0d50100jlge.html
2.在Ubuntu上编译CyanogenMod 2.3 generic----http://www.linuxidc.com/Linux/2012-01/50984.htm
log：
Copy: apicheck (out/host/linux-x86/obj/EXECUTABLES/apicheck_intermediates/apicheck)
Install: out/host/linux-x86/bin/apicheck
Checking API: checkapi-last
(unknown): error 17: Field android.app.Notification.FLAG_SHOW_LIGHTS has changed value from 1 to 0
******************************
You have tried to change the API from what has been previously released in
an SDK.  Please fix the errors listed above.
******************************
make[1]: *** [out/target/common/obj/PACKAGING/checkapi-last-timestamp] Error 38
make[1]: Leaving directory `/home1/work/android_eRD4.2/src'
make: *** [compile] Error 2
//----------------------------------------------------
这是因为修改了framework层里的东西，而且在全编译make源码之前，用了make update-api命令，导致 源码之前的api 与你自己新的源码里的值不一样导致。
解决办法：
自己找到了在framework/base/api 下有n（n取决于你源码的版本，如果你源码的版本是2.1 ，那么就 有分别以 1~ 7命名的xml，就是api level）个xml文档，分别对应android api 的不同版本。   在这些xml中，把你在framework层中修改的地方对应进去。 如我这个错误的地方，在8.xml 和 current.xml 中找到“FLAG_SHOW_LIGHTS”，把里面的value 值改成和你代码中值一致就可以了。
//-----------------------------------------------------
是在make全编译之前忘记make update-api 导致的这个问题。解决办法有2：1、make clean--> make update-api --make ； 2、要么手动添加相应版本的api值，使编译通过。
