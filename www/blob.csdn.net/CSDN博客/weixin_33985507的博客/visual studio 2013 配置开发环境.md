# visual studio 2013 配置开发环境 - weixin_33985507的博客 - CSDN博客
2019年01月13日 07:15:49[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
[https://www.visualstudio.com/explore/xamarin-vs](https://www.visualstudio.com/explore/xamarin-vs)
[http://sourceforge.net/projects/easyeclipse/files/?source=navbar](http://sourceforge.net/projects/easyeclipse/files/?source=navbar)
[https://developer.xamarin.com/guides/ios/getting_started/installation/windows/introduction_to_xamarin_ios_for_visual_studio/](https://developer.xamarin.com/guides/ios/getting_started/installation/windows/introduction_to_xamarin_ios_for_visual_studio/)
It was not possible to complete an automatic installation. This might be due to a problem with your network, proxy servers or an unsolvable installation conflict. At this point, you can continue the installation by manually downloading and installing the independent pieces. The list below shows each component that you need to install.
Java JDK
Download the [Java JDK v1.7.0 installer](http://download.xamarin.com/Installer/MonoForAndroid/jdk-7u71-windows-i586.exe) to any directory on your disk, double-click the downloaded file and follow the on-screen instructions.
It is **very important** to install the 32-bit version of Java JDK even if you're using 64-bit Windows. It is also important that Java JDK v1.6 is installed (although it is fine to have Java JDK 1.7 or newer installed at the same time) 
Android SDK
Download the [Android SDK for Windows version 22.0.0](http://dl.google.com/android/installer_r22-windows.exe) to any directory on your disk and double-click it to start main SDK installation process. You can install the SDK either system wide or only for your account. You will need to point your IDE to that location after the installation is completed.
Click here in order to start the Android SDK Manager application. In the GUI make sure the following components are installed or selected for installation:
- Android SDK Tools
- Android SDK Platform-tools
- Android SDK Build-tools
- Android API 15
- Android API 19
- Android API 21
Once all the required components are selected (you can select others as well, if you wish) start the installation. Click here to open target dir
Download the [Android NDK for Windows version r10d](http://www.cnblogs.com/) to any directory on your disk and double-click it to start the NDK installation.
When the Android SDK installation is finished you might want to start the Android Emulator Manager and create as many emulator images as you might need for your development. This step can be performed at any time, on an as-needed basis.
GTK#
Download the [GTK# v2.12.26 installer](http://download.xamarin.com/GTKforWindows/Windows/gtk-sharp-2.12.26.msi) to any directory on your disk, double-click the downloaded file and follow the on-screen instructions.
Xamarin Studio
Download the [Xamarin Studio v5.9.3 installer](http://download.xamarin.com/studio/Windows/XamarinStudio-5.9.3.1-0.msi) to any directory on your disk, double-click it and follow the on-screen instructions.
Xamarin
Download the [Xamarin v3.11.590 installer](http://download.xamarin.com/XamarinforVisualStudio/Windows/Xamarin.VisualStudio_3.11.590.msi) to any directory on your disk, double-click it and follow the on-screen instructions.
After installation, restart your Windows workstation to make sure all of the installed system services are running.
[https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/)
# Xamarin + Visual Studio
Xamarin's Visual Studio plug-in lets you build for iOS and Android on Windows.
Yes, it seems impossible but you can build iOS and Android apps alongside Windows apps in Visual Studio. The Xamarin plug-in for Visual Studio provides project types, UI Designers, and build options for both Android and iOS.
# Android Apps
[](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/#Android_Apps)
Once you've [installed Xamarin for Windows](https://developer.xamarin.com/guides/android/getting_started/installation/windows/) you can create, build, test, and debug Android apps from within Visual Studio.
[Learn from Microsoft](https://msdn.microsoft.com/en-us/library/dn879698%28v=vs.140%29.aspx) how to build cross-platform apps for Android and Windows Phone!
![Weather sample](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/Images/msdn.png)
Xamarin's [Android UI Designer](https://developer.xamarin.com/guides/android/user_interface/designer_overview/) works in Visual Studio, letting you create your user interface by dragging and dropping controls onto each screen.
The [Xamarin Android Player](http://xamarin.com/android-player) also runs on Windows, and provides a great Android simulator experience for testing your Android apps ([read more](https://developer.xamarin.com/guides/android/getting_started/installation/android-player/)).
# iOS Apps
[](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/#iOS_Apps)
Xamarin brings iOS development to Visual Studio, from UI design to C# coding, deploying, and debugging iPhone and iPad apps. You just need to have a **networked Mac** to take care of the compiler and host the simulator or plug in a device.
Once [installed](https://developer.xamarin.com/guides/ios/getting_started/installation/windows/) the development experience is just like any other Visual Studio project. Check out our [introduction to Xamarin.iOS for Visual Studio](https://developer.xamarin.com/guides/ios/getting_started/installation/windows/introduction_to_xamarin_ios_for_visual_studio/) and see how the [iOS Designer](https://developer.xamarin.com/guides/ios/user_interface/designer/) works.
# Xamarin.Forms
[](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/#Xamarin.Forms)
You can create Android, iOS, and Windows Phone apps - from business logic to user interface - with almost 100% common code using [Xamarin.Forms](https://developer.xamarin.com/guides/cross-platform/xamarin-forms/) in Visual Studio. You can even use our [XAML syntax](https://developer.xamarin.com/guides/cross-platform/xamarin-forms/xaml-for-xamarin-forms/) to declaratively build your screens, with styles, behaviors, and triggers.
Start with our [documentation](https://developer.xamarin.com/guides/cross-platform/xamarin-forms/) and [free downloadable book chapters](https://developer.xamarin.com/guides/cross-platform/xamarin-forms/creating-mobile-apps-xamarin-forms/) to learn more.
# Cross-Platform Apps
[](https://developer.xamarin.com/guides/cross-platform/windows/visual-studio/#Cross-Platform_Apps)
Using Portable Class Libraries or Shared Projects, you can write common code, to share with Xamarin.iOS, Xamarin.Android projects, as well as Windows Phone, WPF, Windows RT or other C#-based platforms from inside Visual Studio. Start with our docs on [setting up a cross-platform app](https://developer.xamarin.com/guides/cross-platform/getting_started/visual_studio_with_xamarin/cross_platform_visual_studio_project_setup_walkthrough/).
Check out [VisualStudio.com](https://www.visualstudio.com/explore/xamarin-vs) for more links and videos on how to build cross-platform apps with Xamarin and Visual Studio.
