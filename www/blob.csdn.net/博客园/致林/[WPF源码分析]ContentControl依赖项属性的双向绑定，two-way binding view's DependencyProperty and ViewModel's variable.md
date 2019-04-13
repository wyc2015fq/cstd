
# [WPF源码分析]ContentControl依赖项属性的双向绑定，two-way binding view's DependencyProperty and ViewModel's variable - 致林 - 博客园






# [[WPF源码分析]ContentControl依赖项属性的双向绑定，two-way binding view's DependencyProperty and ViewModel's variable](https://www.cnblogs.com/bincoding/p/8342791.html)
问题：自定义控件的依赖项属性和VIewModel中的变量不能双向绑定
解决思路：对比.net源码 PresentationFramework  /   System.Windows.Controls
原因：定义依赖项属性时没有设置OnChanged方法
解决方法：初始化时绑定Changed方法
.net 源码如下：
///<summary>///The DependencyProperty for the Content property.///Flags:              None///Default Value:      null///</summary>[CommonDependencyProperty]publicstaticreadonlyDependencyProperty ContentProperty =DependencyProperty.Register("Content",typeof(object),typeof(ContentControl),newFrameworkPropertyMetadata(
                                (object)null,newPropertyChangedCallback(OnContentChanged)));///<summary>///Content is the data used to generate the child elements of this control.///</summary>[Bindable(true), CustomCategory("Content")]publicobjectContent
        {get{returnGetValue(ContentProperty); }set{ SetValue(ContentProperty, value); }
        }///<summary>///Called when ContentProperty is invalidated on "d."///</summary>privatestaticvoidOnContentChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {//根据需要实现自己的方法ContentControl ctrl =(ContentControl) d;
            ctrl.SetValue(HasContentPropertyKey, (e.NewValue!=null) ?BooleanBoxes.TrueBox : BooleanBoxes.FalseBox);
 
            ctrl.OnContentChanged(e.OldValue, e.NewValue);
        }





