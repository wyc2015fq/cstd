
# android ViewPager页面左右滑动切换 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月09日 17:17:06[Jlins](https://me.csdn.net/dyllove98)阅读数：5286标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)[android开发																](https://so.csdn.net/so/search/s.do?q=android开发&t=blog)[viewpager																](https://so.csdn.net/so/search/s.do?q=viewpager&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=android开发&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=android&t=blog)个人分类：[android																](https://blog.csdn.net/dyllove98/article/category/1130032)
[
																								](https://so.csdn.net/so/search/s.do?q=android&t=blog)


我们日常生活中用到的微博，QQ，微信等app在进行页面左右滑动的时候，很多都可以用ViewPager来实现。可以说，ViewPager在android开发中十分常见并且特别实用。
Viewpager在android.support.v4.view这个软件包中， android.support.v4.view.jar是谷歌官方给我们提供的一个兼容低版本安卓设备的软件包，里面包囊了只有在安卓3.0以上可以使用的api。所以低版本开发时必须加入android-support-v4.jar，并且在XML文件使用ViewPager使用时以android.support.v4.view.ViewPager加入ViewPager。同时，该软件包还提供了一个PagerTitleStrip来显示每一个页卡的标题。
使用ViewPager实现页面滑动切换的方法和ListView差不多，同样也需要一个适配器，PagerAdapter，并且同样的也要重写其中的方法来实现。接下来我来说一下具体实现ViewPager的步骤。
1，在XML布局文件中加入ViewPager，以及它相对的标题PagerTitleStrip。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1<RelativeLayoutxmlns:android="http://schemas.android.com/apk/res/android"2xmlns:tools="http://schemas.android.com/tools"3android:layout_width="match_parent"4android:layout_height="match_parent">5<!--这里一定要注意通过android.support.v4.view包来使用ViewPager和PagerTitleStrip-->6<android.support.v4.view.ViewPager7android:id="@+id/viewrpager"8android:layout_width="wrap_content"9android:layout_height="wrap_content"10android:layout_gravity="center">1112<android.support.v4.view.PagerTitleStrip13android:id="@+id/pagertitle"14android:layout_width="wrap_content"15android:layout_height="wrap_content"16android:layout_gravity="top"17android:background="\#00000000">18</android.support.v4.view.PagerTitleStrip>19</android.support.v4.view.ViewPager>2021</RelativeLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)

