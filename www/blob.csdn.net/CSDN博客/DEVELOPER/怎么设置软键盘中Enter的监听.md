# 怎么设置软键盘中Enter的监听 - DEVELOPER - CSDN博客





2014年12月03日 17:03:11[学术袁](https://me.csdn.net/u012827205)阅读数：1973








![](https://img-blog.csdn.net/20141203165937004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```java
/**布局中必须设置属性<singleLine & imeOptions>*/
<EditText                
android:id="@+id/edt_searchMicroFilm"                
android:layout_width="fill_parent"               
 android:layout_height="32dp"                
android:layout_marginRight="10dp"               
 android:background="@drawable/search_frame_shape_circle"               
 android:gravity="center_vertical"                
android:hint="@string/plaseInputKeyWords"               
 android:paddingLeft="25dp"                
android:textColorHighlight="@color/jxInputFrameHintTextColor"               
 android:textColor="@color/jxTitleTextWord"               
 android:imeOptions="actionSearch"               
 android:singleLine="true"                
android:textSize="14sp" >           
 </EditText>
```

```java
/**在Activity类中编写自己的功能代码，即设置监听事件：*/
editText.setOnEditorActionListene( newTextView.OnEditorActionListener() { 
public boolean onEditorAction(TextView v, int actionId, KeyEvent event){ 
if (actionId == EditorInfo.IME_ACTION_SEARCH ||(event!=null && event.getKeyCode()== KeyEvent.KEYCODE_ENTER)) { 
// 在这里编写自己想要实现的功能                           
   ..... return true;                          
}                          
 return false;<pre name="code" class="java">         }   });
```




我想你既然来到这里看这篇文章至少是想要了解怎么去重新设置手机弹出的软键盘上Enter功能。使得点击过Enter键之后，响应自己想要实现的功能。而软件盘的界面替换却有这么一个属性android:imeOptions来满足我们的需要，这个属性的可以取的值有normal，actionUnspecified，actionNone，actionGo，actionSearch，actionSend，actionNext，actionDone，例如：当值为actionNext时enter键外观变成一个向下箭头；当值为actionSearch时候Enter键外观变成“搜索”两个字；而值为actionDone时enter键外观则变成了“完成”两个字。

在进行自己自定义的响应的功能设置时候，要注意<下面三条必须的满足>：

1.在XML布局中设置其属性android:imeOptions，如上图；

2.在XML布局中设置其属性android:singleLine="true"，保证点击Enter键时，能被正确监听到；

3.在代码中设置监听：editText.setOnEditorActionListene();并完成自己想要实现的功能代码；







