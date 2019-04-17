# Android中软键盘常见的问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月15日 22:24:49[boonya](https://me.csdn.net/boonya)阅读数：682








原文地址：[http://blog.csdn.net/mynameishuangshuai/article/details/51567357](http://blog.csdn.net/mynameishuangshuai/article/details/51567357)


## 软键盘显示的原理

       软件盘的本质是什么？软键盘其实是一个Dialog。 

       InputMethodService为我们的输入法创建了一个Dialog，并且将该Dialog的Window的某些参数（如Gravity）进行了设置，使之能够在底部或者全屏显示。当我们点击输入框时，系统对活动主窗口进行调整，从而为输入法腾出相应的空间，然后将该Dialog显示在底部，或者全屏显示。

## 软键盘显示的调整

[Android](http://lib.csdn.net/base/android)定义了一个属性，名字为windowSoftInputMode, 这个属性用于设置Activity主窗口与软键盘的交互模式，用于避免软键盘遮挡内容的问题。我们可以在AndroidManifet.xml中对Activity进行设置。如：android:windowSoftInputMode=”stateUnchanged|adjustPan”。

       该属性可选的值有两部分，一部分为软键盘的状态控制，控制软键盘是隐藏还是显示，另一部分是Activity窗口的调整，以便腾出空间展示软键盘。 

       android:windowSoftInputMode的属性设置必须是下面中的一个值，或一个”state”值加一个”adjust”值的组合，各个值之间用 | 分开。
- **stateUnspecified-未指定状态**：当我们没有设置android:windowSoftInputMode属性的时候，软件默认采用的就是这种交互方式，系统会根据界面采取相应的软键盘的显示模式。
- **stateUnchanged-不改变状态**：当前界面的软键盘状态，取决于上一个界面的软键盘状态，无论是隐藏还是显示。
- **stateHidden-隐藏状态**：当设置该状态时，软键盘总是被隐藏，不管是否有输入的需求。
- **stateAlwaysHidden-总是隐藏状态**：当设置该状态时，软键盘总是被隐藏，和stateHidden不同的是，当我们跳转到下个界面，如果下个页面的软键盘是显示的，而我们再次回来的时候，软键盘就会隐藏起来。
- **stateVisible-可见状态**：当设置为这个状态时，软键盘总是可见的，即使在界面上没有输入框的情况下也可以强制弹出来出来。
- **stateAlwaysVisible-总是显示状态**：当设置为这个状态时，软键盘总是可见的，和stateVisible不同的是，当我们跳转到下个界面，如果下个页面软键盘是隐藏的，而我们再次回来的时候，软键盘就会显示出来。
- **adjustUnspecified-未指定模式**：设置软键盘与软件的显示内容之间的显示关系。当你跟我们没有设置这个值的时候，这个选项也是默认的设置模式。在这中情况下，系统会根据界面选择不同的模式。
- **adjustResize-调整模式**：该模式下窗口总是调整屏幕的大小用以保证软键盘的显示空间；这个选项不能和adjustPan同时使用，如果这两个属性都没有被设置，系统会根据窗口中的布局自动选择其中一个。
- **adjustPan-默认模式**：该模式下通过不会调整来保证软键盘的空间，而是采取了另外一种策略，系统会通过布局的移动，来保证用户要进行输入的输入框肯定在用户的视野范围里面，从而让用户可以看到自己输入的内容。

### 案例

**没有滚动布局xml**



```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
android:layout_width="match_parent"
android:layout_height="match_parent"
android:orientation="vertical">

    <EditText
android:id="@+id/et1"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框1" />

    <Button
android:id="@+id/btn1"
android:layout_width="match_parent"
android:layout_height="wrap_content"
android:layout_marginTop="5dp"
android:text="猴子搬来的救兵按钮"
android:textSize="15sp" />


    <EditText
android:id="@+id/et2"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框2" />

    <EditText
android:id="@+id/et3"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框3" />

    <EditText
android:id="@+id/et4"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框4" />

    <EditText
android:id="@+id/et5"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框5" />

    <EditText
android:id="@+id/et6"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框6" />

    <EditText
android:id="@+id/et7"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框7" />

    <EditText
android:id="@+id/et8"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框8" />

    <EditText
android:id="@+id/et9"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框9" />

    <EditText
android:id="@+id/et10"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框10" />

    <EditText
android:id="@+id/et11"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框11" />

    <EditText
android:id="@+id/et12"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框12" />
</LinearLayout>
```

对于没有滚动控件的布局来说，adjustPan就是默认的设置，比如我们案例应用中的文本输入8，上面的文本输入框123和按钮都会被顶上去，且页面布局不可以滚动。

![这里写图片描述](https://img-blog.csdn.net/20160602144128083)

**有滚动布局xml**



```
<ScrollView xmlns:android="http://schemas.android.com/apk/res/android"
android:layout_width="match_parent"
android:layout_height="match_parent">

    <LinearLayout
android:layout_width="match_parent"
android:layout_height="match_parent"
android:orientation="vertical">

        <EditText
android:id="@+id/et1"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框1" />

        <Button
android:id="@+id/btn1"
android:layout_width="match_parent"
android:layout_height="wrap_content"
android:layout_marginTop="5dp"
android:text="猴子搬来的救兵按钮"
android:textSize="15sp" />


        <EditText
android:id="@+id/et2"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框2" />

        <EditText
android:id="@+id/et3"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框3" />

        <EditText
android:id="@+id/et4"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框4" />

        <EditText
android:id="@+id/et5"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框5" />

        <EditText
android:id="@+id/et6"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框6" />

        <EditText
android:id="@+id/et7"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框7" />

        <EditText
android:id="@+id/et8"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框8" />

        <EditText
android:id="@+id/et9"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框9" />

        <EditText
android:id="@+id/et10"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框10" />

        <EditText
android:id="@+id/et11"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框11" />

        <EditText
android:id="@+id/et12"
android:layout_width="match_parent"
android:layout_height="50dp"
android:text="文本输入框12" />
    </LinearLayout>
   </ScrollView>
```
 对于有滚动控件的布局，则是采用的adjustResize方式，比如我们案例应用中的文本输入8，上面的文本输入框123和按钮都会被顶上去，可以通过滚动来查看被顶上去的内容。

![这里写图片描述](https://img-blog.csdn.net/20160602144128083)

![这里写图片描述](https://img-blog.csdn.net/20160602144147465)

       根据这一原理，我们就可以把开发中遇到的软键盘遮挡页面的问题，利用ScrollView当做根布局，让系统采用adjustResize模式，很好地解决这一问题。

## 自动弹出软键盘

有时候需要一进入Activity后就自动弹出软键盘，可以通过设置一个时间函数来实现，具体写法如下：
**方法一：**



```java
Timer timer=new Timer(); 
timer.schedule(new TimerTask() { 

            public void run() { 
                InputMethodManager inputMethodManager=(InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE); 
                inputMethodManager.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS); 
            } 
}, 1000); // 秒后自动弹出
```

**方法二：**



```java
Timer timer = new Timer();
timer.schedule(new TimerTask() {

  public void run() {
    InputMethodManager inputManager =(InputMethodManager) etInput.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
    inputManager.showSoftInput(etInput, 0);
  }

},1000);// 1秒后自动弹出
```

## 不自动弹出软键盘

有时进入Activity后不希望系统自动弹出软键盘，我们可以按照下面的方法来实现： 
**方法一：**

在AndroidMainfest.xml中选择那个activity，设置windowSoftInputMode属性为adjustUnspecified|stateHidden



```
<activity Android:name=".Main"
            Android:label="@string/app_name"
            Android:windowSoftInputMode="adjustUnspecified|stateHidden"
            Android:configChanges="orientation|keyboardHidden">
            <intent-filter>
                <action Android:name="android.intent.action.MAIN" />
                <category Android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
</activity>
```

**方法二：**

让EditText失去焦点，使用EditText的clearFocus方法：



```java
EditText edit=(EditText)findViewById(R.id.edit);
edit.clearFocus();
```

**方法三：**

强制隐藏Android输入法窗口



```java
EditText edit=(EditText)findViewById(R.id.edit); 
InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
imm.hideSoftInputFromWindow(edit.getWindowToken(),0);
```

**方法四：**

EditText始终不弹出软件键盘



```java
EditText edit=(EditText)findViewById(R.id.edit);
edit.setInputType(InputType.TYPE_NULL);
```

## EditText设置ScrollView压缩背景图片解决办法

在你的Activity里加上 


```java
getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
```

## 动态关闭软键盘

有时希望根据条件动态关闭软键盘，我们可以使用InputMethodManager类，按照下面的方法来实现：
**方法一：**



```java
InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE); //得到InputMethodManager的实例
<span style="font-size:12px;"></span><pre name="code" class="java">//如果开启
```
if (imm.isActive()) { imm.toggleSoftInput(InputMethodManager.SHOW_IMPLICIT,InputMethodManager.HIDE_NOT_ALWAYS);//关闭软键盘，开启方法相同，这个方法是切换开启与关闭状态的}



**方法二：**



```java
// 强制隐藏软键盘
 public void KeyBoardCancle() {
  View view = getWindow().peekDecorView();
  if (view != null) {
   InputMethodManager inputmanger = (InputMethodManager) getSystemService(ActivityBase.INPUT_METHOD_SERVICE);
   inputmanger.hideSoftInputFromWindow(view.getWindowToken(), 0);
  }
 }
```

**方法三：**



```java
int flags = WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM; 
getWindow().addFlags(flags);
```

**方法四：**

在onclick事件下.以下方法可行.(如果是EditText失去焦点/得到焦点,没有效果)



```java
InputMethodManager im = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE); 
im.hideSoftInputFromWindow(getCurrentFocus().getApplicationWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
```

InputMethodManager的具体用法可以参考下面的链接： 
[http://www.apihome.cn/api/android/InputMethodManager.html](http://www.apihome.cn/api/android/InputMethodManager.html)

## 软键盘界面按钮功能设置方法

使用android:imeOptinos可对Android自带的软键盘进行一些界面上的设置：



```
<EditText  
    android:id="@+id/text1"  
    android:layout_width="150dip"  
    android:layout_height="wrap_content" 
    android:imeOptions="flagNoExtractUi"/>
```

android:imeOptions="flagNoExtractUi"  //使软键盘不全屏显示，只占用一部分屏幕



同时,这个属性还能控件软键盘右下角按键的显示内容,默认情况下为回车键



```
android:imeOptions="actionNone"  //输入框右侧不带任何提示 
android:imeOptions="actionGo"    //右下角按键内容为'开始' 
android:imeOptions="actionSearch"  //右下角按键为放大镜图片，搜索 
android:imeOptions="actionSend"    //右下角按键内容为'发送' 
android:imeOptions="actionNext"   //右下角按键内容为'下一步' 
android:imeOptions="actionDone"  //右下角按键内容为'完成'
```

![这里写图片描述](https://img-blog.csdn.net/20160602143924244)

![这里写图片描述](https://img-blog.csdn.net/20160602143934395)

同时，可能EditText添加相应的监听器，捕捉用户点击了软键盘右下角按钮的监听事件，以便进行处理。



```java
editText.setOnEditorActionListener(new OnEditorActionListener() { 
        @Override 
        public boolean onEditorAction(TextView v, int actionId, KeyEvent event) { 
            Toast.makeText(MainActivity.this, "响应了配置后的按键", Toast.LENGTH_SHORT).show(); 
            return false; 
        } 
});
```

## 踩过的坑

### 一、软键盘无法顶起页面

       开发中有个需求是将页面底部的一个按钮顶起，但是开发时发现Android5.0以后的版本设置了adjustResize属性后无法成功顶起。纠结了好久，最后在stackoverflow找到解决方案，那就是在根布局上加上fitsSystemWindow=”true”即可。



```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:fitsSystemWindows="true"
>
```

![这里写图片描述](https://img-blog.csdn.net/20160602144011770)

       这里的fitsSystemWindow具体的作用就是你的contentview是否忽略actionbar,title,屏幕的底部虚拟按键，将整个屏幕当作可用的空间。

       正常情况，contentview可用的空间是去除了actionbar,title,底部按键的空间后剩余的可用区域；这个属性设置为true,则忽略，false则不忽略

### 二、自定义软键盘按钮功能无效

       在edittext上加入Android:imeOptions=”actionSearch”这个属性没响应，最后发现在2.3及以上版本不起作用，解决方案：加上


`android:singleLine="true"`
       因为输入法键盘右下角默认的回车键本来就是换行用的，当设置单行后，回车换行就失去作用了，这样就可以设置为搜索、发送、go等等。



参考链接：[http://winuxxan.blog.51cto.com/2779763/522810](http://winuxxan.blog.51cto.com/2779763/522810)









