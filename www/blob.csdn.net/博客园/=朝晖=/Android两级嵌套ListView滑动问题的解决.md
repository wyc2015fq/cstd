# Android两级嵌套ListView滑动问题的解决 - =朝晖= - 博客园
# [Android两级嵌套ListView滑动问题的解决](https://www.cnblogs.com/dhcn/p/7130832.html)
[Android](http://lib.csdn.net/base/android)下面两级嵌套ListView会出现滑动失效，解决方案，把两级Listview全换成NoScrollListView，代码如下：
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
       然后在父Listview外面放个ScrollView实现滑动就Ok了

