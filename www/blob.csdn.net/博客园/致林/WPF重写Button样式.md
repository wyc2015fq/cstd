
# WPF重写Button样式 - 致林 - 博客园






# [WPF重写Button样式](https://www.cnblogs.com/bincoding/p/8310022.html)
首先指定OverridesDefaultStyle属性为True；
然后添加样式；
重写ControlTemplate；
<Window.Resources><Stylex:Key="UserButtonStyle"TargetType="Button"><SetterProperty="OverridesDefaultStyle"Value="True"/><SetterProperty="Cursor"Value="Hand"/><SetterProperty="Padding"Value="5 3"/><SetterProperty="Background"Value="LightBlue"/><SetterProperty="Template"><Setter.Value><ControlTemplateTargetType="Button"><BorderName="border"BorderThickness="0"BorderBrush="Black"Padding="{TemplateBinding Padding}"Background="{TemplateBinding Background}"><ContentPresenterHorizontalAlignment="Center"VerticalAlignment="Center"/></Border><ControlTemplate.Triggers><TriggerProperty="IsMouseOver"Value="True"><SetterProperty="Opacity"Value="0.8"/></Trigger></ControlTemplate.Triggers></ControlTemplate></Setter.Value></Setter></Style></Window.Resources>





