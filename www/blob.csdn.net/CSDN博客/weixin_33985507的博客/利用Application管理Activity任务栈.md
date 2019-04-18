# 利用Application管理Activity任务栈 - weixin_33985507的博客 - CSDN博客
2016年06月01日 19:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
> 
业务逻辑复杂，突发奇想，记录一种管理 Activity任务栈的方式
..
> 
从哪里来，就回到哪里去的业务需求
A1→B1→C→D;  D回到 A1
A2→B2→C→D;  D回到 A2
这种类似的业务逻辑，很多很多，每次做起来，都比较气死宝宝。。废话不多，直接上代码，有优化建议的，直接告诉我，谢谢
...
> 
记录 一个 bug
当前页面 是 RechargeActivity  从 BuyActivity 直接到 RechargeActivity， 不可以利用Application管理事件，
手机的back finish 功能会带给 BuyActivity，导致 BuyActivity 也 跟着 finish
...
> 
清除 activityName 之上 的 所有Activity， activityName 保留状态
<pre>
//以下代码，写在Application
private static List<Activity> listActivity;
public static App qianGuanApp;
```
@Override
public void onCreate() {
    super.onCreate();
    qianGuanApp = this;
    listActivity = new ArrayList<Activity>();
}
public static App getQgApp(){
        return qianGuanApp;
}
/\*\*
 \* 清除 activityName 之上 的 所有Activity， activityName 保留状态
 \* @param activityName Activity 的名字 Activity.getClass().getSimpleName()
 \* \*/
public void clearActivityFrom(String activityName){
    if((activityName == null) || (listActivity == null)|| (listActivity.size() <= 0)){
        return ;
    }
    for (int i = (listActivity.size()-1); i>=0; i--){
        if(!activityName.equals(listActivity.get(i).getClass().getSimpleName())){
            listActivity.get(i).finish();
        }else{
            break;
        }
    }
}
private final class MyActivityLifecycleCallbacks extends SimpleActivityLifecycleCallbacks{
    @Override
    public void onActivityCreated(Activity activity, Bundle savedInstanceState){
        super.onActivityCreated(activity, savedInstanceState);
        listActivity.add(activity);
    }
    @Override
    public void onActivityDestroyed(Activity activity)      {
        super.onActivityDestroyed(activity);
        activity.finish();
        //LogUtil.e("即将移除 "+activity.getClass().getSimpleName());
        listActivity.remove(activity);
        /\*StringBuilder builder = new StringBuilder();
        builder.append("站内剩余：");
        for (int i = 0; i < listActivity.size(); i++){
            builder.append(listActivity.get(i).getClass().getSimpleName()+"    ");
        }
        LogUtil.e(builder.toString());\*/
        activity = null;
    }
}
```
package com.qiyuan.qianguan.callback;
import android.app.Activity;
import android.app.Application.ActivityLifecycleCallbacks;
import android.os.Bundle;
public abstract class SimpleActivityLifecycleCallbacks  implements ActivityLifecycleCallbacks{
@Override
public void onActivityCreated(Activity activity, Bundle savedInstanceState){
```
}
@Override
public void onActivityStarted(Activity activity){
    
}
@Override
public void onActivityResumed(Activity activity){
    
}
@Override
public void onActivityPaused(Activity activity)
{
    
}
@Override
public void onActivityStopped(Activity activity)
{
    
}
@Override
public void onActivitySaveInstanceState(Activity activity, Bundle outState)
{
    
}
@Override
public void onActivityDestroyed(Activity activity)
{
    
}
```
}
</pre>
