# iPhone静态库基础 - 三少GG - CSDN博客
2013年03月03日 22:34:08[三少GG](https://me.csdn.net/scut1135)阅读数：911
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)
**深入理解iPhone静态库(手把手教你iphone开发 - 基础篇)**
作者：孙东风 2009-11-20（请尊重作者劳动成果，转载务必注明出处）
在实际的编程过程中，通常会把一些公用函数制成函数库，供其它程序使用，一则提搞了代码的复用；二则提搞了核心技术的保密程度。所以在实际的项目开发中，经常会使用到函数库，函数库分为静态库和动态库两种。和多数人所熟悉的动态语言和静态语言一样，**这里的所谓静态和动态是相对编译期和运行期的：静态库在程序编译时会被链接到目标代码中，程序运行时将不再需要改静态库；而动态库在程序编译时并不会被链接到目标代码中，只是在程序运行时才被载入，因为在程序运行期间还需要动态库的存在。**
iPhone官方只支持静态库联编，本文就以静态库为例讲解iPhone中静态库的具体用法。在实际的开发过程中，可能在项目设计阶段把一些功能比如图形界面引擎、网络通讯引擎等都规划到相应的函数库中。
打开XCode新建一个项目，选择Library下的“Cocoa Touch Static Library”并命名为“FirstLibrary”。这个新建的静态库项目下除了“FisrtLibrary_Prefix.pch”外没有任何程序文件，在Classes文件夹上点右键选择“New File…”，然后在“Cocoa Touch Class”下选择“Objective-C class”，笔者在这里先创建Objective-C风格的源文件和头文件并命名为“function”，可以看到在Classes目录下产生了function.h和function.m文件。接着在头文件里输入以下内容：
#import <Foundation/Foundation.h>
@interface function : NSObject {
}
- (int)getMax:(int)a b:(int)b;
- (int)getMin:(int)a b:(int)b;
@end
实现文件function.m内容如下：
#import "function.h"
@implementation function
- (int)getMax:(int)a b:(int)b
{
    return a > a?a:b;
}
- (int)getMin:(int)a b:(int)b
{
    return a < b?a:b;
}
@end
可见，笔者的这个静态函数库的功能很简单，就是提供了两个获取最大值和最小值的函数。但是可能大多数现存的函数库的源码都是C或者C++格式的，没关系，iPhone对C/C++支持的也很好。在Classes文件夹上点右键，选择“New File…”，这次选择Mac OS X下的“C and C++”，之后选择“C File”并命名为“TestCFunction”，可以看到在Classes目录下产生了两个文件“TestCFunction.h”和“TestCFunction.c”，TestCFunction.h如下：
#ifndef TestCFunction_H_
#define TestCFunction_H_
void swapValue(int* a,int* b)
{
    int temp = 0;
    temp = *a;
    *a = *b;
    *b = temp;
}
#endif
TestCFunction.c文件内保持为原样。
至此，这个静态函数库已经编写完毕，尽管很简单，但是包含了Objective-C风格和C风格的源代码文件在内，编译这个程序会看到在Products目录下产生了名为“libFirstLibrary.a”的静态库文件。
新建一个“Window-based Application”项目并命名为“FirstLibraryTest”，下面笔者会演示如何在这个新项目里利用前面生成的静态库libFristLibrary.a文件。
¨         首先，拖拉静态库里的“FirstLibrary.xcodeproj”文件到新项目的Frameworks目录下（当然也可以通过右键点Frameworks->Add->Existing Files..添加），如下图：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686505B4q2.png)
图1
¨         然后选择“FristLibrary.xcodeproj”文件，选择右边编辑窗口中的最后一项，这样就将静态库添加到项目中了，如下图：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686506G7WO.png)
图2
¨         在将静态库添加到项目中后，需要在项目和静态库之间建立“Direct Dependencies（所谓的依赖关系）”。在“Targets”目录下选择“FirstLibraryTest”，然后快捷键Cmd+i（或者右键选择Get Info）可以看到项目的依赖信息，如下图：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686506y2We.png)
图3
点击“Direct Dependencies”下方的“+”按钮，会出现下图：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686506N51a.png)
图4
选择“FirstLibrary”并点击“Add Target”，这样就建立了项目和静态库之间的“依赖关系”，在项目编译阶段，程序会到静态库中查找相应的目标文件。
¨         接下来使用静态库中的函数，修改头文件FirstLibraryTestAppDelegate.h如下：
#import <UIKit/UIKit.h>
#import "function.h"
#include "TestCFunction.h"
@interface FirstLibraryTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    function* iFunc;
}
@property (nonatomic, retain) IBOutlet UIWindow *window;
@end
修改相应的实现文件如下：
#import "FirstLibraryTestAppDelegate.h"
@implementation FirstLibraryTestAppDelegate
@synthesize window;
- (void)applicationDidFinishLaunching:(UIApplication *)application {   
    iFunc = [[function alloc] init];
    int a = 3,b = 6;
    int max = 0,min = 0;
    max = [iFunc getMax:a b:b];
    min = [iFunc getMin:a b:b];
    NSLog(@"max = %d,min = %d",max,min);
    int c = 8,d = 22;
    swapValue(&c, &d);
    NSLog(@"after swapValue c = %d,d = %d",c,d);
    // Override point for customization after application launch
    [window makeKeyAndVisible];
}
- (void)dealloc {
    [window release];
    [super dealloc];
}
@end
编译下，会发现项目无法编译通过，错误信息如下：
error: function.h: No such file or directory
就是说，项目无法找到相应的静态库的头文件，从而也就无法在项目和静态库之间建立“依赖关系”，所以需要在项目的“依赖信息选项”里告诉项目它所依赖的静态库头文件的位置信息，如下图设置静态库头文件的位置信息：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686507ZwDA.png)
图5
最后再编译一下，顺利通过，输出信息如下：
[Session started at 2009-11-20 10:52:22 +0800.]
2009-11-20 10:52:23.805 FirstLibraryTest[768:207] max = 6,min = 3
2009-11-20 10:52:23.807 FirstLibraryTest[768:207] after swapValue c = 22,d = 8
有时候程序会出现如下错误信息：
![](http://hi.csdn.net/attachment/200911/20/649618_1258686508eK2w.png)
图6
这是因为在静态库里使用了Objective-C风格的源文件，需要在项目的“依赖关系信息项”里设置如下：
![](http://hi.csdn.net/attachment/200911/20/649618_125868651000Oz.png)
图7
添加内容项为“-ObjC”。
**[iPhone](http://bbs.189works.com/)项目**使用**静态库**最佳方法是本文要介绍的内容，主要是来学习如何使用**[iphone](http://www.189works.com/tech/devloplat/Iphone/)**中的**静态库**。由于Apple禁止在**iPhone**中使用外部框架或动态链接库，所以要重用代码（第三方库或自己的代码）只能选择如下方法：
直接将代码加入项目。这种方法有点笨。例如，你的一段代码被五个项目共享，如果你发现一个小bug，那么你需要更改五个地方。够麻烦吧？
**使用静态库。**这是我经常使用的方法。但Xcode并未为iPhone提供静态库的模板。有两种解决方法：
如果你的项目主要是使用Objective-C（C/C++也可使用，只要做些简单修改），那么你可使用现成的Xcode 模板。 通过Mac OS X栏下的Static Library中的BSD Static Library建立一个静态库。但要用于iPhone项目，还需做些修改（需要将项目的Base SDK改为iPhone）。 
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643351.jpg)
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643352.jpg)
但怎样在项目中链接静态库呢？如果你熟悉Xcode，那么你可跳过此段。很简单：
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643353.jpg)
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643354.jpg)
指定静态库的头文件路径Head Search Paths。
添加静态库到Xcode项目中。
就这么简单。但是且慢，万事大吉了吗？如果你要修改你的静态库呢？你需要从Find中找到你的静态库项目，打开它修改。有点麻烦，不能像 Visual Studio一样将相关项目包括在应用项目中吗？答案是：可以，虽然还是没有Visual Studio中的工作空间好用。但你可以从Xcode中直接打开相关的静态库。方法是使用Xcode的Cross-Project Reference(跨项目参考)。
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643355.jpg)
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643356.jpg)
![解析Iphone项目使用静态库最佳方法](http://www.189works.com/data/attachment/portal/et2/201211/et69484160643357.jpg)
将静态库的Xcode项目加入到当前项目。
右击Targets下本项目，添加到静态库项目的关联。
在项目下，扩展静态库项目，拖动xxxx.a到Targets下的Link Binary With Libraries。
这样你就可以从当前项目下，双击静态库的项目来打开和修改静态库的代码了。
小结：解析**iPhone项目**使用**静态库**最佳方法的内容介绍完了，希望通过本文的学习能对你有所帮助！
概要 
- **AppStore中的 iphone 程序不允许使用动态链接库。  **
- **如果需要在隐藏代码的前提下提供某些功能，可以使用静态链接库。**
XCode项目中添加静态库的方法
- 右击target，选择add -> new target  
- 在弹出的 NewTarget 界面中选择Cocoa Touch -> Static Library ，为新 library 取个名字。 
- 将源文件添加到这个库。可以直接拖动现存源文件，也可以右击 -> add 。注意选择新建的库为添加的 target 。 
- 设置原项目，令其使用新建的库。 
- 右击原项目的 target ，选择 getInfo 。 
- 选择 General 选项卡 
- 向 linked libraries 中添加新创建的库 （即添加 .a 文件）。
- 编译新建的静态库以产生 .a 文件。（右击静态库的 target ，选择build） 
- 编译原项目，若可以调用静态库中的方法则添加成功。
**怎样隐藏静态库的代码 **
- **可以直接删除静态库中的源代码，只提交链接库文件（.a），这样原项目仍然可以编译执行。**
备注：
- Mac OS 下的静态链接库文件与linux 同样使用 .a 扩展名。
iPhone如何打包成一个通用的静态库文件(模拟器和真机都可以用)？
iPhone如何打包成一个通用的静态库文件(模拟器和真机都可以用)？ 
最近在做一个购票项目，支付与银联合作，他们提供2个.a的静态库文件，一个真机使用，一个是模拟器使用。但是每次真机测试的时候，都要换成真机的.a库，模拟器测试的时候又要换回来，特别麻烦。一天几十次真机和模拟器测试，来回折腾浪费了很多时间，我终于受不了，网上也没一个完整的解决方案，google了下方法，最后通过自己的实践，成功的解决了.a静态库文件通用性问题，希望这个能给大家带来方便，痛苦的童鞋们，可以解脱了!
1.先来一个必须的命令： 
~/Library目录在lion下是默认隐藏的。这样很不方便。用一下命令可以使其显示： 
chflags nohidden ~/Library 
如果想再让其隐藏，可以： 
chflags hidden ~/Library 
2.静态库工程的建立：Xcode New一个新的project，选择IOS下面的Framework&Library，下面有一个Cocoa Touch Static Library。直接next去建立一个MtimeLibrary这样的工程。(里面的功能就不要关注了，简单 2个数相加)
￼ 
![](http://dl.iteye.com/upload/attachment/0065/3770/a9e394a1-b0d1-3f6d-8746-2175606f23a3.png)
3.工程建立以后，删除默认的.h .m 文件，然后自己创建CountNumbers..h CountNumbers..m文件。如下图： 
![](http://dl.iteye.com/upload/attachment/0065/3772/4ffee79a-ff58-30d6-bcbd-953d69ed02d3.png)
4.OK，选择iPhone 5.1Simulator ,Command + B 编译，我们可以在Products下面找到我们编译生成一个模拟器上可运行的libMtimeLibrary.a这个文件,然后选择真机（如下图）再编译一次，这样会生成一个可以真机运行的libMtimeLibrary.a库。
￼ 
![](http://dl.iteye.com/upload/attachment/0065/3774/32d84ea8-5846-3c2b-98c4-a8bb9f56c188.png)
5. 在libMtimeLibrary.a 右键 Open in Finder，可以找到libMtimeLibrary.a所在路径、后面我们会新建一个项目来添加这个.a文件测试。
打开终端：输入以下命令（路径根据自己的来决定） 
cd  /Users/cash/Library/Developer/Xcode/DerivedData/MtimeLibrary-amyqbnwwzcivnyeijggzaorseihj/Build/Products/
这样就到了Products目录下了，ls 以后可以看到：  
￼ 
再输入一个命令： cd  Debug-iphonesimulator/ 
然后通过下面命令来查看libMtimeLibrary.a信息 
命令：lipo -info libMtimeLibrary.a 
显示： 
cashmatoMacBook-Pro:Debug-iphonesimulator cash$ lipo -info libMtimeLibrary.a 
input file libMtimeLibrary.a is not a fat file 
Non-fat file: libMtimeLibrary.a is architecture: i386 
i386是mac上的架构。 
然后再输入下面命令： 
cd ../ 
cd   Debug-iphoneos/ 
继续通过命令来查看  lipo -info libMtimeLibrary.a， 
显示： 
cashmatoMacBook-Pro:Debug-iphoneos cash$ lipo -info libMtimeLibrary.a 
input file libMtimeLibrary.a is not a fat file 
Non-fat file: libMtimeLibrary.a is architecture: armv7 
armv7是iOS上的jia'ge架构。 
到这里我们也明白了，为什么一个在真机上可以使用，一个只能在模拟器上使用了吧！ 
最后我们要做的就是要让这个libMtimeLibrary.a文件同时有i386和armv7信息，这样就是一个通用的静态库文件了。 
6. 新建一个MtimeLibraryDemo应用，然后把真机或者模拟器的libMtimeLibrary.a 和CountNumbers.h文件引入进来，如图： 
￼ 
![](http://dl.iteye.com/upload/attachment/0065/3776/adb071a4-c33c-392a-9bf8-18c7459ce736.png)
添加CountNumbers.h文件到需要调用的类里面，然后就可以调用libMtimeLibrary.a里面的方法了。 
// 
//  AppDelegate.m 
//  MtimeLibraryDemo 
// 
//  Created by cash on 12-3-23. 
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved. 
// 
#import "AppDelegate.h" 
#import "CountNumbers.h" 
@implementation AppDelegate 
@synthesize window = _window; 
- (void)dealloc 
{ 
    [_window release]; 
    [super dealloc]; 
} 
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{ 
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch. 
    CountNumbers *cn = [[CountNumbers alloc] init]; 
    int count = [cn addTwoNumbers:10 :20]; 
    NSLog(@"count:%d",count); 
    self.window.backgroundColor = [UIColor whiteColor]; 
    [self.window makeKeyAndVisible]; 
    return YES; 
} 
7. OK，编译运行这个应用程序。 错误了。。这个就是这篇文档的关键了. 
￼ 
![](http://dl.iteye.com/upload/attachment/0065/3778/c0524b8f-6485-34f8-bd14-bb183ec6008d.png)
我们找到刚才生成真机和模拟器的libMtimeLibrary.a目录。  
debug-iphoneos里面的是基于arm6 arm7编译出来的库文件。debug-iphonesimulator文件夹下面的是基于i386编译出来的文件。 
￼ 
![](http://dl.iteye.com/upload/attachment/0065/3780/eda13302-6858-36cb-9af6-164245fb85e2.png)
10.  最后是关键步骤： 
通过lipo -create 命令来合并2个静态库文件。（-output 后面的/Users/cash/Desktop/test/libMtimeLibrary.a 是合并后的路径和文件名字）
lipo -create "/Users/cash/Library/Developer/Xcode/DerivedData/MtimeLibrary-amyqbnwwzcivnyeijggzaorseihj/Build/Products/Debug-iphonesimulator/libMtimeLibrary.a" "/Users/cash/Library/Developer/Xcode/DerivedData/MtimeLibrary-amyqbnwwzcivnyeijggzaorseihj/Build/Products/Debug-iphoneos/libMtimeLibrary.a"
 -output "/Users/cash/Desktop/test/libMtimeLibrary.a"    
成功！ 
最后通过命令 lipo -info libMtimeLibrary.a  可以知道这个文件已经有i386和armv7信息了。 
cashmatoMacBook-Pro:test cash$ lipo -info libMtimeLibrary.a 
Architectures in the fat file: libMtimeLibrary.a are: i386 armv7  
cashmatoMacBook-Pro:test cash$  
//银联合并成通用静态库 
lipo -create "/Users/cash/Desktop/iphonesimulator/upomp_bypay/lib/libUPOMP.a" "/Users/cash/Desktop/iphoneos/upomp_bypay/lib/libUPOMP.a" -output "/Users/cash/Desktop/test/libUPOMP.a"   
