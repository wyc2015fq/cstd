# [系列好文]Ubuntu 下Android开发环境的搭建(英文官网!!!!!!) - 三少GG - CSDN博客
2010年12月22日 12:21:00[三少GG](https://me.csdn.net/scut1135)阅读数：2177标签：[android																[ubuntu																[components																[documentation																[download																[tools](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=download&t=blog)](https://so.csdn.net/so/search/s.do?q=documentation&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
1. JDK
2. Eclipse
3.SDK
Android 2.3现在可以使用最新的Android SDK啦。此外，全新的工具和文档，加上全新的Android NDK，已经提供啦。您现在已经有一个Android老版本的SDK，可以直接添加新增的Android 2.3 SDK组件。如果您是新的Android应用的开发者可以直接安装Android 2.3的SDK软件包。
[http://androidappdocs.appspot.com/sdk/index.html](http://androidappdocs.appspot.com/sdk/index.html)
如何安装 ？ [http://androidappdocs.appspot.com/sdk/installing.html](http://androidappdocs.appspot.com/sdk/installing.html)
## Step X. Downloading the SDK Starter Package
The SDK starter package is not a fulldevelopment environment—it includes only the core SDK Tools, which you canuse to download the rest of the SDK components (such as the latest Android platform).
If you haven't already, get the latest version of the SDK starter package from the[SDK download page](http://androidappdocs.appspot.com/sdk/index.html).
If you downloaded a `.zip` or `.tgz` package (instead of the SDK installer),unpackit to a safe location on your machine. By default, the SDK files are unpackedinto a directory named`android-sdk-<machine-platform>`.
If you downloaded the Windows installer (`.exe` file), run it now and it will checkwhether the proper Java SE Development Kit (JDK) is installed (installing it, if necessary), theninstall the SDK Tools into a default location (which you can modify).
Make a note of the name and location of the SDK directory on your system—you will need torefer to the SDK directory later, when setting up the ADT plugin and when usingthe SDK tools from command line.
在tools目录下双击android文件，选择在终端中运行即可弹出Android SDK and AVD Manager！
## StepX.  Adding Platforms and Other Components
The last step in setting up your SDK is using the *Android SDK and AVD Manager* (atool included in the SDK starter package) to downloadessential SDK components into your development environment.
The SDK uses a modular structure that separates the major parts of the SDK—Android platformversions, add-ons, tools, samples, and documentation—into a set of separately installablecomponents. The SDK starter package, which you've already downloaded, includes
 only a singlecomponent: the latest version of the SDK Tools. To develop an Androidapplication, you also need to download at least one Android platform and the SDK Platform-tools(tools that the latest platform depend upon). However, downloadingadditional components
 is highly recommended.
If you used the Windows installer, when you complete the installation wizard, it will launch theAndroid SDK and AVD Manager with a default set of platforms and other components selectedfor you to install. Simply click**Install** to accept the
 recommended set ofcomponents and install them. You can then skip to[Step 5](http://androidappdocs.appspot.com/sdk/installing.html#sdkContents), but werecommend you first read the section about the[Available
 Components](http://androidappdocs.appspot.com/sdk/installing.html#components) tobetter understand the components available from the Android SDK and AVD Manager.
You can launch the Android SDK and AVD Manager in one of the following ways:
- From within Eclipse, select **Window > Android SDK and AVD Manager**.
- On Windows, double-click the `SDK Manager.ext` file at the root of the AndroidSDK directory.
- On Mac or Linux, open a terminal and navigate to the `tools/` directory in theAndroid SDK, then execute:
android
To download components, use the graphical UI of the Android SDK and AVDManager, shown in Figure 1, to browse the SDK repository and select new or updatedcomponents. The Android SDK and AVD Manager will install the selected components inyour SDK environment.
 For information about which components you should download, see the followingsection about[Recommended Components](http://androidappdocs.appspot.com/sdk/installing.html#which).![](http://androidappdocs.appspot.com/images/sdk_manager_packages.png)
**Figure 1.** The Android SDK and AVD Manager's**Available Packages** panel, which shows the SDK components that areavailable for you to download into your environment.
4.ADT
[http://androidappdocs.appspot.com/sdk/eclipse-adt.html](http://androidappdocs.appspot.com/sdk/eclipse-adt.html)
Additionally, before you can configure or use ADT, you must install theAndroid SDK starter package, as described in[Downloading the SDK Starter Package](http://www.ideasandroid.com/android/sdk/docs/sdk/installing.html#Installing).Specifically,
 you need to install a compatible version of the Android SDK Toolsand at least one development platform. To simplify ADT setup, we recommendinstalling the Android SDK prior to installing ADT.
When your Eclipse and Android SDK environments are ready, continue with theADT installation as described in the steps below.
### Downloading the ADT Plugin
Use Update Manager feature of your Eclipse installation to install the latestrevision of ADT on your development computer.
Assuming that you have a compatible version of the Eclipse IDE installed, asdescribed in[Preparing for Installation](http://www.ideasandroid.com/android/sdk/docs/sdk/eclipse-adt.html#preparing), above, followthese steps to download the ADT plugin
 and install it in your Eclipseenvironment.
|Eclipse 3.5 (Galileo) and 3.6 (Helios)|Eclipse 3.4 (Ganymede)|
|----|----|
|- Start Eclipse, then select **Help** > **Install NewSoftware...**.- Click **Add**, in the top-right corner.- In the Add Repository dialog that appears, enter "ADT Plugin" for the*Name* and thefollowing URL for the*Location*:https://dl-ssl.google.com/android/eclipse/Note: If you have trouble acquiring the plugin, try using "http" in the Location URL, instead of "https" (https is preferred for security reasons).Click **OK**.- In the Available Software dialog, selectthe checkbox next to Developer Tools and click**Next**.- In the next window, you'll see a list of the tools to be downloaded. Click**Next**.- Read and accept the license agreements, then click **Finish**.- When the installation completes, restart Eclipse.|- Start Eclipse, then select **Help** > **Software Updates...**.In the dialog that appears, click the**Available Software** tab.- Click **Add Site**.- In the Add Site dialog that appears, enter this URL in the "Location" field:https://dl-ssl.google.com/android/eclipse/Note: If you have trouble acquiring the plugin, you can try using "http" in the URL, instead of "https" (https is preferred for security reasons).Click **OK**.- Back in the Available Software view, you should see the plugin listed by the URL, with "Developer Tools" nested within it. Select the checkbox next to Developer Tools, which will automatically select the nested tools. Then click**Install**- On the subsequent Install window, all of the included tools should be checked. Click**Next**.- Read and accept the license agreements, then click **Finish**.- When the installation completes, restart Eclipse.|
### Configuring the ADT Plugin
Once you've successfully downloaded ADT as described above, **the next stepis to modify your ADT preferences in Eclipse to point to the Android SDK directory:**
- **Select Window > Preferences... to open the Preferences panel (Mac OS X: Eclipse > Preferences).**
- **Select Android from the left panel. **
- **For the *SDK Location* in the main panel, click Browse... and locate your downloaded SDK directory.**
- **Click Apply, then OK.**
**Done! If you haven't encountered any problems, then the installation iscomplete. If you're installing the Android SDK for the first time, return to[Installing the SDK](http://www.ideasandroid.com/android/sdk/docs/sdk/installing.html#InstallingADT) to complete your setup.**
**5.**NDK  
**下载NDK： [http://androidappdocs.appspot.com/sdk/ndk/index.html](http://androidappdocs.appspot.com/sdk/ndk/index.html)**
相关内容：
[http://www.ideasandroid.com/android/sdk/docs/sdk/ndk/index.html](http://www.ideasandroid.com/android/sdk/docs/sdk/ndk/index.html)
[http://androidappdocs.appspot.com/sdk/ndk/overview.html#tools](http://androidappdocs.appspot.com/sdk/ndk/overview.html#tools)
[Android NDK, Revision 5](http://androidappdocs.appspot.com/sdk/ndk/index.html)*(December 2010)*
## Installing the NDK
Installing the NDK on your development computer is straightforward and involves extracting the NDK from its download package.
Before you get started make sure that you have downloaded the latest [Android SDK](http://androidappdocs.appspot.com/sdk/index.html) and upgraded your applications and environment as needed. The NDK is compatible with older platform versions but not older versions of the SDK tools. Also, take a moment to review the[System
 andSoftware Requirements](http://androidappdocs.appspot.com/sdk/ndk/overview.html#reqs) for the NDK, if you haven't already.
To install the NDK, follow these steps:
- From the table at the top of this page, select the NDK package that is appropriate for your development computer and download the package.
- Uncompress the NDK download package using tools available on your computer. When uncompressed, the NDK files are contained in a directory called`android-ndk-<version>`. You can rename the NDK directory if necessary and you can move it to any
 location on your computer. This documentation refers to the NDK directory as`<ndk>`.
You are now ready to start working with the NDK.
## Getting Started with the NDK
Once you've installed the NDK successfully, take a few minutes to read the documentation included in the NDK. You can find the documentation in the`<ndk>/docs/` directory. In particular, please read the OVERVIEW.HTML document completely, so that
 you understand the intent of the NDK and how to use it.
If you used a previous version of the NDK, take a moment to review the list of NDK changes in the CHANGES.HTML document.
Here's the general outline of how you work with the NDK tools:
- Place your native sources under `<project>/jni/...`
- Create `<project>/jni/Android.mk` to describe your native sources to the NDK build system
- Optional: Create `<project>/jni/Application.mk`.
- Build your native code by running the 'ndk-build' script from your project's directory. It is located in the top-level NDK directory:
cd <project>
<ndk>/ndk-build
The build tools copy the stripped, shared libraries needed by your application to the proper location in the application's project directory.
- Finally, compile your application using the SDK tools in the usual way. The SDK build tools will package the shared libraries in the application's deployable`.apk` file.
For complete information on all of the steps listed above, please see the documentation included with the NDK package.
HelloWorld例子（SDK）： [http://androidappdocs.appspot.com/resources/tutorials/hello-world.html](http://androidappdocs.appspot.com/resources/tutorials/hello-world.html)
                    例子（NDK）：[http://androidappdocs.appspot.com/sdk/ndk/overview.html#samples](http://androidappdocs.appspot.com/sdk/ndk/overview.html#samples)
