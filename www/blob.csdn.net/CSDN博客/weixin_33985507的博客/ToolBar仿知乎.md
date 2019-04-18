# ToolBar仿知乎 - weixin_33985507的博客 - CSDN博客
2017年03月01日 14:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：20
需要做一个类似于知乎的效果，真的是踩了无数坑。
![2191038-5c245876ff33bde8.jpeg](https://upload-images.jianshu.io/upload_images/2191038-5c245876ff33bde8.jpeg)
291AB9DA5FD.jpeg
xml中
<android.support.v7.widget.Toolbar
android:id="@+id/tb_title"
android:layout_width="match_parent"
android:layout_height="@dimen/dimen_50"
android:background="@color/color_1196db"
toolbar:title="首页"
toolbar:navigationIcon="@mipmap/toolbar_menu"
toolbar:titleTextColor="@color/color_white"
toolbar:titleTextAppearance="@style/ToolBar.TitleText"
toolbar:theme="@style/ToolBar"></[android.support.v7.widget.Toolbar](https://link.jianshu.com?t=http://android.support.v7.widget.Toolbar)>
Activity中
private Toolbar tbTitle;
@Override
public void initViews() {
tbTitle = findView(R.id.tb_title);
}
@Override
public void initData(Bundle bundle) {
tbTitle.inflateMenu(R.menu.userinfo);
}
@Override
public void initEvents() {
//设置左侧图标的点击事件
tbTitle.setNavigationOnClickListener(new View.OnClickListener() {
@Override public void onClick(View v) {
ToastUtils.show(activity,"点点点");
}
});
//设置菜单里的点击事件
tbTitle.setOnMenuItemClickListener(new Toolbar.OnMenuItemClickListener() {
@Override public boolean onMenuItemClick(MenuItem item) {
switch (item.getItemId()){
case R.id.update_data:
ToastUtils.show(activity,"修改资料");
break;
case R.id.update_password:
ToastUtils.show(activity,"修改密码");
break;
}
return true;
}
});
}
style.xml中的样式
<style name="ToolBar" parent="Theme.AppCompat.Light.NoActionBar">
<item name="android:textColorSecondary">@color/color_white</item>
<item name="android:textColorPrimary">@color/color_white</item>
```
<!--设置弹出框的样式及弹出位置-->
<item name="actionOverflowMenuStyle">@style/OverflowMenuStyle</item>
```
</style>
<style name="OverflowMenuStyle" parent="@style/Widget.AppCompat.PopupMenu.Overflow">
<item name="overlapAnchor">false</item>
<item name="android:popupBackground">@color/color_1196db</item>
<item name="android:dropDownVerticalOffset">5dp</item>
<item name="android:dropDownHorizontalOffset">0dp</item>
</style>
<style name="ToolBar.TitleText" parent="TextAppearance.Widget.AppCompat.Toolbar.Title">
<item name="android:textSize">18sp</item>
</style>
