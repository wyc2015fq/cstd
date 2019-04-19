# 完美解决ScollView内嵌ListView的问题 - =朝晖= - 博客园
# [完美解决ScollView内嵌ListView的问题](https://www.cnblogs.com/dhcn/p/7130815.html)
   1、之前看了别人的一代码，解决办法是自己定制一个ListView，代码如下：
- public class NoScrollListView extends ListView {  
- public NoScrollListView(Context context) {  
- super(context);  
-     }  
- 
- public NoScrollListView(Context context, AttributeSet attrs) {  
- super(context, attrs);  
-     }  
- 
- public NoScrollListView(Context context, AttributeSet attrs, int defStyle) {  
- super(context, attrs, defStyle);  
-     }  
- 
- @Override  
- public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {  
- int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2,  
-                 MeasureSpec.AT_MOST);  
- super.onMeasure(widthMeasureSpec, expandSpec);  
-     }  
- 
- }  
     2、上面这个代码有bug，这个改造过的istview内部对某些数据源型的layout计算有问题。于是乎，把这个listview换成RecyclerView，图形渲染效果出来了，但是外层ScrollView的滑动效果比较涩，交互受影响。
    3、切回传统思路，在Listview上加headerView，渲染效果和交互效果都Ok，看来华山只有一条路。

