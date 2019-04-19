# Android - LayoutInflate用法 - 三少GG - CSDN博客
2011年12月09日 00:43:29[三少GG](https://me.csdn.net/scut1135)阅读数：2261标签：[android																[layout																[service																[xml																[button																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
[http://daydayup1989.iteye.com/blog/824735](http://daydayup1989.iteye.com/blog/824735)
通俗的说,inflate就相当于将一个xml中定义的布局找出来.
因为在一个Activity里如果直接用findViewById()的话,对应的是setConentView()的那个layout里的组件.
因此如果你的Activity里如果用到别的layout,比如对话框上的layout,你还要设置对话框上的layout里的组件(像图片ImageView,文字TextView)上的内容,你就必须用inflate()先将对话框上的layout找出来,然后再用这个layout对象去找到它上面的组件,如:
View view = View.inflate(this, R.layout.dialog_layout, null);
TextView dialogTV = (TextView) view.findViewById(R.id.dialog_tv);
dialogTV.setText("abcd");
如果组件R.id.dialog_tv是对话框上的组件,而你直接用this.findViewById(R.id.dialog_tv)肯定会报错.
三种方式可以生成LayoutInflater ：
LayoutInflater inflater = LayoutInflater.from(this);
LayoutInflater inflater = getLayoutInflater();
LayoutInflater inflater = (LayoutInflater) this.getSystemService(LAYOUT_INFLATER_SERVICE);
然后调用inflate方法将xml布局文件转成View
public View inflate (int resource, ViewGroup root, boolean attachToRoot)
在View类中，也有inflate方法
public static View inflate (Context context, int resource, ViewGroup root)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Android - LayoutInflate用法](http://hi.baidu.com/chensb666/blog/item/7bbc1c38874ec1d97c1e7155.html)
2011-09-15 11:21
在实际开发中LayoutInflater这个类还是非常有用的，它的作用类似于findViewById()。不同点是LayoutInflater是用来找res/layout/下的xml布局文件，并且实例化；而findViewById()是找xml布局文件下的具体widget控件(如Button、TextView等)。
具体作用：
1、对于一个没有被载入或者想要动态载入的界面，都需要使用LayoutInflater.inflate()来载入；
2、对于一个已经载入的界面，就可以使用Activiyt.findViewById()方法来获得其中的界面元素。
LayoutInflater 是一个抽象类，在文档中如下声明：
publicabstractclass LayoutInflater extends Object  
**获得 LayoutInflater 实例的三种方式**
**1.**LayoutInflater inflater = getLayoutInflater();  //调用Activity的getLayoutInflater()
**2.**LayoutInflater localinflater = (LayoutInflater)context.getSystemService
                                     (Context.LAYOUT_INFLATER_SERVICE);
**3.** LayoutInflater inflater = LayoutInflater.from(context);   
其实，这三种方式本质是相同的，从源码中可以看出：
**getLayoutInflater()：**
Activity 的 getLayoutInflater() 方法是调用 PhoneWindow 的getLayoutInflater()方法，看一下该源代码：
public PhoneWindow(Context context) {  super(context);          mLayoutInflater = LayoutInflater.from(context);  }
可以看出它其实是调用 LayoutInflater.from(context)。
**LayoutInflater.from(context)：**
**public****static** LayoutInflater from(Context context) {
       LayoutInflater LayoutInflater =               (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);   **if** (LayoutInflater == **null**) {
   **throw****new** AssertionError("LayoutInflater not found.");
       }   **return** LayoutInflater;
   } 
可以看出它其实调用 context.getSystemService()。
**结论：所以这三种方式最终本质是都是调用的Context.getSystemService()。**
**inflate 方法**
通过 sdk 的 api 文档，可以知道该方法有以下几种过载形式，返回值均是 View 对象，如下：
public View inflate (int resource, ViewGroup root)  public View inflate (XmlPullParser parser, ViewGroup root)  public View inflate (XmlPullParser parser, ViewGroup root, boolean attachToRoot)  public View inflate (int resource, ViewGroup root, boolean attachToRoot)  
示意代码：
LayoutInflater inflater = (LayoutInflater)getSystemService(LAYOUT_INFLATER_SERVICE);  View view = inflater.inflate(R.layout.custom, (ViewGroup)findViewById(R.id.test));  //EditText editText = (EditText)findViewById(R.id.content);// errorEditText editText = (EditText)view.findViewById(R.id.content);  
**对于上面代码，指定了第二个参数 ViewGroup root，当然你也可以设置为 null 值。**
注意：
·inflate方法与 findViewById 方法不同；
·inflater 是用来找 res/layout下的 xml 布局文件，并且实例化；
·findViewById() 是找具体 xml 布局文件中的具体 widget 控件(如:Button、TextView 等)。
