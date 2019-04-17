# 《Training:Creating an Android Project》 - 一世豁然的专栏 - CSDN博客





2017年04月09日 21:40:45[一世豁然](https://me.csdn.net/Explorer_day)阅读数：264标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方说明。







本课程向您展示如何使用Android Studio创建新的Android项目，并介绍项目中的一些文件。





1、在Android Studio中，创建一个新项目：

 1）、如果您没有一个已经打开项目，请在“Welcome”屏幕中单击“New Project”。

 2）、如果您打开了一个项目，从File菜单中选择New Project。 此时出现创建新建项目屏幕。




2、填写相关项目信息。Applicaotion Name使用“My First App”。 对于Company Domain，请使用“example.com”。 对于其他字段，使用默认值，然后单击下一步

以下简要说明项目中每个待填写的信息：


 1）、Applicaotion Name：是显示给用户的APP名称。

 2）、Company Domain：提供将附加到包名称的限定符; Android Studio将会为您创建的每个新项目记住这个限定词（意思就是java代码都放在这里）。

 3）、Package name：是项目的完全限定名称（遵循与Java编程语言中的命名软件包相同的规则）。在安装在Android系统上的所有软件包中，你的软件包名称必须是唯一的。您可以独立于应用程序名称或公司域编辑此值。

 4）、Project location：是您的系统上保存项目文件的目录。




3、在Target Android Devices下，接受默认值，然后单击下一步。

Minimum Required SDK 是您的应用程序支持的最早版本的Android，使用API级别表示。为了支持尽可能多的设备，您应该将其设置为允许应用提供其核心功能集的最低版本。如果您的app的任何功能仅在较新版本的Android上可用，并且对app的核心功能集来说并不重要，只有在支持该功能的版本的时候（如支持不同的平台版本中所述），你才能启用该功能。




4、在“Add an Activity to Mobile”下，选择“Empty Activity”，然后单击“下一步”。




5、在“Customize the Activity”下，接受默认值，然后单击Finish。

您的Android项目现在是包含一些默认文件的基本“Hello World”应用程序。 花点时间来游览其中最重要的：


 1）、app/src/main/java/com.example.myfirstapp/MainActivity.java


新建项目向导完成后，此文件将显示在Android Studio中。 它包含您之前创建的活动类定义。 当您构建并运行该应用程序时，活动将启动并加载说明“Hello World！”的布局文件。


 2）、app/src/main/res/layout/activity_main.xml


此XML文件定义活动的布局。 它包含一个文本“Hello world！”的TextView元素。


 3）、app/src/main/AndroidManifest.xml

清单文件描述了应用程序的基本特征，并定义了每个组件。 当您遵循这些课程并在应用程序中添加更多组件时，您将重新访问此文件。


 4）、app/build.gradle


Android Studio使用Gradle编译和构建您的应用程序。您的项目的每个模块都有一个build.gradle文件，以及整个项目的build.gradle文件。通常，在app或应用程序模块这种情况下，您只对该模块的build.gradle文件感兴趣。这是您的应用程序的构建依赖项设置的位置，包括defaultConfig设置：


   I、compiledSdkVersion是针对您将编译应用程序的平台版本。默认情况下，这是设置为SDK中最新的Android版本。默认情况下，这是设置在开发机器上安装的最新的Android SDK版本。您仍然可以构建您的应用程序来支持旧版本，但将其设置为最新版本可以启用新功能并优化您的应用程序，以便在最新设备上获得良好的用户体验。

   II、applicationId是您在“New Project ”向导中指定的应用程序的完全限定的包名称。

   III、minSdkVersion是您在“New Project”向导期间指定的最低SDK版本。 这是您的应用程序支持的Android SDK的最早版本。

   IV、targetSdkVersion表示您测试了您的应用程序的最高的Android版本。 随着新的Android版本可用，您应该在新版本上测试您的应用，并更新此值以匹配最新的API级别，从而利用新的平台功能。 有关详细信息，请参阅Supporting Different Platform Versions。




注：有关Gradle的更多信息，请参阅Building Your Project with Gradle。




还请注意包含应用程序资源的/ res子目录：


   I、drawable-<density>/

 drawable resources目录，除了启动器图标，设计用于各种densities。


   II、layout/

定义app用户界面的文件目录，如上面讨论的activity_main.xml，它描述了MainActivity类的基本布局。


   III、menu/

用于定义app菜单项的文件目录。


   IV、mipmap/

启动器图标驻留在mipmap /文件夹中，而不是drawable / folders。 此文件夹包含运行默认应用程序时显示的ic_launcher.png图像。


   V、values/

包含资源集合的其他XML文件的目录，例如字符串和颜色定义。




