# A step-by-step guide for debugging native code (Sequoyah/ndk guide) - xqhrs232的专栏 - CSDN博客
2012年05月30日 22:13:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：995标签：[debugging																[application																[eclipse																[android																[session																[dependencies](https://so.csdn.net/so/search/s.do?q=dependencies&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=debugging&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://wiki.eclipse.org/Sequoyah/ndk_guide](http://wiki.eclipse.org/Sequoyah/ndk_guide)
### Pre-requisites 
- Sequoyah Native Debug feature must be installed. You can install it from Sequoyah update site either from
[http://download.eclipse.org/sequoyah/updates/1.1](http://download.eclipse.org/sequoyah/updates/1.1) or [http://download.eclipse.org/sequoyah/updates/2.0](http://download.eclipse.org/sequoyah/updates/2.0). 
![Image:Ndk guide 01.PNG](http://wiki.eclipse.org/images/f/f3/Ndk_guide_01.PNG)
It will install CDT's dependencies if needed: ![Image:Ndk guide 02.PNG](http://wiki.eclipse.org/images/2/26/Ndk_guide_02.PNG)
- The platform must be Android 2.2 (android-8) or greater 
- The ndk version must be r4b or newer 
- Eclipse CDT 7.0 or newer must be installed 
- The AndroidManifest.xml must have the property of the application node android:debuggable="true" or the project must have been built with the flag "NDK_DEBUG=1" (for r5):
- The build must have been done with the ndk-build (if using the Sequoyah Android components, it will be automatic)
### Setting up the debug session 
- Create a debug configuration for an Android application (can be done with Eclipse or MOTODEV Studio)
- Open the ndk-gdb script that came with the android NDK and comment the last line
(we are not calling the usual gdb client, but we will attach an Eclipse gdb session instead):
#$GDBCLIENT -x $GDBSETUP -e $APP_PROCESS
- Insert a breakpoint in your Java code, preferably after all *System.loadLibrary()* calls, in order to make sure that the debugger is correctly attached to the Java process.
- Launch the android debug and wait for it to reach the breakpoint 
- From a Terminal session, in the project folder, run the modified ndk-gdb command. It should not attach to an gdb client, but call the gdbserver on the emulator and open a TCP port for connection.
- Create a debug configuration for a C/C++ application 
- Set the following properties: 
![Image:Ndk guide 04.PNG](http://wiki.eclipse.org/images/1/1b/Ndk_guide_04.PNG)
- The process launcher must be the Standard Process Launcher. This is selected at the bottom of the Main tab:
![Image:Ndk guide 05.PNG](http://wiki.eclipse.org/images/a/ac/Ndk_guide_05.PNG)
![Image:Ndk guide 03.PNG](http://wiki.eclipse.org/images/c/cd/Ndk_guide_03.PNG)
- On the "Main" tab: the Field C/C++ Application: 
$PROJECT_PATH/obj/local/armeabi/app_process
![Image:Ndk guide 08.PNG](http://wiki.eclipse.org/images/7/75/Ndk_guide_08.PNG)
- On the "Debugger" tab: 
- field Debugger: gdbserver 
- On the "Main" subtab: 
- GDB debugger: 
(for NDK r4b)
$NDK_PATH/build/prebuilt/$ARCH/arm-eabi-$GCC_VERSION/bin/arm-eabi-gdb 
(for NDK r5)
$NDK_PATH/toolchains/arm-eabi-$GCC_VERSION/prebuilt/$ARCH/bin/arm-eabi-gdb
- GDB command file: 
$PROJECT_PATH/obj/local/armeabi/gdb2.setup
[Windows users] Uncheck the "Use full file path to set breakpoints" option
![Image:Ndk guide 06.PNG](http://wiki.eclipse.org/images/3/37/Ndk_guide_06.PNG)
- On the "Connection" subtab: 
- Type: TCP 
- Hostname or IP address: localhost 
- Port number: 5039 
![Image:Ndk guide 07.PNG](http://wiki.eclipse.org/images/9/94/Ndk_guide_07.PNG)
- Modify the **gdb.setup** file located in the folder 
$PROJECT_PATH/obj/local/armeabi/
removing the "*target remote:5039*" statement. 
(For some reason, the Eclipse GDB session does not like this statement being done in the commands file) Rename this new file to gdb2.setup. This step needs to be run just once, on the first debug session. (I am working on further tweaking the ndk-gdb script
 to generate the gdb.setup file without the target statement, but for the time being, this workaround will work).
- Launch the C/C++ Application debug and wait for the Eclipse GDB session to fully connect to the emulator's gdbserver instance.
After following these steps, one can continue to debug the application as usual, using the "continue" option to let the execution flow until the next breakpoint is hit or by using the usual "step-in" to execute each statement individually. Setting a breakpoint
 on a Java statement that calls a native function through JNI and stepping into will place the user at the beginning of the native code.
Retrieved from "[http://wiki.eclipse.org/Sequoyah/ndk_guide](http://wiki.eclipse.org/Sequoyah/ndk_guide)"

