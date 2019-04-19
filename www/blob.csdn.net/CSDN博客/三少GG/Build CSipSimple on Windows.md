# Build CSipSimple on Windows - 三少GG - CSDN博客
2013年01月21日 23:52:26[三少GG](https://me.csdn.net/scut1135)阅读数：4366
[http://stackoverflow.com/questions/11846236/sipdroid-another-incoming-call-is-not-display-while-one-is-continue](http://stackoverflow.com/questions/11846236/sipdroid-another-incoming-call-is-not-display-while-one-is-continue)
||[Sipdroid](http://code.google.com/p/sipdroid/) that I what started with but after researching on it and from developers reviews I found that application has some missing functionality like multiple call and transferring calls etc...I don't know but may who is **expert in sip stack or have time to do** can implement missing functionality but I am neither one of them so my desire to get an application that have all this functionality and by modifying it I can integrate into my application and [csipsimple](http://code.google.com/p/csipsimple/) is the answer.However its not easy job to build csipsimple into windows environment but there is hack how to do it. so carefully read its[wiki](http://code.google.com/p/csipsimple/wiki/HowToBuild#For_Windows) about building project into window environment.According to wiki you can [build native library](http://code.google.com/p/csipsimple/wiki/HowToBuild#Build_native_library) or you can go ahead [without building the native library](http://code.google.com/p/csipsimple/wiki/HowToBuild#Without_building_the_native_library). I suggest for windows user to go with [without building the native library](http://code.google.com/p/csipsimple/wiki/HowToBuild#Without_building_the_native_library).If you have read wiki then in *"without building the native library"* section last line is like> "*To get the .so files, download latest nightly build from nightly build website and unpack the apk file (apk files are just zip files). Then get lib/ARCH/.so files and copy it into CSipSimple libs/ARCH/.so*"**I have to clarify one important thing about that:**First get [.apk](http://nightlies.csipsimple.com/trunk/) file of csipsimple. The version of***.apk*** file must same to download version of your[csipsimple source code.](http://code.google.com/p/csipsimple/source/browse/trunk/CSipSimple).Here, you can see the source code version. Download the same version [.apk](http://nightlies.csipsimple.com/trunk/) and do further as described in wiki.![enter image description here](http://i.stack.imgur.com/BZEwR.png)||
|----|----|----|
|| | |
Check pic.
To remove native library issue you have to put ".so file" into the **libs** folder.
i) If you do not have this folder then right click on "csipsimple" project and add folder named "libs". Folder name should be "libs".
ii) Now next you need ".so" files so for that get .apk from [here](http://nightlies.csipsimple.com/trunk/).
    Be careful your downloaded .apk file version must be same as your downloaded source code version.
iii)  Now next, 
right click on .apk (as we do for .zip package) and extract it.
iv) In extracted folder you will get "lib" folder. That's it.
v) Copy all folder inside the "lib" folder and paste simply into the "libs"(Eclipse folder that we create in first step) folder.
Problem solved.
![](https://lh6.googleusercontent.com/-7rBUDC-f_2Y/UDSjZoGny_I/AAAAAAAAAAY/Bnc4gUjHi2Y/s320/sip.png)
||[https://groups.google.com/forum/?fromgroups=#!searchin/csipsimple-dev/windows/csipsimple-dev/Cb26P2fCRac/HgmS1UJdPMQJ](https://groups.google.com/forum/?fromgroups=#!searchin/csipsimple-dev/windows/csipsimple-dev/Cb26P2fCRac/HgmS1UJdPMQJ)|
Rename it to zip... 
If you were under linux this kind of problem doesn't appear and you would also understand that windows is just crap with extension management and that extension is just something in the name. The associated application to open a file could be anything. But
 this kind of thing is hidden to windows users and many users remains in ignorance...
**Bug调试：**
**1.Ubuntu 由于网络问题导致未能下载完全，需要开外网！！！下不来的( silk, zrtp4pj, openssl)手动下载，修改makefile.   **
**2. Windows：**
|Kinjal Shah|![](https://groups.google.com/forum/clear.cache.gif)发布回复![对帖子的更多操作](https://groups.google.com/forum/clear.cache.gif)12-10-2|
@gdarmon
Hello,
i just tried below steps for building project on windows and it build and run successfully.. so u can try it..
1) import Actionbarsherlock from svn link
2) then import CSipSimple from svn link and give Actionbarsherlock as a library in CSipSimple application
3) then from this link , [nightlies.csipsimple.com/dev_helpers/org.tar.gz](http://nightlies.csipsimple.com/dev_helpers/org.tar.gz) ,download
 it and unrar it..and copy pjsua folder from it and paste it in your src/org/pjsip folder of csipsimple project.
4) then from this link, [http://nightlies.csipsimple.com/trunk/](http://nightlies.csipsimple.com/trunk/) , download
 latest trunk and rename it to .zip and unpack it.then from its lib folder ,copy two folder armeabi and armeabi-v7a and paste it into libs folder of your csipsimple project.
5) i think after all these steps, all errors should be removed..
关于ActionBarSherlock的编译错误： 重复导入几次，clean就ok了。。。
而SipHome
表象错误
### [Errors running builder 'Android Pre Compiler' on project NullPoint....](http://blog.csdn.net/lijin_2009/article/details/8446323)
### [https://groups.google.com/forum/?fromgroups=#!searchin/csipsimple-dev/Android$20Pre$20Compiler/csipsimple-dev/FYotthm7rxk/RXc0cc4tdvcJ](https://groups.google.com/forum/?fromgroups=#!searchin/csipsimple-dev/Android$20Pre$20Compiler/csipsimple-dev/FYotthm7rxk/RXc0cc4tdvcJ)
Ok, before import ActionBarSherlock, you install android API-16 via SDK-Manager and restart eclipse. Then
ActionBarSherlock get importet with no error.
After that that, i import SipHome alone, and get this errors:
![](https://lh6.googleusercontent.com/-r_9_FREKmgM/UNy0h_aKI7I/AAAAAAAAAC8/khFjE5SVWL0/s320/Screenshot_4.jpg)
But when i**import all projects, i dont get this errors,**
 and i can build SipHome!
![](https://lh4.googleusercontent.com/-2rkCCLGOams/UNy1gSemVUI/AAAAAAAAADI/M9KX6ho2inE/s320/Screenshot_5.jpg)
Now i can start playing ;)
Kristijan
### ![](https://img-my.csdn.net/uploads/201301/30/1359476865_7844.jpg)
### The right answer:
### 以上过程中，如果ADT插件没有自动生成ICWJStub类在工程的gen目录下时，可以手动在sdk根目录下platform-tools目录下，手动实用AIDL.exe来生成，这样可以看清楚AIDL文件到底是哪里存在格式上的错误。
### aidl.exe -IE:\workspace\...\src E:\workspace\...\I...Configuration.aidl
### ![](https://img-my.csdn.net/uploads/201301/30/1359476055_9084.jpg)
### // doesn't work
### **Eclipse Pre Compiler doesn't create directory under gen for AIDL in differenttop-level package**
**work!!!!!! **终极解决方案：[http://code.google.com/p/csipsimple/issues/detail?id=2187&can=1&q=Isipservice](http://code.google.com/p/csipsimple/issues/detail?id=2187&can=1&q=Isipservice)
**What steps will reproduce the problem?**
1. Checkout latest CSipSimple [r2093](http://code.google.com/p/csipsimple/source/detail?r=2093)
2. Make CSipSimple JNI dependencies
3. Install Eclipse / Android SDK / Android NDK
4. Load ABS and SipHome in to Eclipse per the how-to-build Wiki
**What is the expected output? What do you see instead?**
SipHome should build without error. Instead Eclipse reports two null pointer exceptions. In addition the error output is flooded with reports of ISipConfiguration and ISipService not being resolved.
**What version of the product are you using? On what operating system?**
Android ADT bundle v21.0.1-543035
Android SDK [r21](http://code.google.com/p/csipsimple/source/detail?r=21).0.1
Android NDK r8d
**Please provide any additional information below.**
The problem is related to a files in the CSipSimple <SRC> folder which do not have extensions:
<CSipSimple>/src/com/csipsimple/api/NOTICE
<CSipSimple>/src/org/webrtc/videoengine/README
See this Android bug report:
[http://code.google.com/p/android/issues/detail?id=43057](http://code.google.com/p/android/issues/detail?id=43057)
My android log file in <eclipse>/.metadata/.log shows the following:
!ENTRY org.eclipse.core.resources 4 2 2013-01-22 11:40:35.034
!MESSAGE Problems occurred when invoking code from plug-in: "org.eclipse.core.resources".
!STACK 0
java.lang.NullPointerException
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.scanFolderForSourceFiles(SourceProcessor.java:374)
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.scanFolderForSourceFiles(SourceProcessor.java:380)
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.scanFolderForSourceFiles(SourceProcessor.java:380)
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.scanFolderForSourceFiles(SourceProcessor.java:380)
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.buildSourceFileList(SourceProcessor.java:353)
        at com.android.ide.eclipse.adt.internal.build.SourceProcessor.prepareFullBuild(SourceProcessor.java:194)
        at com.android.ide.eclipse.adt.internal.build.builders.PreCompilerBuilder.build(PreCompilerBuilder.java:304)
        at org.eclipse.core.internal.events.BuildManager$2.run(BuildManager.java:728)
        at org.eclipse.core.runtime.SafeRunner.run(SafeRunner.java:42)
        at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:199)
        at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:239)
        at org.eclipse.core.internal.events.BuildManager$1.run(BuildManager.java:292)
        at org.eclipse.core.runtime.SafeRunner.run(SafeRunner.java:42)
        at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:295)
        at org.eclipse.core.internal.events.BuildManager.basicBuildLoop(BuildManager.java:351)
        at org.eclipse.core.internal.events.BuildManager.build(BuildManager.java:374)
        at org.eclipse.core.internal.events.AutoBuildJob.doBuild(AutoBuildJob.java:143)
        at org.eclipse.core.internal.events.AutoBuildJob.run(AutoBuildJob.java:241)
        at org.eclipse.core.internal.jobs.Worker.run(Worker.java:54)
The problem is resolved by adding a ".txt" extension to the files noted above.
Jan 22 (6 days ago)
Project Member[#1](http://code.google.com/p/csipsimple/issues/detail?id=2187#c1)[r3gis...@gmail.com](http://code.google.com/u/103053537395257486222/)Thanks for the early report and for sharing the possible workaround.
The root cause seems to be a bug in this new ADT version (fixed in dev channel of ADT plugin) but renaming the files with problem is affordable, so your workaround is a very good idea.
若：
![](https://img-my.csdn.net/uploads/201301/30/1359477064_5601.jpg)
则：
**关闭工程(eclipse)**并重新打开，不行则清理工程，还不行就重新导入import.
+++++++++++++++++++参考内容+++++++++++++++++++++++
[step-by-step: How to build the app.](https://groups.google.com/forum/?fromgroups=#!searchin/csipsimple-dev/Android$20Pre$20Compiler/csipsimple-dev/FYotthm7rxk/RXc0cc4tdvcJ)
++++++++++++++++++++++++++++++++++++++++++++++++++
### // doesn't work by grant
找了半天原来是svn 上更新下来的东西我没整合。就有好几个名字相同的类。还有就是我的aidl的文件不能自动生成java文件，我就手动导入的。还有个问题就是我配置的环境是20.0的。我 用的是低版本的项目。是从别人那里拷过来的。这样也会导致各种错误。 有时候Manifest.xml文件dismiss 的错误就是这个原因。于是我手动的添加到bin目录下了。就没问题了。
![](https://img-my.csdn.net/uploads/201212/31/1356947263_5936.jpg)
确保android pre Compiler 打了对勾
/**/ doesn't work by grant**
You can see the first line about drawable-xxhdpi folder that is 
considered **as invalid by the version of AAPT used. **
So aapt doesn't generates auto generated resources such as aidl 
autogenerated interface. That's why there is all next errors about 
ISipConfiguration and ISipService (that are autogenerated files). Fixing 
the first problem will fix all others. 
Aapt probably doesn't recognize drawable-xxhdpi folder because the sdk 
version tools are outdated. So you have two simple solutions : 
  1- Update the sdk in eclipse. It's pretty simple there is the little 
droid icon in eclipse bar that you can click and ask for update. You'll 
also need to update ADT probably that you can do by searching updates 
(in eclipse window > search for update menu). 
Or 
  2 - (worse solution but faster), remove the drawable-xxhdpi (however 
I'm not sure that there is not other things that depends on latest sdk 
version too). 
or
![](https://csipsimple-dev.googlegroups.com/attach/d67d9cbb6932648c/android2.JPG?view=1&part=4)
So, here in errors I can see from the screenshot apparently, it's not 
able to "import com.csipsimple.R;". 
This package is autogenerated by the android apk toolchain when it 
generates the resources identifiers. It generates a class xxx.yyyyyy.R 
where xxx.yyyy.R is the package name of your application. 
I can see from the screenshot that there is apparently some modified 
files regarding the current source code version. Did you modify the 
AndroidManifest.xml file to change the com.csipsimple package name into 
your package name? 
If not, there is maybe something else that prevents generation of 
resources files. Maybe you can try to clean the project (go in Projects 
 > clean all). The important error when android builds is the first you 
get (eclipse problems list unfortunately sometimes re-order it). Often 
when something goes wrong at very first it results in many other errors 
that has only one root cause. 
////
In Android I am Getting `BuildConfig.java` file
 but it can not generate 
```
R.java
 File
```
.
Please Help me this regards. I tried with 
```
Android
 Tool -> fix project properties
```
. but still failed to run.
I also tried Clean and build but again failed to generate R.Java.
It is not "instead of" -- you should have both `BuildConfig.java` and `R.java`.
 You may have errors in your Eclipse console, or red X marks over a resource **(`res/`)**
 or your `AndroidManifest.xml`,
 that are preventing `R.java` from
 being rebuilt successfully.
**// doesn't work by grant**
# [Eclipse “cannot be resolved to a type” error](http://www.cnblogs.com/xuxm2007/archive/2011/10/20/2219104.html)
[http://chenxiaohu612.blog.163.com/blog/static/192214286201171004911955/](http://chenxiaohu612.blog.163.com/blog/static/192214286201171004911955/)
[http://zhaoningbo.iteye.com/blog/1137215](http://zhaoningbo.iteye.com/blog/1137215)
**引言：**
    eclipse新导入的项目经常可以看到“XX cannot be resolved to a type”的报错信息。本文将做以简单总结。 
**正文：     （1）jdk不匹配（或不存在）**
    项目指定的jdk为“jdk1.6.0_18”，而当前eclipse使用的是“jdk1.6.0_22”。需要在BuildPath | Libraries，中做简单调整。 
**（2）jar包缺失或冲突**
    当找不到“XX”所在的jar包时，会报这个错。解决只需要找到（ctrl+点击，可能能看到jar包名称）对应jar包导入项目即可。 
    另外，出现相同的XX时也会报此错。可能需要调包、解包、选删。 
**（3）eclipse查找项目类型策略所致**
    eclipse下，上述两条都对比过了，没有任何问题，可偏偏还报这错。这时，需要操作一下 Project | Clean... ，问题即可解决。原因是，机制所致。因为某些特殊原因，eclipse没能自动编译源代码到build/classes（或其他classes目录），导致类型查找不到。 
Based on the comments here, I started checking how I could make sure that my Ant build 
wouldn’t interfere with my Eclipse build.
I found an option in Eclipse that clears up the problem (and is possibly more efficient than
 changing the 2 build system’s output folders).
Windows–>Preferences–>Java–>Compiler–>Building–>Output folder–>
”Rebuild class files modified by others”. This exists in Eclipse 3.5; I’m not sure about earlier versions.
Checking this seems to have fixed my problems.
