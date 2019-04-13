
# WPF自定义行为Behavior，实现双击控件复制文本 - 致林 - 博客园






# [WPF自定义行为Behavior，实现双击控件复制文本](https://www.cnblogs.com/bincoding/p/8626280.html)
WPF引用xmlns:i="clr-namespace:System.Windows.Interactivity;assembly=System.Windows.Interactivity"后可以设置很多自定义的行为：
<i:Interaction.Triggers><i:EventTriggerEventName="ValueChanged"><i:InvokeCommandActionCommand="{Binding ValueChangedCommand}"/></i:EventTrigger></i:Interaction.Triggers>
<UserControl.Resources><ControlTemplatex:Key="trackThumb"TargetType="{x:Type Slider}"><BorderBackground="{TemplateBinding Background}"BorderBrush="{TemplateBinding BorderBrush}"BorderThickness="{TemplateBinding BorderThickness}"><Grid><Trackx:Name="PART_Track"><Track.Thumb><ThumbWidth="10"><i:Interaction.Triggers><i:EventTriggerEventName="DragCompleted"><i:InvokeCommandActionCommand="{Binding ValueChangedCommand}"/></i:EventTrigger></i:Interaction.Triggers></Thumb></Track.Thumb></Track></Grid></Border></ControlTemplate></UserControl.Resources>
当时当我们有一些自定义的需求时，需要自定义行为，例如给每个控件添加一个双击复制文本的行为。
1. 定义行为
publicclassMouseDoubleClickCopyTextBehavior : Behavior<FrameworkElement>{///<summary>///需要复制的内容///</summary>publicstringCopyText
        {get{return(string)GetValue(CopyTextProperty); }set{ SetValue(CopyTextProperty, value); }
        }publicstaticreadonlyDependencyProperty CopyTextProperty = DependencyProperty.Register("CopyText",typeof(string),typeof(MouseDoubleClickCopyTextBehavior),newPropertyMetadata(null));protectedoverridevoidOnAttached()
        {base.OnAttached();
            AssociatedObject.PreviewMouseLeftButtonDown+=AssociatedObject_PreviewMouseLeftButtonDown;
        }protectedoverridevoidOnDetaching()
        {base.OnDetaching();
            AssociatedObject.PreviewMouseLeftButtonDown-=AssociatedObject_PreviewMouseLeftButtonDown;
        }voidAssociatedObject_PreviewMouseLeftButtonDown(objectsender, MouseButtonEventArgs e)
        {if(e.ClickCount >=2)
                Clipboard.SetDataObject(CopyText);
        }
2. 控件绑定行为
<StyleTargetType="Label"BasedOn="{StaticResource {x:Type Label}}"><SetterProperty="VerticalAlignment"Value="Center"/><SetterProperty="Template"><Setter.Value><ControlTemplateTargetType="Label"><Border><ContentPresenterName="ContentPresenter"HorizontalAlignment="{TemplateBinding HorizontalContentAlignment}"VerticalAlignment="{TemplateBinding VerticalContentAlignment}"RecognizesAccessKey="True"><!--绑定自定义行为--><i:Interaction.Behaviors>
<hei:MouseDoubleClickCopyTextBehaviorCopyText="{Binding ElementName=ContentPresenter,Path=Content}"/></i:Interaction.Behaviors></ContentPresenter></Border><ControlTemplate.Triggers><TriggerProperty="IsEnabled"Value="false"><SetterProperty="Foreground"><Setter.Value><SolidColorBrushColor="{DynamicResource DisabledForegroundColor}"/></Setter.Value></Setter></Trigger></ControlTemplate.Triggers></ControlTemplate></Setter.Value></Setter></Style>






