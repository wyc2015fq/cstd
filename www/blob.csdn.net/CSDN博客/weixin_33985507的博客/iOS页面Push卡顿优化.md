# iOS页面Push卡顿优化 - weixin_33985507的博客 - CSDN博客
2019年01月29日 23:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：25
在了解这个问题之前，我们先复习一下：[iOS 视图生命周期](https://www.jianshu.com/p/e36a5d64ede2)
> 
initWithCoder:(NSCoder *)aDecoder：使用Storyboard或者Xib创建View的的时候
loadView：加载view的时候
viewDidLoad：view加载完毕的时候
viewWillAppear：控制器的view将要显示的时候
viewWillLayoutSubviews：控制器的view将要布局子控件的时候
viewDidLayoutSubviews：控制器的view布局子控件完成的时候
viewDidAppear:控制器的view完全显示的时候
viewWillDisappear：控制器的view即将消失的时候
viewDidDisappear：控制器的view完全消失的时候
deinit：控制器销毁的时候
**首先，我们新建一个空的工程，在工程中添加两个控制器：页面01，页面02。然后重写以下方法：**
> 
override func loadView() {
super.loadView()
print("--- 页面01 - loadView 创建View ---")
}
override func viewDidLoad() {
super.viewDidLoad()
print("--- 页面01 - viewDidLoad 初始化完毕 ---")
}
override func viewWillAppear(_ animated: Bool) {
print("--- 页面01 - viewWillAppear 将要显示 ---")
}
override func viewDidAppear(_ animated: Bool) {
print("--- 页面01 - viewDidAppear 显示 ---")
}
override func viewWillDisappear(_ animated: Bool) {
print("--- 页面01 - viewWillDisappear 将要消失 ---")
}
override func viewDidDisappear(_ animated: Bool) {
print("--- 页面01 - viewDidDisappear 消失 ---")
}
deinit {
print("--- 页面01 - deinit 销毁 ---")
}
**页面02类似，也是重写以上方法。**
接下来是重点了：
**1、启动工程，终端日志：**
> 
---页面01 - loadView创建View ---
---页面01 - viewDidLoad初始化完毕---
---页面01 - viewWillAppear将要显示---
---页面01 - viewDidAppear显示---
**2、页面01 Push到 页面02，终端日志：**
> 
---页面02 - loadView创建View ---
---页面02 - viewDidLoad初始化完毕---
---页面01 - viewWillDisappear将要消失---
**---页面02 - viewWillAppear将要显示---**
---页面01 - viewDidDisappear消失---
**分析：**
在页面02显示之前（---页面02 - viewWillAppear将要显示---），分别调用了三个方法，如果在这三个方法中有耗时操作，那么就会造成卡顿。
**我们修改一下代码，在页面01中加入耗时操作：**
> 
override func viewWillDisappear(_ animated: Bool) {
print("--- 页面01 - viewWillDisappear 将要消失 ---")
sleep(3)
}
此时由页面01Push到页面02，就会卡顿（可以看到，按钮已经响应了事件，但页面02没有Push过来）。
![2559321-fbcd0b68b6fb9b4f.gif](https://upload-images.jianshu.io/upload_images/2559321-fbcd0b68b6fb9b4f.gif)
卡顿
然后我们把耗时的代码修改到：
> 
override func viewDidDisappear(_animated:Bool) {
print("--- 页面01 - viewDidDisappear 消失 ---")
sleep(3)
}
再次由页面01Push到页面02，不会卡顿。
![2559321-1f517f9cd763115d.gif](https://upload-images.jianshu.io/upload_images/2559321-1f517f9cd763115d.gif)
流畅
**3、页面02Pop回页面01，终端日志：**
> 
---页面02 - viewWillDisappear将要消失---
**---页面01 - viewWillAppear将要显示---**
---页面02 - viewDidDisappear消失---
---页面01 - viewDidAppear显示---
---页面02 - deinit销毁---
**分析：**
在页面01再次显示的时候（---页面01 - viewWillAppear将要显示---），会调用页面02的viewWillDisappear方法，如果在这个方法中有耗时操作，就会引起Pop时卡顿。
**总结：**
在开发时，不要将耗时操作放在页面显示之前调用。比如：如果一个页面消失时需要执行一些操作，可以放在viewDidDisappear方法中执行，不要放在viewWillDisappear中。
Android开发中，如果在主线有超过5秒的耗时操作，就会导致 Force Close。虽然iOS没有这种机制，系统不会杀掉我们的应用，但如果有这种情况，建议放在子线程中去执行。
