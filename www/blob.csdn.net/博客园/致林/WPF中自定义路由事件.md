
# WPF中自定义路由事件 - 致林 - 博客园






# [WPF中自定义路由事件](https://www.cnblogs.com/bincoding/p/8665489.html)

publicclassMyButtonSimple: Button
{//Create a custom routed event by first registering a RoutedEventID//This event uses the bubbling routing strategypublicstaticreadonlyRoutedEvent TapEvent =EventManager.RegisterRoutedEvent("Tap", RoutingStrategy.Bubble,typeof(RoutedEventHandler),typeof(MyButtonSimple));//Provide CLR accessors for the eventpubliceventRoutedEventHandler Tap
    {
            add { AddHandler(TapEvent, value); } 
            remove { RemoveHandler(TapEvent, value); }
    }//This method raises the Tap eventvoidRaiseTapEvent()
    {
            RoutedEventArgs newEventArgs=newRoutedEventArgs(MyButtonSimple.TapEvent);
            RaiseEvent(newEventArgs);
    }//For demonstration purposes we raise the event when the MyButtonSimple is clickedprotectedoverridevoidOnClick()
    {
        RaiseTapEvent();
    }
}
<Windowxmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"xmlns:custom="clr-namespace:SDKSample;assembly=SDKSampleLibrary"x:Class="SDKSample.RoutedEventCustomApp"><Window.Resources><StyleTargetType="{x:Type custom:MyButtonSimple}"><SetterProperty="Height"Value="20"/><SetterProperty="Width"Value="250"/><SetterProperty="HorizontalAlignment"Value="Left"/><SetterProperty="Background"Value="\#808080"/></Style></Window.Resources><StackPanelBackground="LightGray"><custom:MyButtonSimpleName="mybtnsimple"Tap="TapHandler">Click to see Tap custom event work</custom:MyButtonSimple></StackPanel></Window>
备注：在MVVM模式中，不能直接绑定控件的路由事件到ViewModel，可以将事件绑定后台.cs中的方法，然后再调用ViewModel中的方法。
.cs文件：
privatevoidTapHandler(objectsender, RoutedEventArgs e)
{
    ViewModel vm=this.DataContext;
    vm.method;
}





