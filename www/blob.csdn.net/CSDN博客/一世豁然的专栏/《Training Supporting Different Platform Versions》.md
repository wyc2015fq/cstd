# 《Training:Supporting Different Platform Versions》 - 一世豁然的专栏 - CSDN博客





2017年04月14日 20:28:34[一世豁然](https://me.csdn.net/Explorer_day)阅读数：194标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，翻译此博客只是为了自学使用，如有疑问，请参考官方文档




虽然最新版本的Android通常为您的应用程序提供了很棒的API，但应该继续支持较旧版本的Android，直到更多设备更新。 本课程向您展示如何利用最新的API，同时继续支持旧版本。





平台版本的仪表板会定期更新，以显示运行每个Android版本的活动设备的分布情况，具体取决于访问Google Play商店的设备数量。 一般来说，在将应用程序定位到最新版本的同时，支持大约90％的活动设备是一个很好的做法。


注意：为了在多个Android版本中提供最佳功能和功能，您应该在应用程序中使用Android支持库，从而可以在旧版本上使用几种最新的平台API。







Specify Minimum and Target API Levels


AndroidManifest.xml文件描述了app的详细信息，并标识了其支持的Android版本。 具体来说，<uses-sdk>元素的minSdkVersion和targetSdkVersion属性标识您的应用程序兼容的最低API级别，以及您设计和测试应用程序的最高API级别。





例如：



```
<manifest xmlns:android="http://schemas.android.com/apk/res/android" ... >
    <uses-sdk android:minSdkVersion="4" android:targetSdkVersion="15" />
    ...
</manifest>
```






随着新版Android的发布，一些风格和行为可能会改变。 为了让您的应用程序能够利用这些更改，并确保您的应用程序适合每个用户设备的风格，您应该将targetSdkVersion值设置为与最新版本的Android版本相匹配







Check System Version at Runtime

Android在Build常量类中为每个平台版本提供了一个唯一的代码。 在您的app中使用这些代码来构建条件，以确保仅在系统上提供这些API时才执行依赖于更高API级别的代码。


```java
private void setUpActionBar() {
    // Make sure we're running on Honeycomb or higher to use ActionBar APIs
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
        ActionBar actionBar = getActionBar();
        actionBar.setDisplayHomeAsUpEnabled(true);
    }
}
```
注意：解析XML资源时，Android会忽略当前设备不支持的XML属性。 因此，您可以安全地使用仅由较新版本支持的XML属性，而不用担心旧版本遇到该代码时会破坏旧版本。 例如，如果您设置targetSdkVersion =“11”，则应用程序在Android 3.0及更高版本上默认包含ActionBar。 然后将菜单项添加到操作栏，您需要在菜单资源XML中设置android：showAsAction =“ifRoom”。 可以在跨版本的XML文件中做到这一点，因为旧版本的Android只是忽略showAsAction属性（也就是说，在res
 / menu-v11 /中不需要单独的版本）。









Use Platform Styles and Themes

Android提供用户体验主题，为应用程序提供底层操作系统的外观和视觉。 这些主题可以使用清单文件来应用到app中。 通过使用这些内置的样式和主题，您的应用程序将自然地遵循Android的最新外观和每个新版本的视觉。




要使您的活动看起来像一个对话框：


`<activity android:theme="@android:style/Theme.Dialog">`





要使您的活动具有透明背景：


`<activity android:theme="@android:style/Theme.Translucent">`

要应用您定制在/res/values/styles.xml中的自定义主题：




`<activity android:theme="@style/CustomTheme">`

要将主题应用于整个应用程序（所有活动），请将android：theme属性添加到<application>元素中：

`<application android:theme="@style/CustomTheme">`

有关创建和使用主题的更多信息，请阅读Styles and Themes指南。





