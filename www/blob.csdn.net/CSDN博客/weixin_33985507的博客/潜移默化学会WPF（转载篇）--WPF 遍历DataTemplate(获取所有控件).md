# 潜移默化学会WPF（转载篇）--WPF 遍历DataTemplate(获取所有控件) - weixin_33985507的博客 - CSDN博客
2012年04月09日 18:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

```
情况1：在设定DataTemplate的Name,并且他是在前台表示时，获取DataTemplate里的指定控件。 方法： http://blog.csdn.net/wackelbh/article/details/6003947(参考这篇文章) 情况2：当没有设定DataTemplate的Name或是以Resource方式调用时，获取DataTemplate里的指定控件。 方法： 1、这里需要有一个从DataTemplate里获取控
```
情况1：在设定DataTemplate的Name,并且他是在前台表示时，获取DataTemplate里的指定控件。
　　方法：
　　http://blog.csdn.net/wackelbh/article/details/6003947(参考这篇文章)
　　情况2：当没有设定DataTemplate的Name或是以Resource方式调用时，获取DataTemplate里的指定控件。
　　方法：
　　1、这里需要有一个从DataTemplate里获取控件的函数
```
public T FindFirstVisualChild<T>(DependencyObject obj, string childName) where T : DependencyObject  
        {  
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(obj); i++)  
            {  
                DependencyObject child = VisualTreeHelper.GetChild(obj, i);  
                if (child != null && child is T && child.GetValue(NameProperty).ToString() == childName)  
                {  
                    return (T)child;  
                }  
                else  
                {  
                    T childOfChild = FindFirstVisualChild<T>(child, childName);  
                    if (childOfChild != null)  
                    {  
                        return childOfChild;  
                    }  
                }  
            }  
            return null;  
        }
```
　2、稍微改动一下前篇里的代码：
```
<ItemsControl  x:Name="itemsControl"  Background="#B28BB2F1">    
    <ItemsControl.ItemsPanel>    
        <ItemsPanelTemplate>    
          <WrapPanel Orientation="Horizontal"/>    
        </ItemsPanelTemplate>    
    </ItemsControl.ItemsPanel>    
    <ItemsControl.ItemTemplate>    
        <DataTemplate>    
          <Border Padding="3">    
            <WrapPanel>    
              <TextBox x:Name="txtID"/>    
              <TextBlock x:Name="txtName" Text="Good"/>     
            </WrapPanel>    
          </Border>    
        </DataTemplate>    
    </ItemsControl.ItemTemplate>    
</ItemsControl>
```
或者
```
<Page.Resource>  
        <DataTemplate x:Key="data">    
          <Border Padding="3">    
            <WrapPanel>    
              <TextBox x:Name="txtID"/>    
              <TextBlock x:Name="txtName" Text="Good"/>     
            </WrapPanel>    
          </Border>    
        </DataTemplate>   
</Page.Resources>  
  
<ItemsControl  x:Name="itemsControl"  Background="#B28BB2F1"  ItemTemplate="{StaticResource data}">    
    <ItemsControl.ItemsPanel>    
        <ItemsPanelTemplate>    
          <WrapPanel Orientation="Horizontal"/>    
        </ItemsPanelTemplate>    
    </ItemsControl.ItemsPanel>    
</ItemsControl>
```
3、解下来就写按钮的处理函数：
我需要获取DataTemplate里名为txtName的TextBlock控件并显示他的Text内容。 private void Button_Click( object sender,System.Windows.RoutedEventArgse) { TextBlocktxt=FindFirstVisualChildTextBox(itemsContro
我需要获取DataTemplate里名为"txtName"的TextBlock控件并显示他的Text内容。
- **private****void** Button_Click(**object** sender, System.Windows.RoutedEventArgs e)  
- {  
-     TextBlock txt = FindFirstVisualChild<TextBox>(itemsControl, "txtName");  
- **if** (txt != **null**)//判断是否找到 
-         MessageBox.Show(txt.Text.ToString());  
- }  
　　情况3：当没有设定DataTemplate的里的控件Name或者你压根不知道里面有哪些控件，但是你又想获取他们的值时。例如上一篇，当我动态生成CheckBox后，我想知道哪些CheckBox被选中了。
　　方法：
　　1、也需要一个获取DataTemplate控件的函数，但是返回的是一个集合。
```
public List<T> GetChildObjects<T>(DependencyObject obj, string name) where T : FrameworkElement  
{  
     DependencyObject child = null;  
     List<T> childList = new List<T>();  
     for (int i = 0; i <= VisualTreeHelper.GetChildrenCount(obj) - 1; i++)  
         {  
           child = VisualTreeHelper.GetChild(obj, i);  
           if (child is T && (((T)child).Name == name || string.IsNullOrEmpty(name)))  
              {  
                 childList.Add((T)child);  
              }  
           childList.AddRange(GetChildObjects<T>(child, ""));//指定集合的元素添加到List队尾   
         }  
     return childList;  
 }
```
　2、xaml中代码(详细请看前一篇)
```
<ItemsControl  x:Name="itemsControl"  Background="#B28BB2F1">    
    <ItemsControl.ItemsPanel>    
        <ItemsPanelTemplate>    
          <WrapPanel Orientation="Horizontal"/>    
        </ItemsPanelTemplate>    
    </ItemsControl.ItemsPanel>    
    <ItemsControl.ItemTemplate>    
        <DataTemplate>    
          <Border Padding="3">    
            <WrapPanel>    
              <CheckBox  Content="{Binding txt}"/>    
            </WrapPanel>    
          </Border>    
        </DataTemplate>    
    </ItemsControl.ItemTemplate>    
</ItemsControl>
```
　3、解下来就写按钮的处理函数：
```
private void Button_Click(object sender, System.Windows.RoutedEventArgs e)  
{  
   DataVisualTreeHelper VTHelper = new DataVisualTreeHelper();  
   List<CheckBox> collection = VTHelper.GetChildObjects<CheckBox>(itemsControl, "")//第2个参数为空，表示查找所有指定类型的控件（返回   
  
一个CheckBox集合）     
   foreach (CheckBox item in collection //遍历这个集合   
   {  
      if (item.IsChecked == true)  
          MessageBox.Show(item.Content.ToString() + "被选中了!");  
   }              
}
```
先写到这了，以后有发现更好的方法再补上。
　　本文来自wushang923的博客，原文地址：http://blog.csdn.net/wushang923/article/details/6742378
