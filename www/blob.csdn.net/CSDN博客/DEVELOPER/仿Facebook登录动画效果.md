# 仿Facebook登录动画效果 - DEVELOPER - CSDN博客





2016年09月02日 19:30:32[学术袁](https://me.csdn.net/u012827205)阅读数：1232








自己的博客已经好久没有写过了。今天写了一天的代码，写项目项目时候，要求非得跟脸书的登录效果一模一样！然后我还专门下载了看了一下，他的登录效果到底是啥样的。结果懵逼了，因为我得研究一下；已实现的效果图(不准失真变形的)：

![](https://img-blog.csdn.net/20160902191328892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160902191356955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

直接上代码吧：


```java
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@color/colorBackgroundSignIn"
    android:orientation="vertical">


    <!--应用logo-->
    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="2">

        <ImageView
            android:id="@+id/icon_imgY"
            android:layout_width="120dp"
            android:layout_height="120dp"
            android:layout_alignParentBottom="true"
            android:layout_centerHorizontal="true"
            android:layout_marginTop="10dp"
            android:src="@drawable/logo" />
    </RelativeLayout>


    <!--输入邮箱-->
    <RelativeLayout
        android:id="@+id/lay_input_email"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginLeft="32dp"
        android:layout_marginRight="32dp">

        <ImageView
            android:id="@+id/img_email"
            android:layout_width="28dp"
            android:layout_height="14dp"
            android:layout_centerVertical="true"
            android:src="@drawable/icon_signin_email" />
        <!--输入：个人邮箱账户-->
        <EditText
            android:id="@+id/edt_email_signIn"
            android:layout_width="match_parent"
            android:layout_height="45dp"
            android:layout_centerVertical="true"
            android:layout_toRightOf="@+id/img_email"
            android:background="@color/colorBackgroundSignIn"
            android:paddingLeft="@dimen/custom_xml_margin5dp"
            android:singleLine="true"
            android:text="1530702811@qq.com"
            android:textColor="@color/text_word_color"
            android:textCursorDrawable="@color/color_cursor"
            android:textSize="@dimen/custom_xml_word_size16sp" />

        <ImageView
            android:id="@+id/line_edt_1"
            android:layout_width="match_parent"
            android:layout_height="1dp"
            android:layout_below="@+id/edt_email_signIn"
            android:background="@color/color_line_editText_selected" />
    </RelativeLayout>

    <RelativeLayout
        android:id="@+id/lay_input_pw"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_below="@+id/lay_input_email"
        android:layout_marginLeft="32dp"
        android:layout_marginRight="32dp">

        <!--输入密码-->
        <ImageView
            android:id="@+id/img_pw"
            android:layout_width="16dp"
            android:layout_height="27dp"
            android:layout_centerVertical="true"
            android:layout_marginLeft="6dp"
            android:layout_marginRight="5dp"
            android:src="@drawable/icon_signin_password" />
        <!--输入：个人密码-->
        <EditText
            android:id="@+id/edt_sigIn_password"
            android:layout_width="match_parent"
            android:layout_height="45dp"
            android:layout_centerVertical="true"
            android:layout_marginLeft="3dp"
            android:layout_toRightOf="@+id/img_pw"
            android:background="@color/colorBackgroundSignIn"
            android:inputType="textPassword"
            android:paddingLeft="@dimen/custom_xml_margin5dp"
            android:singleLine="true"
            android:text="rose123456"
            android:textColor="@color/text_word_color"
            android:textCursorDrawable="@color/color_cursor"
            android:textSize="@dimen/custom_xml_word_size16sp" />

        <ImageView
            android:id="@+id/line_edt_2"
            android:layout_width="match_parent"
            android:layout_height="1dp"
            android:layout_below="@+id/edt_sigIn_password"
            android:background="@color/color_line_editText" />

        <ImageView
            android:id="@+id/img_pw_visiable"
            android:layout_width="22dp"
            android:layout_height="15dp"
            android:layout_alignParentRight="true"
            android:layout_centerVertical="true"
            android:layout_marginRight="5dp"
            android:src="@drawable/ic_not_visiable" />
    </RelativeLayout>

    <!--按钮：登录-->
    <RelativeLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_below="@+id/lay_checkBox"
        android:layout_marginTop="@dimen/custom_widgets_high20dp">

        <Button
            android:id="@+id/btn_signIn"
            android:layout_width="match_parent"
            android:layout_height="45dp"
            android:layout_marginLeft="36dp"
            android:layout_marginRight="36dp"
            android:background="@drawable/button_shape_selector"
            android:text="@string/activity_signIn_button"
            android:textColor="@color/colorWhite"
            android:textSize="@dimen/custom_xml_word_size18sp" />
        <!--忘记密码-->
        <TextView
            android:id="@+id/tv_signIn_forgetPassWord"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_below="@+id/btn_signIn"
            android:layout_centerHorizontal="true"
            android:layout_marginTop="10dp"
            android:background="@drawable/text_line_bottom_layer_list"
            android:text="@string/activity_signIn_forgetPassword"
            android:textAlignment="center"
            android:textColor="@color/color_text_word"
            android:textSize="@dimen/custom_xml_word_size14sp" />

    </RelativeLayout>

    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="0.8" />

    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="1.2">

        <TextView
            android:id="@+id/tv_createTeam_signIn"
            android:layout_width="match_parent"
            android:layout_height="@dimen/activity_signIn_icon_email_high"
            android:layout_alignParentBottom="true"
            android:background="@drawable/button_line_top"
            android:gravity="center"
            android:text="@string/activity_createTeam_textView"
            android:textColor="@color/color_button_selected"
            android:textSize="@dimen/custom_xml_word_size18sp" />
    </RelativeLayout>


</LinearLayout>
```



其实很简单，他不需要逻辑代码实现。仅仅就需要布局就好。但是别忘了在AndroidManifest.xml 中设置：`android:windowSoftInputMode="adjustResize|stateHidden"`


特别注意的是：可以看到上的布局控件设置，只有设置权重的控件所占的空间会被缩放，压缩。因此产生平移上推screen的效果。


















































































































































































































































































































































































































