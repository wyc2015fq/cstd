# android 关于Button在StateListDrawable 中使用AnimationDrawable的问题 - weixin_33985507的博客 - CSDN博客
2016年05月05日 21:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
最近在实现一个button的效果时，UI要求未点击时，button有相应的动画，按住时有相应的效果，于是自然而然的想到了使用StateListDrawable ，以及帧动画。
遇到了一个异常：android.graphics.drawable.StateListDrawable cannot be cast to android.graphics.drawable.AnimationDrawable。
以下是我的相应代码：
xml中：
```
<Button android:id="@+id/setting_user_rank_id"
            android:layout_width="@dimen/person_setting_user_rank_btn_width"
            android:layout_height="@dimen/person_setting_user_rank_btn_height"
            android:layout_marginRight="@dimen/person_setting_user_rank_btn_margin_right"
            android:layout_marginTop="@dimen/person_setting_user_rank_btn_margin_top"
            android:background="@drawable/person_setting_user_rank_style"
            android:layout_alignParentRight="true"
            android:onClick="onClick"/>
```
person_setting_user_rank_style.xml:
```
<?xml version="1.0" encoding="UTF-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android">
   <item android:drawable="@drawable/person_setting_user_rank_pressed" android:state_pressed="true"/>
   <item android:drawable="@anim/user_rank_btn_anim" android:state_focused="true"/>
   <item android:drawable="@anim/user_rank_btn_anim" />
</selector>
```
res/anim/user_rank_btn_anim.xml:
```
<?xml version="1.0" encoding="utf-8"?>
<animation-list
  xmlns:android="http://schemas.android.com/apk/res/android"
  android:oneshot="false">
    <item android:drawable="@drawable/user_rank_btn_bg_0" android:duration="125"></item>
    <item android:drawable="@drawable/user_rank_btn_bg_1" android:duration="125"></item>
    <item android:drawable="@drawable/user_rank_btn_bg_2" android:duration="125"></item>
    <item android:drawable="@drawable/user_rank_btn_bg_3" android:duration="125"></item>
    <item android:drawable="@drawable/user_rank_btn_bg_4" android:duration="125"></item>
</animation-list>
```
一开始在activity中是直接调用了：
```
mUserRankAnim= (AnimationDrawable) mUserRankBtn.getBackground(); 
mUserRankAnim.start();
```
结果出现了：
android.graphics.drawable.StateListDrawable cannot be cast to android.graphics.drawable.AnimationDrawable异常。
由于button的android:background采用的是StateListDrawable ，那么mUserRankBtn.getBackground()得到的也就会是StateListDrawable ，而不是AnimationDrawable，所以会出现这个异常。
## 正确的写法是：
```
private void startUserRankAnim() {
        //让按钮获取到焦点（为了进入activity后自动开始动画，如果是点击button后才开始动画，那么可以不用获取焦点）
        mUserRankBtn.setFocusable(true);
        mUserRankBtn.setFocusableInTouchMode(true);
        mUserRankBtn.requestFocus();
        mUserRankBtn.requestFocusFromTouch();
        StateListDrawable background = (StateListDrawable) mUserRankBtn.getBackground();
        Drawable current = background.getCurrent();
        if (current instanceof AnimationDrawable) {
            mUserRankAnim = (AnimationDrawable) current;
            mUserRankAnim.start();
        }
    }
```
## 参考资料：
[http://stackoverflow.com/questions/5299219/button-states-with-background-as-animationdrawable-in-android](https://link.jianshu.com?t=http://stackoverflow.com/questions/5299219/button-states-with-background-as-animationdrawable-in-android)
