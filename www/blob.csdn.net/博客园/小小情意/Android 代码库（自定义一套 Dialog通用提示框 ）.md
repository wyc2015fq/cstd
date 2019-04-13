
# Android 代码库（自定义一套 Dialog通用提示框 ） - 小小情意 - 博客园






# [Android 代码库（自定义一套 Dialog通用提示框 ）](https://www.cnblogs.com/xiaoxiaoqingyi/p/6738108.html)
做Android开发五年了，期间做做停停（去做后台开发，服务器管理），当回来做Android的时候，发现很生疏，好些控件以前写得很顺手，现在好像忘记些什么了，总要打开这个项目，打开那个项目，有时未必还找得到。
总结起来，还是源于没有好好做一个属于自己的代码库，把平时开发项目中一些自定义的控件，或一些耦合性很低的模块封装起来，或者平时比较少写博客。如果你是一个刚学会开发的程序猿，或者是有过好几年开发经验的大鸟，也该开始整理整理自己的代码，这也不枉此生敲代码的岁月，同时在面试中，也会给你带来不少印象分喔。所以，我也开始准备自己的代码库，放在github 或者微博上，希望可以跟各位大神多交流。下面我先放一到两个自定义控件。
自定义一套 Dialog通用提示框：
![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170420121530618-869950677.jpg)![](https://images2015.cnblogs.com/blog/804587/201704/804587-20170420121640837-773620744.jpg)
上诉提示框都是一种类型，当然有可能你不大满意，或者与你们设计师的要求的风格不一致，没关系，你只要进去修改一下dialog 的布局就可以了。当然，我希望在自定义这些控件的时候，能用xml 来渲染的，尽量不要用图片去做背景之类的。每个app 的提示框风格其实大体一致的，不会每个页面的提示框都不一样，如果真的变化太大，我们就重新自定义一个dialog即可。其它的只需设置一下信息即可：
newCommomDialog(mContext, R.style.dialog, "您确定删除此信息？",newCommomDialog.OnCloseListener() {
    @OverridepublicvoidonClick(booleanconfirm) {if(confirm){
               Toast.makeText(this,"点击确定", Toast.LENGTH_SHORT).show();
               dialog.dismiss();
         }
           
    }
})
        .setTitle("提示").show();
我们先看 CommomDialog 类， 这个类定义的时候，里面的方法尽量做成链式的，方便后期调用
publicclassCommomDialogextendsDialogimplementsView.OnClickListener{privateTextView contentTxt;privateTextView titleTxt;privateTextView submitTxt;privateTextView cancelTxt;privateContext mContext;privateString content;privateOnCloseListener listener;privateString positiveName;privateString negativeName;privateString title;publicCommomDialog(Context context) {super(context);this.mContext =context;
    }publicCommomDialog(Context context,intthemeResId, String content) {super(context, themeResId);this.mContext =context;this.content =content;
    }publicCommomDialog(Context context,intthemeResId, String content, OnCloseListener listener) {super(context, themeResId);this.mContext =context;this.content =content;this.listener =listener;
    }protectedCommomDialog(Context context,booleancancelable, OnCancelListener cancelListener) {super(context, cancelable, cancelListener);this.mContext =context;
    }publicCommomDialog setTitle(String title){this.title =title;returnthis;
    }publicCommomDialog setPositiveButton(String name){this.positiveName =name;returnthis;
    }publicCommomDialog setNegativeButton(String name){this.negativeName =name;returnthis;
    }
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {super.onCreate(savedInstanceState);
        setContentView(R.layout.dialog_commom);
        setCanceledOnTouchOutside(false);
        initView();
    }privatevoidinitView(){
        contentTxt=(TextView)findViewById(R.id.content);
        titleTxt=(TextView)findViewById(R.id.title);
        submitTxt=(TextView)findViewById(R.id.submit);
        submitTxt.setOnClickListener(this);
        cancelTxt=(TextView)findViewById(R.id.cancel);
        cancelTxt.setOnClickListener(this);
        contentTxt.setText(content);if(!TextUtils.isEmpty(positiveName)){
            submitTxt.setText(positiveName);
        }if(!TextUtils.isEmpty(negativeName)){
            cancelTxt.setText(negativeName);
        }if(!TextUtils.isEmpty(title)){
            titleTxt.setText(title);
        }
    }
    @OverridepublicvoidonClick(View v) {switch(v.getId()){caseR.id.cancel:if(listener !=null){
                    listener.onClick(this,false);
                }this.dismiss();break;caseR.id.submit:if(listener !=null){
                    listener.onClick(this,true);
                }break;
        }
    }publicinterfaceOnCloseListener{voidonClick(Dialog dialog,booleanconfirm);
    }
}
自定义了监听事件，设置了消息后，返回该句柄， return this;
再看看 R.layout.dialog_commom xml 文件
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="match_parent"android:background="@drawable/bg_round_white"android:orientation="vertical" >
        <TextView
            android:id="@+id/title"android:layout_width="match_parent"android:layout_height="wrap_content"android:gravity="center_horizontal"android:padding="12dp"android:layout_marginTop="12dp"android:text="提示"android:textSize="16sp"android:textColor="@color/black"/>
    <TextView
        android:id="@+id/content"android:layout_width="match_parent"android:layout_height="wrap_content"android:gravity="center"android:layout_gravity="center_horizontal"android:lineSpacingExtra="3dp"android:layout_marginLeft="40dp"android:layout_marginTop="20dp"android:layout_marginRight="40dp"android:layout_marginBottom="30dp"android:text="签到成功，获得200积分"android:textSize="12sp"android:textColor="@color/font_common_1"/>
    <View
        android:layout_width="match_parent"android:layout_height="1dp"android:background="@color/commom_background"/>
    <LinearLayout
        android:layout_width="match_parent"android:layout_height="50dp"android:orientation="horizontal">
        <TextView
            android:id="@+id/cancel"android:layout_width="match_parent"android:layout_height="match_parent"android:background="@drawable/bg_dialog_left_white"android:layout_weight="1.0"android:gravity="center"android:text="@string/cancel"android:textSize="12sp"android:textColor="@color/font_common_2"/>
        <View
            android:layout_width="1dp"android:layout_height="match_parent"android:background="@color/commom_background"/>
        <TextView
            android:id="@+id/submit"android:layout_width="match_parent"android:layout_height="match_parent"android:background="@drawable/bg_dialog_right_white"android:gravity="center"android:layout_weight="1.0"android:text="@string/submit"android:textSize="12sp"android:textColor="@color/font_blue"/>
    </LinearLayout>
</LinearLayout>
整个背景我使用了圆角，这样不显得特别生硬 android:background="@drawable/bg_round_white"
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android" android:shape="rectangle">
    <solid android:color="@color/white" />
    <corners android:radius="8dp" />
</shape>
当然底部两个按钮也是要做相应的圆角处理：
左下按钮：android:background="@drawable/bg_dialog_left_white"
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android" android:shape="rectangle">
    <solid android:color="@color/white" />
    <corners android:bottomLeftRadius="8dp" />
</shape>
右下按钮：android:background="@drawable/bg_dialog_right_white"

<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android" android:shape="rectangle">
    <solid android:color="@color/white" />
    <corners android:bottomRightRadius="8dp" />
</shape>
展示的 style 也要设置一下：
<style name="dialog" parent="@android:style/Theme.Dialog">
    <item name="android:windowFrame">@null</item>
    <!--边框-->
    <item name="android:windowIsFloating">true</item>
    <!--是否浮现在activity之上-->
    <item name="android:windowIsTranslucent">false</item>
    <!--半透明-->
    <item name="android:windowNoTitle">true</item>
    <!--无标题-->
    <item name="android:windowBackground">@android:color/transparent</item>
    <!--背景透明-->
    <item name="android:backgroundDimEnabled">true</item>
    <!--模糊-->
</style>
这样基本大功告成，通过设置消息头，信息体，按钮名称，还有点击事件，就可以随意控制你的提示框了。
**源码链接：**[https://github.com/xiaoxiaoqingyi/mine-android-repository](https://github.com/xiaoxiaoqingyi/mine-android-repository)
后面我会把自己的代码库都放上来，与大家一起学习。









