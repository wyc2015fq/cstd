# 如何编译 smspopup---出自Android平台值得关注的开源项目 - 三少GG - CSDN博客
2012年01月19日 15:59:36[三少GG](https://me.csdn.net/scut1135)阅读数：1218
# [Android平台值得关注的开源项目](http://developer.51cto.com/art/201008/217158.htm)
之 android-smspopup  [http://code.google.com/p/android-smspopup/source/checkout](http://code.google.com/p/android-smspopup/source/checkout)
此代码引用了库项目！！！
Android-ViewPagerIndicator
   该项目可以用于实现工作区的UI设计。 
（摘自 
## [免费的Android UI库及组件推荐](http://www.strongd.net/?p=1060)
[项目网站](https://github.com/JakeWharton/Android-ViewPagerIndicator)/[示例](https://market.android.com/details?id=com.viewpagerindicator.sample)
重点：  android官网：[http://developer.android.com/guide/developing/projects/projects-eclipse.html#ReferencingLibraryProject](http://developer.android.com/guide/developing/projects/projects-eclipse.html#ReferencingLibraryProject)
## Setting up a Library Project
A library project is a standard Android project, so you can create a new one in the same way as you would a new application project.
When you are creating the library project, you can select any application name, package, and set other fields as needed, as shown in figure 1.
Next, set the project's properties to indicate that it is a library project:
- 
In the **Package Explorer**, right-click the library project and select **Properties**.
- 
In the **Properties** window, select the "Android" properties group at left and locate the **Library** properties at right.
- 
Select the "is Library" checkbox and click **Apply**.
- 
Click **OK** to close the *Properties* window.
## Referencing a library project
If you are developing an application and want to include the shared code or resources from a library project, you can do so easily by adding a reference to the library project in the application project's Properties.
To add a reference to a library project, follow these steps:
- 
In the **Package Explorer**, right-click the dependent project and select **Properties**.
- 
In the **Properties** window, select the "Android" properties group at left and locate the **Library** properties at right.
- 
Click **Add** to open the **Project Selection** dialog.
- 
From the list of available library projects, select a project and click **OK**.
- 
When the dialog closes, click **Apply** in the **Properties** window.
- 
Click **OK** to close the **Properties** window.
1. 详细的库引用项目中文介绍：
[http://www.ogeo.com.tw/root5/android-fb-p01-01.asp](http://www.ogeo.com.tw/root5/android-fb-p01-01.asp)
左上方的工具列中 File -> New -> Android Project 填入以下的參數在選 Finish 即可。
[http://www.ogeo.com.tw/root5/android-fb-p01-02.asp](http://www.ogeo.com.tw/root5/android-fb-p01-02.asp)
﻿﻿ 進入視窗之後，在左邊點選 ｢Android｣ 標籤，在右下方的方框中可以看到 library reference 表列，建議您先點選所有的條列，然後 ｢Remove｣ 刪除，再點選
 ｢Add...｣ 應該會看到為一的選擇 FacebookSDK 這個選項，選擇之後按 ｢OK｣ 就成功了。
2. 
[](http://dongxi.net/content/bilingual/left/b04mD)[使用库项目开发Android应用](http://dongxi.net/content/bilingual/left/b04mD)
Android库项目是一个开发项目，拥有共享的Android源代码和资源。其它的Android应用项目能够引用库项目，而且在构建时能将其源代码编译到.apk文件中。多个应用项目能够引用相同的库项目，并且任何单个应用项目能够引用多个库项目。
如果你有在多个项目中通用的源代码和资源，你可以将它们移到一个库项目中，这样更容易在多个应用和版本中进行维护。下面是一些使用库项目的常见场景：
·如果您正在开发多个相关的应用，它们使用了一些相同的组件，你应该从各自的应用项目中移除这些冗余的组件，并在库项目中创建单独的、可重复使用的组件集。
·如果您正在创建的应用程序有免费和付费版本，你应该将这两个版本的通用部分移到一个库项目。使用不同包名的两个相关项目都将引用库项目，并仅提供两个应用版本之间差异的部分。
从结构上来讲，一个库项目和标准的Android应用项目差不多。例如，在项目根目录中包含了清单文件，与src/,
 res/一样类似的目录。库项目能够包含与标准Android项目相同类型的源代码和资源，并用相同的方式存储。例如，库项目的源代码能够通过它的R类来访问自身的资源。
然而，库项目与标准的Android应用项目的不同点在于您不能将其直接编译为.apk文件或在Android平台运行它们。同样，您不能像真正的库那样将库项目导出为自包含的JAR文件。相反，你必须间接编译相关应用的构建路径中所引用的库，然后构建应用。
当您依赖一个库项目来构建应用时，SDK工具编译这些库，并将其源代码合并到主项目中，然后再生成.apk文件。一个资源ID同时在应用和库中被定义，工具确保在应用中声明的资源获得优先权，不会将库项目中的资源编译到应用的.apk文件。这给了应用程序灵活性，是使用还是重新定义任何库中任何资源的行为或值。
要进一步组织代码，您的应用可以增加多个库项目的引用，然后指定每个库中资源的相对优先级。这让您采用累积的方式，在应用中建立起实际使用的资源。当一个应用引用定义了相同资源ID的两个库，这些工具会从库中选择较高优先级的资源，并放弃另一个。
一旦您加入了引用，这些工具会让您编辑应用项目的构建参数来设置它们的相对优先级。在构建时，这些工具按照优先级从低到高，将库一个一个地与应用合并。
请注意，库项目本身不能引用其它库项目，并且在构建时，在没有和应用合并之前，库项目之间不能彼此合并。不过，请注意，库项目能够使用正常的方式来导入一个外部库(JAR)。
下面的章节将描述如何使用ADT来建立和管理您的项目库。一旦您建立了库项目，并向它们移入了源代码，您就能够使用正常的方式向应用导入库的类和资源。
++++++++++++++++++++++
补充知识：
ViewPager是google SDk中自带的一个附加包的一个类（在android-compatibility这个文件夹下），可以用来实现屏幕间的切换。
（摘自 [](http://blog.csdn.net/shaojie519/article/details/7028601)
### **[android-Viewpager ﻿﻿﻿﻿﻿﻿](http://blog.csdn.net/shaojie519/article/details/7028601)）**
博主其他android编程:[http://blog.csdn.net/shaojie519/article/category/824779?viewmode=list](http://blog.csdn.net/shaojie519/article/category/824779?viewmode=list)
