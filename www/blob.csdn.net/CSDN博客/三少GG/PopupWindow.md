# PopupWindow - 三少GG - CSDN博客
2011年12月09日 12:47:27[三少GG](https://me.csdn.net/scut1135)阅读数：3141标签：[android																[layout																[encoding																[menu																[permissions																[button](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)](https://so.csdn.net/so/search/s.do?q=menu&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
                
事实证明：popupwindow出现是很麻烦的，要符合好几个条件
1.要有触发源，比如按键监听器啦，gesture监听器啦，这里参考别人代码，用idle的loop循环作为监听器，这个选择8错，因为视频播放器不可能让用户按键后出现视频控制按键
2.popupwindow的showatlocation和update都要调用?，看源码showatlocation只是指明了popupwindow到底要在哪里显示，update的源码没看太明白，估计是控制显示吧，不知道android下层怎么控制的
3.对于里面有组件的popupwindow来说，findviewbyid一定要慎重，要指明popupwindow所在的view
4.对于希望满屏显示的，可以使用DisplayMetrics dm=new DisplayMetrics();
popupwindow.update(0,0,dm.widthPixels,dm.heightPixels);
### [android 中的PopupWindow 弹出菜单](http://lzy3169421.blog.163.com/blog/static/1135452772011818114635763/)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [半透明PopupWindow](http://blog.csdn.net/wai2014kuaile/article/details/6593445)
分类： [android](http://blog.csdn.net/wai2014kuaile/article/category/809387)2011-07-08 17:46
416人阅读[评论](http://blog.csdn.net/wai2014kuaile/article/details/6593445#comments)(0)收藏[举报](http://blog.csdn.net/wai2014kuaile/article/details/6593445#report)
1.设置半透明主题
2.设置window的alpha值
> 
- //                        WindowManager.LayoutParams lp = getWindow().getAttributes();
- //            lp.alpha = 0.5f; //0.0-1.0
- //            getWindow().setAttributes(lp);
*复制代码*
发现这两种都不能满足要求，起码的颜色就不太对。想做好点，做成类似alertDialog的样子，带边框，弹出窗口带动画效果，之后背景置灰，那多帅。
看到那个仿uc浏览器的源码，是用alertdialog做的，达到那种效果，加点动画就行了。下图是从那个ucweb源码里面弄出来的。
![](http://hi.csdn.net/attachment/201107/8/0_1310118355c13R.gif)
       上面的代码就不贴了，我上传的项目文件里面也有。
        下面是弹出popupwindow的图片，第一张是动画中，第二张是完全弹出的：
![](http://hi.csdn.net/attachment/201107/8/0_1310118386SLt4.gif)
         弹出popwindow的代码如下，比较乱，多包涵：
![](http://hi.csdn.net/attachment/201107/8/0_1310118447SnI3.gif)
> 
- popupWindow = new PopupWindow(menuView, LayoutParams.FILL_PARENT,
-                                 LayoutParams.FILL_PARENT, true);
-                 popupWindow.showAtLocation(findViewById(R.id.parent), Gravity.CENTER
-                                 | Gravity.CENTER, 0, 0);
-                 popupWindow.setAnimationStyle(R.style.PopupAnimation);
-                 // 加上下面两行可以用back键关闭popupwindow，否则必须调用dismiss();
-                 ColorDrawable dw = new ColorDrawable(-00000);
-                 popupWindow.setBackgroundDrawable(dw);
-                 popupWindow.update();
*复制代码*
下面是实现步骤：
**1。背景置灰：**
     popupWindow = new PopupWindow(menuView, LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT, true);
     第二三个参数必须是LayoutParams.FILL_PARENT，这样才能填充整个屏幕，达到背景置灰的目的。
     整个popupwindow里面是一个GridView，图片什么的也是用的那个仿UC浏览器界面项目的，在此谢谢了。
     关键的东西都在xml里面。
> 
- <?xml version="1.0" encoding="utf-8"?>
- <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
-            android:orientation="vertical" android:layout_width="fill_parent"
-             android:gravity="center" android:layout_height="fill_parent"
-            android:layout_gravity="center" android:background="#b0000000" >
-          <LinearLayout android:orientation="vertical"
-                    android:layout_width="wrap_content" android:gravity="center"
-                    android:layout_height="wrap_content" android:layout_gravity="center"
-                    android:background="@drawable/downbutton_corner">
-                  <GridView android:id="@+id/gridview" android:layout_width="wrap_content"
-                             android:layout_height="wrap_content" android:numColumns="4"
-                             android:verticalSpacing="5dip" android:horizontalSpacing="5dip"
-                             android:stretchMode="columnWidth" android:gravity="center"
-                             android:layout_gravity="center" /></LinearLayout></LinearLayout>
*复制代码*
第一个linearlayout里面的android:background="#b0000000"，就是全屏背景，网上搜的好多半透明都是“#e0000000”，我觉得那颜色太深，“#b0000000”更合适。
第二个linearlayout是popupwind的背景，里面的android:background="@drawable/downbutton_corner"是关键，边框，圆角都是里面定义的。
**2。popupwindow的边框，圆角背景。downbutton_corne.xml**
> 
- <shape xmlns:android="http://schemas.android.com/apk/res/android"
-         android:shape="rectangle">
-         <gradient android:startColor="#c0000000" android:endColor="#c0000000"
-                 android:angle="90" /><!--背景颜色渐变 -->
-         <stroke android:dashWidth="2dp" android:dashGap="2dp"
-                 android:width="2dp" android:color="#FF00ff00"></stroke>
-         <!--描边 -->
-         <corners android:bottomRightRadius="5dp"
-                 android:bottomLeftRadius="5dp" android:topLeftRadius="5dp"
-                 android:topRightRadius="5dp" /><!--设置圆角-->
- </shape>
*复制代码*
这个涉及到shape画图，要是不懂的话。网上很多资料，搜一下就是了。我博客里面也有，http://blog.csdn.net/ymdcr/archive/2010/12/01/6048256.aspx
<gradient android:startColor="#c0000000" android:endColor="#c0000000" android:angle="90" /><!--背景颜色渐变 -->
我就设置了一个固定的颜色"#c0000000"。android:angle="90"这个是设置颜色渐变方向，从上到下啊，从左到右啊，貌似只能90的倍数，也只有四个方向嘛。
<stroke ></stroke>,边框就是这个实现的。
dashWidth指的是边线的宽度 dashGap 指的是每条线之间的间距，（因为是边线是很多小横线组成的）。
**3。淡入淡出动画**
popupWindow.setAnimationStyle(R.style.PopupAnimation);
这条代码是设置style的，动画文件就是在style文件里面引入的。下面是淡入的动画，动画教程网上也很多。淡出的动画就这些参数值交换位置就是了。android:duration这个是持续时间，为了截图，我把它弄成5秒了。
> 
- <set xmlns:android="http://schemas.android.com/apk/res/android">
-         <scale android:fromXScale="0.6" android:toXScale="1.0"
-                 android:fromYScale="0.6" android:toYScale="1.0" android:pivotX="50%"
-                 android:pivotY="50%" android:duration="5000" />
-         <alpha android:interpolator="@android:anim/decelerate_interpolator"
-                 android:fromAlpha="0.0" android:toAlpha="1.0" android:duration="5000" />
- </set>
*复制代码*
大概就是这些了。
还有一个关键的问题。弹出pop之后，back键无效了，必须在pop里面设置事件dismiss掉。下面是问题的描述，哪位解决了，告诉我一下，谢谢。我的邮箱：[ytdcr@tom.com](mailto:ytdcr@tom.com)
> 问题解决了，是因为没设置背景的原因。   
popupWindow.setBackgroundDrawable(new BitmapDrawable());
//把这一行放在showAtLocation前面就行了，以前是放在后面的，粗心了。
popupWindow.showAtLocation(findViewById(R.id.parent), Gravity.CENTER
                | Gravity.CENTER, 0, 0);
网上也有很多人说，弹出pop之后，不响应键盘事件了，这个其实是焦点在pop里面的view去了。
以这个为例，焦点就在gridview上面去了。28楼的兄弟提示的，谢了。
在gridview加上setOnKeyListener，就能解决。
menuGrid.setOnKeyListener(new OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                switch (keyCode) {
                case KeyEvent.KEYCODE_MENU:
                    if (popupWindow != null && popupWindow.isShowing()) {
                        popupWindow.dismiss();
                        System.out.println("menuGridfdsfdsfdfd");
                    }
                    break;
                }
                return true;
            }
        });
*复制代码*
[ code]/**
     * ColorDrawable dw = new ColorDrawable(-00000);
     * popupWindow.setBackgroundDrawable(dw);
     * 本来看了个示例，加上上面这两行就不用调用dismiss，点击窗口之外的部位，或者按back键都能关闭窗口。 但是我这样写了，还是不行。
     * 而且竟然捕获不到键盘事件，杯具，希望哪个解决了这个问题告诉我，谢谢。 [ytdcr@tom.com](mailto:ytdcr@tom.com)
     */
    /*
     * @Override public boolean onKeyDown(int keyCode, KeyEvent event) { switch
     * (keyCode) { case KeyEvent.KEYCODE_BACK: if (popupWindow != null) {
     * popupWindow.dismiss(); }
     *
     * Toast.makeText(this, "fd", 1000).show(); break;
     *
     * } return super.onKeyUp(keyCode, event); }
     */[/code]
package org.ym;
import java.util.ArrayList;
import java.util.HashMap;
import android.app.Activity;
import android.app.AlertDialog;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.GridView;
import android.widget.ListAdapter;
import android.widget.PopupWindow;
import android.widget.SimpleAdapter;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;
/**
 * @author 牛叔
 * 
 */
public class Go extends Activity {
 private Button dialog;
 private Button popupwin;
 private String[] menu_name_array = { "搜索", "文件管理", "下载管理", "全屏", "网址",
   "书签", "加入书签", "分享页面", "退出", "夜间模式", "刷新", "关闭" };
 int[] menu_image_array = { R.drawable.menu_search,
   R.drawable.menu_filemanager, R.drawable.menu_downmanager,
   R.drawable.menu_fullscreen, R.drawable.menu_inputurl,
   R.drawable.menu_bookmark, R.drawable.menu_bookmark_sync_import,
   R.drawable.menu_sharepage, R.drawable.menu_quit,
   R.drawable.menu_nightmode, R.drawable.menu_refresh,
   R.drawable.menu_more };
 private GridView menuGrid;
 private PopupWindow popupWindow;
 /** Called when the activity is first created. */
 @Override
 public void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  setContentView(R.layout.main);
  dialog = (Button) findViewById(R.id.dialog);
  popupwin = (Button) findViewById(R.id.popup);
  dialog.setOnClickListener(new OnClickListener() {
   @Override
   public void onClick(View v) {
    openDialog();
   }
  });
  popupwin.setOnClickListener(new OnClickListener() {
   @Override
   public void onClick(View v) {
    openPopupwin();
   }
  });
 }
 private void openDialog() {
  View menuView = View.inflate(this, R.layout.gridview_menu, null);
  //View menuView =getLayoutInflater().inflate(R.layout.gridview_menu, null);
  // 创建AlertDialog
  final AlertDialog menuDialog = new AlertDialog.Builder(this).create();
  menuDialog.setView(menuView);
  menuGrid = (GridView) menuView.findViewById(R.id.gridview);
  menuGrid.setAdapter(getMenuAdapter(menu_name_array, menu_image_array));
  menuGrid.setOnItemClickListener(new OnItemClickListener() {
   @Override
   public void onItemClick(AdapterView<?> parent, View view, int position,long id) {
    if (position == 11) {
     menuDialog.cancel();
    }
   }
  });
  menuDialog.show();
 }
 private ListAdapter getMenuAdapter(String[] menuNameArray,
   int[] menuImageArray) {
  ArrayList<HashMap<String, Object>> data = new ArrayList<HashMap<String, Object>>();
  for (int i = 0; i < menuNameArray.length; i++) {
   HashMap<String, Object> map = new HashMap<String, Object>();
   map.put("itemImage", menuImageArray[i]);
   map.put("itemText", menuNameArray[i]);
   data.add(map);
  }
  SimpleAdapter simperAdapter = new SimpleAdapter(this, data,
    R.layout.item_menu, new String[] { "itemImage", "itemText" },
    new int[] { R.id.item_image, R.id.item_text });
  return simperAdapter;
 }
 private void openPopupwin() {
  LayoutInflater mLayoutInflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
  ViewGroup menuView = (ViewGroup) mLayoutInflater.inflate(R.layout.gridview_pop, null, true);
  menuGrid = (GridView) menuView.findViewById(R.id.gridview);
  menuGrid.setAdapter(getMenuAdapter(menu_name_array, menu_image_array));
  menuGrid.requestFocus();
  menuGrid.setOnItemClickListener(new OnItemClickListener() {
   @Override
   public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
     long arg3) {
    if (arg2 == 11) {
     popupWindow.dismiss();
    }
   }
  });
  menuGrid.setOnKeyListener(new OnKeyListener() {// 焦点到了gridview上，所以需要监听此处的键盘事件。否则会出现不响应键盘事件的情况
     @Override
     public boolean onKey(View v, int keyCode, KeyEvent event) {
      switch (keyCode) {
      case KeyEvent.KEYCODE_MENU:
       if (popupWindow != null && popupWindow.isShowing()) {
        popupWindow.dismiss();
       }
       break;
      }
      System.out.println("menuGridfdsfdsfdfd");
      return true;
     }
    });
  popupWindow = new PopupWindow(menuView, LayoutParams.FILL_PARENT,
    LayoutParams.FILL_PARENT, true);
  popupWindow.setBackgroundDrawable(new BitmapDrawable());
  popupWindow.setAnimationStyle(R.style.PopupAnimation);
  popupWindow.showAtLocation(findViewById(R.id.parent), Gravity.CENTER| Gravity.CENTER, 0, 0);
  popupWindow.update();
 }
 /**
  * ColorDrawable dw = new ColorDrawable(-00000);
  * popupWindow.setBackgroundDrawable(dw);
  * 本来看了个示例，加上上面这两行就不用调用dismiss，点击窗口之外的部位，或者按back键都能关闭窗口。 但是我这样写了，还是不行。
  * 而且竟然捕获不到键盘事件，杯具，希望哪个解决了这个问题告诉我，谢谢。 [ytdcr@tom.com](mailto:ytdcr@tom.com) 问题解决了，是因为没设置背景的原因。
  * popupWindow.setBackgroundDrawable(new BitmapDrawable());
  * //把这一行放在showAtLocation前面就行了，以前是放在后面的，粗心了。
  * popupWindow.showAtLocation(findViewById(R.id.parent), Gravity.CENTER |
  * Gravity.CENTER, 0, 0); 网上也有很多人说，弹出pop之后，不响应键盘事件了，这个其实是焦点在pop里面的view去了。
  * 以这个为例，焦点就在gridview上面去了。28楼的兄弟提示的，谢了。 在gridview加上setOnKeyListener，就能解决。
  */
 /*
  * @Override public boolean onKeyDown(int keyCode, KeyEvent event) { switch
  * (keyCode) { case KeyEvent.KEYCODE_MENU: if (popupWindow != null &&
  * popupWindow.isShowing()) { popupWindow.dismiss();
  * 
  * } else { openPopupwin(); }
  * 
  * Toast.makeText(this, "fd", 1000).show(); break;
  * 
  * } return false; }
  */
}
gridview_menu.xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
    android:orientation="vertical"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    >
<GridView
         android:id="@+id/gridview"
         android:layout_width="fill_parent"
         android:layout_height="fill_parent"
         android:numColumns="4"
         android:verticalSpacing="10dip"
         android:horizontalSpacing="10dip"
         android:stretchMode="columnWidth"
         android:gravity="center"
         />
</LinearLayout>
gridview_pop.xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
 android:orientation="vertical" android:layout_width="fill_parent"
 android:gravity="center" android:layout_height="fill_parent"
 android:layout_gravity="center" android:background="#b5555555" >
 <LinearLayout android:orientation="vertical"
  android:layout_width="wrap_content" android:gravity="center"
  android:layout_height="wrap_content" android:layout_gravity="center"
  android:background="@drawable/downbutton_corner">
  <GridView android:id="@+id/gridview" android:layout_width="wrap_content"
   android:layout_height="wrap_content" android:numColumns="4"
   android:verticalSpacing="5dip" android:horizontalSpacing="5dip"
   android:stretchMode="columnWidth" android:gravity="center"
   android:layout_gravity="center" />
 </LinearLayout>
</LinearLayout>
item_menu.xml
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
 android:id="@+id/RelativeLayout_Item"
 android:layout_width="wrap_content" android:layout_height="wrap_content"
 android:paddingBottom="5dip">
 <ImageView android:id="@+id/item_image"
  android:layout_centerHorizontal="true" android:layout_width="wrap_content"
  android:layout_height="wrap_content" ></ImageView>
 <TextView android:layout_below="@id/item_image" android:id="@+id/item_text"
  android:layout_centerHorizontal="true" android:layout_width="wrap_content"
  android:layout_height="wrap_content" android:text="选项" ></TextView>
</RelativeLayout>
main.xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
 android:orientation="vertical" android:layout_width="fill_parent"
 android:layout_height="fill_parent" android:background="@drawable/apple1600_3018" android:id="@+id/parent">
 <TextView android:layout_width="fill_parent"
  android:layout_height="wrap_content" android:text="@string/hello" />
   <Button android:id="@+id/dialog" android:layout_width="wrap_content" android:layout_height="wrap_content" android:text="dialog"></Button>
  <Button android:id="@+id/popup" android:layout_width="wrap_content" android:layout_height="wrap_content" android:text="popupwindow"></Button>
</LinearLayout>
popup_enter.xml
<?xml version="1.0" encoding="utf-8"?>
 <!--
  Copyright (C) 2008 Romain Guy Licensed under the Apache License,
  Version 2.0 (the "License"); you may not use this file except in
  compliance with the License. You may obtain a copy of the License at
[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0) Unless required by
  applicable law or agreed to in writing, software distributed under the
  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either express or implied. See the License for
  the specific language governing permissions and limitations under the
  License.
 -->
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)">
 <scale android:fromXScale="0.6" android:toXScale="1.0"
  android:fromYScale="0.6" android:toYScale="1.0" android:pivotX="50%"
  android:pivotY="50%" android:duration="1000" />
 <alpha android:interpolator="@android:anim/decelerate_interpolator"
  android:fromAlpha="0.0" android:toAlpha="1.0" android:duration="1000" />
</set>

popup_exit.xml
<?xml version="1.0" encoding="utf-8"?>
<!--
   Copyright (C) 2008 Romain Guy
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
-->
<set xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)">
    <scale
        android:fromXScale="1.0"
        android:toXScale="0.5"
        android:fromYScale="1.0"
        android:toYScale="0.5"
        android:pivotX="50%"
        android:pivotY="50%"
        android:duration="500" />
    <alpha
        android:interpolator="@android:anim/accelerate_interpolator"
        android:fromAlpha="1.0"
        android:toAlpha="0.0"
        android:duration="500" />
</set>

style.xml
<?xml version="1.0" encoding="utf-8"?>
<resources>
 <style name="mytheme" parent="@android:style/Theme.Light">
  <item name="android:windowNoTitle"> true </item>
 </style>
 <style name="PopupAnimation" parent="android:Animation">
  <item name="android:windowEnterAnimation">@anim/popup_enter</item>
  <item name="android:windowExitAnimation">@anim/popup_exit</item>
 </style>
</resources>
