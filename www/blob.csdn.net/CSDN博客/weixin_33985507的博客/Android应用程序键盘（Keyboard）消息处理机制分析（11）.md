# Android应用程序键盘（Keyboard）消息处理机制分析（11） - weixin_33985507的博客 - CSDN博客
2011年10月24日 00:59:11[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Step 8. NativeInputManager.setInputWindows
这个函数定义在frameworks/base/services/jni/com_android_server_InputManager.cpp 文件中：
- void NativeInputManager::setInputWindows(JNIEnv* env, jobjectArray windowObjArray) {  
-     Vector<InputWindow> windows;  
- 
-     jsize length = env->GetArrayLength(windowObjArray);  
- for (jsize i = 0; i < length; i++) {  
-         jobject inputTargetObj = env->GetObjectArrayElement(windowObjArray, i);  
- if (! inputTargetObj) {  
- break; // found null element indicating end of used portion of the array
-         }  
- 
-         windows.push();  
-         InputWindow& window = windows.editTop();  
- bool valid = populateWindow(env, inputTargetObj, window);  
- if (! valid) {  
-             windows.pop();  
-         }  
- 
-         env->DeleteLocalRef(inputTargetObj);  
-     }  
- 
-     mInputManager->getDispatcher()->setInputWindows(windows);  
- }  
        这个函数首先将Java层的Window转换成C++层的InputWindow，然后放在windows向量中，最后将这些输入窗口设置到InputDispatcher中去。
        Step 9. InputDispatcher.setInputWindows
        这个函数定义在frameworks/base/libs/ui/InputDispatcher.cpp文件中：
- void InputDispatcher::setInputWindows(const Vector<InputWindow>& inputWindows) {  
-     ......  
- 
-     { // acquire lock
-         AutoMutex _l(mLock);  
- 
- // Clear old window pointers.
-         sp<InputChannel> oldFocusedWindowChannel;  
- if (mFocusedWindow) {  
-             oldFocusedWindowChannel = mFocusedWindow->inputChannel;  
-             mFocusedWindow = NULL;  
-         }  
- 
-         mWindows.clear();  
- 
- // Loop over new windows and rebuild the necessary window pointers for
- // tracking focus and touch.
-         mWindows.appendVector(inputWindows);  
- 
- size_t numWindows = mWindows.size();  
- for (size_t i = 0; i < numWindows; i++) {  
- const InputWindow* window = & mWindows.itemAt(i);  
- if (window->hasFocus) {  
-                 mFocusedWindow = window;  
- break;  
-             }  
-         }  
- 
-         ......  
- 
-     } // release lock
- 
-     ......  
- }  
        这里InputDispatcher的成员变量mFocusedWindow就代表当前激活的窗口的。这个函数首先清空mFocusedWindow，然后再通过一个for循环检查当前的输入窗口中的哪一个窗口是获得焦点的，获得焦点的输入窗口即为当前激活的窗口。
        这样，InputManager就把当前激活的Activity窗口保存在InputDispatcher中了，后面就可以把键盘消息分发给它来处理。
        回到Step 1中的ViewRoot.setView函数中，接下来就调用下面语句来注册键盘消息接收通道的一端到InputManager中去：
- mInputChannel = new InputChannel();  
- try {  
-     res = sWindowSession.add(mWindow, mWindowAttributes,  
-             getHostVisibility(), mAttachInfo.mContentInsets,  
-             mInputChannel);  
- } catch (RemoteException e) {  
-     ......  
- } finally {  
-     ......  
- }  
        前面说过，这里的sWindowSession是WindowManagerService内部类Session的一个远程接口，通过它可以进入到WindowManagerService中去。
        Step 10. WindowManagerService.Session.add
        这个函数定义在frameworks/base/services/java/com/android/server/WindowManagerService.java 文件中：
- publicclass WindowManagerService extends IWindowManager.Stub  
- implements Watchdog.Monitor {  
-     ......  
- 
- privatefinalclass Session extends IWindowSession.Stub  
- implements IBinder.DeathRecipient {  
-         ......  
- 
- publicint add(IWindow window, WindowManager.LayoutParams attrs,  
- int viewVisibility, Rect outContentInsets, InputChannel outInputChannel) {  
- return addWindow(this, window, attrs, viewVisibility, outContentInsets,  
-                 outInputChannel);  
-         }  
- 
-         ......  
-     }  
- 
-     ......  
- }  
        这里调用WindowManagerService类的addWindow函数来进一步执行操作。
        Step 11. WindowManagerService.addWindow
        这个函数定义在frameworks/base/services/java/com/android/server/WindowManagerService.java 文件中：
- publicclass WindowManagerService extends IWindowManager.Stub  
- implements Watchdog.Monitor {  
-     ......  
- 
- publicint addWindow(Session session, IWindow client,  
-             WindowManager.LayoutParams attrs, int viewVisibility,  
-             Rect outContentInsets, InputChannel outInputChannel) {  
-         ......  
- 
-         WindowState win = null;  
- 
- synchronized(mWindowMap) {  
-             ......  
- 
-             win = new WindowState(session, client, token,  
-                 attachedWindow, attrs, viewVisibility);  
- 
-             ......  
- 
- if (outInputChannel != null) {  
-                 String name = win.makeInputChannelName();  
-                 InputChannel[] inputChannels = InputChannel.openInputChannelPair(name);  
-                 win.mInputChannel = inputChannels[0];  
-                 inputChannels[1].transferToBinderOutParameter(outInputChannel);  
-                 mInputManager.registerInputChannel(win.mInputChannel);  
-             }  
- 
-             ......  
-         }  
- 
-         ......  
-     }  
- 
-     ......  
- }  
        这里的outInputChannel即为前面在Step 1中创建的InputChannel，它不为NULL，因此，这里会通过InputChannel.openInputChannelPair函数来创建一对输入通道，其中一个位于WindowManagerService中，另外一个通过outInputChannel参数返回到应用程序中：
- inputChannels[1].transferToBinderOutParameter(outInputChannel);  
        创建输入通道之前，WindowManagerService会为当前Activity窗口创建一个WindowState对象win，用来记录这个Activity窗口的状态信息。当创建这对输入管道成功以后，也会把其中的一个管道保存在这个WindowState对象win的成员变量mInputChannel中，后面要注销这个管道的时候，就是从这个WindownState对象中取回这个管道的：
- win.mInputChannel = inputChannels[0];  
        接下来我们就看一下InputChannel.openInputChannelPair函数的实现。
