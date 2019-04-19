# Android Data Binding使用笔记 - =朝晖= - 博客园
# [Android Data Binding使用笔记](https://www.cnblogs.com/dhcn/p/7124997.html)
说在前面：先来三个文档，官网文档:https://developer.[Android](http://lib.csdn.net/base/android).com/topic/libraries/data-binding/index.html 官网文档的汉化版：http://www.jianshu.com/p/b1df61a4df77  github的第三方文档：https://github.com/LyndonChin/MasteringAndroidDataBinding
1、[Java](http://lib.csdn.net/base/java)的语言动态属性非常有限，所以这个绑定的实现是依赖于代码生成技术，从技术的角度感觉不是很Nice。官网文档个错误就和静态语言特性有关，比如对Bind设定变量用的是setVariable函数，而不是官方指南的那个函数。用法如下：
- viewDataBinding.setVariable(BR.variable_name,varaible);  
2、比如渲染List<SomeTyep>,因为data属性定义里面不能存在“<”">"字符，需要把两个符号换成HTML转义字符串<和>, 而且里面的类型除了Java内建类型，其他的最好写成类全名，否则找不到。
3、如果渲染ImageView则需要运用BindingAdapter特性，先实现一个BindingAdapter：
- @BindingAdapter({"imageUrl"})  
- public static void loadImage(ImageView view, String url){  
- if (url==null || "".equals(url)){  
-            view.setImageDrawable(view.getContext().getResources().getDrawable(R.drawable.place_image));  
-        }else{  
-            Glide.with(view.getContext()).load(url).into(view);  
-        }  
-    }  
然后就可以直接渲染url了：
- <ImageView  
- xmlns:app="http://schemas.android.com/apk/res-auto"<span style="white-space:pre"> </span>  
- android:layout_width="32dp"  
- android:layout_height="32dp"  
- app:imageUrl="@{user.avatar}"  
- />  
4、onCreate加载问题：在如果在Activity onCreate中加载网络对象，且布局中有依赖这个对象的bool变量，则可能导致null对象调用错误，估计原因是因为网络加载时异步的，所以视图渲染时，还未传对象导致，解决方法，在网路对象加载前可以传一个占位对象，里面把需要的布尔变量赋值了，充一时(最多几百毫秒)之用。