2，接下来我们要添加三个页卡布局文件。具体内容可以自己添加，我为了方便每张页卡上只是用了一张图片作为背景。
3，为ViewPager添加一个适配器，这里我写了一个子类继承PagerAdapter。在子类中我们最重要的是要重写以下四个方法：
[instantiateItem(ViewGroup, int)](http://www.android-doc.com/reference/android/support/v4/view/PagerAdapter.html#instantiateItem%28android.view.ViewGroup,%20int%29)
[destroyItem(ViewGroup,
 int, Object)](http://www.android-doc.com/reference/android/support/v4/view/PagerAdapter.html#destroyItem%28android.view.ViewGroup,%20int,%20java.lang.Object%29)
[getCount()](http://www.android-doc.com/reference/android/support/v4/view/PagerAdapter.html#getCount%28%29)
[isViewFromObject(View,
 Object) ](http://www.android-doc.com/reference/android/support/v4/view/PagerAdapter.html#isViewFromObject%28android.view.View,%20java.lang.Object%29)
当然，如果想要更多更具体的功能，我们还可以重写其中的其他方法，但是以上四个方法是最必要的也是必须要重写实现的。
以下是该部分代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1packagecom.example.android_viewpage;23importjava.util.List;45importandroid.content.Context;6importandroid.support.v4.view.PagerAdapter;7importandroid.support.v4.view.ViewPager;8importandroid.view.View;910publicclassMyAdapterextendsPagerAdapter {11privateList<View> list;12privateList<String> titlelist;1314publicMyAdapter(List<String> titlelist, List<View> list) {15this.titlelist = titlelist;16this.list = list;17}18//这里获得当前页卡的标题19@Override20publicCharSequence getPageTitle(intposition) {21returntitlelist.get(position);22}23@Override24publicObject instantiateItem(View container,intposition) {25((ViewPager) container).addView(list.get(position));26returnlist.get(position);27}28//销毁页卡29@Override30publicvoiddestroyItem(View container,intposition, Object object) {31((ViewPager) container).removeView(list.get(position));32}33//得到所有页卡的个数34@Override35publicintgetCount() {36returnlist.size();37}38//判断当前显示页卡是否匹配39@Override40publicbooleanisViewFromObject(View arg0, Object arg1) {41returnarg0 == arg1;42}4344}![复制代码](http://common.cnblogs.com/images/copycode.gif)
4，最后在MainActivity中将你想要的页卡和标题分别存放在List中，然后将定义好的适配器供ViewPager使用就可以了。
以下是该部分代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1packagecom.example.android_viewpage;23importjava.util.ArrayList;4importjava.util.List;5importandroid.app.Activity;6importandroid.os.Bundle;7importandroid.support.v4.view.PagerTabStrip;8importandroid.support.v4.view.PagerTitleStrip;9importandroid.support.v4.view.ViewPager;10importandroid.view.LayoutInflater;11importandroid.view.View;1213publicclassMainActivityextendsActivity {14privateViewPager viewPager;15privatePagerTitleStrip pagerTitleStrip;16privateMyAdapter adapter;17privateList<View> list;18privateList<String> titlelist;1920@Override21protectedvoidonCreate(Bundle savedInstanceState) {22super.onCreate(savedInstanceState);23setContentView(R.layout.activity_main);24viewPager = (ViewPager) findViewById(R.id.viewrpager);25pagerTitleStrip = (PagerTitleStrip) findViewById(R.id.pagertitle);26list =newArrayList<View>();27list.add(LayoutInflater.from(this).inflate(R.layout.view1,null));28list.add(LayoutInflater.from(this).inflate(R.layout.view2,null));29list.add(LayoutInflater.from(this).inflate(R.layout.view3,null));30titlelist =newArrayList<String>();31titlelist.add("第一页");32titlelist.add("第二页");33titlelist.add("第三页");34adapter =newMyAdapter(titlelist, list);35viewPager.setAdapter(adapter);36}3738}![复制代码](http://common.cnblogs.com/images/copycode.gif)
这样就简单的实现了页面滑动切换的功能了。简要效果图如下：

但是我发现当我在滑动的时候标题的位置也跟着在一起移动，我想让页面在滑动的时候，标题不移动，但是要显示出不同的状态来。这里我简要的弄了一个类似微信的切换风格，在页卡滑动的时候让下方的按钮标题不同但显示出当前的状态，同时要求在点击下方的按钮时，也让页面切换到相对的状态下。
相比上面那个程序，这个主要的是设置了按钮的监听事件和页面滑动的监听事件。
其中页面滑动的监听事件是setOnPageChangeListener（）。它有onPageSelected（），onPageScrolled（），onPageScrollStateChanged（）三个方法，我这里只是对onPageSelected（）方法进行了重写。
以下是MainActivity部分的代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1packagecom.example.viewpagertest;23importjava.util.ArrayList;4importjava.util.List;56importandroid.R.integer;7importandroid.app.Activity;8importandroid.os.Bundle;9importandroid.support.v4.view.ViewPager;10importandroid.support.v4.view.ViewPager.OnPageChangeListener;11importandroid.view.LayoutInflater;12importandroid.view.View;13importandroid.view.View.OnClickListener;14importandroid.view.Window;15importandroid.widget.ImageButton;16importandroid.widget.LinearLayout;1718publicclassMainActivityextendsActivityimplementsOnClickListener {1920privateList<View> mPage;21privateMyPagerAdapter adapter;22privateViewPager viewPager;2324privateImageButton weixinImg;25privateImageButton friendImg;26privateImageButton addressImg;27privateImageButton settingImg;28privateLinearLayout weixin;29privateLinearLayout friend;30privateLinearLayout address;31privateLinearLayout setting;3233privatestaticfinalintWEIXIN_STATE = 0;34privatestaticfinalintFRIEND_STATE = 1;35privatestaticfinalintADDRESS_STATE = 2;36privatestaticfinalintSETTING_STATE = 3;3738@Override39protectedvoidonCreate(Bundle savedInstanceState) {40super.onCreate(savedInstanceState);41requestWindowFeature(Window.FEATURE_NO_TITLE);42setContentView(R.layout.activity_main);43intiView();44getClickEvent();45}46474849privatevoidintiView() {5051//呈现页面滑动效果52viewPager = (ViewPager) findViewById(R.id.viewpage);53mPage =newArrayList<View>();54mPage.add(LayoutInflater.from(this).inflate(R.layout.pageone,null));55mPage.add(LayoutInflater.from(this).inflate(R.layout.pagetwo,null));56mPage.add(LayoutInflater.from(this).inflate(R.layout.pagethree,null));57mPage.add(LayoutInflater.from(this).inflate(R.layout.pagefour,null));58adapter =newMyPagerAdapter(mPage);59viewPager.setAdapter(adapter);6061//加载控件62weixinImg = (ImageButton) findViewById(R.id.id_weixin_img);63friendImg = (ImageButton) findViewById(R.id.id_frd_img);64addressImg = (ImageButton) findViewById(R.id.id_address_img);65settingImg = (ImageButton) findViewById(R.id.id_settint_img);6667weixin = (LinearLayout) findViewById(R.id.id_weixin);68friend = (LinearLayout) findViewById(R.id.id_frd);69address = (LinearLayout) findViewById(R.id.id_address);70setting = (LinearLayout) findViewById(R.id.id_setting);7172}73/**74*          按钮的点击事件：当点击按钮时，先让所有的按钮变成暗色，75*          然后将所点击的按钮设置成亮色，并转到按钮所属页面76*/77@Override78publicvoidonClick(View v) {79setImg();80switch(v.getId()) {81caseR.id.id_weixin:82viewPager.setCurrentItem(WEIXIN_STATE);83weixinImg.setImageResource(R.drawable.tab_weixin_pressed);84break;8586caseR.id.id_frd:87viewPager.setCurrentItem(FRIEND_STATE);88friendImg.setImageResource(R.drawable.tab_find_frd_pressed);89break;9091caseR.id.id_address:92viewPager.setCurrentItem(ADDRESS_STATE);93addressImg.setImageResource(R.drawable.tab_address_pressed);94break;9596caseR.id.id_setting:97viewPager.setCurrentItem(SETTING_STATE);98settingImg.setImageResource(R.drawable.tab_settings_pressed);99break;100101}102103}104privatevoidgetClickEvent() {105weixin.setOnClickListener(this);106friend.setOnClickListener(this);107address.setOnClickListener(this);108setting.setOnClickListener(this);109/**110*   页面滑动的监听事件：滑动页面时，先将所有按钮变成暗色111*   再把页面所属的按钮变成亮色，表示所属页面112*/113viewPager.setOnPageChangeListener(newOnPageChangeListener() {114115@Override116publicvoidonPageSelected(intarg0) {117setImg();118switch(arg0) {119caseWEIXIN_STATE:120weixinImg.setImageResource(R.drawable.tab_weixin_pressed);121break;122caseFRIEND_STATE:123friendImg.setImageResource(R.drawable.tab_find_frd_pressed);124break;125caseADDRESS_STATE:126addressImg.setImageResource(R.drawable.tab_address_pressed);127break;128caseSETTING_STATE:129settingImg130.setImageResource(R.drawable.tab_settings_pressed);131break;132133}134}135136@Override137publicvoidonPageScrolled(intarg0,floatarg1,intarg2) {138139}140141@Override142publicvoidonPageScrollStateChanged(intarg0) {143144}145});146147}148//在点击按钮或者活动页面的时候，让所有按钮变成暗色149privatevoidsetImg() {150weixinImg.setImageResource(R.drawable.tab_weixin_normal);151friendImg.setImageResource(R.drawable.tab_find_frd_normal);152addressImg.setImageResource(R.drawable.tab_address_normal);153settingImg.setImageResource(R.drawable.tab_settings_normal);154155}156157}![复制代码](http://common.cnblogs.com/images/copycode.gif)
这样就基本完成了想要的效果，以下是效果图：



参考资料：


## android学习笔记（60）使用Bundle在Activity之间文换教接
## android学习笔记（59） android开发启动和关闭activity

## [android学习笔记（58）Activity学习过程](http://www.itmmd.com/201502/597.html)
## 更多
## [android开发](http://www.itmmd.com/)
## 教程

![](https://img-blog.csdn.net/20150318165449269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

