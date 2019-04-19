# 'Application windows are expected to have a root view controller at the end of application launch' - 三少GG - CSDN博客
2016年02月18日 23:19:49[三少GG](https://me.csdn.net/scut1135)阅读数：1740
http://stackoverflow.com/questions/30884896/application-windows-are-expected-to-have-a-root-view-controller-at-the-end-of-a
# [“Application windows are expected to have a root view controller at the end of application launch” error when running a project with Xcode 7, iOS 9](http://stackoverflow.com/questions/30884896/application-windows-are-expected-to-have-a-root-view-controller-at-the-end-of-a)
After running function
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
```
there is a crash:
```
Assertion failure in 
-[UIApplication _runWithMainScene:transitionContext:completion:], /BuildRoot/Library/Caches/com.apple.xbs/Sources/UIKit_Sim/UIKit-
 *** Terminating app due to uncaught exception 'NSInternalInconsistencyException', `enter code here`reason: 'Application windows are expected to have a root view controller at the end of application launch'
*** First throw call stack:
(
    0   CoreFoundation                      0x0000000109377885 __exceptionPreprocess + 165
    1   libobjc.A.dylib                     0x0000000108df0df1 objc_exception_throw + 48
    2   CoreFoundation                      0x00000001093776ea +[NSException raise:format:arguments:] + 106
    3   Foundation                          0x0000000108a42bb1 -[NSAssertionHandler handleFailureInMethod:object:file:lineNumber:description:] + 198
    4   UIKit                               0x000000010760e350 -[UIApplication _runWithMainScene:transitionContext:completion:] + 2875
    5   UIKit                               0x000000010760b73f -[UIApplication workspaceDidEndTransaction:] + 188
    6   FrontBoardServices                  0x000000010b87fd7b FrontBoardServices + 163195
    7   FrontBoardServices                  0x000000010b880118 FrontBoardServices + 164120
    8   CoreFoundation                      0x00000001092a20f1 __CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE0_PERFORM_FUNCTION__ + 17
    9   CoreFoundation                      0x0000000109297eac __CFRunLoopDoSources0 + 556
    10  CoreFoundation                      0x0000000109297363 __CFRunLoopRun + 867
    11  CoreFoundation                      0x0000000109296d78 CFRunLoopRunSpecific + 488
    12  UIKit                               0x000000010760b091 -[UIApplication _run] + 402
    13  UIKit                               0x000000010760f79b UIApplicationMain + 171
    14  bbwc                                0x00000001037a9998 main + 344
    15  libdyld.dylib                       0x000000010a45ca05 libdyld.dylib + 10757
    16  ???                                 0x0000000000000001 0x0 + 1
)
libc++abi.dylib: terminating with uncaught exception of type NSException
```
This project is an old project, what should I do to make it build and run with Xcode 7 and iOS 9?
|63down voteaccepted|From your error message:> Application windows are expected to have a root view controller at the end of application launchHow old is this "old" project? If it's more than a few years, do you still have:```[window addSubview:viewController.view];```**You should instead replace it with:**`**[window setRootViewController:viewController];**`|
