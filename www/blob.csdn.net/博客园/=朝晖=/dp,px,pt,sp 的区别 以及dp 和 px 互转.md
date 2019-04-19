# dp,px,pt,sp 的区别 以及dp 和 px 互转 - =朝晖= - 博客园
# [dp,px,pt,sp 的区别 以及dp 和 px 互转](https://www.cnblogs.com/dhcn/p/7120967.html)
dp = dip : device independent pixels(设备独立像素). 不同设备有不同的显示效果,这个和设备硬件有关，一般我们为了支持WVGA、HVGA和QVGA 推荐使用这个，不依赖像素。
px: pixels(像素). 不同设备显示效果相同，一般我们HVGA代表320x480像素，这个用的比较多。
pt: point，是一个标准的长度单位，1pt＝1/72英寸，用于印刷业，非常简单易用；
sp: scaled pixels(放大像素). 主要用于字体显示best for textsize。
由此，根据 google 的建议，TextView 的字号最好使用 sp 做单位，而且查看
TextView
的源码可知 Android 默认使用 sp 作为字号单位。
在 Android 中，  1pt 大概等于 2.22sp
以上供参考，如果 UI 能够以 sp 为单位提供设计是最好的，如果设计中没有 sp
的概念，则开发人员也可以通过适当的换算取近似值。
过去，程序员通常以像素为单位设计计算机用户界面。例如，定义一个宽度为300像素的表单字段，列之间的间距为5个像素，图标大小为16×16像素 等。这样处理的问题在于，如果在一个每英寸点数（dpi）更高的新显示器上运行该程序，则用户界面会显得很小。在有些情况下，用户界面可能会小到难以看清 内容。
与分辨率无关的度量单位可以解决这一问题。Android支持下列所有单位。
px（像素）：屏幕上的点。
in（英寸）：长度单位。
mm（毫米）：长度单位。
pt（磅）：1/72英寸。
dp（与密度无关的像素）：一种基于屏幕密度的抽象单位。在每英寸160点的显示器上，1dp = 1px。
dip：与dp相同，多用于android/ophone示例中。
sp（与刻度无关的像素）：与dp类似，但是可以根据用户的字体大小首选项进行缩放。
为了使用户界面能够在现在和将来的显示器类型上正常显示，建议大家始终使用sp作为文字大小的单位，将dip作为其他元素的单位。当然，也可以考虑使用矢量图形，而不是用位图
------------------------------------------------------------------------------------
- **dp：**A dp is a density-independent pixel that corresponds to the physical size of a pixel at 160 dpi(dots per inch:每英寸点数). 
- dp也就是dip:device independent pixels(设备独立像素)
- dp是一种与密度无关的像素单位，在每英寸160点的屏幕上，1dp = 1px
- 不同设备有不同的显示效果,这个和设备硬件有关，一般我们为了支持WVGA、HVGA和QVGA 推荐使用这个，不依赖像素
> 
> 
- <Button android:layout_width="wrap_content"   
- android:layout_height="wrap_content"   
- android:text="@string/clickme"  
- android:layout_marginTop="20dp" />  
> - **sp：**An sp is the same base unit, but is scaled by the user's preferred text size (it’s a scale-independent pixel), so you should use this measurement unit when defining text size (but never for layout sizes).
- scaled pixels(刻度像素). 主要用于定义字体的大小，而从来不再layout上使用
- 
- <TextView android:layout_width="match_parent"   
- android:layout_height="wrap_content"   
- android:textSize="20sp" />  
> - **px：**pixels(像素). 不同设备显示效果相同，一般我们HVGA代表320x480像素，这个用的比较多
**总结：**dp也就是dip。这个和sp基本类似。如果设置表示长度、高度等属性时可以使用dp或sp。但如果设置字体，需要使用sp。dp是与密度无关，sp除了与密度无关外，还与scale无关。如果屏幕密度为160，这时dp和sp和px是一样的。1dp=1sp=1px，但如果使用px作单位，如果屏幕大小不变（假设还是3.2寸），而屏幕密度变成了320。那么原来TextView的宽度设成160px，在密度为320的3.2寸屏幕里看要比在密度为160的3.2寸屏幕上看短了一半。但如果设置成160dp或160sp的话。系统会自动将width属性值设置成320px的。也就是160 * 320 / 160。其中320 / 160可称为密度比例因子。也就是说，如果使用dp和sp，系统会根据屏幕密度的变化自动进行转换.
附：px 和 dp 互转换
- package com.hujl.util;
import android.content.Context;  
public class DensityUtil {  
    /** 
     * 根据手机的分辨率从 dp 的单位 转成为 px(像素) 
     */  
    public static int dip2px(Context context, float dpValue) {  
        final float scale = context.getResources().getDisplayMetrics().density;  
        return (int) (dpValue * scale + 0.5f);  
    }  
    /** 
     * 根据手机的分辨率从 px(像素) 的单位 转成为 dp 
     */  
    public static int px2dip(Context context, float pxValue) {  
        final float scale = context.getResources().getDisplayMetrics().density;  
        return (int) (pxValue / scale + 0.5f);  
    }  
}  
- 

