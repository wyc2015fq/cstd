# Android视图篇之一：Android常见基本布局 - weixin_33985507的博客 - CSDN博客
2013年11月03日 22:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Android中，布局都是直接或间接的继承自ViewGroup类，其中，ViewGroup的直接子类目前有：
AbsoluteLayout, AdapterView<T extends Adapter>, DrawerLayout, FragmentBreadCrumbs, FrameLayout, GridLayout, LinearLayout, PagerTitleStrip, RelativeLayout, SlidingDrawer, SlidingPaneLayout, ViewPager。
其中，DrawerLayout、PagerTitleStrip、SlidingPaneLayout和ViewPager 是Android API level 4（1.6版本）及其以后通过引入android.support.v4得以支持。GridLayout是Android API level 7（2.1版本）通过引入android.support.v7得以支持。SlidingDrawer在Android API level 17（4.2版本）废弃。FragmentBreadCrumbs也较少用到。
同时，虽然TableLayout是继承自LinearLayout，但TableLayout在日常开发中也经常用到，TableLayout也算在其中。因此，本文将主要讲解Android中最常见的几种布局方式：FrameLayout、LinearLayout、RelativeLayout、TableLayout和AbsoluteLayout。AdapterView<T extends Adapter>将在下一篇中详细阐述。
1、FrameLayout
Framelayout是Android中最简单的一种布局方式。**其内部的view都是从屏幕左上角开始堆放**，如果有多个view，后一个view的可见区域将遮挡前一个view相应区域。
可以简单的理解成墙角堆放积木的效果。
**若想达到不遮挡前一view，可以通过对view设置layout_gravity进行简单定位**。
layout_gravity含义：此view相对于其父布局的对其方式。
2、LinearLayout
LinearLayout即线性布局，可以通过设定设定其android:orientation属性值来确定是水平方向上的线性布局还是垂直方向上的线性布局。默认为水平方向的线性布局方式。
**LinearLayout中的view有一个非常重要的属性android:weight，表示在相应方向上剩余空间的比重，优先级高于view本身相应方向上的长度。**以水平方向为例，如果设置了android:weight，则android:width属性值建议设置成0dp，以取得更好的性能效果。对于两个view而言，其中一个未设置android:weight属性，则默认值为0，另一个设置了android:weight属性值为1，由1/(1+0)=100%，其将占据此方向上全部的剩余空间。
**同时，对于LinearLayout中的view，同样可以使用android:layout_gravity属性，但是，是有条件的**。对于水平线性布局而言，android:layout_gravity在水平方向上失效；对于垂直线性布局而言，android:layout_gravity在垂直方向上失效。
3、RelativeLayout
RelativeLayout是Android中相对最灵活的一种布局方式。主要可以设置与父控件的对齐方式和相对于其他控件的对其方式。通过灵活设置不同的对齐属性，可以达到复杂的布局效果，**但前提是view需要具有相应的android:id="@+id/viewid"**。
4.TableLayout
TableLayout即表格布局。**使用TableLayout时需要注意以下几点**：
1). TbaleLayout类似于Html中的table，但是默认行列式没有边框的；
2). TableLayout是LinearLayout的子类，因为可以对其中的view使用android:weight属性；
3). Tablelayout的children不一定是TableRow，也可以直接是其他的view。行列也不像html中的table那样具有明显的对其模式；
4). TableLayout中的children无需指定宽度，宽度默认都是match_parent。为其指定其他宽度值都是无效的；
5). TableLayout中的children如果为TableRow，则无需为其指定高度，高度默认都是wrap_content。为其指定其他高度都是无效的，若children为其他view，则可以指定其他宽度值；
6). TableLayout的属性android:collapseColumns设置隐藏，但是只针对child为tablerow有效；
7). tablelayout的属性android:stretchColumns设置可拉伸列，android:shrinkColumns 设置可收缩列，优先级都是大于weight属性。
5、AbsoluteLayout
AbsoluteLayout即绝对布局（坐标布局），是相对于父控件的左上角开始的布局方式。通过给AbsoluteLayout中的view设置android:layout_x和android:layout_y属性值来确定此view本身左上角的位置，以此完成定位。
**AbsoluteLayout在Android API level 3（1.5版本）中已经被废弃**，由于此种布局方式不能较好的适配Android多种不同的屏幕效果，不推荐使用。
