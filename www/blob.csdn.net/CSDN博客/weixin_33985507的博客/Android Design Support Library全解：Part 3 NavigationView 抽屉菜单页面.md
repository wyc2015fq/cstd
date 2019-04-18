# Android Design Support Library全解：Part 3 NavigationView 抽屉菜单页面 - weixin_33985507的博客 - CSDN博客
2017年08月20日 00:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 
[Android Design Support Library系列](https://www.jianshu.com/p/f58566ec7014)第三弹，NavigationView 抽屉菜单的实现
![3297585-3d076c4a57bfc67d.png](https://upload-images.jianshu.io/upload_images/3297585-3d076c4a57bfc67d.png)
左侧弹出菜单
抽屉菜单页面是一中很常见的页面设计，虽然有很多第三方库（SlidingMenu）来帮助我们实现该功能，但是如果自己来实现的话，确实是很麻烦的工作。在Android Design Support Library中，谷歌官方提供了*NavigationView*来帮助我们轻松地实现抽屉导菜单页面。
如果读者在使用Android Design Support Library之前有过开发抽屉菜单的开发经验就会知道，之前都是在**DrawerLayout**中自定义*ListView*或是*LinerLayout*来展示内容达到效果，现在只需要在**DrawerLayout**中加入**NavigationView**就能实现抽屉菜单的效果。布局如下：
```
<?xml version="1.0" encoding="utf-8"?>
<android.support.v4.widget.DrawerLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    android:id="@+id/drawer_layout"
    android:layout_height="match_parent"
    android:layout_width="match_parent"
    android:fitsSystemWindows="true">
    <!-- 添加内容部分 -->
    <include layout="@layout/content_layout"/>
    <!-- Navigation view -->
    <android.support.design.widget.NavigationView
        android:id="@+id/navigation_view"
        android:layout_height="match_parent"
        android:layout_width="wrap_content"
        android:layout_gravity="start"
        android:fitsSystemWindows="true"
        app:headerLayout="@layout/drawer_header"
        app:menu="@menu/drawer_view"/>
</android.support.v4.widget.DrawerLayout>
```
其中对于*NavigationView*有三个属性很关键：
- **app:menu**：表示菜单中的内容，是一个Item的列表，我们创建一个标准的menu文件即可；
- **app:headerLayout** – 抽屉菜单的头部，展示背景图片和相关文字；
- **android:layout_gravity** 代表菜单弹出的方向，必须要设置，一般可以设置**Start**从x轴始端，也就是左侧弹出，或者是**End**, 从x轴末端的方向，也就是右侧侧弹出。
在抽屉菜单中添加内容有两种方法：
- 在设计时利用提前准备好的menu文件添加菜单内容；
- 在运行时利用代码动态添加菜单内容。
在下文中，两种情况都会都为大家介绍。
## 利用menu文件
### 第一步，创建menu文件
创建带有menu内容的想XML文档（drawer_view.xml），其内容如下：
```
<group android:checkableBehavior="single">
        <item
            android:id="@+id/nav_home"
            android:icon="@drawable/ic_drawer_home"
            android:title="@string/nav_home" />
        <item
            android:id="@+id/nav_about"
            android:icon="@drawable/ic_drawer_about"
            android:title="@string/nav_about" />
        <item
            android:id="@+id/nav_settings"
            android:icon="@drawable/ic_drawer_settings"
            android:title="@string/nav_settings" />
        <item
            android:id="@+id/navigation_subheader"
            android:title="@string/nav_sub_header">
            <menu>
                <item
                    android:id="@+id/navigation_sub_item_1"
                    android:icon="@drawable/ic_drawer_about"
                    android:title="@string/nav_sub_item_1" />
                <item
                    android:id="@+id/navigation_sub_item_2"
                    android:icon="@drawable/ic_drawer_home"
                    android:title="@string/nav_sub_item_2" />
            </menu>
        </item>
    </group>
</menu>
```
注意，对于menu，可以通过设定属性**android:checkableBehavior **来定义菜单选项的响应模式：
- *single* – 只有一个Item能被选中(类似radio buttons)；
- *all* – 所有的Item都可以被选中(类似checkboxes)；
- *none* –没有Item能被选中。
### 第二步 创建header layout
从创建另一个XML布局文件（drawer_header.xml）来定义菜单的头部：
```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="192dp"
    android:background="?attr/colorPrimaryDark"
    android:padding="16dp"
    android:theme="@style/ThemeOverlay.AppCompat.Dark"
    android:orientation="vertical"
    android:gravity="bottom">
    <TextView
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Username"
        android:textAppearance="@style/TextAppearance.AppCompat.Body1"/>
</LinearLayout>
```
很简单都是线性布局，这里就不在细说了。
### 第三步 引用布局
就像上文提到的，在NavigationView的布局中，使用头部布局和内容布局。
### 第四部 初始化
在Activity中初始化NavigationView：
```
private NavigationView mNavigationView;
mNavigationView = (NavigationView) findViewById(R.id.navigation_view);
```
### 第五步 设定菜单监听器
设置菜单中的选项被选中后的事件：
```
//setting up selected item listener
navigationView.setNavigationItemSelectedListener(
       new NavigationView.OnNavigationItemSelectedListener() {
              @Override
              public boolean onNavigationItemSelected(MenuItem menuItem) {
                        menuItem.setChecked(true);
                        mDrawerLayout.closeDrawers();
                        return true;
              }
       });
```
## 利用代码，动态添加菜单内容
利用*NavigationView*对象的*getMenu()*方法来获取菜单对象，以添加内容：
```
final Menu menu = navigationView.getMenu();
for (int i = 1; i <= 3; i++) {
    menu.add("Runtime item "+ i);
}
```
利用*Menu*对象的子菜单属性，设定SubMenu：
```
// adding a section and items into it
final SubMenu subMenu = menu.addSubMenu("SubMenu Title");
for (int i = 1; i <= 2; i++) {
    subMenu.add("SubMenu Item " + i);
}
```
在添加菜单内容之后，需要刷新菜单内容的适配器：
```
for (int i = 0, count = mNavigationView.getChildCount(); i < count; i++) {
            final View child = mNavigationView.getChildAt(i);
            if (child != null && child instanceof ListView) {
                final ListView menuView = (ListView) child;
                final HeaderViewListAdapter adapter = (HeaderViewListAdapter) menuView.getAdapter();
                final BaseAdapter wrapped = (BaseAdapter) adapter.getWrappedAdapter();
                wrapped.notifyDataSetChanged();
            }
        }
```
## 其他注意事项
### ActionBarDrawerToggle
如果想监听抽屉菜单的开关状态，可以利用**ActionBarDrawerToggle** （V7）对象：
```
/**
     * In case if you require to handle drawer open and close states
     */
    private void setupActionBarDrawerToogle() {
        mDrawerToggle = new ActionBarDrawerToggle(this,                  /* host Activity */
                                                  mDrawerLayout,         /* DrawerLayout object */
                                                  R.string.drawer_open,  /* "open drawer" description */
                                                  R.string.drawer_close  /* "close drawer" description */
        ) {
            /**
             * Called when a drawer has settled in a completely closed state.
             */
            public void onDrawerClosed(View view) {
                Snackbar.make(view, R.string.drawer_close, Snackbar.LENGTH_SHORT).show();
            }
            /**
             * Called when a drawer has settled in a completely open state.
             */
            public void onDrawerOpened(View drawerView) {
                Snackbar.make(drawerView, R.string.drawer_open, Snackbar.LENGTH_SHORT).show();
            }
        };
        mDrawerLayout.setDrawerListener(mDrawerToggle);
    }
```
### ToolBar
利用原有的ActionBar不能实现NavigationView的效果，需要用ToolBar代替：
```
//在onCreate()方法中调用
private void setupToolbar() {
    //使用ToolBar替换原本的ActionBar
    Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
    setSupportActionBar(toolbar);
    final ActionBar ab = getSupportActionBar();
    if(ab!=null) {
        //设置ToolBar的图标
        ab.setHomeAsUpIndicator(R.drawable.ic_menu);
        //是否展示图标
        ab.setDisplayHomeAsUpEnabled(true);
    }
}
```
同时注意，Activity要使用***无ActionBar的主题***，否者ActionBar会和ToolBar冲突：
```
<style name="AppTheme" parent="Base.Theme.DesignDemo">
    </style>
    <style name="Base.Theme.DesignDemo" parent="Theme.AppCompat.Light.NoActionBar">
        <item name="colorPrimary">#2196F3</item>
        <item name="colorPrimaryDark">#1976D2</item>
        <item name="colorAccent">#E040FB</item>
        <item name="android:windowBackground">@color/window_background</item>
    </style>
```
最后还要为ToolBar设定点击事件，在点击菜单Home时，弹出菜单：
```
@Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            //当点击Toolbar的Home图标时
            case android.R.id.home:
                mDrawerLayout.openDrawer(GravityCompat.START);
                return true;
            case R.id.action_settings:
                return true;
        }
        return super.onOptionsItemSelected(item);
    }
```
如果不设定，抽屉菜单将无法弹出。
### 控制Back键
如果要实现点击Back键后菜单自动消失，还需要控制Back键响应事件：
```
@Override
    public void onBackPressed() {
        if (isNavDrawerOpen()) {
            closeNavDrawer();
        } else {
            super.onBackPressed();
        }
    }
    protected boolean isNavDrawerOpen() {
        return mDrawerLayout != null && mDrawerLayout.isDrawerOpen(GravityCompat.START);
    }
    protected void closeNavDrawer() {
        if (mDrawerLayout != null) {
            mDrawerLayout.closeDrawer(GravityCompat.START);
        }
    }
```
在点击Back键之后，先判断NavigationView是否弹出，如果处于打开状态，就关闭NavigationView。
注意我们在布局文件中定义NavigationView的 *android:layout_gravity="start"*，所以打开或者关闭菜单时要使用相应的方向*GravityCompat.START*。如果都是使用*GravityCompat.END*,效果就是从右侧弹出菜单。
![3297585-9b2c2f91a149095a.png](https://upload-images.jianshu.io/upload_images/3297585-9b2c2f91a149095a.png)
右侧菜单弹出
更多关于Design Support Library中控件的讲解将会在持续更新，欢迎关注。
最后给出[Github源码](https://link.jianshu.com?t=https://github.com/sunrongxin7666/DesignSupportLibraryDemo)
