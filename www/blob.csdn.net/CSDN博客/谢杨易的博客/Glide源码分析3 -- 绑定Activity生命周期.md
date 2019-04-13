
# Glide源码分析3 -- 绑定Activity生命周期 - 谢杨易的博客 - CSDN博客

2016年08月07日 16:13:42[谢杨易](https://me.csdn.net/u013510838)阅读数：4899



## 1. 概述和核心类
Glide中一个重要特性是Request可以随Activity或Fragment的onStart而resume，onStop而pause，onDestroy而clear，从而节约流量和内存，并且防止内存泄露，这一切都由Glide在内部实现了。用户唯一要注意的是，Glide.with()方法中尽量传入Activity或Fragment，而不是Application，不然没办法进行生命周期管理。后面分绑定流程和生命周期调用流程来分析整个实现原理。实现这一切的核心类如下。
1）RequestManager，实现了LifeCycleListener，主要作用为结合Activity或Fragment生命周期，对Request进行管理，如pauseRequests(), resumeRequests(), clearRequests()
2） RequestManagerRetriever，获取RequestManager，和SupportRequestManagerFragment，并将二者绑定，从而在fragment的生命周期方法中可回调到RequestManager对request进行生命周期管理的相应方法。
3） SupportRequestManagerFragment, 空白Fragment，与RequestManager进行了绑定，作用为提供Fragment生命周期管理方法入口，如onStart(), onStop(), onDestroy()。
4）ActivityFragmentLifecycle, 管理LifecycleListener， 空白Fragment会回调它的onStart(), onStop(), onDestroy()
5）LifecycleListener，接口，定义生命周期管理方法，onStart(), onStop(), onDestroy(). RequestManager实现了它。
# 2.绑定流程
RequestManager和SupportRequestManagerFragment会进行绑定，流程图如下
![这里写图片描述](http://i2.piimg.com/567571/4e534343d5e8d914.png)
1）我们由Glide.with(activity）或Glide.with(fragment)将当前Activity或Fragment传入，从而Glide可以将Request和他们进行生命周期绑定。以Activity为例
```python
public
```
```python
class
```
```python
Glide {
```
```python
public
```
```python
static
```
```python
RequestManager
```
```python
with
```
```python
(Activity activity) {
```
```python
// 获取RequestManagerRetriever中的单例INSTANCE
```
```python
RequestManagerRetriever retriever = RequestManagerRetriever.
```
```python
get
```
```python
();
```
```python
// 由RequestManagerRetriever获取RequestManager
```
```python
return
```
```python
retriever.
```
```python
get
```
```python
(activity);
    }
 }
```
2）retriever.get(activity)
```python
public
```
```python
class
```
```python
RequestManagerRetriever
```
```python
implements
```
```python
Handler
```
```python
.
```
```python
Callback
```
```python
{
```
```python
public
```
```python
RequestManager
```
```python
get
```
```python
(FragmentActivity activity) {
```
```python
if
```
```python
(Util.isOnBackgroundThread()) {
```
```python
return
```
```python
get(activity.getApplicationContext());
        }
```
```python
else
```
```python
{
            assertNotDestroyed(activity);
```
```python
// 获取FragmentManager
```
```python
FragmentManager fm = activity.getSupportFragmentManager();
```
```python
// 创建空白Fragment，和RequestManager，并将二者绑定
```
```python
return
```
```python
supportFragmentGet(activity, fm);
        }
    }
```
```python
// 获取SupportRequestManagerFragment，创建RequestManager
```
```python
RequestManager supportFragmentGet(Context context, FragmentManager fm) {
```
```python
// 获取SupportRequestManagerFragment， 它是一个空白的Fragment，主要作用是利用这个Fragment进行request生命周期管理
```
```python
SupportRequestManagerFragment current = getSupportRequestManagerFragment(fm);
        RequestManager requestManager = current.getRequestManager();
```
```python
if
```
```python
(requestManager ==
```
```python
null
```
```python
) {
```
```python
// 创建RequestManager，并将SupportRequestManagerFragment中的LifeCycle传入，从而将Fragment和RequestManager这个LifeCycleListener绑定
```
```python
requestManager =
```
```python
new
```
```python
RequestManager(context, current.getLifecycle(), current.getRequestManagerTreeNode());
            current.setRequestManager(requestManager);
        }
```
```python
return
```
```python
requestManager;
    }
```
```python
// 创建SupportRequestManagerFragment
```
```python
SupportRequestManagerFragment getSupportRequestManagerFragment(
```
```python
final
```
```python
FragmentManager fm) {
```
```python
// 如果已经创建过，直接由tag可以找到。故可知，同一个Activity或父Fragment中只会创建一个SupportRequestManagerFragment
```
```python
SupportRequestManagerFragment current = (SupportRequestManagerFragment) fm.findFragmentByTag(

           FRAGMENT_TAG);
```
```python
if
```
```python
(current ==
```
```python
null
```
```python
) {
            current = pendingSupportRequestManagerFragments.get(fm);
```
```python
if
```
```python
(current ==
```
```python
null
```
```python
) {
```
```python
// 创建SupportRequestManagerFragment
```
```python
current =
```
```python
new
```
```python
SupportRequestManagerFragment();
                pendingSupportRequestManagerFragments.put(fm, current);
```
```python
// 常见的加入Fragment的方式，利用FragmentManager的一次transaction来commit这个add任务
```
```python
fm.beginTransaction().add(current, FRAGMENT_TAG).commitAllowingStateLoss();
```
```python
// fragment创建好后，将之前的FragmentManager删除掉，因为已经用不上它了，之前说过，同一个Activity或父Fragment中只会创建一个SupportRequestManagerFragment
```
```python
handler.obtainMessage(ID_REMOVE_SUPPORT_FRAGMENT_MANAGER, fm).sendToTarget();
            }
        }
```
```python
return
```
```python
current;
    }
}
```
3） new SupportRequestManagerFragment() 创建空白fragment，主要作用为与RequestManager关联，在它的生命周期方法中回调RequestManager对request生命周期管理的方法，流程图如下
![这里写图片描述](http://i1.piimg.com/567571/7aadfdb0c7e80c57.png)
```python
public
```
```python
class
```
```python
SupportRequestManagerFragment
```
```python
extends
```
```python
Fragment
```
```python
{
```
```python
private
```
```python
RequestManager requestManager;
```
```python
// lifecycle会在创建RequestManager时传入，并在RequestManager构造方法中将requestManager这个LifeCycleListener add到lifeCycle中
```
```python
private
```
```python
final
```
```python
ActivityFragmentLifecycle lifecycle;
```
```python
public
```
```python
SupportRequestManagerFragment
```
```python
() {
```
```python
// 创建LifeCycle，并传入
```
```python
this
```
```python
(
```
```python
new
```
```python
ActivityFragmentLifecycle());
    }
```
```python
@SuppressLint
```
```python
(
```
```python
"ValidFragment"
```
```python
)
```
```python
public
```
```python
SupportRequestManagerFragment
```
```python
(ActivityFragmentLifecycle lifecycle) {
```
```python
this
```
```python
.lifecycle = lifecycle;
    }
}
```
4） new ActivityFragmentLifecycle(), 实现了LifeCycle接口，回调LifeCycleListener中的onStart(), onStop(), onDestroy()方法, 如下
```python
class
```
```python
ActivityFragmentLifecycle
```
```python
implements
```
```python
Lifecycle
```
```python
{
```
```python
@Override
```
```python
public
```
```python
void
```
```python
addListener(LifecycleListener listener) {
        lifecycleListeners.add(listener);
```
```python
if
```
```python
(isDestroyed) {
            listener.onDestroy();
        }
```
```python
else
```
```python
if
```
```python
(isStarted) {
            listener.onStart();
        }
```
```python
else
```
```python
{
            listener.onStop();
        }
    }
}
```
5）new RequestManager(context, current.getLifecycle(), current.getRequestManagerTreeNode())
将空白fragment的lifeCycle传入，并将RequestManager这个listener添加到lifeCycle中，从而实现绑定。如下
```python
public
```
```python
class
```
```python
RequestManager
```
```python
implements
```
```python
LifecycleListener
```
```python
{
```
```python
// addListener()方式管理LifecycleListener，从而提供对request生命周期管理
```
```python
private
```
```python
final
```
```python
Lifecycle lifecycle;
```
```python
// request队列，以队列的方式管理request的add remove restart等
```
```python
private
```
```python
final
```
```python
RequestTracker requestTracker;
```
```python
// 构造方法
```
```python
RequestManager(Context context,
```
```python
final
```
```python
Lifecycle lifecycle, RequestManagerTreeNode treeNode,
            RequestTracker requestTracker, ConnectivityMonitorFactory factory) {
```
```python
// 省略一段代码
```
```python
// RequestManager和lifeCycle关联的关键点
```
```python
if
```
```python
(Util.isOnBackgroundThread()) {
```
```python
new
```
```python
Handler(Looper.getMainLooper()).post(
```
```python
new
```
```python
Runnable() {
```
```python
@Override
```
```python
public
```
```python
void
```
```python
run
```
```python
() {
```
```python
// 将RequestManager加入到lifecycle中，从而回调LifeCycleListener时可以回调到RequestManager中的request生命周期管理方法
```
```python
lifecycle.addListener(RequestManager.
```
```python
this
```
```python
);
                }
            });
        }
```
```python
else
```
```python
{
            lifecycle.addListener(
```
```python
this
```
```python
);
        }
        lifecycle.addListener(connectivityMonitor);
    }
}
```
# 3 生命周期回调流程
SupportRequestManagerFragment生命周期中会回调RequestManager对Request管理的相关方法，如下
1）SupportRequestManagerFragment的生命周期方法，如下
```python
public
```
```python
class
```
```python
SupportRequestManagerFragment
```
```python
extends
```
```python
Fragment
```
```python
{
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onAttach
```
```python
(Activity activity) {
```
```python
super
```
```python
.onAttach(activity);
        rootRequestManagerFragment = RequestManagerRetriever.get()
                .getSupportRequestManagerFragment(getActivity().getSupportFragmentManager());
```
```python
if
```
```python
(rootRequestManagerFragment !=
```
```python
this
```
```python
) {
            rootRequestManagerFragment.addChildRequestManagerFragment(
```
```python
this
```
```python
);
        }
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onDetach
```
```python
() {
```
```python
super
```
```python
.onDetach();
```
```python
if
```
```python
(rootRequestManagerFragment !=
```
```python
null
```
```python
) {
            rootRequestManagerFragment.removeChildRequestManagerFragment(
```
```python
this
```
```python
);
            rootRequestManagerFragment =
```
```python
null
```
```python
;
        }
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onStart
```
```python
() {
```
```python
super
```
```python
.onStart();
```
```python
// 回调lifecycle中方法
```
```python
lifecycle.onStart();
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onStop
```
```python
() {
```
```python
super
```
```python
.onStop();
        lifecycle.onStop();
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onDestroy
```
```python
() {
```
```python
super
```
```python
.onDestroy();
        lifecycle.onDestroy();
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onLowMemory
```
```python
() {
```
```python
super
```
```python
.onLowMemory();
```
```python
// If an activity is re-created, onLowMemory may be called before a manager is ever set.
```
```python
// See #329.
```
```python
if
```
```python
(requestManager !=
```
```python
null
```
```python
) {
            requestManager.onLowMemory();
        }
    }
}
```
2） 以onStart为例说明生命周期管理流程。ActivityFragmentLifecycle中的onStart如下
```python
class
```
```python
ActivityFragmentLifecycle
```
```python
implements
```
```python
Lifecycle
```
```python
{
```
```python
void
```
```python
onStart() {
        isStarted =
```
```python
true
```
```python
;
```
```python
for
```
```python
(LifecycleListener lifecycleListener : Util.getSnapshot(lifecycleListeners)) {
```
```python
// 回调lifeCycleListener中的相关方法，RequestManager就是一个lifeCycleListener，故可以回调到它里面的对request生命周期进行管理的方法
```
```python
lifecycleListener.onStart();
        }
    }
}
```
3） 在绑定阶段，RequestManager的构造方法中，将RequestManager加入到了lifeCycle中，故上面的lifeCycleListener即为RequestManager，查看RequestManager中相关方法如下
```python
public
```
```python
class
```
```python
RequestManager
```
```python
implements
```
```python
LifecycleListener
```
```python
{
```
```python
public
```
```python
void
```
```python
resumeRequests
```
```python
() {
        Util.assertMainThread();
```
```python
// 调用requestTracker队列，resume所有的requests
```
```python
requestTracker.resumeRequests();
    }
```
```python
@Override
```
```python
public
```
```python
void
```
```python
onStart
```
```python
() {
```
```python
// Activity或Fragment onStart时被回调，此时会resume所有的requests
```
```python
resumeRequests();
    }
}
```
## 4 总结
Glide中巧妙的用一个空白Fragment来实现了生命周期调用，并使用LifeCycleListener来回调相应的request管理方法。设计模式上也很规范，是一个典型的MVC，SupportRequestManagerFragment用来接入生命周期方法，RequestManager用来实现生命周期中处理request的方法，RequestManagerRetriever用来绑定二者，作为桥梁。我们在网络请求request中同样可以用此方法来实现生命周期绑定。

