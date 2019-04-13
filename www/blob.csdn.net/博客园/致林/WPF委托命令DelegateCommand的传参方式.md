
# WPF委托命令DelegateCommand的传参方式 - 致林 - 博客园






# [WPF委托命令DelegateCommand的传参方式](https://www.cnblogs.com/bincoding/p/8288677.html)
首先引用  Microsoft.Practices.Prism
MVVM模式代码如下：
XAML代码：
<!--无参方式--><ButtonContent="Test Command"Command="{Binding TestCommand}"/><!--将自己作为参数--><ButtonContent="Test Command2"Command="{Binding TestCommand2}"CommandParameter="{Binding RelativeSource={x:Static RelativeSource.Self}}"><!--将父元素作为参数--><ButtonContent="Test Command3"Command="{Binding TestCommand3}"CommandParameter="{Binding RelativeSource={x:Static RelativeSource.TemplatedParent}}">
后台代码：
//code-behind构造函数中添加：this.DataContext =newViewModel();
ViewModel代码：
//ViewModel 构造函数publicViewModel()
{CallCOmmand1 =newDelegateCOmmmand(Call1);
    CallCOmmand2=newDelegateCOmmmand<Object>(Call2);
    CallCOmmand3=newDelegateCOmmmand<Object>(Call3);
}//命令声明publicDelegateCommand CallCommand {get;privateset; }publicDelegateCommand<Object> CallCommand2 {get;privateset; }publicDelegateCommand<Object> CallCommand3 {get;privateset; }//命令实现publicvoidCall1()
{
}publicvoidCall2( Object obj )
{
    Button button= objasButton;
}publicvoidCall3( Object obj )
{
    ParentType parent= objasParentType;
}





